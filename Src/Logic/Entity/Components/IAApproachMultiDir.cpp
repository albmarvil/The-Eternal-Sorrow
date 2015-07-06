#include "IAApproachMultiDir.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include "AIIntermediate.h"

namespace Logic 
{
	IMP_FACTORY(CIAApproachMultiDir);
	 
	bool CIAApproachMultiDir::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("accelMovApproach"))
		{
			_accelMov = entityInfo->getFloatAttribute("accelMovApproach");
		}

		if(entityInfo->hasAttribute("maxVelApproach"))
		{
			_velMov = entityInfo->getFloatAttribute("maxVelApproach");
		}

		if(entityInfo->hasAttribute("minDistSlowdownApproach"))
		{
			_minDistSlowdown = entityInfo->getIntAttribute("minDistSlowdownApproach");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAApproachMultiDir::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("accelMovApproach"))
		{
			_accelMov = entityInfo->getFloatAttribute("accelMovApproach");
		}

		if(entityInfo->hasAttribute("maxVelApproach"))
		{
			_velMov = entityInfo->getFloatAttribute("maxVelApproach");
		}

		if(entityInfo->hasAttribute("minDistSlowdownApproach"))
		{
			_minDistSlowdown = entityInfo->getIntAttribute("minDistSlowdownApproach");
		}

		return true;
	}

	//---------------------------------------------------------

	bool CIAApproachMultiDir::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CAN_MOVE" ||
			message->getType() == "RUN_AWAY" ||
			message->getType() == "CAN_IA" ||
			message->getType() == "CHANGE_TARGET";

		return true;
	} // accept

	//---------------------------------------------------------

	void CIAApproachMultiDir::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CAN_MOVE")
		{
			_canMove = dynamic_cast<CAN_MOVE*>(message.get())->getBool();
		}

		else if (message->getType() == "RUN_AWAY")
		{
			_runningAway = dynamic_cast<CAN_MOVE*>(message.get())->getBool(); // AUN SIN USAR
		}

		else if (message->getType() == "CAN_IA")
		{
			_canMove = dynamic_cast<CAN_IA*>(message.get())->getBool();

			if (_canMove)
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

	CIAApproachMultiDir::~CIAApproachMultiDir()
	{
		
	} // ~Controller

	bool CIAApproachMultiDir::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_direction = 0;
		_velFinal = 0;
		_canMove = true;
		_runningAway = false;

		//Registro el movimiento en el intermediario
		_entity->getComponent<CAIIntermediate*>()->addNewMovement(SteeringMovement::SeekMultiDir,this);

		return true;

	} // activate

	bool CIAApproachMultiDir::onAwake()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_direction = 0;
		_velFinal = 0;
		_canMove = true;
		_runningAway = false;

		return true;
	}

	void CIAApproachMultiDir::deactivate()
	{

	} // deactivate

	
	void CIAApproachMultiDir::onTick(unsigned int msecs)
	{
		
	} // tick

	Vector3 CIAApproachMultiDir::calculateVelocity(unsigned int msecs, float &velMax)
	{
		velMax = _velMov;

		if (_canMove)
		{
			_direction = (_target->getCenterPosition() - _entity->getPosition()) * Vector3(1,1,0); // No nos interesa el z
			float distance = _direction.length();

			_direction.normalise();
			
			// Dirección a la que mirar
			if (_target->getCenterPosition().x >= _entity->getCenterPosition().x)
			{
				std::shared_ptr<Logic::TURN> m(new Logic::TURN());
				m->setInt(1);
				_entity->emitMessage(m);
			}

			else
			{
				std::shared_ptr<Logic::TURN> m(new Logic::TURN());
				m->setInt(-1);
				_entity->emitMessage(m);
			}

			//Aceleracion multiDir = 0.0002
			return _direction * Vector3(0.0002,0.0001,0);

		}

		

		return Vector3::ZERO;
	}

} // namespace Logic

