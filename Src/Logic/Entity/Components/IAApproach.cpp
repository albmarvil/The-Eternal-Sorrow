#include "IAApproach.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include "AIIntermediate.h"

namespace Logic 
{
	IMP_FACTORY(CIAApproach);
	 
	bool CIAApproach::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("accelMovApproach"))
		{
			_accelMov = entityInfo->getFloatAttribute("accelMovApproach");
		}

		if(entityInfo->hasAttribute("maxVelApproach"))
		{
			_maxVel = entityInfo->getFloatAttribute("maxVelApproach");
		}

		if(entityInfo->hasAttribute("minDistSlowdownApproach"))
		{
			_minDistSlowdown = entityInfo->getFloatAttribute("minDistSlowdownApproach");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAApproach::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("accelMov"))
		{
			_accelMov = entityInfo->getFloatAttribute("accelMov");
		}

		if(entityInfo->hasAttribute("maxVelApproach"))
		{
			_maxVel = entityInfo->getFloatAttribute("maxVelApproach");
		}

		if(entityInfo->hasAttribute("minDistSlowdownApproach"))
		{
			_minDistSlowdown = entityInfo->getIntAttribute("minDistSlowdownApproach");
		}

		return true;
	}

	//---------------------------------------------------------

	bool CIAApproach::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CAN_MOVE" ||
			   message->getType() == "RUN_AWAY" ||
			   message->getType() == "CHANGE_TARGET" ||
			   message->getType() == "CAN_IA";
	} // accept

	//---------------------------------------------------------

	void CIAApproach::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CAN_MOVE")
		{
			_canMove = dynamic_cast<CAN_MOVE*>(message.get())->getBool();
		}

		else if (message->getType() == "RUN_AWAY")
		{
			_runningAway =  dynamic_cast<CAN_MOVE*>(message.get())->getBool(); // AUN SIN USAR
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
			_canIA = dynamic_cast<CAN_IA*>(message.get())->getBool();
		}
	} // process

	//---------------------------------------------------------

	CIAApproach::~CIAApproach()
	{
		
	} // ~Controller

	bool CIAApproach::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_velMov = 0;
		_canMove = true;
		_canIA = true;

		if (_target->getCenterPosition().x > _entity->getCenterPosition().x)
			_direction = 1;
		else
			_direction = -1;

		std::shared_ptr<Logic::TURN> m(new Logic::TURN());
		m->setInt(_direction);
		_entity->emitMessage(m);

		_isSlowdown = false;
		_runningAway = false;

		_entity->getComponent<CAIIntermediate*>()->addNewMovement(SteeringMovement::SeekSingleDir,this);
		
		return true;

	} // activate

	bool CIAApproach::onAwake()
	{
		_velMov = 0;
		_canMove = true;
		_canIA = true;
		_target = CServer::getSingletonPtr()->getPlayer();

		if (_target->getCenterPosition().x > _entity->getCenterPosition().x)
			_direction = 1;
		else
			_direction = -1;

		std::shared_ptr<Logic::TURN> m(new Logic::TURN());
		m->setInt(_direction);
		_entity->emitMessage(m);

		_isSlowdown = false;
		_runningAway = false;

		return true;
	}

	void CIAApproach::deactivate()
	{

	} // deactivate

	
	void CIAApproach::onTick(unsigned int msecs)
	{
		

	} // tick

	Vector3 CIAApproach::calculateVelocity(unsigned int msecs, float &velMax)
	{
		velMax = _maxVel;
		if (_canMove && _canIA)
		{
			// Si la distancia en x que separa al player del enemigo es menor que la estipulada, frenamos
			Vector3 dist = (_target->getCenterPosition() - _entity->getCenterPosition()) * Vector3(1,0,0);
			float distLength = dist.squaredLength();

			if (distLength <= _minDistSlowdown)
			{
				_isSlowdown = true;
			}

			// No estamos frenando
			if(!_isSlowdown)
			{
				//Aceleracion de comienzo: 0.0002
				_accelMov = 0.0002;

				// Si estamos más lejos del rango de frenado, andamos
				if (_entity->getCenterPosition().x < _target->getCenterPosition().x)
				{
					if (_direction == -1) // Con esto enviará solo el mensaje cuando cambie de dirección y no siempre
					{
						std::shared_ptr<Logic::TURN> m(new Logic::TURN());
						m->setInt(1);
						_entity->emitMessage(m);
					}

					_direction = 1;
				}
				else
				{
					if (_direction == 1) // Con esto enviará solo el mensaje cuando cambie de dirección y no siempre
					{
						std::shared_ptr<Logic::TURN> m(new Logic::TURN());
						m->setInt(-1);
						_entity->emitMessage(m);
					}

					_direction = -1;
				}
			}

			// Frenamos
			else
			{
				//Aceleracion de frenado = -0.0001
				_accelMov = -0.0001;

				_isSlowdown = false;
			}
		}

		else
		{
			return Vector3::ZERO;
		}

		
		return Vector3(_direction*_accelMov, 0,0);
	}

} // namespace Logic

