/**
@file LifeEnemy.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLifeEnemy
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "LifeEnemy.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "LUA/ScriptManager.h"
#include "Logic/Entity/Components/BarraLifeController.h"
#include "MovableText.h"
#include "Logic/Maps/Map.h"
#include "AltStatesController.h"

namespace Logic 
{
	IMP_FACTORY(CLifeEnemy);
	
	//---------------------------------------------------------

	bool CLifeEnemy::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_life = _lifeMax = 0;

		if(entityInfo->hasAttribute("vida"))
			_life = _lifeMax = entityInfo->getFloatAttribute("vida");

		if(entityInfo->hasAttribute("tier"))
			_tier = entityInfo->getIntAttribute("tier");

		_level = 1;
		if(entityInfo->hasAttribute("level"))
			_level = entityInfo->getIntAttribute("level");

		/*BaseSubsystems::Log::Debug("Vida enemigo "+_entity->getName()+": "+std::to_string(_life));
		BaseSubsystems::Log::Debug("Tier enemigo "+_entity->getName()+": "+std::to_string(_tier));
		BaseSubsystems::Log::Debug("level enemigo "+_entity->getName()+": "+std::to_string(_level));*/

		return true;

	} // Onspawn
	
	//---------------------------------------------------------

	bool CLifeEnemy::respawn(const Map::CEntity *entityInfo) 
	{
		_life = _lifeMax = 0;
		//iniciamos la vida
		if(entityInfo->hasAttribute("vida"))
			_life = _lifeMax = entityInfo->getFloatAttribute("vida");

		if(entityInfo->hasAttribute("tier"))
			_tier = entityInfo->getIntAttribute("tier");

		_level = 1;
		if(entityInfo->hasAttribute("level"))
			_level = entityInfo->getIntAttribute("level");


		/*BaseSubsystems::Log::Debug("Vida enemigo "+_entity->getName()+": "+std::to_string(_life));
		BaseSubsystems::Log::Debug("Tier enemigo "+_entity->getName()+": "+std::to_string(_tier));
		BaseSubsystems::Log::Debug("level enemigo "+_entity->getName()+": "+std::to_string(_level));*/

		return true;

	} // respawn
	
	//---------------------------------------------------------

	void CLifeEnemy::damageEntity(float damage , const std::string &objetoAsesino )
	{	
		if(_lifeBar && _life > 0)
		{
			_life -= damage;
			//BaseSubsystems::Log::Debug(_entity->getName()+" Vida: "+ std::to_string(_life));
			_lifeBar->updateLifeBar(_life, _lifeMax);

			CEntity* entityMovableText = Logic::CEntityFactory::getSingletonPtr()->createEntityByType("MovableTextDamage", Vector3(_entity->getCenterPosition().x, _entity->getCenterPosition().y, 40), _entity->getMap());
			CMovableText2 *comp = entityMovableText->getComponent<CMovableText2*>();
			comp->setText(std::to_string((int)damage));
			comp->setColor("blanco");

			if (_life <= 0) 
			{
				armaAsesina(objetoAsesino);

				if(_entity->getComponent<CAltStatesController*>() != NULL)
				{
					//Ejecutamos esto, para saber en que estado muere el enemigo
					_entity->getComponent<CAltStatesController*>()->enemyDie();
				}

				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
				
				CEntityFactory::getSingletonPtr()->createEntityByType("HumoMuerteEnemigo",_entity->getCenterPosition(), _entity->getMap());
				_dead = true;
				//BaseSubsystems::Log::Debug(_entity->getName() + " ha muerto.");
				int id = _entity->getEntityID();
				
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "EnemyDie", _tier, _level, id, _entity->getPosition());
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("Drop","EnemyDie", _entity->getType(), _level, _entity->getPosition());
			}
		}
	}

	void CLifeEnemy::exploteKami()
	{
		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
				
		CEntityFactory::getSingletonPtr()->createEntityByType("ExplosionAranha",_entity->getCenterPosition(), _entity->getMap());
		Logic::CEntity* cam = _entity->getMap()->getEntityByType("Camera");

		std::shared_ptr<Logic::SHAKE> m(new Logic::SHAKE());
		m->setMaxShake(8.0f);
		m->setMinShake(2.0f);
		cam->emitMessage(m);
		_dead = true;

		//BaseSubsystems::Log::Debug(_entity->getName() + " ha muerto.");
		int id = _entity->getEntityID();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("Drop","EnemyDie", _entity->getType(), _level, _entity->getPosition());
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "EnemyDie", _tier, _level, id, _entity->getPosition());
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "PosLastEnemyDied", _entity->getPosition());
	}

	void CLifeEnemy::healEntity(float heal)
	{
		_life += heal;

		if (_life >= _lifeMax)
		{
			_life = _lifeMax;
		}

		_lifeBar->updateLifeBar(_life, _lifeMax);

	}

	void CLifeEnemy::armaAsesina(const std::string &objeto)
	{
		if(objeto.compare("GreenSwordSlash") == 0 || objeto.compare("GreenSword") == 0)
		{
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","MatarEnemigosEspada");
		}
		else if(objeto.compare("Shield") == 0 || objeto.compare("ShieldSpecial") == 0)
		{
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","MatarEnemigosEscudo");
		}
		if(objeto.compare("Tongue") == 0)
		{
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","MatarEnemigosLatigo");
		}
		if(objeto.compare("Laser") == 0 || objeto.compare("LaserBall") == 0)
		{
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","MatarEnemigosLaser");
		}
		if(objeto.compare("BulletDart") == 0 || objeto.compare("BulletDartCanibal") == 0 || objeto.compare("BulletDartCanibalSpecial") == 0)
		{
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","MatarEnemigosLanzadardos");
		}
	}
} // namespace Logic

