#include "IAWander.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include <time.h>
#include "AIIntermediate.h"

namespace Logic 
{
	IMP_FACTORY(CIAWander);
	 
	bool CIAWander::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxTimeWait"))
			_maxTimeWaitBase = entityInfo->getIntAttribute("maxTimeWait");

		if(entityInfo->hasAttribute("maxTimeWander"))
			_maxTimeWanderBase = entityInfo->getIntAttribute("maxTimeWander");

		if(entityInfo->hasAttribute("maxVelWander"))
			_maxVelWander = entityInfo->getFloatAttribute("maxVelWander");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAWander::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxTimeWait"))
			_maxTimeWaitBase = entityInfo->getIntAttribute("maxTimeWait");

		if(entityInfo->hasAttribute("maxTimeWander"))
			_maxTimeWanderBase = entityInfo->getIntAttribute("maxTimeWander");

		if(entityInfo->hasAttribute("maxVelWander"))
			_maxVelWander = entityInfo->getFloatAttribute("maxVelWander");

		return true;
	}

	//---------------------------------------------------------

	bool CIAWander::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "SIDE_COLLISION"; // Contemplar tambien para falling y para up_collision (voladores)
 	} // accept

	//---------------------------------------------------------

	void CIAWander::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "SIDE_COLLISION")
		{
			if (_delaySideCollision > 1000)
			{
				_isTouchingSideTile = true;
				_timeWander = _maxTimeWanderBase; // iniciar wait (o stop y luego wait)
				_delaySideCollision = 0;

			}
			else 
				_isTouchingSideTile = false;
		}

	} // process

	//---------------------------------------------------------

	CIAWander::~CIAWander()
	{
		
	} // ~Controller

	bool CIAWander::activate()
	{
		_timeWait = 0;
		_timeWander = 0;
		_direction = 1;
		_velMovement = 0;
		_maxTimeWanderNow = 0;
		_maxTimeWaitNow = 0;
		_delaySideCollision = 0;

		_isWaiting = true;
		_dirStablished = false;
		srand(time(NULL));

		_entity->getComponent<CAIIntermediate*>()->addNewMovement(SteeringMovement::Wander,this);

		return true;

	} // activate

	void CIAWander::deactivate()
	{
	} // deactivate

	bool CIAWander::onAwake()
	{
		_timeWait = 0;
		_timeWander = 0;
		_direction = 1;
		_velMovement = 0;
		_maxTimeWanderNow = 0;
		_maxTimeWaitNow = 0;
		_delaySideCollision = 0;

		_isWaiting = true;
		_dirStablished = false;
		srand(time(NULL));

		return true;
	}

	void CIAWander::onTick(unsigned int msecs)
	{
		
	} // tick

	Vector3 CIAWander::calculateVelocity(unsigned int msecs,float &velMax)
	{
		bool enviar = false;
		velMax = _maxVelWander;
		if (_isWaiting)
		{
			_timeWait += msecs;
			_delaySideCollision += msecs;

			if (_timeWait >= _maxTimeWaitNow)
			{

				_isWaiting = false;
				_timeWait = 0;
			}
		}

		else
		{
			if (!_dirStablished)
			{
				if (_isTouchingSideTile)
				{
					_direction.x *= -1;
				}
				else
				{
					int randDir = rand() % 2;

					if (randDir == 1)
						_direction.x = -1;
					else
						_direction.x = 1;
				}

				int randTimeWander = rand() % 71 + 10;
				float percentRand = randTimeWander / 100.f;

				_maxTimeWanderNow = _maxTimeWanderBase * percentRand;
				
				_dirStablished = true;
			}

			_timeWander += msecs;

			enviar = true;

			if (_timeWander >= _maxTimeWanderNow) 
			{

				_isWaiting = true;
				_dirStablished = false;
				_timeWander = 0;

				int randTimeWait = rand() % 71 + 10;
				float percentRand = randTimeWait / 100.f;

				_maxTimeWaitNow = _maxTimeWaitBase * percentRand;
			}
		}

		if (_direction.x == 1)
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

		//Aceleracion Wander = 0.0002
		if(enviar)
			return _direction * 0.0002;

		return Vector3::ZERO;
	}
} // namespace Logic

