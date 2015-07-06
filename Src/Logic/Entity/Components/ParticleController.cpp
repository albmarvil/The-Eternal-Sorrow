#include "ParticleController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

#include "Graphics/Particle.h"
#include "Graphics/Scene.h"
#include "Graphics/Server.h"
#include "Graphics/ParticleManager.h"

namespace Logic
{
	IMP_FACTORY(CParticleController);

	CParticleController::~CParticleController()
	{
		//Graphics::CParticleManager::getSingletonPtr()->deleteObserver(_entity);
	}

	//---------------------------------------------------------

	bool CParticleController::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("sourceParticles"))
		{
			_vecSources = entityInfo->getVectorSTLAttribute("sourceParticles");
		}

		_startVisibleParticles = true;

		if(entityInfo->hasAttribute("startVisibleParticles"))
		{
			_startVisibleParticles = entityInfo->getBoolAttribute("startVisibleParticles");
		}

		if(entityInfo->hasAttribute("boneParticles"))
		{
			_vecBoneParticles = entityInfo->getVectorSTLAttribute("boneParticles");
		}
		else
		{
			for (int i = 0; i < _vecSources.size(); ++i)
			{
				_vecBoneParticles.push_back("-");
			}
		}

		setVisible(_startVisibleParticles);

		_activate = false;
		return true;
	}

	//---------------------------------------------------------

	bool CParticleController::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("sourceParticles"))
		{
			_vecSources = entityInfo->getVectorSTLAttribute("sourceParticles");
		}

		_startVisibleParticles = true;

		if(entityInfo->hasAttribute("startVisibleParticles"))
		{
			_startVisibleParticles = entityInfo->getBoolAttribute("startVisibleParticles");
		}

		if (entityInfo->hasAttribute("boneParticles"))
		{
			_vecBoneParticles = entityInfo->getVectorSTLAttribute("boneParticles");
		}
		else
		{
			for (int i = 0; i < _vecSources.size(); ++i)
			{
				_vecBoneParticles.at(i) = "-";
			}
		}
		
		setVisible(_startVisibleParticles);

		_activate = false;
		return true;
	}

	//-----------------------------------------------------

	bool CParticleController::activate()
	{
		//BaseSubsystems::Log::Debug("Nombre de particula2 "+*_vecSources.begin()+" Nombre: "+_entity->getName());
		if(_vecSources.begin()->compare("-") != 0 && !_activate)
		{
			///Apuntarse como observeR
			std::vector<Graphics::ParticleInfo> _vectorParticleInfo = Graphics::CParticleManager::getSingletonPtr()->addObserver(_entity, _vecSources, _vecBoneParticles);

			std::vector<Graphics::ParticleInfo>::iterator it = _vectorParticleInfo.begin();
			for(; it!= _vectorParticleInfo.end(); ++it)
			{
				//Obtenemos todas las particulas graficas
				_vectorParticulas.push_back((*it).particle);
			}
		}


		setVisible(_startVisibleParticles);
		_activate= true;
		return true;
	}

	//-----------------------------------------------------

	void CParticleController::deactivate()
	{
		if(_vecSources.begin()->compare("-") != 0)
		{
			Graphics::CParticleManager::getSingletonPtr()->deleteObserver(_entity);
		}

		_vectorParticulas.clear();

		_activate = false;
	}

	//-----------------------------------------------------

	void CParticleController::setQuota(int quota)
	{
		std::vector<Graphics::CParticle*>::iterator it = _vectorParticulas.begin();

		for(; it!= _vectorParticulas.end(); ++it)
		{
			(*it)->setQuota(quota);
		}

	}

	void CParticleController::startParticle()
	{
		std::vector<Graphics::CParticle*>::iterator it = _vectorParticulas.begin();

		for(; it!= _vectorParticulas.end(); ++it)
		{
			(*it)->start();
		}
	}

	void CParticleController::stopParticle()
	{
		std::vector<Graphics::CParticle*>::iterator it = _vectorParticulas.begin();

		for(; it!= _vectorParticulas.end(); ++it)
		{
			(*it)->stopFade(1.0f);
		}
	}

	bool CParticleController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "ADD_PARTICLE_OBSERVER" ||
			   message->getType() == "DELETE_PARTICLE_OBSERVER" ||
			   message->getType() == "DE_ACTIVATE_PARTICLE" ||
			   message->getType() == "SET_SCALE_PARTICLE" ||
			   message->getType() == "START_PARTICLES";

	} // accept

	void CParticleController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("ADD_PARTICLE_OBSERVER") == 0)
		{
			Graphics::CParticleManager::getSingletonPtr()->addObserver(_entity,_vecSources,_vecBoneParticles);
		}
		else if(message->getType().compare("DELETE_PARTICLE_OBSERVER") == 0)
		{
			Graphics::CParticleManager::getSingletonPtr()->deleteObserver(_entity);
		}
		else if(message->getType().compare("DE_ACTIVATE_PARTICLE") == 0)
		{
			bool activPart = dynamic_cast<DE_ACTIVATE_PARTICLE*>(message.get())->getBool();
			std::string source = dynamic_cast<DE_ACTIVATE_PARTICLE*>(message.get())->getString();

			if (activPart)
				Graphics::CParticleManager::getSingletonPtr()->activateParticle(_entity, source);
			else
				Graphics::CParticleManager::getSingletonPtr()->deactivateParticle(_entity, source);
		}
		else if(message->getType().compare("SET_SCALE_PARTICLE") == 0)
		{
			float scale = dynamic_cast<SET_SCALE_PARTICLE*>(message.get())->getScale();
			Logic::CEntity* owner = dynamic_cast<SET_SCALE_PARTICLE*>(message.get())->getOwner();
			std::string source = dynamic_cast<SET_SCALE_PARTICLE*>(message.get())->getSourceParticle();

			for (int i = 0; i < _vectorParticulas.size(); ++i)
			{
				if (_vectorParticulas.at(i)->getSource() == source)
				{
					_vectorParticulas.at(i)->setScale(scale);
				}
			}
		}
		else if(message->getType().compare("START_PARTICLES") == 0)
		{
			bool start = dynamic_cast<START_PARTICLES*>(message.get())->getBool();

			setVisible(start);
		}

	} // process

	void CParticleController::setVisible(bool visible)
	{
		std::vector<Graphics::CParticle*>::iterator it = _vectorParticulas.begin();

		for(; it!= _vectorParticulas.end(); ++it)
		{
			(*it)->setVisible(visible);

			if (visible)
				(*it)->start();
			else 
				(*it)->stop();
		}
	}
}