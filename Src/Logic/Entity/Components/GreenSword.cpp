/*
@file GreenSword.h

Contiene la implementación del componente que controla la lógica y los ataques del arma GreenSword

@author Alberto Martínez Villarán
@date Junio de 2015

*/

#include "GreenSword.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Map/MapParser.h"
#include "Logic/Server.h"

#include "LUA/ScriptManager.h"

#include "AnimatedGraphics.h"
#include "Arms.h"
#include "WeaponController.h"
#include "SwordSlashController.h"

namespace Logic 
{
	IMP_FACTORY(CGreenSword);

	CGreenSword::~CGreenSword()
	{

	}

	//---------------------------------------------------------

	bool CGreenSword::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(!Weapon::OnSpawn(entityInfo))
		{
			return false;
		}

		if(entityInfo->hasAttribute("materialEspadaVerde1"))
		{
			
			_material1 = entityInfo->getStringAttribute("materialEspadaVerde1");
		}

		if(entityInfo->hasAttribute("materialEspadaVerde2"))
		{
			_material2 = entityInfo->getStringAttribute("materialEspadaVerde2");
		}

		if(entityInfo->hasAttribute("materialEspadaVerde3"))
		{
			_material3 = entityInfo->getStringAttribute("materialEspadaVerde3");
		}

		if(entityInfo->hasAttribute("materialEspadaVerde4"))
		{
			_material4 = entityInfo->getStringAttribute("materialEspadaVerde4");
		}

		if(entityInfo->hasAttribute("materialEspadaVerde5"))
		{
			_material5 = entityInfo->getStringAttribute("materialEspadaVerde5");
		}


		if(entityInfo->hasAttribute("Slash1"))
		{
			_slash1 = entityInfo->getStringAttribute("Slash1");
		}

		if(entityInfo->hasAttribute("Slash2"))
		{
			_slash2 = entityInfo->getStringAttribute("Slash2");
		}

		if(entityInfo->hasAttribute("Slash3"))
		{
			_slash3 = entityInfo->getStringAttribute("Slash3");
		}

		if(entityInfo->hasAttribute("Slash4"))
		{
			_slash4 = entityInfo->getStringAttribute("Slash4");
		}

		if(entityInfo->hasAttribute("Slash5"))
		{
			_slash5 = entityInfo->getStringAttribute("Slash5");
		}


		return true;

	} // spawn


	bool CGreenSword::respawn(const Map::CEntity *entityInfo) 
	{
		if(!Weapon::respawn(entityInfo))
		{
			return false;
		}


		if(entityInfo->hasAttribute("materialEspecial1"))
		{
			_material1 = entityInfo->getStringAttribute("materialEspecial1");
		}

		if(entityInfo->hasAttribute("materialEspecial2"))
		{
			_material2 = entityInfo->getStringAttribute("materialEspecial2");
		}

		if(entityInfo->hasAttribute("materialEspadaVerde3"))
		{
			_material3 = entityInfo->getStringAttribute("materialEspadaVerde3");
		}

		if(entityInfo->hasAttribute("materialEspadaVerde4"))
		{
			_material4 = entityInfo->getStringAttribute("materialEspadaVerde4");
		}

		if(entityInfo->hasAttribute("materialEspadaVerde5"))
		{
			_material5 = entityInfo->getStringAttribute("materialEspadaVerde5");
		}

		if(entityInfo->hasAttribute("Slash1"))
		{
			_slash1 = entityInfo->getStringAttribute("Slash1");
		}

		if(entityInfo->hasAttribute("Slash2"))
		{
			_slash2 = entityInfo->getStringAttribute("Slash2");
		}

		if(entityInfo->hasAttribute("Slash3"))
		{
			_slash3 = entityInfo->getStringAttribute("Slash3");
		}

		if(entityInfo->hasAttribute("Slash4"))
		{
			_slash4 = entityInfo->getStringAttribute("Slash4");
		}

		if(entityInfo->hasAttribute("Slash5"))
		{
			_slash5 = entityInfo->getStringAttribute("Slash5");
		}

		return true;

	} // respawn
	////---------------------------------------------------------


	bool CGreenSword::activate() 
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

		_numGolpesActuales = 0;
		return true;

	} // activate

	void CGreenSword::deactivate() 
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

	bool CGreenSword::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return Weapon::accept(message) ||
			   message->getType() == "TOUCHED";

	} // accept
	
	//---------------------------------------------------------

	void CGreenSword::process(const std::shared_ptr<Logic::IMessage> &message)
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

						///al dañar a un enemigo aumento el contador de golpes
						++_numGolpesActuales;
						updateMaterial();
					}
					else if(otherEntity->getTag() == "door" || otherEntity->getTag() == "objetoRompible")
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

	void CGreenSword::mainAttack(Vector3 mousePos, float baseDamage)
	{
		
		/*_numGolpesActuales += 5;
		updateMaterial();*/

		//BaseSubsystems::Log::Debug("Ataco con la GreenSword");
		_delayMainAtk = 0;

		
		//Aunque parezca que no es el delay inicial configurado en Data_Player
		int initDelay = 1000;
		LUA::CScriptManager::GetPtrSingleton()->getField("Data_Player.player_info", "max_velocidad_ataque_normal", initDelay);

		initDelay *= _multVelMainAttack;
		float initSpeed = _mainAttackSpeedAnimation/ _multVelMainAttack;
		float speed = (initSpeed * initDelay) / _maxDelayMainAtk;


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
		
		switchWeaponPhysic(true);
		switchWeaponTrail(true);

		std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
		std::string s = "/Varios/GreenSword";
		sound->setSound(s);
		_entity->emitMessage(sound, this);

	}
	
	void CGreenSword::specialAttack(Vector3 mousePos, float baseDamage)
	{
		_delaySpecialAtk = 0;
		
		//lanzar animacion

		if(_arm.compare("RightArm") == 0)
		{
			sendSET_ANIMATION("AttackSword1R", false, true, 2.0f);
		}
		else
		{
			sendSET_ANIMATION("AttackSword1L", false, true, 2.0f);
		}


		//caluclo de variables segun la carga acumulada
		//daño

		float damage = baseDamage * _specialDamageMultiplier * _materialCount;

		///calculamos el tipo de slash a instanciar
		std::string slashType = "-";
		switch (_materialCount)
		{
		case 1:
			slashType = "GreenSwordSlash";
			break;
		case 2:
			slashType = "GreenSwordSlash2";
			break;
		case 3:
			slashType = "GreenSwordSlash3";
			break;
		case 4:
			slashType = "GreenSwordSlash4";
			break;
		case 5:
			slashType = "GreenSwordSlash5";
			break;
		default:
			slashType = "GreenSwordSlash";
			break;
		}

		//instanciar
		float lookingDir = (mousePos.x - _entity->getPosition().x);


		///por defecto el slash esta horientado a la izq, si esta mirando a derechas lo variamos a derechas
		Quaternion qgiro = Quaternion::IDENTITY;

		Vector3 position = CServer::getSingletonPtr()->getPlayer()->getCenterPosition();
		position.z = 40;

		if(lookingDir >= 0)
		{
			Radian r = Radian(Math::fromDegreesToRadians(180));
			qgiro = Quaternion(r, Vector3(0,0,1));
			position.x += 10;
		}
		else
		{
			position.x -= 10;
		}

		Matrix4 transform;
		transform.makeTransform(position, _entity->getComponent<CGraphics*>()->getScale(),qgiro);
		

		CEntity *ent = CEntityFactory::getSingletonPtr()->createEntityByTypeTransform(slashType, transform, _entity->getMap());

		//configurar daño y dir al componente
		CSwordSlashController* cmp = (CSwordSlashController*) ent->getComponent("CSwordSlashController");
		if(cmp)
		{
			cmp->setSlashConfiguration(mousePos, damage);
		}

		_numGolpesActuales = 0;
		updateMaterial();

		std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
		std::string s = "/Varios/GreenSwordSpecial";
		sound->setSound(s);
		_entity->emitMessage(sound, this);
	}
	//------------------------------------------------------------------------

	void CGreenSword::ApplyDamage(CEntity* entity)
	{
		//BaseSubsystems::Log::Debug("Daño del ataque: "+std::to_string(_actualDamage));
		std::shared_ptr<DAMAGED> m(new DAMAGED());
		m->setFloat(_actualDamage);
		m->setString(_entity->getType());
		entity->emitMessage(m);
	}
	//------------------------------------------------------------------------


	void CGreenSword::animationFinished(const std::string &animation)
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

	void CGreenSword::switchWeaponPhysic(bool activate)
	{
		std::shared_ptr<SET_SIMULATION> m (new SET_SIMULATION());
		m->setSimulation(activate);
		_entity->emitMessage(m, this);
	}

	void CGreenSword::switchWeaponTrail(bool activate)
	{
		std::shared_ptr<SET_TRAIL_VISIBLE> m (new SET_TRAIL_VISIBLE());
		m->setVisible(activate);
		_entity->emitMessage(m, this);
	}


	void CGreenSword::sendSET_ANIMATION(char *animacion, bool loop, bool restart, float speed)
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


	void CGreenSword::sendSET_MATERIAL(const std::string &material)
	{
		std::shared_ptr<SET_MATERIAL> m (new SET_MATERIAL());
		m->setString(material);
		_entity->emitMessage(m, this);
	}

	void CGreenSword::updateMaterial()
	{

		if(0 <= _numGolpesActuales && _numGolpesActuales < 2)
		{
			sendSET_MATERIAL(_material1);
			_materialCount = 1;
		}
		else if(2 <= _numGolpesActuales && _numGolpesActuales < 4)
		{
			sendSET_MATERIAL(_material2);
			_materialCount = 2;
		}
		else if(4 <= _numGolpesActuales && _numGolpesActuales < 6)
		{
			sendSET_MATERIAL(_material3);
			_materialCount = 3;
		}
		else if(6 <= _numGolpesActuales && _numGolpesActuales < 8)
		{
			sendSET_MATERIAL(_material4);
			_materialCount = 4;
		}
		else if(8 <= _numGolpesActuales)
		{
			_materialCount = 5;
			sendSET_MATERIAL(_material5);
		}
	}

} // namespace Logic