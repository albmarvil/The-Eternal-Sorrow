/*
@filename Sound.cpp

Contiene la implementeación de la clase CSound que encapsula el manejo de eventos de FMOD

@author Alberto Martínez Villarán
@date Marzo, 2015
*/


#include "Sound.h"

#include "Server.h"
#include "BaseSubsystems/Log.h"
#include <cassert>

//includes de FMOD
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.hpp>

namespace Sounds
{
	///definición nazi de ERRCHECK sacado de los samples
	void ERRCHECK_sound(FMOD_RESULT result, const char *file, int line);
	#define ERRCHECK(_result) ERRCHECK_sound(_result, __FILE__, __LINE__)

	void ERRCHECK_sound(FMOD_RESULT result, const char *file, int line)
	{
#ifdef _DEBUG
		if (result != FMOD_OK)
		{
			BaseSubsystems::Log::Error(FMOD_ErrorString(result));
		}
#endif
	}


	CSound::CSound(std::string &eventName, SoundType type) : _eventName(eventName), _type(type), _volume(1.0)
	{
		_server = CServer::getSingletonPtr();
		if(_server)
		{
			std::string fullName = std::string("event:") + _eventName;

			_description = NULL;
			FMOD_RESULT result =  _server->getStudioSystem()->getEvent(fullName.c_str(), &_description) ;
			ERRCHECK(result);

			/*
			Por defecto cargamos la info de los samples por cada sonido
			*/
			result = _description->loadSampleData();
			ERRCHECK(result);

			_instance = NULL;
			result = _description->createInstance(&_instance);
			ERRCHECK(result);

			int parameterCount = 0;
			result = _instance->getParameterCount(&parameterCount);
			ERRCHECK(result);

			//iteramos sobre los parámetros y sacamos su información, y creamos los controladores de los mismos
			for(int i = 0; i < parameterCount; ++i)
			{
				FMOD::Studio::ParameterInstance* parameter;
				result = _instance->getParameterByIndex(i, &parameter);
				ERRCHECK(result);

				FMOD_STUDIO_PARAMETER_DESCRIPTION parameterDescription;
				result = parameter->getDescription(&parameterDescription);
				ERRCHECK(result);

				std::string parameterName = parameterDescription.name;

				_parameters.insert(std::pair<std::string, FMOD::Studio::ParameterInstance*>(parameterName, parameter));
			}

			_server->AddSound(this);

			setVolume(1.0f);
			
		}
	}

	CSound::~CSound()
	{
		_description->unloadSampleData();
		_instance->release();
		_description->releaseAllInstances();
		_server = 0;
	}


	void CSound::play()
	{
		//BaseSubsystems::Log::Debug("Play a "+_eventName);
		ERRCHECK(_instance->start());
	}

	void CSound::stop()
	{
		ERRCHECK(_instance->stop(FMOD_STUDIO_STOP_IMMEDIATE));
	}

	void CSound::setVolume(float volume)
	{
		_volume = volume;
		float finalVolumen = _volume;

		switch (_type)
		{
		case Sounds::MUSIC:
			finalVolumen *= CServer::getSingletonPtr()->getMusicMasterVolume();
			break;
		case Sounds::EFFECT:
			finalVolumen *= CServer::getSingletonPtr()->getEffectsMasterVolume();
			break;
		}
		ERRCHECK(_instance->setVolume(finalVolumen));
	}

	void CSound::setParameter(std::string &parameter, float value)
	{
		TParameters::iterator find = _parameters.find(parameter);

		if(find != _parameters.end())//si lo encontramos en el mapa
		{
			ERRCHECK(find->second->setValue(value));
		}
	}

	float CSound::getParameter(std::string &parameter)
	{
		float result = -999.9f;
		TParameters::iterator find = _parameters.find(parameter);

		if(find != _parameters.end())//si lo encontramos en el mapa
		{
			
			ERRCHECK(find->second->getValue(&result));
		}

		return result;
	}

};//namespace Sounds