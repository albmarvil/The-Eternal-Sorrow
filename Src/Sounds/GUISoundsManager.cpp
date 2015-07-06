/*
@filename GUISoundsManager.h

Contiene la definición del manager de música del motor de juego de The Eternal Sorrow

Este manager se encarga de registrar y lanzar las pistas correspondientes a efectos de interfaz y GUI

Es un singleton

@author Alberto Martínez Villarán
@date Mayo, 2015
*/

#include "GUISoundsManager.h"
#include "Sound.h"
#include "LUA\ClassToLUA.h"

#include "BaseSubsystems/Log.h"
#include <cassert>


namespace Sounds
{

	CGUISoundsManager* CGUISoundsManager::_instance = 0;

	//--------------------------------------------------------------
	CGUISoundsManager::CGUISoundsManager()
	{
		assert(!_instance && "Segunda inicialización de Sounds::CGUISoundsManager no permitida!");

		_instance = this;
	}//GUISoundsManager

	//--------------------------------------------------------------

	CGUISoundsManager::~CGUISoundsManager()
	{
		assert(_instance);

		_instance = 0;
	}// ~CGUISoundsManager

	//--------------------------------------------------------------

	bool CGUISoundsManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Sounds::CGUISoundsManager no permitida!");

		new CGUISoundsManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;
	}//Init

	//--------------------------------------------------------------

	void CGUISoundsManager::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//-------------------------------------------------------------

	bool CGUISoundsManager::open()
	{

		///Creamos todos los sonidos de música
		CSound *sound = new CSound(std::string("/Varios/IniciarPartida"), SoundType::EFFECT);
		_mapSounds["/Varios/IniciarPartida"] = sound;


		sound = new CSound(std::string("/Escenario/RecogerMoneda"), SoundType::EFFECT);
		_mapSounds["/Escenario/RecogerMoneda"] = sound;


		sound = new CSound(std::string("/Escenario/RecogerObjeto1"), SoundType::EFFECT);
		_mapSounds["/Escenario/RecogerObjeto1"] = sound;

		//nos publicamos en LUA
		LUA::CClassToLUA ctl;
		ctl.publicarGUISoundsManager();
		return true;

	} // open

	//--------------------------------------------------------

	void CGUISoundsManager::close() 
	{
		///borramos todos los sonidos

		TMapSounds::iterator it = _mapSounds.begin();

		for(; it != _mapSounds.end(); ++it)
		{
			delete (*it).second;
		}

	} // close

	//--------------------------------------------------------


	CSound* CGUISoundsManager::findSound(const std::string &sound)
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

	void CGUISoundsManager::playSound(const std::string &sound)
	{
		CSound *s = findSound(sound);
		if(!s)
		{
			BaseSubsystems::Log::Warning("Estas accediendo a un sonido no registrado en GUISoundsManager");
		}
		else
		{
			s->play();
		}
	}//playSound
	//--------------------------------------------------------


	void CGUISoundsManager::stopSound(const std::string &sound)
	{
		CSound *s = findSound(sound);
		if(!s)
		{
			BaseSubsystems::Log::Warning("Estas accediendo a un sonido no registrado en GUISoundsManager");
		}
		else
		{
			s->stop();
		}
	}//stopSound
	//--------------------------------------------------------

	void CGUISoundsManager::stopAllSounds()
	{
		TMapSounds::iterator it = _mapSounds.begin();

		for(; it != _mapSounds.end(); ++it)
		{
			(*it).second->stop();
		}

	}//stopAllSounds
	//--------------------------------------------------------


	void CGUISoundsManager::setSoundParameter(const std::string &sound, std::string &parameter, float value)
	{
		CSound *s = findSound(sound);
		if(!s)
		{
			BaseSubsystems::Log::Warning("Estas accediendo a un sonido no registrado en GUISoundsManager");
		}
		else
		{
			s->setParameter(parameter, value);
		}
	}//setSoundParameter
	//--------------------------------------------------------

};//namespace Sounds