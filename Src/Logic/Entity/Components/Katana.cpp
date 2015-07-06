/*
@file Katana.h

Contiene la implementación del componente que controla la lógica y los ataques del arma Katana

@author Alberto Martínez Villarán
@date Febrero de 2015

@refactor Barbara Domínguez de la Torre
@date Mayo de 2015

@refactor Alberto Martínez Villarán
@date Mayo de 2015
*/

#include "Katana.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Server.h"

#include "LUA/ScriptManager.h"

#include "AnimatedGraphics.h"
#include "Arms.h"
#include "WeaponController.h"

namespace Logic 
{
	IMP_FACTORY(CKatana);
	

	CKatana::CKatana() : Weapon() 
	{

	}

	CKatana::~CKatana()
	{

	}

	//---------------------------------------------------------

	//bool CKatana::OnSpawn(const Map::CEntity *entityInfo) 
	//{
	//	if(!Weapon::OnSpawn(entityInfo))
	//	{
	//		return false;
	//	}
	//	return true;

	//} // spawn


	//bool CKatana::respawn(const Map::CEntity *entityInfo) 
	//{
	//	if(!Weapon::respawn(entityInfo))
	//	{
	//		return false;
	//	}
	//	return true;

	//} // respawn
	////---------------------------------------------------------


	bool CKatana::activate() 
	{
		if(!Weapon::activate())
		{
			return false;
		}

		_player = CServer::getSingletonPtr()->getPlayer();

		_playerAnimatedEntity = (_player->getComponent<CAnimatedGraphics*>())->getAnimatedEntity();
		_playerAnimatedEntity->setObserver(this, "AttackSword1R");
		_playerAnimatedEntity->setObserver(this, "AttackSword1L");

		//animación por defecto al inicio
		if(_arm == "RightArm")
		{
			_animActual = "IdleSword1R";
		}
		else if(_arm == "LeftArm")
		{
			_animActual = "IdleSword1L";
		}

		//Desactivo la física de la espada desde el inicio

		switchWeaponPhysic(false);
		switchWeaponTrail(false);
		return true;

	} // activate

	void CKatana::deactivate() 
	{
		Weapon::deactivate();
		switchWeaponPhysic(false);
		switchWeaponTrail(false);
		_playerAnimatedEntity->removeObserver(this, "AttackSword1R");
		_playerAnimatedEntity->removeObserver(this, "AttackSword1L");
		_playerAnimatedEntity = 0;
		_player = 0;
	} // deactivate

	////---------------------------------------------------------

	bool CKatana::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return Weapon::accept(message) ||
			   message->getType() == "TOUCHED";

	} // accept
	
	//---------------------------------------------------------

	void CKatana::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		Weapon::process(message);
		if(message->getType().compare("TOUCHED") == 0)
		{
			CEntity* otherEntity = dynamic_cast<TOUCHED*>(message.get())->getEntidad();
			if( otherEntity != NULL)
			{
				if(otherEntity->getType() != "Player")
				{
					ApplyDamage(otherEntity);

					//Si golpeo a un enemigo genero chispa roja, si golpeo la puerta la normal
					if(otherEntity->getTag() == "enemy")
					{
						Logic::CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoEnemy",
							_entity->getComponent<CWeaponController*>()->getPivotTransform().getTrans(), 
							_entity->getMap());
					}
					else if(otherEntity->getTag() == "door")
					{
						Logic::CEntityFactory::getSingletonPtr()->createEntityByType("Chispa",
						_entity->getComponent<CWeaponController*>()->getPivotTransform().getTrans(), 
						_entity->getMap());
					}
					//BaseSubsystems::Log::Error("Trigger de daño de espada activado");
				}
			}
			//Si golpeo un tile genera chispa normal
			else
			{
				Logic::CEntityFactory::getSingletonPtr()->createEntityByType("Chispa",
						_entity->getComponent<CWeaponController*>()->getPivotTransform().getTrans(), 
						_entity->getMap());
			}
		}

	} // process

	//---------------------------------------------------------------------------------

	void CKatana::mainAttack(Vector3 mousePos, float baseDamage)
	{
		//BaseSubsystems::Log::Debug("Ataco con la katana");
		_delayMainAtk = 0;

		////Calculamos la vel final
		///---Vel inicial arquetipo
		/// ---Delay inicial * mult
		///regla de tres inversa
		
		//Aunque parezca que no es el delay inicial configurado en Data_Player
		int initDelay = 1000;
		LUA::CScriptManager::GetPtrSingleton()->getField("Data_Player.player_info", "max_velocidad_ataque_normal", initDelay);

		initDelay *= _multVelMainAttack;
		float initSpeed = _mainAttackSpeedAnimation/ _multVelMainAttack;
		float speed = (initSpeed * initDelay) / _maxDelayMainAtk;

		//BaseSubsystems::Log::Debug("AnimSpeed: " + std::to_string(speed) + " / " + std::to_string(_mainAttackSpeedAnimation));

		if(_arm.compare("RightArm") == 0)
		{
			sendSET_ANIMATION("AttackSword1R", false, true, speed);
		}
		else
		{
			sendSET_ANIMATION("AttackSword1L", false, true, speed);
		}



		//Configuramos el daño final del ataque actual
		_actualDamage = baseDamage * _mainDamageMultiplier;
		/*BaseSubsystems::Log::Debug("Daño base: "+std::to_string(baseDamage));
		BaseSubsystems::Log::Debug("Mult del ataque: "+std::to_string(_mainDamageMultiplier));
		BaseSubsystems::Log::Debug("Daño del ataque: "+std::to_string(_actualDamage));*/
		switchWeaponPhysic(true);
		switchWeaponTrail(true);


	}
	
	void CKatana::specialAttack(Vector3 mousePos, float baseDamage)
	{
		_delaySpecialAtk = 0;
		/*TODO
		Lanzamos animacion
		Activamos la fisica
		*/
	}
	//------------------------------------------------------------------------

	void CKatana::ApplyDamage(CEntity* entity)
	{
		//BaseSubsystems::Log::Debug("Daño del ataque: "+std::to_string(_actualDamage));
		std::shared_ptr<DAMAGED> m(new DAMAGED());
		m->setFloat(_actualDamage);
		m->setString(_entity->getType());
		entity->emitMessage(m);
	}
	//------------------------------------------------------------------------


	void CKatana::animationFinished(const std::string &animation)
	{
		//BaseSubsystems::Log::Debug("Ha parado la animacion: "+animation);
		if(((animation.compare("AttackSword1R") == 0) && (_arm.compare("RightArm") == 0))
			|| ((animation.compare("AttackSword1L") == 0) && (_arm.compare("LeftArm") == 0)))
		{
			//dejo de aplicar daño y desactivo el trigger
			switchWeaponPhysic(false);
			switchWeaponTrail(false);
			_actualDamage = 0.0f;

			if(_arm.compare("RightArm") == 0)
			{
				sendSET_ANIMATION("IdleSword1R", true, true, 1.0f);
			}
			else
			{
				sendSET_ANIMATION("IdleSword1L", true, true, 1.0f);
			}
			
		}
	}

	void CKatana::switchWeaponPhysic(bool activate)
	{
		std::shared_ptr<SET_SIMULATION> m (new SET_SIMULATION());
		m->setSimulation(activate);
		_entity->emitMessage(m, this);
	}

	void CKatana::switchWeaponTrail(bool activate)
	{
		std::shared_ptr<SET_TRAIL_VISIBLE> m (new SET_TRAIL_VISIBLE());
		m->setVisible(activate);
		_entity->emitMessage(m, this);
	}


	void CKatana::sendSET_ANIMATION(char *animacion, bool loop, bool restart, float speed)
	{
		//Solo mandamos el mensaje si la animacion es diferente
		if(_animActual.compare(animacion) != 0)
		{
			//BaseSubsystems::Log::Debug("Cambio de animacion a "+std::string(animacion));

			float duration = 0;

			if(!LUA::CScriptManager::GetPtrSingleton()->executeFunction("getTransitionTime", "SaiAnimations", _animActual, animacion, duration))
			{
				duration = 0;
			}

			CAnimatedGraphics* cmp = (CAnimatedGraphics*) _armComponent->getEntity()->getComponent("CAnimatedGraphics");

			cmp->setAnimation(animacion, _arm, loop, restart, duration, speed);

			_animActual = animacion;
		}
	}//sendSET_ANIMACION
	
	//--------------------------------------------------------------

} // namespace Logic