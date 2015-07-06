#include "SoundSystem.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Sounds/Sound.h"
#include "Sounds/Server.h"

namespace Logic 
{
	IMP_FACTORY(SoundSystem);
	


	SoundSystem::SoundSystem() : IComponent()
	{

	}
	//--------------------------------------------------------

	SoundSystem::~SoundSystem()
	{
		//borramos todos los sonidos
		TSoundMap::iterator iter = _sounds.begin();
		for(; iter != _sounds.end(); ++iter)
		{
			Sounds::CSound* s = (*iter).second;
			delete s;
		}

		_sounds.clear();
	}

	//---------------------------------------------------------

	bool SoundSystem::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("sounds"))
		{
			_soundsStrings = entityInfo->getVectorSTLAttribute("sounds");
		}
		if(entityInfo->hasAttribute("started_sounds"))
		{
			_startedSounds = entityInfo->getVectorSTLAttribute("started_sounds");
		}
		
		return true;

	} // spawn


	bool SoundSystem::respawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("sounds"))
		{
			_soundsStrings = entityInfo->getVectorSTLAttribute("sounds");
		}
		if(entityInfo->hasAttribute("started_sounds"))
		{
			_startedSounds = entityInfo->getVectorSTLAttribute("started_sounds");
		}
		return true;

	} // respawn
	//---------------------------------------------------------


	bool SoundSystem::activate() 
	{
		std::vector<std::string> toErase;
		///vemos los sonidos que se han quedado antiguos y los apuntamos para borrar
		TSoundMap::iterator iter = _sounds.begin();
		for(; iter != _sounds.end(); ++iter)
		{
			std::string name = (*iter).first;
			std::vector<std::string>::iterator find = std::find(_soundsStrings.begin(), _soundsStrings.end(), name);

			if(find == _soundsStrings.end())
			{
				toErase.push_back(name);
			}
		}

		//Borramos los que habiamos apuntado para borrar
		std::vector<std::string>::iterator errit = toErase.begin();
		for(; errit != toErase.end(); ++errit)
		{
			std::string name = (*errit);
			Sounds::CSound* s = _sounds[name];
			delete s;
			_sounds.erase(name);
		}
		
		//Ahora añadimos solo aquellos sonidos que hemos recibido nuevos
		std::vector<std::string>::iterator it = _soundsStrings.begin();
		
		for(; it != _soundsStrings.end(); ++it)
		{
			std::string name = (*it);
			TSoundMap::iterator find = _sounds.find(name);
			if(find == _sounds.end())
			{
				Sounds::CSound* s = new Sounds::CSound((*it), Sounds::SoundType::EFFECT);
				_sounds.insert(std::pair<std::string, Sounds::CSound*>((*it), s));
			}
		}

		///Ahora inicializamos los sonidos indicados por defecto
		std::vector<std::string>::iterator started = _startedSounds.begin();
		for(; started != _startedSounds.end(); ++started)
		{
			std::string soundName = (*started);
			play(soundName);
		}
		return true;

	} // activate
	//---------------------------------------------------------

	void SoundSystem::deactivate() 
	{
		//paramos todos los sonidos
		TSoundMap::iterator stopped = _sounds.begin();
		for(; stopped != _sounds.end(); ++stopped)
		{
			stopped->second->stop();
		}

	} // deactivate
	//--------------------------------------------------------

	bool SoundSystem::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "PLAY_SOUND" ||
				message->getType() == "STOP_SOUND" ||
			   message->getType() == "SET_SOUND_PARAMETER";

	} // accept
	
	//---------------------------------------------------------

	void SoundSystem::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "PLAY_SOUND")
		{
			std::string sound = dynamic_cast<PLAY_SOUND*>(message.get())->getSound();

			//controlarlo mejor
			play(sound);
		}
		else if(message->getType() == "STOP_SOUND")
		{
			std::string sound = dynamic_cast<STOP_SOUND*>(message.get())->getSound();

			//controlarlo mejor
			stop(sound);
		}
		else if(message->getType() == "SET_SOUND_PARAMETER")
		{
			SET_SOUND_PARAMETER* m = dynamic_cast<SET_SOUND_PARAMETER*>(message.get());
			setParameter(m->getSound(), m->getParameter(), m->getValue());
		}
		
	} // process

	//---------------------------------------------------------------------------------

	void SoundSystem::onTick(unsigned int msecs)
	{
		
	}

	void SoundSystem::play(const std::string &sound)
	{
		TSoundMap::iterator find = _sounds.find(sound);

		if(find != _sounds.end())
		{
			find->second->play();
		}
	}

	void SoundSystem::stop(const std::string &sound)
	{
		TSoundMap::iterator find = _sounds.find(sound);

		if(find != _sounds.end())
		{
			find->second->stop();
		}
	}

	void SoundSystem::setParameter(std::string &sound, std::string &parameter, float value)
	{
		TSoundMap::iterator find = _sounds.find(sound);

		if(find != _sounds.end())
		{
			find->second->setParameter(parameter, value);
		}
	}

} // namespace Logic