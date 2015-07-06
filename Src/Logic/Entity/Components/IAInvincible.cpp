#include "IAInvincible.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"

namespace Logic 
{
	IMP_FACTORY(CIAInvincible);
	 
	bool CIAInvincible::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxTimeInvincible"))
			_maxTimeInvincible = entityInfo->getFloatAttribute("maxTimeInvincible");

		if(entityInfo->hasAttribute("rangeDefend"))
			_rangeDefend = entityInfo->getFloatAttribute("rangeDefend");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAInvincible::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxTimeInvincible"))
			_maxTimeInvincible = entityInfo->getFloatAttribute("maxTimeInvincible");

		if(entityInfo->hasAttribute("rangeDefend"))
			_rangeDefend = entityInfo->getFloatAttribute("rangeDefend");

		return true;
	}

	//---------------------------------------------------------

	bool CIAInvincible::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CHANGE_TARGET";
	} // accept

	//---------------------------------------------------------

	void CIAInvincible::process(const std::shared_ptr<Logic::IMessage> &message)
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

	} // process

	//---------------------------------------------------------

	CIAInvincible::~CIAInvincible()
	{
		
	} // ~Controller

	bool CIAInvincible::activate()
	{
		_currentTimeInvincible = 0;
		_activated = false;
		_target = CServer::getSingletonPtr()->getPlayer();

		return true;

	} // activate

	void CIAInvincible::deactivate()
	{
		
	} // deactivate

	bool CIAInvincible::onAwake()
	{
		_currentTimeInvincible = 0;
		_activated = false;

		return true;
	}

	bool CIAInvincible::onSleep()
	{
		return true;
	}

	void CIAInvincible::onTick(unsigned int msecs)
	{
		if (_entity->getCenterPosition().x > _target->getCenterPosition().x - _rangeDefend && 
			_entity->getCenterPosition().x < _target->getCenterPosition().x + _rangeDefend)
		{
			_activated = true;
		}
		
		if (_activated)
		{
			_currentTimeInvincible += msecs;

			std::shared_ptr<INVINCIBLE> m(new INVINCIBLE());
			m->setBool(true);
			_entity->emitMessage(m);

			if (_currentTimeInvincible >= _maxTimeInvincible) 
			{
				std::shared_ptr<INVINCIBLE> m(new INVINCIBLE());
				m->setBool(false);
				_entity->emitMessage(m);

				_currentTimeInvincible = 0;
				_activated = false;
			}
		}

	} // tick

} // namespace Logic

