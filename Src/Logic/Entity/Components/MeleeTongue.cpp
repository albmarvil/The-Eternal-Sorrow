#include "MeleeTongue.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Graphics/Entity.h"
#include "LUA/ScriptManager.h"
#include "Logic/Server.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Entity/Components/TongueBallController.h"
#include "Physics/Server.h"

namespace Logic 
{
	IMP_FACTORY(CMeleeTongue);
	
	/*bool CMeleeTongue::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("percentPoison"))
			_percentPoison = entityInfo->getIntAttribute("percentPoison");

		return true;
	}

	bool CMeleeTongue::respawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("percentPoison"))
			_percentPoison = entityInfo->getIntAttribute("percentPoison");

		return true;
	}*/

	bool CMeleeTongue::activate() 
	{
		if(!Weapon::activate())
		{
			return false;
		}

		_animEntity = (_entity->getComponent<CAnimatedGraphics*>())->getAnimatedEntity();
		_animEntity->setObserver(this, "AttackFront");

		_animActual = "Idle";

		_percentPoison = 60;
		_mousePos = 0;
		//Desactivo la física de la espada desde el inicio
		switchWeaponPhysic(false);

		return true;

	} // activate

	void CMeleeTongue::deactivate() 
	{
		Weapon::deactivate();

		_animEntity->removeObserver(this, "AttackFront");

		switchWeaponPhysic(false);

	} // deactivate

	bool CMeleeTongue::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return Weapon::accept(message) ||
			   message->getType() == "TOUCHED";

	} // accept
	
	//---------------------------------------------------------

	void CMeleeTongue::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		Weapon::process(message);

	} // process
	
	//---------------------------------------------------------------------------------

	void CMeleeTongue::mainAttack(Vector3 mousePos, float baseDamage)
	{
		_delayMainAtk = 0;

		_mousePos = mousePos;

		sendSET_ANIMATION("AttackFront", false, true, 1.0f);
		
		//Configuramos el daño final del ataque actual
		_actualDamage = baseDamage * _mainDamageMultiplier;

		switchWeaponPhysic(true);
	}
	
	void CMeleeTongue::specialAttack(Vector3 mousePos, float baseDamage)
	{
		_delaySpecialAtk = 0;

		CEntity* ball = CEntityFactory::getSingletonPtr()->createEntityByType("TongueBall", _entity->getCenterPosition(), _entity->getMap());
		ball->getComponent<CTongueBallController*>()->launch(mousePos);

	}
	//------------------------------------------------------------------------

	void CMeleeTongue::animationFinished(const std::string &animation)
	{
		// Aqui es donde dañaremos a los enemigos, cuando haya finalizado la animación
		if(animation.compare("AttackFront") == 0)
		{
			Vector3 dir = (_mousePos - _entity->getPosition()) * Vector3(1,1,0);
			dir.normalise();

			// Lanzamos un rayo multiple para alcanzar enemigos
			std::vector<Vector3> pointsCollision;
			Physics::FilterMask mask = Physics::FilterMask(Physics::FilterMask::eEnemyTMask |Physics::FilterMask::eEnemyVMask | Physics::FilterMask::eDoorMask | Physics::FilterMask::eTilesMask | Physics::FilterMask::ePropsMask);
			std::vector<Logic::CEntity*> vecLEnt = Physics::CServer::getSingletonPtr()->raycastMultiple(_entity->getPosition(), dir, 40, 64, mask, pointsCollision);

			int randPoison = rand() % 101;
			bool canPoison = false;

			if (randPoison >= _percentPoison)
			{
				canPoison = true;
			}

			for (int i = 0; i < vecLEnt.size(); ++i)
			{
				ApplyDamage(vecLEnt.at(i));

				if (canPoison && vecLEnt.at(i)->getTag() == "enemy")
				{
					std::shared_ptr<SET_ALTSTATE>m(new SET_ALTSTATE());
					m->setString("Poison");
					vecLEnt.at(i)->emitMessage(m);
				}

				//Generamos particula de choque
				CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoEnemy", pointsCollision.at(i), _entity->getMap());
				break;
			}

			//dejo de aplicar daño y desactivo el trigger
			switchWeaponPhysic(false);
			_actualDamage = 0.0f;

			sendSET_ANIMATION("AttackBack", false, true, 1.0f);
		}
	}

	void CMeleeTongue::ApplyDamage(CEntity* entity)
	{
		std::shared_ptr<DAMAGED> m(new DAMAGED());
		m->setFloat(_actualDamage);
		m->setString(_entity->getType());
		entity->emitMessage(m);
	}


	void CMeleeTongue::sendSET_ANIMATION(char *animacion, bool loop, bool restart, float multSpeed)
	{
		//Solo mandamos el mensaje si la animacion es diferente
		if(_animActual.compare(animacion) != 0)
		{
			float duration = 0;

			// Calculamos la velocidad de animación
			int initDelay = 1000;
			LUA::CScriptManager::GetPtrSingleton()->getField("Data_Player.player_info", "max_velocidad_ataque_normal", initDelay);

			initDelay *= _multVelMainAttack;
			float initSpeed = _mainAttackSpeedAnimation/ _multVelMainAttack;
			float speed = (initSpeed * initDelay) / _maxDelayMainAtk;
			speed *= multSpeed;

			// Establecemos la animación
			CAnimatedGraphics* cmp = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");

			cmp->setAnimation(animacion, "default", loop, restart, 0.0f, speed);

			_animActual = animacion;
		}
	}//sendSET_ANIMACION

	void CMeleeTongue::switchWeaponPhysic(bool activate)
	{
		std::shared_ptr<SET_SIMULATION> m (new SET_SIMULATION());
		m->setSimulation(activate);
		_entity->emitMessage(m, this);
	}
} // namespace Logic