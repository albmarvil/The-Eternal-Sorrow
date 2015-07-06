/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "AIIntermediate.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Physics/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "IAAvoid.h"
#include "IAApproachMultiDir.h"
#include "IAApproach.h"
#include "IAApproachPosition.h"
#include "IAWander.h"
#include "PhysicController.h"
namespace Logic
{
	IMP_FACTORY(CAIIntermediate);
	bool CAIIntermediate::OnSpawn(const Map::CEntity* entityInfo)
	{
		_velocity = Vector3::ZERO;
		_velocityMult = 1.f;
		_aceleration = Vector3::ZERO;

		return true;
	}

	bool CAIIntermediate::activate()
	{
		_controller = (CPhysicController*) _entity->getComponent("CPhysicController");
		assert(_controller);
		return true;
	}

	void CAIIntermediate::deactivate()
	{
		_vectorSteeringMovement.clear();
		_velocity = Vector3::ZERO;
		_velocityMult = 1.f;
	}

	bool CAIIntermediate::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "SET_VEL_MULT";

	} // accept

	//---------------------------------------------------------

	void CAIIntermediate::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_VEL_MULT") == 0)
		{
			_velocityMult = dynamic_cast<SET_VEL_MULT*>(message.get())->getFloat();

		}
	} // process

	void CAIIntermediate::onTick(unsigned int msecs)
	{
		float velMax = 0;
		bool canMove = true;

		std::map<int, Logic::IComponent* >::const_iterator it = _vectorSteeringMovement.begin();

		//Invocamos cada movimiento. LOS VALORES DE VELOCIDAD MAXIMA TENGO QUE PARAMETRIZAROS.

		for(; it!= _vectorSteeringMovement.end(); ++it)
		{
			switch((*it).first)
			{
			case SteeringMovement::Avoid:
				_aceleration += ((Logic::CIAAvoid*)((*it).second))->calculateVelocity(msecs);
				break;
			case SteeringMovement::SeekMultiDir:
				if(!((Logic::CIAApproachMultiDir*)((*it).second))->getSleeping())
				{
					_aceleration += ((Logic::CIAApproachMultiDir*)((*it).second))->calculateVelocity(msecs,velMax);
					canMove &= ((Logic::CIAApproachMultiDir*)((*it).second))->canMove();
				}
				break;
			case SteeringMovement::SeekSingleDir:
				if(!((Logic::CIAApproach*)((*it).second))->getSleeping())
				{
					_aceleration += ((Logic::CIAApproach*)((*it).second))->calculateVelocity(msecs,velMax);
					canMove &= ((Logic::CIAApproach*)((*it).second))->canMove();
				}

				break;
			case SteeringMovement::Wander:

				if(!((Logic::CIAWander*)((*it).second))->getSleeping())
				{
					_aceleration += ((Logic::CIAWander*)((*it).second))->calculateVelocity(msecs,velMax);
					//Si en el wander me devuelve que no hay aceleracion, implica que ya no queremos mas movimiento, por lo que
					//reiniciamos de nuevo la velocidad
					if(_aceleration == Vector3::ZERO)
					{
						_velocity = Vector3::ZERO;
					}
				}
				break;
			}
			
		}

		if(canMove)
		{
			_velocity += _aceleration * msecs; 
		
			velMax *= _velocityMult;

			//LIMITAMOS POR VELOCIDAD MAXIMA
			if(_velocity.x > velMax)
				_velocity.x = velMax;
			else if(_velocity.x < -velMax)
				_velocity.x = -velMax;

			if(_velocity.y > velMax/1.5)
				_velocity.y = velMax/1.5;
			else if(_velocity.y < -velMax/1.5)
				_velocity.y = -velMax/1.5;

			_velocity.z = 0;

			Vector3 movimiento = _velocity * _velocityMult * msecs;
	
			_controller->avatarWalk(movimiento);
		}
		else
		{
			_velocity = Vector3::ZERO;
			_controller->avatarWalk(_velocity);
		}

		_aceleration = Vector3::ZERO;
	}

	void CAIIntermediate::addNewMovement(int idMovement, Logic::IComponent* funcionCalculoVelocidad)
	{
		_vectorSteeringMovement[idMovement] = funcionCalculoVelocidad;

	}

}