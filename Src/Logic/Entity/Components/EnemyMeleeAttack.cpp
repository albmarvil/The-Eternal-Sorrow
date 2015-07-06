/**
@file EnemyMeleeAttack.cpp

Contiene la declaración del componente que controla el ataque melee de los enemigos

@see Logic::CEnemyMeleeAttack
@see Logic::IComponent

@author Alberto Martínez
@date Febrero, 2015
*/

#include "EnemyMeleeAttack.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Physics/Server.h"
#include "LUA/ScriptManager.h"

#include "Logic/Server.h"
#include "LifeEnemy.h"

namespace Logic 
{
	IMP_FACTORY(CEnemyMeleeAttack);
	
	//---------------------------------------------------------

	bool CEnemyMeleeAttack::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("ataque"))
		{
			_damage = entityInfo->getFloatAttribute("ataque");
		}
		//Inicializamos los datos
		if(entityInfo->hasAttribute("vel_ataque"))
		{
			_timeBetweenAttacks = entityInfo->getFloatAttribute("vel_ataque");
		}

		_upOffset = 0.0f;
		if(entityInfo->hasAttribute("physic_height"))
		{
			_upOffset = entityInfo->getFloatAttribute("physic_height");
		}

		_maxDist = 1.0f;
		if(entityInfo->hasAttribute("physic_radius"))
		{
			_maxDist = entityInfo->getFloatAttribute("physic_radius");
		}

		//BaseSubsystems::Log::Debug("VelAtaque enemigo "+_entity->getName()+": "+std::to_string(_timeBetweenAttacks));
		//BaseSubsystems::Log::Debug("Ataque enemigo "+_entity->getName()+": "+std::to_string(_damage));
		return true;

	} // Onspawn
	
	//---------------------------------------------------------

	bool CEnemyMeleeAttack::respawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("ataque"))
		{
			_damage = entityInfo->getFloatAttribute("ataque");
		}

		if(entityInfo->hasAttribute("vel_ataque"))
		{
			_timeBetweenAttacks = entityInfo->getFloatAttribute("vel_ataque");
		}

		_upOffset = 0.0f;
		if(entityInfo->hasAttribute("physic_height"))
		{
			_upOffset = entityInfo->getFloatAttribute("physic_height");
		}
		_maxDist = 1.0f;
		if(entityInfo->hasAttribute("physic_radius"))
		{
			_maxDist = entityInfo->getFloatAttribute("physic_radius");
		}

		//BaseSubsystems::Log::Debug("VelAtaque enemigo "+_entity->getName()+": "+std::to_string(_timeBetweenAttacks));
		//BaseSubsystems::Log::Debug("Ataque enemigo "+_entity->getName()+": "+std::to_string(_damage));
		return true;

	} // Onspawn
	
	//---------------------------------------------------------

	bool CEnemyMeleeAttack::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CONTROLLER_TOUCHED" ||
			   message->getType() == "CHANGE_TARGET";

	} // accept
	
	//---------------------------------------------------------

	void CEnemyMeleeAttack::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "CONTROLLER_TOUCHED")
		{
			CEntity* other = dynamic_cast<CONTROLLER_TOUCHED*>(message.get())->getEntidad();

			if(other->getEntityID() == _targetID && _entity->getTag() == "enemy" && canDoDamage())
			{
				ApplyDamage(other);
			}
			
		}

		else if (message->getType() == "CHANGE_TARGET")
		{
			// Si no encuentra la palabra "Minion" en el tipo de entidad, entonces puede canibalizar
			if (_entity->getType().find("Minion") == std::string::npos)
			{
				_target =  dynamic_cast<CHANGE_TARGET*>(message.get())->getLogicEntity(); 

				if (_target)
					_targetID = _target->getEntityID();
			}
		}

	} // process


	void CEnemyMeleeAttack::onTick(unsigned int msecs)
	{
		//BaseSubsystems::Log::Debug(std::to_string(_count)+" - "+_entity->getName() + " Acumulando tiempo de ataque: "+std::to_string(_timeAcum)+" / "+std::to_string(_timeBetweenAttacks));
		_timeAcum += msecs;
		if(canDoDamage())
		{
			Vector3 dir = _target->getPosition() - _entity->getPosition();

			if( dir.squaredLength() <= _maxDist2)
			{
				Vector3 offsetPos = _entity->getPosition();
				offsetPos.y += _upOffset;
				CEntity* ent;

				Vector3 collisionPoint;

				if (_target->getTag() == "player")
				{
					ent = Physics::CServer::getSingletonPtr()->raycastSimple(offsetPos, dir, _maxDist ,Physics::CollisionGroup::ePlayer,collisionPoint);

					if (ent)
					{
						ApplyDamage(ent);

						//Instanciamos la chispa en el punto de colision
						CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoPlayer", collisionPoint, _entity->getMap());
						damageEnemy(collisionPoint);
					
					}

				}
				else if (_target->getTag() == "enemy")
				{
					std::vector<Vector3> pointsCollision;
					Physics::FilterMask mask = Physics::FilterMask(Physics::FilterMask::eEnemyTMask |Physics::FilterMask::eEnemyVMask);
					std::vector<Logic::CEntity*> vecLEnt = Physics::CServer::getSingletonPtr()->raycastMultiple(offsetPos, dir, _maxDist, 64, mask,pointsCollision);
					
					for (int i = 0; i < vecLEnt.size(); ++i)
					{
						if (vecLEnt.at(i)->getEntityID() == _targetID)
						{
							ApplyDamage(vecLEnt.at(i));

							//Generamos particula de choque
							CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoEnemy", pointsCollision.at(i), _entity->getMap());
							break;
						}
					}
				}
			}
		}

	}//tick


	bool CEnemyMeleeAttack::activate()
	{
		//asignamos un valor alto para poder hacer daño desde el inicio
		_timeAcum = 0;
		//_count = false;
		//_left = Vector3(-1, 0, 0);
		//_right = Vector3 (1, 0, 0);
		
		_maxDist2 = (_maxDist*2) * (_maxDist*2);
		_target = CServer::getSingletonPtr()->getPlayer();
		_targetID = _target->getEntityID();
		return true;
	}

	void CEnemyMeleeAttack::deactivate()
	{
		_timeAcum = 0.0f;
	}

	bool CEnemyMeleeAttack::canDoDamage()
	{
		return _timeAcum >= _timeBetweenAttacks;
	}

	void CEnemyMeleeAttack::ApplyDamage(CEntity* entity)
	{
		std::shared_ptr<DAMAGED> m(new DAMAGED());
		m->setFloat(_damage);
		entity->emitMessage(m);
		_timeAcum = 0.0f;
		//_count = true;
		

		//Cuando hacemos daño al player, miramos tambien si el player tiene la habilidad de hacer daño al enemigo cuando este lo embiste
		//int damageToApply = LUA::CScriptManager::GetPtrSingleton()->getField("player_skills","damageEnemigoMelee",0);

		//Cuando hacemos daño al player miramos si éste tiene la habilidad de hacer daño al enemigo. Para ello cogemos el contador
		//de la Habilidad CDamageMeleeEnemy, si el player no tiene esta habilidad entonces devolverá 0, por el contrario si la ha
		//conseguido devolverá el contador de la tabla de habilidades
		
	}

	void CEnemyMeleeAttack::damageEnemy(Vector3 collisionPoint)
	{
		int damageToApply;
		std::string nombre = LUA::CScriptManager::GetPtrSingleton()->getField("Nombre_Habilidad", "CDamageMeleeEnemy", "");
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("HadHability", nombre, damageToApply);

		if(damageToApply>0)
		{
			_entity->getComponent<CLifeEnemy*>()->damageEntity(damageToApply, "damageMeleeEnemy");

			CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoEnemy", collisionPoint, _entity->getMap());
		}
	}

} // namespace Logic

