/*
@filename Sound.h

Contiene la definición de un Sonido parametrizado en FMOD Studio.

@author Alberto Martínez Villarán
@date Marzo, 2015
@refactor Alberto Martínez Villarán
@date Mayo de 2015
*/

#ifndef __Sounds_Sound_H
#define __Sounds_Sound_H

#include <string>
#include <map>
#include "Server.h"

namespace FMOD
{
	namespace Studio
	{
		class EventDescription;
		class EventInstance;
		class ParameterInstance;
	}
}

namespace Sounds
{

	class CSound
	{
	public:

		CSound(std::string &eventName, SoundType type);

		virtual ~CSound();

		void play();

		void stop();

		///el volumen va de 0 a 1
		void setVolume(float volume);

		float getVolume() { return _volume; };

		void setParameter(std::string &parameter, float value);

		float getParameter(std::string &parameter);

		std::string getName() { return _eventName; }

		SoundType getSoundType() { return _type; };

	protected:

		CServer* _server;

		typedef std::map<std::string, FMOD::Studio::ParameterInstance*> TParameters;
		TParameters _parameters;

		FMOD::Studio::EventDescription* _description;

		FMOD::Studio::EventInstance* _instance;

		std::string _eventName;

		SoundType _type;

		float _volume;


	};//CServer

};//namespace Sounds

#endif //__Sounds_Sound_H