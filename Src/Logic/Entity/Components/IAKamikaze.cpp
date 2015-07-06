#include "IAKamikaze.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include "Logic/Maps/Map.h"
#include "LifeEnemy.h"
#include "ParticleController.h"

namespace Logic 
{
	IMP_FACTORY(CIAKamikaze);
	 
	bool CIAKamikaze::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("damageKamikaze"))
			_damageKamikaze = entityInfo->getFloatAttribute("damageKamikaze");

		if(entityInfo->hasAttribute("rangeKamikaze"))
			_rangeKamikaze = entityInfo->getIntAttribute("rangeKamikaze");

		if(entityInfo->hasAttribute("maxTimeKamikaze"))
			_maxTimeKamikaze = entityInfo->getIntAttribute("maxTimeKamikaze");


		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAKamikaze::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("damageKamikaze"))
			_damageKamikaze = entityInfo->getFloatAttribute("damageKamikaze");

		if(entityInfo->hasAttribute("rangeKamikaze"))
			_rangeKamikaze = entityInfo->getIntAttribute("rangeKamikaze");

		if(entityInfo->hasAttribute("maxTimeKamikaze"))
			_maxTimeKamikaze = entityInfo->getIntAttribute("maxTimeKamikaze");

		return true;
	}

	//---------------------------------------------------------

	bool CIAKamikaze::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CAN_IA" ||
			   message->getType() == "CHANGE_TARGET";
	} // accept

	//---------------------------------------------------------

	void CIAKamikaze::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CAN_IA")
		{
			_canIA = dynamic_cast<CAN_IA*>(message.get())->getBool();
			
			if (_canIA)
				setSleeping(false);
			else
				setSleeping(true);
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

	} // process

	//---------------------------------------------------------

	CIAKamikaze::~CIAKamikaze()
	{
		
	} // ~Controller

	bool CIAKamikaze::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_canIA = true;
		
		_clockKamikaze = 0;
		_bombActivated = false;

		return true;

	} // activate

	void CIAKamikaze::deactivate()
	{
		
	} // deactivate

	void CIAKamikaze::onTick(unsigned int msecs)
	{
		if (_canIA)
		{
			_distTarget = _target->getCenterPosition() - _entity->getCenterPosition();
			_distLength = _distTarget.length();

			if (_distLength <= _rangeKamikaze && !_bombActivated)
			{
				_bombActivated = true;

				_entity->getComponent<CParticleController*>()->setVisible(true);
			}

			if (_bombActivated)
			{	
				_clockKamikaze += msecs;

				if (_clockKamikaze >= _maxTimeKamikaze)
				{
					if (_distLength <= _rangeKamikaze)
					{
						float distPercent = (_distLength * 1) / _rangeKamikaze;
						int dmgTotal;

						if (distPercent <= 0.25) // Con un 25% d proximidad el daño es máximo
						{
							dmgTotal = _damageKamikaze;
						}
						else // Si no, cuanta mayor distancia menos daño inflinge
						{	
							dmgTotal= (1-distPercent) * _damageKamikaze;
						}

						//Si ha chocado contra un objetivo, enviamos un mensaje de daño
						std::shared_ptr<DAMAGED> m(new DAMAGED());
						m->setFloat(dmgTotal);
						_target->emitMessage(m);
					}

					_entity->getComponent<CLifeEnemy*>()->exploteKami();

					_bombActivated = false;
				}
			}
		}

	} // tick

} // namespace Logic

