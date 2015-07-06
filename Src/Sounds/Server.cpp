/*
@filename Server.cpp

Contiene la definición del servidor de Sonidos del motor de juego Eternal Sorrow

Este servidor contiene la inicialización de FMOD Studio,´y funciona como punto de acceso para el resto del sistema al sistema de sonido

Es un singleton

@author Alberto Martínez Villarán
@date Marzo, 2015
*/


#include "Server.h"
#include "Bank.h"
#include "Listener.h"
#include "Sound.h"
#include "Sound3D.h"
#include "MusicManager.h"
#include "GUISoundsManager.h"

#include "BaseSubsystems/Log.h"
#include "LUA/ScriptManager.h"
#include "LUA\ClassToLUA.h"
#include <cassert>

//includes de FMOD
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.hpp>

namespace Sounds
{

	///definición nazi de ERRCHECK sacado de los samples
	bool ERRCHECK_server(FMOD_RESULT result, const char *file, int line);
	#define ERRCHECK(_result) ERRCHECK_server(_result, __FILE__, __LINE__)

	bool ERRCHECK_server(FMOD_RESULT result, const char *file, int line)
	{
#ifdef _DEBUG
		if (result != FMOD_OK)
		{
			BaseSubsystems::Log::Error(FMOD_ErrorString(result));
			return false;
		}
		else
		{
			return true;
		}
#endif
		return true;
	}

	CServer* CServer::_instance = 0;

	//--------------------------------------------------------------
	CServer::CServer()
	{
		assert(!_instance && "Segunda inicialización de Sounds::CServer no permitida!");

		_instance = this;
	}//Server

	//--------------------------------------------------------------

	CServer::~CServer()
	{
		assert(_instance);

		_instance = 0;
	}// ~CServer

	//--------------------------------------------------------------

	bool CServer::Init()
	{
		assert(!_instance && "Segunda inicialización de Sounds::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;
	}//Init

	//--------------------------------------------------------------

	void CServer::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//-------------------------------------------------------------

	bool CServer::open()
	{

		/*
		Inicialización de FMOD y del system adecuadamente
		*/

		FMOD_RESULT result;
		_studioSystem = 0;

		result = FMOD::Studio::System::create(&_studioSystem); // Create the Studio System object.
		if(!ERRCHECK(result))
		{
			return false;
		}

		result = _studioSystem->getLowLevelSystem(&_lowSystem); // Create the low level System object.
		if(!ERRCHECK(result))
		{
			return false;
		}

		result = _lowSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);
		if(!ERRCHECK(result))
		{
			return false;
		}
		//usamos centimetros en nuestro mundo! por eso el 0.01
		result = _lowSystem->set3DSettings(1.0, 0.01, 1.0);
		if(!ERRCHECK(result))
		{
			return false;
		}
		// Initialize FMOD Studio, which will also initialize FMOD Low Level
		result = _studioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
		if(!ERRCHECK(result))
		{
			return false;
		}

		_listener = 0;

		std::set<CSound*> musicSet;
		std::set<CSound*> effectSet;

		_soundTypeMap.insert(std::pair<SoundType, std::set<CSound*>> (MUSIC, musicSet));
		_soundTypeMap.insert(std::pair<SoundType, std::set<CSound*>> (EFFECT, effectSet));

		_musicMasterVolume = 1.0f;
		_effectsMasterVolume = 1.0f;

		//loadBanks();

		////EJEMPLO DE REPRODUCCION DE UN SONIDO
		//FMOD::Sound* s;
		//result = _lowSystem->createSound("./media/sounds/test.mp3", FMOD_2D, 0, &s);
		//ERRCHECK(result);
		//FMOD::Channel* channel;
		//result = _lowSystem->playSound(s, 0, false, &channel);
		//ERRCHECK(result);
		
		return true;

	} // open

	//--------------------------------------------------------

	void CServer::InitManagers()
	{
		//Inicializamos el manager de músicas
		CMusicManager::Init();
		//Inicializamos el manager de efectos de GUI
		CGUISoundsManager::Init();
	}//InitManagers

	//------------------------------------------------------

	void CServer::close() 
	{
		//Liberamos el manager de músicas
		CMusicManager::Release();
		//Liberamos el manager de efectos de GUI
		CGUISoundsManager::Release();

		unloadBanks();
		/*
		Liberación de los recursos de FMOD y del system
		*/
		//_lowSystem->release();
		_studioSystem->release();

		_lowSystem = 0;
		_studioSystem = 0;
	

	} // close

	//--------------------------------------------------------

	std::string CServer::getSoundPath()
	{
		return LUA::CScriptManager::GetPtrSingleton()->getField("Paths","pathSounds", ".");
	}
	//--------------------------------------------------------

	//--------------------------------------------------------

	void CServer::unloadBanks()
	{
		std::set<CBank*>::iterator it = _banks.begin();

		for(; it != _banks.end(); ++it)
		{
			delete (*it);
		}
		_banks.clear();

	}
	//--------------------------------------------------------

	void CServer::AddBank(const std::string &bankName)
	{
		CBank* bank = new CBank(bankName);
		_banks.insert(bank);
	}

	//-------------------------------------------------------
	
	void CServer::AddListener(CListener* listener)
	{
		assert(!_listener && "Ya existe un listener!! FMOD Studio solo admite tener un listener a la vez");
		_listener = listener;
	}

	//---------------------------------------------------------

	void CServer::AddSound3D(CSound3D* sound)
	{
		_sounds3D.insert(sound);
	}

	//---------------------------------------------------------

	void CServer::AddSound(CSound* sound)
	{
		_soundTypeMap[sound->getSoundType()].insert(sound);
	}

	//---------------------------------------------------------

	void CServer::RemoveListener(CListener* listener)
	{
		_listener = 0;
	}

	//---------------------------------------------------------

	void CServer::RemoveSound3D(CSound3D* sound)
	{
		_sounds3D.erase(sound);
	}

	//---------------------------------------------------------

	void CServer::RemoveSound(CSound* sound)
	{
		_soundTypeMap[sound->getSoundType()].erase(sound);
	}

	//---------------------------------------------------------

	void CServer::tick(unsigned int msecs)
	{
		if(_studioSystem)
		{
			//actualizamos los listeners
			if(_listener != NULL)
			{
				_listener->tick(msecs);
			}
			
			//Actualizamos los sonidos posicionales
			std::set<CSound3D*>::iterator its = _sounds3D.begin();
			for(; its != _sounds3D.end(); ++its)
			{
				(*its)->tick(msecs);
			}

			ERRCHECK(_studioSystem->update());
		}
	}


	void CServer::setMusicMasterVolume(float volume)
	{
		float level = abs(std::min(volume, 1.0f));
		_musicMasterVolume = level;

		std::set<CSound*>::iterator it = _soundTypeMap[MUSIC].begin();

		for(; it !=  _soundTypeMap[MUSIC].end(); ++it)
		{
			//coger volumen
			//setear de nuevo
			(*it)->setVolume((*it)->getVolume());
		}
	}

	void CServer::setEffectsMasterVolume(float volume)
	{
		float level = abs(std::min(volume, 1.0f));
		_effectsMasterVolume = level;

		std::set<CSound*>::iterator it = _soundTypeMap[EFFECT].begin();

		for(; it !=  _soundTypeMap[EFFECT].end(); ++it)
		{
			//coger volumen
			//setear de nuevo
			(*it)->setVolume((*it)->getVolume());
		}
	}



};//namespace Sounds