#include "BulletController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Components/PhysicEntity.h"
#include "Application/BaseApplication.h"
#include "MovableText.h"

namespace Logic 
{
	IMP_FACTORY(CBulletController);
	
	//---------------------------------------------------------

	bool CBulletController::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_touch = false;

		if(_entity->getEntityInfo()->hasAttribute("speed"))
		{
			_speed = _entity->getEntityInfo()->getFloatAttribute("speed");
		}

		if(_entity->getEntityInfo()->hasAttribute("rotateBullet"))
		{
			_rotateBullet = _entity->getEntityInfo()->getFloatAttribute("rotateBullet");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CBulletController::respawn(const Map::CEntity *entityInfo)
	{	
		_touch = false;

		if(_entity->getEntityInfo()->hasAttribute("speed"))
		{
			_speed = _entity->getEntityInfo()->getFloatAttribute("speed");
		}

		if(_entity->getEntityInfo()->hasAttribute("rotateBullet"))
		{
			_rotateBullet = _entity->getEntityInfo()->getFloatAttribute("rotateBullet");
		}


		return true;
	}

	bool CBulletController::activate()
	{
		_damage = 0;
		_canMove = true;
		_direction = 0;

		_physicComponent =(CPhysicEntity*) _entity->getComponent("CPhysicEntity");

		assert(_physicComponent);

		return true;
	}

	//---------------------------------------------------------

	bool CBulletController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "TOUCHED" ||
			   message->getType() == "CAN_MOVE";
	} // accept
	
	//---------------------------------------------------------

	void CBulletController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if((message->getType().compare("TOUCHED") == 0) && (_touch == false))
		{
			Logic::CEntity *otherEntity = dynamic_cast<TOUCHED*>(message.get())->getEntidad();

			// Comprobamos si ha chocado con un tile (otherEntity = null) y si colisiona con ellos la eliminamos
			if(!otherEntity)
			{
				_touch = true;
				_tipoEntidadColisionada = "Tile";
			}
			else //O si ha chocado contra alguna entidad lógica "viva" EN este caso estamos suponiendo que solo hacemos daño a los enemigos
			{
				if(_damage>0)
				{
					/*comp->setColor("blanco");
					comp->setTimeToLive(0.5);
					comp->setScale(Vector3(5,5,1));
					comp->setSpeed(0.02);*/

					//ha chocado contra un objetivo, enviamos un mensaje de daño
					//BaseSubsystems::Log::Debug("BALA CHOCA CONTRA ENEMIGO");
					std::shared_ptr<DAMAGED> m(new DAMAGED());
					m->setFloat(_damage);
					m->setString(_entity->getType());
					otherEntity->emitMessage(m);


					/*std::shared_ptr<PUSH> p(new PUSH());
					p->setDirection(_direction);
					p->setSpeed(0.3);
					otherEntity->emitMessage(p, this);*/
					
					if (otherEntity->getTag() == "enemy" && _entity->getType() == "BulletDartCanibal")
					{
						std::shared_ptr<SET_ALTSTATE> n(new SET_ALTSTATE());
						n->setString("Canibal");
						otherEntity->emitMessage(n);
					}
				}
				
				_tipoEntidadColisionada = otherEntity->getTag();

				_touch = true;
			}

			 
		}

		else if (message->getType() == "CAN_MOVE")
		{
			_canMove = dynamic_cast<CAN_MOVE*>(message.get())->getBool();
		}
	} // process

	//---------------------------------------------------------

	void CBulletController::setDirection(Vector3 posDest)
	{
		_direction = (posDest - _entity->getPosition()) * Vector3(1,1,0);
		_direction.normalise();
	}

	//--------------------------------------------------------

	void CBulletController::onTick(unsigned int msecs)
	{
		if(_touch)
		{
			if(_tipoEntidadColisionada == "Tile" || _tipoEntidadColisionada == "objetoRompible")
			{
				Logic::CEntityFactory::getSingletonPtr()->createEntityByType("Chispa",_entity->getPosition(), _entity->getMap());
			}
			else if(_tipoEntidadColisionada == "enemy")
			{
				Logic::CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoEnemy",_entity->getPosition(), _entity->getMap());
			}
			else if(_tipoEntidadColisionada == "player")
			{
				Logic::CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoPlayer",_entity->getPosition(), _entity->getMap());
			}
			
			Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}
		else if (_canMove)
		{
			Vector3 vel = _speed * _direction * msecs; 

			/*std::shared_ptr<KINEMATIC_MOVE> m(new KINEMATIC_MOVE());
			m->setVector3(vel);
			_entity->emitMessage(m);*/

			_physicComponent->kinematicMove(vel);
		}
	}

	void CBulletController::setBulletConfiguration(Vector3 direction, float damage)
	{
		setDirection(direction);
		_damage = damage;
	}

} // namespace Logic

