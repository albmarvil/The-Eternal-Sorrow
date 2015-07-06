#include "IATeleport.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include "Logic/Entity/Components/LifeEnemy.h"

namespace Logic 
{
	IMP_FACTORY(CIATeleport);
	 
	bool CIATeleport::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayTeleporting"))
			_maxDelayTeleporting = entityInfo->getIntAttribute("maxDelayTeleporting");

		if(entityInfo->hasAttribute("maxCooldownTeleport"))
			_maxCooldownTeleport = entityInfo->getIntAttribute("maxCooldownTeleport");

		
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIATeleport::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayTeleporting"))
			_maxDelayTeleporting = entityInfo->getIntAttribute("maxDelayTeleporting");

		if(entityInfo->hasAttribute("maxCooldownTeleport"))
			_maxCooldownTeleport = entityInfo->getIntAttribute("maxCooldownTeleport");

		return true;
	}

	//---------------------------------------------------------

	bool CIATeleport::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CHANGE_TARGET";
	}

	void CIATeleport::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CHANGE_TARGET")
		{
			if (message->getType() == "CHANGE_TARGET")
		{
			CEntity* ent = dynamic_cast<CHANGE_TARGET*>(message.get())->getLogicEntity();
			if(ent != NULL)
			{
				if(!ent->getIsDead())
				{
					_target = ent; 
				}
			}
			
		}  
		}
	} // process

	//---------------------------------------------------------

	CIATeleport::~CIATeleport()
	{
		
	} // ~Controller

	bool CIATeleport::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_currentDelayTeleporting = 0;
		_currentCooldownTeleport = 0;
		_stepIA = 0;
		return true;

	} // activate

	void CIATeleport::deactivate()
	{
		
	} // deactivate

	bool CIATeleport::onAwake()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_currentDelayTeleporting = 0;
		_currentCooldownTeleport = 0;
		_stepIA = 0;
		_canBeHit = true;
		_initIA = false;

		return true;
	}

	bool CIATeleport::onSleep()
	{
		return true;
	}

	void CIATeleport::onTick(unsigned int msecs)
	{
		_currentCooldownTeleport += msecs;

		if (_currentCooldownTeleport >= _maxCooldownTeleport)
			_initIA = true;

		// Si se ha activado la IA, la ejecutamos
		if (_initIA)
		{
			// 1. Iniciamos el primer/ultimo intervalo de invencibilidad
			if(_stepIA == 0|| _stepIA == 2)
			{
				if (_canBeHit == true)
				{
					std::shared_ptr<INVINCIBLE> m(new INVINCIBLE());
					m->setBool(true);
					_entity->emitMessage(m);

					_canBeHit = false;
				}

				_currentDelayTeleporting += msecs;
			}

			// 2. Teletransportamos
			else if (_stepIA == 1)
			{
				Vector3 direction = (_target->getCenterPosition() - _entity->getCenterPosition()) * Vector3(1,1,0);
				int randPointX = rand() % 35  + 10;
				int randPointY = rand() % 35  + 10;

				direction *= Vector3(randPointX/100.f, randPointY/100.f, 0);

				std::shared_ptr<TELEPORT> m(new TELEPORT());
				m->setVector3(_entity->getCenterPosition() + direction);
				_entity->emitMessage(m);

				_currentDelayTeleporting = 0;
				_stepIA++;
			}

			// 3. Terminamos la IA
			else if (_stepIA == 3)
			{
				_initIA = false;
				_currentDelayTeleporting = 0;
				_currentCooldownTeleport = 0;
				_stepIA = 0;
				_canBeHit = true;
			}

			// Si ha superado el tiempo máximo de invencibilidad, vamos al siguiente paso
			if (_currentDelayTeleporting >= _maxDelayTeleporting) 
			{
				std::shared_ptr<INVINCIBLE> m(new INVINCIBLE());
				m->setBool(false);
				_entity->emitMessage(m);

				_canBeHit = true;

				_stepIA++;
				_currentDelayTeleporting = 0;
			}
		}
		
	} // tick

} // namespace Logic

