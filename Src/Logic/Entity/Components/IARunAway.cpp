#include "IARunAway.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"

namespace Logic 
{
	IMP_FACTORY(CIARunAway);
	 
	bool CIARunAway::OnSpawn(const Map::CEntity *entityInfo)
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

	bool CIARunAway::respawn(const Map::CEntity *entityInfo)
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

	bool CIARunAway::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		
		return message->getType() == "CHANGE_TARGET";
	} // accept

	//---------------------------------------------------------

	void CIARunAway::process(const std::shared_ptr<Logic::IMessage> &message)
	{

		if (message->getType() == "CHANGE_TARGET")
		{
			CEntity* ent = dynamic_cast<CHANGE_TARGET*>(message.get())->getLogicEntity();
			if(ent != NULL)
			{
				if(!ent->getIsDead())
				{
					_target = ent; 
					if (_target->getCenterPosition().x > _entity->getCenterPosition().x)
					{
						_direction = -1;
					}
					else
					{
						_direction = 1;
					}
						
					std::shared_ptr<Logic::TURN> m(new Logic::TURN());
					m->setInt(_direction);
					_entity->emitMessage(m);
				}
			}  

			
		}
	} // process

	//---------------------------------------------------------

	CIARunAway::~CIARunAway()
	{
		
	} // ~Controller

	bool CIARunAway::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_velMov = 0;

		if (_target->getCenterPosition().x > _entity->getCenterPosition().x)
			_direction = -1;
		else
			_direction = 1;

		std::shared_ptr<Logic::TURN> m(new Logic::TURN());
		m->setInt(_direction);
		_entity->emitMessage(m);

		_isSlowdown = false;

		return true;

	} // activate

	bool CIARunAway::onAwake()
	{
		_velMov = 0;

		_target = CServer::getSingletonPtr()->getPlayer();

		if (_target->getCenterPosition().x > _entity->getCenterPosition().x)
			_direction = -1;
		else
			_direction = 1;

		std::shared_ptr<Logic::TURN> m(new Logic::TURN());
		m->setInt(_direction);
		_entity->emitMessage(m);

		_isSlowdown = false;

		return true;
	}

	void CIARunAway::deactivate()
	{

	} // deactivate

	
	void CIARunAway::onTick(unsigned int msecs)
	{
		// No estamos frenando
		if(!_isSlowdown)
		{
			// Si la distancia en x que separa al player del enemigo es menor que la estipulada, frenamos
			Vector3 dist = (_target->getCenterPosition() - _entity->getCenterPosition()) * Vector3(1,0,0);
			float distLength = dist.squaredLength();

			if (distLength >= _minDistSlowdown)
			{
				_isSlowdown = true;
			}

			// Si estamos más cerca del rango de frenado, andamos
			else
			{
				if (_entity->getCenterPosition().x < _target->getCenterPosition().x)
				{
					if (_direction == 1) // Con esto enviará solo el mensaje cuando cambie de dirección y no siempre
					{
						std::shared_ptr<Logic::TURN> m(new Logic::TURN());
						m->setInt(-1);
						_entity->emitMessage(m);
					}

					_direction = -1;
				}
				else
				{
					if (_direction == -1) // Con esto enviará solo el mensaje cuando cambie de dirección y no siempre
					{
						std::shared_ptr<Logic::TURN> m(new Logic::TURN());
						m->setInt(1);
						_entity->emitMessage(m);
					}

					_direction = 1;
				}

				// Si no hemos alcanzado la velocidad máxima, incrementamos la actual
				if(abs(_velMov) < _maxVel) 
				{
					_velMov += _accelMov * _direction * msecs;
				}
				else // Si la hemos alcanzado, establecemos la actual como la máxima
				{
					_velMov = _direction * _maxVel;
				}
			}
		}

		// Frenamos
		else
		{
			_velMov -= _accelMov * _direction* msecs;

			if(_velMov * _direction <= 0) 
			{
				_velMov = 0;
				_isSlowdown = false;
			}
		}
		
		// Aplicamos resultados
		Vector3 resultMovement = Vector3( _velMov * msecs, 0, 0 );

		//La  direccion ya viene normalizada		
		std::shared_ptr<AVATAR_WALK> m(new AVATAR_WALK());
		m->setVector3(resultMovement);
		_entity->emitMessage(m);

	} // tick

} // namespace Logic

