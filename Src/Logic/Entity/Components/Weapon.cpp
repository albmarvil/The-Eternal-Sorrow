#include "Weapon.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "LUA/ScriptManager.h"

#include "Logic/Entity/Components/Arms.h"

namespace Logic 
{
	IMP_FACTORY(Weapon);
	
	//---------------------------------------------------------

	bool Weapon::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("MultiVelMainAttack"))
		{
			_multVelMainAttack = entityInfo->getFloatAttribute("MultiVelMainAttack");
		}

		if(entityInfo->hasAttribute("MultiVelSpecialAttack"))
		{
			_multVelSpecialAttack = entityInfo->getFloatAttribute("MultiVelSpecialAttack");
		}

		if(entityInfo->hasAttribute("mainDamage"))
		{
			_mainDamageMultiplier = entityInfo->getFloatAttribute("mainDamage");
		}

		if(entityInfo->hasAttribute("specialDamage"))
		{
			_specialDamageMultiplier = entityInfo->getFloatAttribute("specialDamage");
		}

		if(entityInfo->hasAttribute("maxDurationSpecialAttack"))
		{
			_maxDurationSpecial = entityInfo->getIntAttribute("maxDurationSpecialAttack");
		}

		if(entityInfo->hasAttribute("mainAttackSpeedAnimation"))
		{
			_mainAttackSpeedAnimation = entityInfo->getFloatAttribute("mainAttackSpeedAnimation");
		}

		_isPressingAttackButton = false;
		return true;

	} // spawn


	bool Weapon::respawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("MultiVelMainAttack"))
		{
			_multVelMainAttack = entityInfo->getFloatAttribute("MultiVelMainAttack");
		}

		if(entityInfo->hasAttribute("MultiVelSpecialAttack"))
		{
			_multVelSpecialAttack = entityInfo->getFloatAttribute("MultiVelSpecialAttack");
		}

		if(entityInfo->hasAttribute("mainDamage"))
		{
			_mainDamageMultiplier = entityInfo->getFloatAttribute("mainDamage");
		}

		if(entityInfo->hasAttribute("specialDamage"))
		{
			_specialDamageMultiplier = entityInfo->getFloatAttribute("specialDamage");
		}

		if(entityInfo->hasAttribute("maxDurationSpecialAttack"))
		{
			_maxDurationSpecial = entityInfo->getIntAttribute("maxDurationSpecialAttack");
		}

		if(entityInfo->hasAttribute("mainAttackSpeedAnimation"))
		{
			_mainAttackSpeedAnimation = entityInfo->getFloatAttribute("mainAttackSpeedAnimation");
		}


		_isPressingAttackButton = false;

		return true;

	} // respawn
	//---------------------------------------------------------


	bool Weapon::activate() 
	{
		_delayMainAtk = FLT_MAX;
		_delaySpecialAtk = 0;
		_specialStillOn = false;
		_timeSpecialActive = 0;

		_armComponent = (CArms*) CServer::getSingletonPtr()->getPlayer()->getComponent("CArms");

		return true;

	} // activate
	//---------------------------------------------------------

	bool Weapon::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return  message->getType() == "MOUSE_POS" ||
			   //message->getType() == "SPECIAL_ATTACK_STATE" ||
			   //message->getType() == "ATTACK_STATE" ||
			   message->getType() == "SET_ARMTOWEAPON";

	} // accept
	
	//---------------------------------------------------------

	void Weapon::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("MOUSE_POS") == 0)
		{
			_posMouse = dynamic_cast<MOUSE_POS*>(message.get())->getVector3();
		}

		else if(message->getType().compare("SET_ARMTOWEAPON") == 0)
		{
			bool grab = dynamic_cast<SET_ARMTOWEAPON*>(message.get())->getBool();


			_arm = dynamic_cast<SET_ARMTOWEAPON*>(message.get())->getArm() + "Arm";
			if(grab)
			{
				_armComponent->setWeaponComponent(_arm, this);
			}
			
		}

	} // process

	//---------------------------------------------------------------------------------

	void Weapon::onTick(unsigned int msecs)
	{
		_delayMainAtk += msecs;
		_delaySpecialAtk += msecs;

		if (!_specialStillOn && _isPressingAttackButton && !_specialPressed && _delayMainAtk >= _maxDelayMainAtk)
		{
			mainAttack(_posMouse, _baseDamage);

		}
		else if (_specialStillOn || (_isPressingAttackButton && _specialPressed && _delaySpecialAtk > _maxDelaySpecialAtk))
		{
			_timeSpecialActive += msecs;

			specialAttack(_posMouse, _baseDamage);
			_delaySpecialAtk = 0;
			std::shared_ptr<SPECIAL_ATTACK_LAUNCHED> n(new SPECIAL_ATTACK_LAUNCHED());
			_entity->emitMessage(n);

		}
		
	}


	void Weapon::attackOrder(bool attack, float baseDamage, float mainDelay, float specialDelay)
	{
		_baseDamage = baseDamage;
		_maxDelayMainAtk = mainDelay * _multVelMainAttack;
		_maxDelaySpecialAtk =  specialDelay* _multVelSpecialAttack;

		_isPressingAttackButton = attack;

		
	}

	void Weapon::specialAttackOrder(bool attack, float baseDamage, float mainDelay, float specialDelay)
	{
		_baseDamage = baseDamage;
		_maxDelayMainAtk = mainDelay * _multVelMainAttack;
		_maxDelaySpecialAtk =  specialDelay* _multVelSpecialAttack;

		
		_isPressingAttackButton = attack;

		_specialPressed = attack;

	}

} // namespace Logic