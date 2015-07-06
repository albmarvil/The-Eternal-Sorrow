
#include "MeleeShield.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Server.h"
#include "Logic/Entity/Components/ShieldSpecialController.h"
#include "Logic/Entity/Components/AvatarController.h"
#include "LUA/ScriptManager.h"

#include "AnimatedGraphics.h"
#include "Arms.h"
#include "WeaponController.h"

namespace Logic 
{
	IMP_FACTORY(CMeleeShield);
	

	CMeleeShield::CMeleeShield() : Weapon() 
	{
		_shieldReleased = false;
		_materialSangre = "-";
		_material = "-";

		_cleanUpTime = 0;
		_timeAcum = 0;
		_countToClean = false;
	}

	CMeleeShield::~CMeleeShield()
	{

	}

	bool CMeleeShield::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("materialShieldSangre"))
		{
			_materialSangre = entityInfo->getStringAttribute("materialShieldSangre");
		}

		if(entityInfo->hasAttribute("material"))
		{
			_material = entityInfo->getStringAttribute("material");
		}

		if(entityInfo->hasAttribute("cleanUpTime"))
		{
			_cleanUpTime = entityInfo->getIntAttribute("cleanUpTime");
		}

		return Weapon::OnSpawn(entityInfo);
	}


	bool CMeleeShield::activate() 
	{
		if(!Weapon::activate())
		{
			return false;
		}

		_player = CServer::getSingletonPtr()->getPlayer();

		_playerAnimatedEntity = (_player->getComponent<CAnimatedGraphics*>())->getAnimatedEntity();
		_playerAnimatedEntity->setObserver(this, "AttackShieldR");
		_playerAnimatedEntity->setObserver(this, "AttackShieldL");

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

		_shieldReleased = false;

		///seteamos el material por defecto
		sendSET_MATERIAL(_material);

		_timeAcum = 0;
		_countToClean = false;

		return true;

	} // activate

	void CMeleeShield::deactivate() 
	{
		Weapon::deactivate();
		//if(_arm == "LeftArm")
		//{
		//	//_armComponent->InitializeHandsBones(true);
		//	BaseSubsystems::Log::Debug("Reestablezco la orientacion del bone simetrico");
		//}
			
		switchWeaponPhysic(false);
		_playerAnimatedEntity->removeObserver(this, "AttackShieldR");
		_playerAnimatedEntity->removeObserver(this, "AttackShieldL");
		_playerAnimatedEntity = 0;
		_player = 0;
	} // deactivate

	////---------------------------------------------------------

	bool CMeleeShield::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return Weapon::accept(message) ||
			   message->getType() == "TOUCHED";

	} // accept
	
	//---------------------------------------------------------

	void CMeleeShield::process(const std::shared_ptr<Logic::IMessage> &message)
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
						int look = _player->getComponent<CAvatarController*>()->getLookingDirection();

						Vector3 dir  = _entity->getPosition() * Vector3(look, 0,0);
						dir.normalise();

						std::shared_ptr<PUSH>m(new PUSH());
						m->setDirection(dir);
						m->setSpeed(0.6f);
						otherEntity->emitMessage(m, this);

						std::shared_ptr<DAMAGED> dmg(new DAMAGED());
						dmg->setFloat(_actualDamage);
						dmg->setString(_entity->getType());
						otherEntity->emitMessage(dmg, this);

						Logic::CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoEnemy",
							_entity->getComponent<CWeaponController*>()->getPivotTransform().getTrans(), 
							_entity->getMap());


						///ponemos el material de sangre
						sendSET_MATERIAL(_materialSangre);
						_timeAcum = 0;
						_countToClean = true;

					}
					else if(otherEntity->getTag() == "door" || otherEntity->getTag() == "objetoRompible")
					{
						Logic::CEntityFactory::getSingletonPtr()->createEntityByType("Chispa",
						_entity->getComponent<CWeaponController*>()->getPivotTransform().getTrans(), 
						_entity->getMap());

						std::shared_ptr<DAMAGED> dmg(new DAMAGED());
						dmg->setFloat(_mainDamageMultiplier * _baseDamage);
						dmg->setString(_entity->getType());
						otherEntity->emitMessage(dmg, this);
					}
				}
			}
		}
		else if(message->getType() == "SET_ARMTOWEAPON")
		{
			if(_arm == "LeftArm")
			{
				bool grab = dynamic_cast<SET_ARMTOWEAPON*>(message.get())->getBool();
				if(grab)
				{
					_armComponent->changeHandBoneSimetry("Left", Quaternion(0,1,0,0));
				}
				else
				{
					Quaternion q = Quaternion::IDENTITY;
					q = q * Quaternion(0,-1, 0, 0);
					q = q * Quaternion(0, 0, 1, 0);
					_armComponent->changeHandBoneSimetry("Left", q);
				}
			}

		}

	} // process

	//--------------------------------------------------------------------------------
	
	
	void CMeleeShield::onTick(unsigned int msecs)
	{
		Weapon::onTick(msecs);
		if(_countToClean)
		{
			_timeAcum += msecs;

			if(_timeAcum >= _cleanUpTime)
			{
				_timeAcum = 0;
				_countToClean = false;
				sendSET_MATERIAL(_material);
			}
		}
	}//onTick

	///--------------------------------------------
	

	void CMeleeShield::mainAttack(Vector3 mousePos, float baseDamage)
	{
		if(!_shieldReleased)
		{
			//BaseSubsystems::Log::Debug("Ataco con la katana");
			_delayMainAtk = 0;

			//Aunque parezca que no es el delay inicial configurado en Data_Player
			int initDelay = 1000;
			LUA::CScriptManager::GetPtrSingleton()->getField("Data_Player.player_info", "max_velocidad_ataque_normal", initDelay);

			initDelay *= _multVelMainAttack;
			float initSpeed = _mainAttackSpeedAnimation/ _multVelMainAttack;
			float speed = (initSpeed * initDelay) / _maxDelayMainAtk;

			//BaseSubsystems::Log::Debug("AnimSpeed: " + std::to_string(speed) + " / " + std::to_string(_mainAttackSpeedAnimation));

			if(_arm.compare("RightArm") == 0)
			{
				sendSET_ANIMATION("AttackShieldR", false, true, speed);
			}
			else
			{
				sendSET_ANIMATION("AttackShieldL", false, true, speed);
			}



			//Configuramos el daño final del ataque actual
			_actualDamage = baseDamage * _mainDamageMultiplier;
		
			switchWeaponPhysic(true);
		}
	}
	
	void CMeleeShield::specialAttack(Vector3 mousePos, float baseDamage)
	{
		if(!_shieldReleased)
		{
			_delaySpecialAtk = 0;

			Vector3 pos = _entity->getCenterPosition();
			pos.z = 0.0f;

			Logic::CEntity* shield = Logic::CEntityFactory::getSingletonPtr()->createEntityByType("ShieldSpecial",pos, _entity->getMap());

			shield->getComponent<CShieldSpecialController*>()->setShieldControllerConfiguration(mousePos, _baseDamage * _specialDamageMultiplier, this);

			std::shared_ptr<SET_PHYSIC_GROUP>m(new SET_PHYSIC_GROUP());
			m->setGroup(9);
			shield->emitMessage(m);


			///Hacemos invisible el arma
			setShieldVisible(false);


			if(_arm.compare("RightArm") == 0)
			{
				sendSET_ANIMATION("AttackShieldR", false, true, 1.8f);
			}
			else
			{
				sendSET_ANIMATION("AttackShieldL", false, true, 1.8f);
			}
		}
		

	}
	//------------------------------------------------------------------------

	void CMeleeShield::ApplyDamage(CEntity* _entity)
	{
		//BaseSubsystems::Log::Debug("Daño del ataque: "+std::to_string(_actualDamage));
		std::shared_ptr<DAMAGED> m(new DAMAGED());
		m->setFloat(_actualDamage);
		m->setString(_entity->getType());
		_entity->emitMessage(m);
	}
	//------------------------------------------------------------------------


	void CMeleeShield::animationFinished(const std::string &animation)
	{
		//BaseSubsystems::Log::Debug("Ha parado la animacion: "+animation);
		if(((animation.compare("AttackShieldR") == 0) && (_arm.compare("RightArm") == 0))
			|| ((animation.compare("AttackShieldL") == 0) && (_arm.compare("LeftArm") == 0)))
		{
			//dejo de aplicar daño y desactivo el trigger
			switchWeaponPhysic(false);
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

	void CMeleeShield::switchWeaponPhysic(bool activate)
	{
		std::shared_ptr<SET_SIMULATION> m (new SET_SIMULATION());
		m->setSimulation(activate);
		_entity->emitMessage(m, this);
	}

	void CMeleeShield::sendSET_ANIMATION(char *animacion, bool loop, bool restart, float speed)
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
	

	void CMeleeShield::setShieldVisible(bool visible)
	{
		_armComponent->setWeaponVisible(_arm, visible);
		//aprovechamos para indicar que el escudo esta liberado o no
		_shieldReleased = !visible;
	}
	//--------------------------------------------------------------


	void CMeleeShield::sendSET_MATERIAL(const std::string &material)
	{
		std::shared_ptr<SET_MATERIAL> m(new SET_MATERIAL());
		m->setString(material);
		_entity->emitMessage(m, this);
	}

} // namespace Logic