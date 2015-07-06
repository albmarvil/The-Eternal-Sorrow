#include "SwordSlashController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Components/PhysicEntity.h"
#include "Application/BaseApplication.h"
#include "MovableText.h"

namespace Logic 
{
	IMP_FACTORY(CSwordSlashController);
	
	//---------------------------------------------------------

	bool CSwordSlashController::OnSpawn(const Map::CEntity *entityInfo) 
	{

		if(_entity->getEntityInfo()->hasAttribute("speed"))
		{
			_speed = _entity->getEntityInfo()->getFloatAttribute("speed");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CSwordSlashController::respawn(const Map::CEntity *entityInfo)
	{	

		if(_entity->getEntityInfo()->hasAttribute("speed"))
		{
			_speed = _entity->getEntityInfo()->getFloatAttribute("speed");
		}

		return true;
	}

	bool CSwordSlashController::activate()
	{
		_damage = 0;
		_direction = 0;

		_physicComponent =(CPhysicEntity*) _entity->getComponent("CPhysicEntity");

		assert(_physicComponent);

		return true;
	}

	//---------------------------------------------------------

	bool CSwordSlashController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "TOUCHED";
	} // accept
	
	//---------------------------------------------------------

	void CSwordSlashController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if((message->getType().compare("TOUCHED") == 0))
		{
			Logic::CEntity *otherEntity = dynamic_cast<TOUCHED*>(message.get())->getEntidad();

			if(otherEntity)
			{
				if (otherEntity->getTag() == "enemy")
				{
					if(_damage>0)
					{
						std::shared_ptr<DAMAGED> m(new DAMAGED());
						m->setFloat(_damage);
						m->setString(_entity->getType());
						otherEntity->emitMessage(m);

						std::shared_ptr<PUSH> pushMessage(new PUSH());
						pushMessage->setDirection(_direction);
						pushMessage->setSpeed(0.5f);
						otherEntity->emitMessage(pushMessage, this);

						//BaseSubsystems::Log::Debug("ÑAPA: " + std::to_string(_damage));
					}

					Logic::CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoEnemy",
						otherEntity->getCenterPosition(), 
						_entity->getMap());
				}
			}
		}
	} // process

	//---------------------------------------------------------

	void CSwordSlashController::setDirection(Vector3 posDest)
	{
		_direction = (posDest - _entity->getPosition()) * Vector3(1,0,0);
		_direction.normalise();


		///por defecto el slash esta horientado a la izq, si esta mirando a derechas lo variamos a derechas
		/*if(_direction.x >= 0)
		{
			Matrix4 transform = _entity->getTransform();
			Math::rotate(Vector3(0,0,1), Math::PI, transform);
			_physicComponent->kinematicMove(Vector3(0,0,0), transform);
		}*/
	}

	//--------------------------------------------------------

	void CSwordSlashController::onTick(unsigned int msecs)
	{
	
		Vector3 vel = _speed * _direction * msecs; 


		_physicComponent->kinematicMove(vel);
	}

	void CSwordSlashController::setSlashConfiguration(Vector3 direction, float damage)
	{
		setDirection(direction);
		_damage = damage;
	}

} // namespace Logic

