//---------------------------------------------------------------------------
// Log.cpp
//---------------------------------------------------------------------------

/**
@file Log.cpp

Contiene la declaración de la clase estática de log. Esta clase permite configurar 4 niveles diferentes de log:
	-DEBUG		: Este nivel esta destinado para las trazas y logs comunes
	-WARNING	: Este nivel esta destinado para notificar de las alertas de ejecución
	-ERROR		: Este nivel esta destinado para notificar de errores de ejecución.
	-NONE		: Este nivel deshabilita por completo el sistema de Log


Los niveles de log son acumulativos, en orden de granularidad. Es decir, el nivel de DEBUG mostrará también el nivel WARNING y ERROR.
Mientras que los niveles siguientes no podrán mostrar información de menor orden de granularidad. El nivel de log NONE no admitirá ninguna
entrada de log.

Por defecto todas las entradas del log se guardarán en fichero, excepto cuando el nivel NONE esta activo.

Además este sistema de log permite imprimir por pantalla cuando se lanza la aplicación en Debug. Este parámetro también es configurable


@see BaseSubsystems::Log

@author Alberto Martínez Villarán
@date Febrero, 2015
*/


#include "Log.h"
#include "LUA\ScriptManager.h"

using namespace std;

namespace BaseSubsystems {

	///PARAMETROS DE CONFIGURACION

	#define _LOGFILE LUA::CScriptManager::GetPtrSingleton()->getField("Paths","pathLOG", ".")


	LogLevel Log::_level = BaseSubsystems::LogLevel::DEBUG_LEVEL;


	bool Log::_print = true;

	bool Log::_showTick = true;

	int Log::_numTicks = 0;
	////FIN DE PARAMETROS DE CONFIG


	bool Log::OpenLogFile()
	{
		_file.open (_LOGFILE);
		if(!_file.is_open())
		{
			cerr<<"Ha habido un fallo al abrir el fichero de sistema de LOG. Arrancando sin LOG"<<endl;
			Log::_level = LogLevel::NONE_LEVEL;
		}
		return true;
	}

	void Log::CloseLogFile()
	{
		if(_file.is_open())
		{
			_file.close();
		}
	}

	void Log::Debug(const string &info)
	{
		if (_level != LogLevel::NONE_LEVEL && _level == LogLevel::DEBUG_LEVEL)
		{
			string level = " DEBUG: ";
			string time = Log::Time();
			time.append(level);
			time.append(info);
			const char* result = time.c_str();
		
			_file<<result<<endl;

	#ifdef _DEBUG
			if(_print)
			{
				cout<<result<<endl;
			}
	#endif
			_file.flush();
		}
		
	}//DEBUG

	//--------------------------------------------------------------

	void Log::Warning(const string &info)
	{
		if(_level != LogLevel::NONE_LEVEL && (_level == LogLevel::DEBUG_LEVEL || _level == LogLevel::WARNING_LEVEL) )
		{
			string level = " WARNING: ";
			string time = Log::Time();
			time.append(level);
			time.append(info);
			const char* result = time.c_str();
		
			_file<<result<<endl;

	#ifdef _DEBUG
			if(_print)
			{
				cout<<result<<endl;
			}
	#endif
			_file.flush();
		}
	}//DEBUG

	//--------------------------------------------------------------

	void Log::Error(const string &info)
	{
		if(_level != LogLevel::NONE_LEVEL &&( _level == LogLevel::DEBUG_LEVEL || _level == LogLevel::WARNING_LEVEL || _level == LogLevel::ERROR_LEVEL))
		{
			string level = " ERROR: ";
			string time = Log::Time();
			time.append(level);
			time.append(info);
			const char* result = time.c_str();
		
			_file<<result<<endl;

	#ifdef _DEBUG
			if(_print)
			{
				cout<<result<<endl;
			}
	#endif
			_file.flush();
		}
	}//DEBUG

	//--------------------------------------------------------------

	string Log::Time()
	{
		string result = "";

		if(_showTick)
		{
			result = "Tick: "+to_string(_numTicks);
		}
		else
		{
			time_t rawtime;
			struct tm * timeinfo;

			time (&rawtime);

			timeinfo = localtime (&rawtime);

			string hora = to_string(timeinfo->tm_hour);

			if(hora.size() < 2)
			{
				hora = "0"+hora;
			}

			string min = to_string(timeinfo->tm_min);

			if(min.size() < 2)
			{
				min = "0"+min;
			}

			string sec = to_string(timeinfo->tm_sec);

			if(sec.size() < 2)
			{
				sec = "0"+sec;
			}

			string horaTotal = hora + ":" + min + ":" + sec;

			string year = to_string(timeinfo->tm_year + 1900);

			string yearTotal = to_string(timeinfo->tm_mday) + "/" + to_string(timeinfo->tm_mon) + "/" + year;
		
			result = yearTotal + " - " + horaTotal;
		}

		return result;
	}

	ofstream Log::_file = ofstream();

};///BaseSubsystems

