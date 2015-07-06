#include "IADefend.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include "Logic/Entity/Components/LifeEnemy.h"

namespace Logic 
{
	IMP_FACTORY(CIADefend);
	 
	bool CIADefend::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxTimeDefend"))
			_maxTimeDefend = entityInfo->getFloatAttribute("maxTimeDefend");

		if(entityInfo->hasAttribute("rangeDefend"))
			_rangeDefend = entityInfo->getFloatAttribute("rangeDefend");

		if(entityInfo->hasAttribute("sucessPercentDefend"))
			_successPercent = entityInfo->getIntAttribute("sucessPercentDefend");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIADefend::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxTimeDefend"))
			_maxTimeDefend = entityInfo->getFloatAttribute("maxTimeDefend");

		if(entityInfo->hasAttribute("rangeDefend"))
			_rangeDefend = entityInfo->getFloatAttribute("rangeDefend");

		if(entityInfo->hasAttribute("sucessPercentDefend"))
			_successPercent = entityInfo->getIntAttribute("sucessPercentDefend");

		return true;
	}

	//---------------------------------------------------------

	CIADefend::~CIADefend()
	{
		
	} // ~Controller

	bool CIADefend::activate()
	{
		_currentTimeDefend = 0;
		_initIA = false;
		_target = CServer::getSingletonPtr()->getPlayer();

		return true;

	} // activate

	void CIADefend::deactivate()
	{
		
	} // deactivate

	bool CIADefend::onAwake()
	{
		_currentTimeDefend = 0;
		_initIA = false;

		return true;
	}

	bool CIADefend::onSleep()
	{
		return true;
	}

	bool CIADefend::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "DAMAGED" ||
			   message->getType() == "CHANGE_TARGET" ||
			   message->getType() == "CAN_IA";
	}

	void CIADefend::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "DAMAGED")
		{
			// Si no está esquivando....
			if (!_initIA)
			{
				// Comprobamos primero si el enemigo está cerca del personaje
				Vector3 dist = _target->getCenterPosition() - _entity->getCenterPosition();
				float distLength = dist.length();

				if (distLength < _rangeDefend)
				{
					// Comprobamos si el aleatorio es un % viable
					int random = rand()% 101;

					if (random <= _successPercent)
					{
						_initIA = true;
					}
				}

				// Si sigue sin activarse la IA (es decir, no se cumplen las condiciones), restamos vida al enemigo.
				if (!_initIA)
				{
					CLifeEnemy* compLife = (CLifeEnemy*)_entity->getComponent("CLifeEnemy");

					if (!compLife->getInvincible())
					{	
						compLife->damageEntity(dynamic_cast<DAMAGED*>(message.get())->getFloat(),
							dynamic_cast<DAMAGED*>(message.get())->getString());
					}
				}
			}
		}

		else if (message->getType() == "CHANGE_TARGET")
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

		else if (message->getType() == "CAN_IA")
		{
			_initIA  = dynamic_cast<CAN_IA*>(message.get())->getBool();
		}

	} // process

	void CIADefend::onTick(unsigned int msecs)
	{
		if (_initIA)
		{
			_currentTimeDefend += msecs;

			std::shared_ptr<INVINCIBLE> m(new INVINCIBLE());
			m->setBool(true);
			_entity->emitMessage(m);

			if (_currentTimeDefend >= _maxTimeDefend) 
			{
				std::shared_ptr<INVINCIBLE> m(new INVINCIBLE());
				m->setBool(false);
				_entity->emitMessage(m);

				_currentTimeDefend = 0;
				_initIA = false;
			}
		}

	} // tick

} // namespace Logic

