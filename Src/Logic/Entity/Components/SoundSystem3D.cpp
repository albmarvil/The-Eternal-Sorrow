#include "SoundSystem3D.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Sounds/Sound3D.h"
#include "Sounds/Server.h"

namespace Logic 
{
	IMP_FACTORY(SoundSystem3D);
	


	SoundSystem3D::SoundSystem3D() : IComponent()
	{

	}
	//--------------------------------------------------------

	SoundSystem3D::~SoundSystem3D()
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

	bool SoundSystem3D::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("sounds3D"))
		{
			_soundsStrings = entityInfo->getVectorSTLAttribute("sounds3D");
		}
		if(entityInfo->hasAttribute("started_sounds"))
		{
			_startedSounds = entityInfo->getVectorSTLAttribute("started_sounds");
		}
		
		//BaseSubsystems::Log::Debug("HELLO SOUND");
		
		return true;

	} // spawn


	bool SoundSystem3D::respawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("sounds3D"))
		{
			_soundsStrings = entityInfo->getVectorSTLAttribute("sounds3D");
		}
		if(entityInfo->hasAttribute("started_sounds"))
		{
			_startedSounds = entityInfo->getVectorSTLAttribute("started_sounds");
		}
		return true;

	} // respawn
	//---------------------------------------------------------


	bool SoundSystem3D::activate() 
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
			Sounds::CSound3D* s = _sounds[name];
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
				Sounds::CSound3D* s = new Sounds::CSound3D((*it), Sounds::SoundType::EFFECT );
				_sounds.insert(std::pair<std::string, Sounds::CSound3D*>((*it), s));
			}
		}

		///Ahora inicializamos los sonidos indicados por defecto
		std::vector<std::string>::iterator started = _startedSounds.begin();
		for(; started != _startedSounds.end(); ++started)
		{
			std::string soundName = (*started);
			play(soundName);
		}

		setSoundTransform(_entity->getTransform());

		//Nos registramos como observador de la pos de la entidad logica
		_entity->addEntityTransformListener(this);
		return true;

	} // activate
	//---------------------------------------------------------

	void SoundSystem3D::deactivate() 
	{
		//paramos todos los sonidos
		TSoundMap::iterator stopped = _sounds.begin();
		for(; stopped != _sounds.end(); ++stopped)
		{
			stopped->second->stop();
		}

		//Nos desregistramos de observador de la pos logica de la entidad
		_entity->removeEntityTransformListener(this);
		
	} // deactivate
	//--------------------------------------------------------

	bool SoundSystem3D::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "PLAY_SOUND" ||
			   message->getType() == "STOP_SOUND" ||
			   message->getType() == "SET_SOUND_PARAMETER"; /*||
			   message->getType() == "SET_TRANSFORM";*/

	} // accept
	
	//---------------------------------------------------------

	void SoundSystem3D::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "PLAY_SOUND")
		{
			std::string sound = dynamic_cast<PLAY_SOUND*>(message.get())->getSound();

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
		//else if(message->getType() == "SET_TRANSFORM")
		//{
		//	Matrix4 trans = dynamic_cast<SET_TRANSFORM*>(message.get())->getTransform();
		//	//cambiamos la posicion a todos los sonidos emitidos desde esa entidad
		//	setSoundTransform(trans);
		//}
		
	} // process

	//---------------------------------------------------------------------------------

	void SoundSystem3D::onTick(unsigned int msecs)
	{
		
	}

	void SoundSystem3D::play(const std::string &sound)
	{
		TSoundMap::iterator find = _sounds.find(sound);

		if(find != _sounds.end())
		{
			find->second->play();
		}
	}

	void SoundSystem3D::stop(const std::string &sound)
	{
		TSoundMap::iterator find = _sounds.find(sound);

		if(find != _sounds.end())
		{
			find->second->stop();
		}
	}

	void SoundSystem3D::setParameter(std::string &sound, std::string &parameter, float value)
	{
		TSoundMap::iterator find = _sounds.find(sound);

		if(find != _sounds.end())
		{
			find->second->setParameter(parameter, value);
		}
	}

	void SoundSystem3D::setSoundTransform(const Matrix4 &transform)
	{
		TSoundMap::iterator it = _sounds.begin();
		Vector3 _position = transform.getTrans();

		for(; it != _sounds.end(); ++it)
		{
			it->second->setPosition(transform.getTrans());
			//Por ahora solo seteamos la posicion, los vectores de dirección UP y Forward se quedarán los por defecto
		}
	}

	void SoundSystem3D::OnEntitySetTransform(const Matrix4 &transform)
	{
		setSoundTransform(transform);
	}

} // namespace Logic