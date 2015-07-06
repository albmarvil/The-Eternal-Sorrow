//---------------------------------------------------------------------------
// Log.h
//---------------------------------------------------------------------------

/**
@file Log.h

Contiene la declaraci�n de la clase est�tica de log. Esta clase permite configurar 4 niveles diferentes de log:
	-DEBUG		: Este nivel esta destinado para las trazas y logs comunes
	-WARNING	: Este nivel esta destinado para notificar de las alertas de ejecuci�n
	-ERROR		: Este nivel esta destinado para notificar de errores de ejecuci�n.
	-NONE		: Este nivel deshabilita por completo el sistema de Log


Los niveles de log son acumulativos, en orden de granularidad. Es decir, el nivel de DEBUG mostrar� tambi�n el nivel WARNING y ERROR.
Mientras que los niveles siguientes no podr�n mostrar informaci�n de menor orden de granularidad. El nivel de log NONE no admitir� ninguna
entrada de log.

Por defecto todas las entradas del log se guardar�n en fichero, excepto cuando el nivel NONE esta activo.

Adem�s este sistema de log permite imprimir por pantalla cuando se lanza la aplicaci�n en Debug. Este par�metro tambi�n es configurable


@see BaseSubsystems::Log

@author Alberto Mart�nez Villar�n
@date Febrero, 2015
*/


#ifndef __BaseSubsystems_Log_H
#define __BaseSubsystems_Log_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>

namespace BaseSubsystems {


	enum LogLevel {
		NONE_LEVEL,
		ERROR_LEVEL,
		WARNING_LEVEL,
		DEBUG_LEVEL
	};


	class Log {

	public:

		/*
		Abre el fichero de log

		@return true si todo ha ido bien
		*/
		static bool OpenLogFile();

		/*
		Cierra el fichero de log
		*/
		static void CloseLogFile();


		/*
		Modificador del nivel de Log

		@param level nivel de log a configurar. Sobreescribe el nivel que hab�a
		*/
		static void setLogLevel(LogLevel level) { _level = level; }


		/*
		Modificado del Flag de impresi�n por pantalla

		@param print. Flag de impresi�n a configurar. Sobreescribe el flag que hab�a
		*/
		static void setPrintFlag(bool print) { _print = print; }

		/*
		Log de nivel Debug

		@param info Informaci�n para el log
		*/
		static void Debug( const std::string &info);

		/*
		Log de nivel Warning

		@param info Informaci�n para el log
		*/
		static void Warning(const std::string &info);

		/*
		Log de nivel Error

		@param info Informaci�n para el log
		*/
		static void Error(const std::string &info);

		/*
		N�mero de ticks transcurridos desde la inicializaci�n del programa
		*/
		static int _numTicks;

	protected:

		/*
		Devuelve la cadena de la fecha del sistema

		@return cadena con el tiempo del sistema
		*/
		static std::string Time();
		
		/*
		Nivel de log configurado actualmente
		*/
		static LogLevel _level;


		/*
		Flag que habilita la impresi�n por pantalla. Solo se usa cuando estamos ejecutando en DEBUG
		*/
		static bool _print;

		/*
		Fichero en el cual se esciben los datos del log
		*/
		static std::ofstream _file;

		/*
		Flag que hace que el log muestre el numero de ticks en vez del tiempo
		*/
		static bool _showTick;

	};


};///BaseSubsystems

#endif