/*
@filename MusicManager.h

Contiene la definición del manager de música del motor de juego de The Eternal Sorrow

Este manager se encarga de registrar y lanzar las pistas correspondientes a músicas de ambiente y banda sonora.

Es un singleton

@author Alberto Martínez Villarán
@date Mayo, 2015
*/


#include "MusicManager.h"
#include "Sound.h"
#include "LUA\ClassToLUA.h"

#include "BaseSubsystems/Log.h"
#include <cassert>


namespace Sounds
{

	CMusicManager* CMusicManager::_instance = 0;

	//--------------------------------------------------------------
	CMusicManager::CMusicManager()
	{
		assert(!_instance && "Segunda inicialización de Sounds::CMusicManager no permitida!");

		_instance = this;
	}//MusicManager

	//--------------------------------------------------------------

	CMusicManager::~CMusicManager()
	{
		assert(_instance);

		_instance = 0;
	}// ~CMusicManager

	//--------------------------------------------------------------

	bool CMusicManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Sounds::CMusicManager no permitida!");

		new CMusicManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;
	}//Init

	//--------------------------------------------------------------

	void CMusicManager::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//-------------------------------------------------------------

	bool CMusicManager::open()
	{

		///Creamos todos los sonidos de música
		CSound *sound = new CSound(std::string("/Musica/MainTheme3"), SoundType::MUSIC);
		_mapSounds["/Musica/MainTheme3"] = sound;

		sound = new CSound(std::string("/Musica/MenuTheme"), SoundType::MUSIC);
		_mapSounds["/Musica/MenuTheme"] = sound;


		sound = new CSound(std::string("/Player/Muerte1"), SoundType::MUSIC);
		_mapSounds["/Player/Muerte1"] = sound;


		sound = new CSound(std::string("/Player/Muerte2"), SoundType::MUSIC);
		_mapSounds["/Player/Muerte2"] = sound;

		sound = new CSound(std::string("/Player/Muerte3"), SoundType::MUSIC);
		_mapSounds["/Player/Muerte3"] = sound;

		//nos publicamos en LUA
		LUA::CClassToLUA ctl;
		ctl.publicarMusicManager();
		return true;

	} // open

	//--------------------------------------------------------

	void CMusicManager::close() 
	{
		///borramos todos los sonidos

		TMapSounds::iterator it = _mapSounds.begin();

		for(; it != _mapSounds.end(); ++it)
		{
			delete (*it).second;
		}

	} // close

	//--------------------------------------------------------


	CSound* CMusicManager::findSound(const std::string &sound)
	{
		CSound* res = 0;

		TMapSounds::iterator find = _mapSounds.find(sound);

		if(find != _mapSounds.end())
		{
			res = (*find).second;
		}

		return res;
	} //findSound
	//--------------------------------------------------------

	void CMusicManager::playSound(const std::string &sound)
	{
		CSound *s = findSound(sound);
		if(!s)
		{
			BaseSubsystems::Log::Warning("Estas accediendo a un sonido no registrado en MusicManager");
		}
		else
		{
			s->play();
		}
	}//playSound
	//--------------------------------------------------------


	void CMusicManager::stopSound(const std::string &sound)
	{
		CSound *s = findSound(sound);
		if(!s)
		{
			BaseSubsystems::Log::Warning("Estas accediendo a un sonido no registrado en MusicManager");
		}
		else
		{
			s->stop();
		}
	}//stopSound
	//--------------------------------------------------------

	void CMusicManager::stopAllSounds()
	{
		TMapSounds::iterator it = _mapSounds.begin();

		for(; it != _mapSounds.end(); ++it)
		{
			(*it).second->stop();
		}

	}//stopAllSounds
	//--------------------------------------------------------


	void CMusicManager::setSoundParameter(const std::string sound, std::string parameter, float value)
	{
		CSound *s = findSound(sound);
		if(!s)
		{
			BaseSubsystems::Log::Warning("Estas accediendo a un sonido no registrado en MusicManager");
		}
		else
		{
			s->setParameter(parameter, value);
		}
	}//setSoundParameter
	//--------------------------------------------------------

};//namespace Sounds