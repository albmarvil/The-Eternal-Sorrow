
#include "Arms.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "Logic/Entity/Components/Weapon.h"

#include "Logic/Entity/Components/AvatarController.h"

#include "Map/MapEntity.h"
#include "LUA\ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CArms);

	//---------------------------------------------------------

	bool CArms::OnSpawn(const Map::CEntity *entityInfo) 
	{
		//ponemos el booleano is_weapon a false
		is_weapon = false;
		//Comprobamos si el personaje tiene armas
		std::string typeWeapon;

		//Comprobamos si hay un arma en el brazo izquierdo
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("GetWeapon", "LeftArm", typeWeapon);

		if(typeWeapon.compare("") != 0)
		{
			is_weapon = true;
		}
		else
		{
			_weaponL.entity = 0;
			_weaponL.weaponComponent = 0;
			_weaponL.attached = false;
		}

		LUA::CScriptManager::GetPtrSingleton()->executeFunction("GetWeapon", "RightArm", typeWeapon);

		if(typeWeapon.compare("") != 0)
		{
			is_weapon = true;
		}
		else
		{
			_weaponR.entity = 0;
			_weaponR.weaponComponent = 0;
			_weaponR.attached = false;
		}

		if(entityInfo->hasAttribute("animation_config"))
		{
			_animationConfig = entityInfo->getStringAttribute("animation_config");
		}

		if(entityInfo->hasAttribute("maxDelayMainAtk"))
		{
			_baseMainDelay = entityInfo->getFloatAttribute("maxDelayMainAtk");
		}

		if(entityInfo->hasAttribute("maxDelaySpecialAtk"))
		{
			_baseSpecialDelay = entityInfo->getFloatAttribute("maxDelaySpecialAtk");
		}

		if(entityInfo->hasAttribute("maxDelayGrabWeapon"))
		{
			_maxDelayGrabWeapon = entityInfo->getFloatAttribute("maxDelayGrabWeapon");
		}

		if(entityInfo->hasAttribute("maxDistance"))
		{
			_maxDistance = entityInfo->getFloatAttribute("maxDistance");
			_maxDistance *= _maxDistance;
		}

		return true;

	} // spawn

	//---------------------------------------------------------

	bool CArms::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "TOUCHING_WEAPON" ||
			   message->getType() == "UNTOUCHED" ||
			   message->getType() == "GRAB_WEAPON" ||
			   message->getType() == "SWAP_WEAPONS" ||
			   message->getType() == "MOUSE_POS" ||
			   //message->getType() == "ATTACK_ORDER" ||
			   //message->getType() == "SPECIAL_ATTACK_ORDER" ||
			   message->getType() == "SET_STAT";
	} // accept

	//---------------------------------------------------------

	void CArms::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("TOUCHING_WEAPON") == 0)
		{
			CEntity* otherEntity = dynamic_cast<TOUCHING_WEAPON*>(message.get())->getEntidad();
			std::string wtype = dynamic_cast<TOUCHING_WEAPON*>(message.get())->getStringWeaponType();
			std::string uotype = dynamic_cast<TOUCHING_WEAPON*>(message.get())->getStringUsableObjectType();

			// Lo metemos en el vector de elementos que nos están tocando 
			if (otherEntity->getTag() == "weapon")
			{
				WeaponInArm wArm;
				wArm.entity = otherEntity;
				wArm.weaponType = wtype;
				wArm.usableObjectType = uotype;
				wArm.attackType = "";
				wArm.attached = false;

				vecTouching.push_back(wArm);
			}
		}

		else if(message->getType().compare("UNTOUCHED") == 0)
		{
			CEntity *otherEntity = dynamic_cast<UNTOUCHED*>(message.get())->getEntidad();
			// Buscamos el objeto en la lista y lo sacamos
			for (int i = 0; i < vecTouching.size(); i++)
			{
				if (vecTouching.at(i).entity == otherEntity)
				{
					vecTouching.erase(vecTouching.begin() + i);
					break;
				}
			}
		}

		else if (message->getType().compare("GRAB_WEAPON") == 0)
		{
			// Si había un arma tocando al jugador y ha pasado más tiempo del necesitado para cogerla...
			if (vecTouching.size() != 0 && _delayGrabWeapon >= _maxDelayGrabWeapon)
			{
				std::string arm = dynamic_cast<GRAB_WEAPON*>(message.get())->getString();

				grabWeapon(arm);
			}
		}

		else if (message->getType().compare("SWAP_WEAPONS") == 0)
		{
			swapWeapons();
		}

		else if (message->getType().compare("MOUSE_POS") == 0)
		{
			_posMouse = dynamic_cast<MOUSE_POS*>(message.get())->getVector3();


			//Comprobamos que la posición del ratón está a una distancia mayor de la mínima

			float distance = pow(_entity->getPosition().x - _posMouse.x, 2);

			if(distance < _maxDistance)
			{
				//Si la distancia es menor que la máxima entonces trasladamos el punto en x
				float d = _maxDistance - distance;

				int lookAt = ((Logic::CAvatarController*) _entity->getComponent("CAvatarController"))->getLookingDirection();

				if(lookAt == -1)
				{
					_posMouse.x -= sqrt(d);
				}
				else
				{
					_posMouse.x += sqrt(d);
				}
			}

			if (_weaponR.entity)
			{
				std::shared_ptr<MOUSE_POS> m(new MOUSE_POS());
				m->setVector3(_posMouse);
				_weaponR.entity->emitMessage(m, this);
			}

			if (_weaponL.entity)
			{
				std::shared_ptr<MOUSE_POS> m(new MOUSE_POS());
				m->setVector3(_posMouse);
				_weaponL.entity->emitMessage(m, this);
			}
		}
		else if (message->getType().compare("SET_STAT") == 0)
		{
			std::string typeHability = dynamic_cast<SET_STAT*>(message.get())->getString();
			
			if(typeHability.compare("ataque") == 0)
			{
				_baseDamage = dynamic_cast<SET_STAT*>(message.get())->getFloat();
			}
			if(typeHability.compare("velocidad_ataque_normal") == 0)
			{
				//antes de cambiar el valor cogemos el valor anterior
				_baseMainDelay = dynamic_cast<SET_STAT*>(message.get())->getFloat();
			}
			if(typeHability.compare("velocidad_ataque_especial") == 0)
			{
				_baseSpecialDelay = dynamic_cast<SET_STAT*>(message.get())->getFloat();
			}

			//BaseSubsystems::Log::Debug("Stats actualizadas: Attk: " + std::to_string(_baseDamage) + " Normal: " + std::to_string(_baseMainDelay) + " Special: " + std::to_string(_baseSpecialDelay));
		}
	} // process
	//-------------------------------------------------------------------------------

	void CArms::onTick(unsigned int msecs)
	{
		if (_delayGrabWeapon <= _maxDelayGrabWeapon)
			_delayGrabWeapon += msecs;

		chooseArmsToRotate();
	}

	bool CArms::activate()
	{
		CAnimatedGraphics* animComp =_entity->getComponent<CAnimatedGraphics*>();
		_animEntity = animComp->getAnimatedEntity();

		_leftArmBone = "";
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("getLeftArmBone", _animationConfig, _leftArmBone);
		_rightArmBone = "";
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("getRightArmBone", _animationConfig, _rightArmBone);

		_leftHandBone = "";
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("getLeftHandBone", _animationConfig, _leftHandBone);
		_rightHandBone = "";
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("getRightHandBone", _animationConfig, _rightHandBone);

		InitializeHandsBones(true);
		
		weaponPermanency();


		return true;
	}

	//------------------------------------------------------------------------

	void CArms::grabWeapon(std::string arm)
	{
		//si tiene las dos armas actualmente en el brazo o si tiene un arma y esa arma esta actualmente en el brazo, entonces y solo entonces se puede hacer un swap de las armas
		bool canGrab = false;/*(!_weaponL.entity) || (!_weaponR.entity) || (_weaponL.entity && _weaponR.entity && _weaponL.attached && _weaponR.attached) ||
			(_weaponL.entity && !_weaponR.entity && _weaponL.attached) ||
			(_weaponR.entity && !_weaponL.entity && _weaponR.attached);*/

		if(arm == "Right")
		{
			canGrab = (!_weaponR.entity) || (_weaponL.entity && _weaponR.entity && _weaponL.attached && _weaponR.attached) || (_weaponR.entity && !_weaponL.entity && _weaponR.attached);
		}
		else if ("Left")
		{
			canGrab = (!_weaponL.entity) || (_weaponL.entity && _weaponR.entity && _weaponL.attached && _weaponR.attached) || (_weaponL.entity && !_weaponR.entity && _weaponL.attached);
		}


		if(canGrab)
		{
				// Creamos la entidad del arma
			CEntity* weapon = CEntityFactory::getSingletonPtr()->createEntityByType(vecTouching.at(0).weaponType, _entity->getPosition(), _entity->getMap());

			// Comprobamos y asignamos datos según el brazo en que vaya a estar
			std::string bone;
			std::string type;
			std::string usable_type;

			if (arm == "Left")
			{
				bone = _leftHandBone;

				// Si ya teníamos un arma en la mano izquierda, la soltamos en el escenario
				if (_weaponL.entity && _weaponL.attached)
				{
				
					//TEnemos apuntado el usable Object del cual obtenemos el arma

					Logic::CEntityFactory::getSingletonPtr()->createEntityByType(_weaponL.usableObjectType, _entity->getPosition() + Vector3(0,30,0), _entity->getMap());
				
					// Enviamos mensaje para quitar el arma del hashmap de observadores
					_animEntity->deattachObjectFromBone(bone, _weaponL.entity);

					std::shared_ptr<SET_ARMTOWEAPON> mSoltar (new SET_ARMTOWEAPON());
					mSoltar->setBool(false);
					mSoltar->setArm("Left");
					_weaponL.entity->emitMessage(mSoltar, this);

					CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_weaponL.entity);
				}
				else if(_weaponL.entity && !_weaponL.attached)//en este caso no hacemos nada mas, no cogemos el arma
				{
					return;
				}

				// Guardamos los datos del nuevo arma a acoplar
				_weaponL.entity = weapon;
				_weaponL.weaponType = weapon->getType();
				_weaponL.usableObjectType = vecTouching.at(0).usableObjectType;
				_weaponL.attackType = weapon->getEntityInfo()->getStringAttribute("attackType");
				_weaponL.attached = true;

				//Actualizamos el valor del arma en las stats del personaje
				type = weapon->getType();
				usable_type = vecTouching.at(0).usableObjectType;
			
				//Enviamos el mensaje para setear la animacion
				SendAnimationMessage("Left");

			}
			else if (arm == "Right")
			{	
				bone = _rightHandBone;

				// Si ya teníamos un arma en la mano derecha, la soltamos en el escenario
				if (_weaponR.entity && _weaponR.attached)
				{
					// Buscaríamos donde almacenemos todos los tipos de objetosusables, aquel cuyo subtipo sea el que tenemos guardado
					// en la mano derecha. De este modo nos devolverá todos los datos para pasarle a createObject y crear el objeto.
					Logic::CEntityFactory::getSingletonPtr()->createEntityByType(_weaponR.usableObjectType, _entity->getPosition() + + Vector3(0,30,0), _entity->getMap());
			
					// Enviamos mensaje para quitar el arma del hashmap de observadores
					_animEntity->deattachObjectFromBone(bone, _weaponR.entity);

					std::shared_ptr<SET_ARMTOWEAPON> mSoltar (new SET_ARMTOWEAPON());
					mSoltar->setBool(false);
					mSoltar->setArm("Right");
					_weaponR.entity->emitMessage(mSoltar, this);

					CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_weaponR.entity);
				}
				else if (_weaponR.entity && !_weaponR.attached)
				{
					return;
				}

				// Guardamos los datos del nuevo arma a acoplar
				_weaponR.entity = weapon;
				_weaponR.weaponType = weapon->getType();
				_weaponR.usableObjectType = vecTouching.at(0).usableObjectType;
				_weaponR.attackType = weapon->getEntityInfo()->getStringAttribute("attackType");
				_weaponR.attached = true;

				//Actualizamos el valor del arma en las stats del personaje
				type = weapon->getType();
				usable_type = vecTouching.at(0).usableObjectType;

			
				//Enviamos el mensaje para setear la animacion
				SendAnimationMessage("Right");
			}

			// Enviamos mensaje para enganchar el arma al hueso
			_animEntity->attachObjectToBone(bone, weapon);
		
			//enviamos un mensaje para avisar al arma enganchada cual es su brazo
			std::shared_ptr<SET_ARMTOWEAPON> mWeapon (new SET_ARMTOWEAPON());
			mWeapon->setBool(true);
			mWeapon->setArm(arm);
			weapon->emitMessage(mWeapon);

			std::shared_ptr<SET_ARMTOWEAPONHUD> mWeaponhud (new SET_ARMTOWEAPONHUD());
			mWeaponhud->setArm(arm);
			weapon->emitMessage(mWeaponhud);

			// Eliminamos el UsableObject y lo sacamos de la lista de elementos que estamos tocando
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(vecTouching.at(0).entity);
			vecTouching.erase(vecTouching.begin());

			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("SetWeapon", arm, type, usable_type, weapon->getEntityInfo()->getFloatAttribute("MultiVelSpecialAttack"));

			//Seteamos un delay para que no pueda coger un arma hasta que no pase cierto tiempo
			_delayGrabWeapon = 0;
		}
	}

	void CArms::swapWeapons()
	{
		// Nos encargamos de hacer DEATTACH del arma actual en cada brazo y luego ATTACH de esa en el otro.
		// Ésto se hará solo si tenemos un arma en un brazo

		//si tiene las dos armas actualmente en el brazo o si tiene un arma y esa arma esta actualmente en el brazo, entonces y solo entonces se puede hacer un swap de las armas
		bool canSwap = (_weaponL.entity && _weaponR.entity && _weaponL.attached && _weaponR.attached) ||
			(_weaponL.entity && !_weaponR.entity && _weaponL.attached) ||
			(_weaponR.entity && !_weaponL.entity && _weaponR.attached);


		if(canSwap)
		{
			if (_weaponL.entity)
			{
				_animEntity->deattachObjectFromBone(_leftHandBone, _weaponL.entity);
				//avisamos al arma de que va a ser soltada de esta mano
				std::shared_ptr<SET_ARMTOWEAPON> mSoltar (new SET_ARMTOWEAPON());
				mSoltar->setBool(false);
				mSoltar->setArm("Left");
				_weaponL.entity->emitMessage(mSoltar, this);

				_animEntity->attachObjectToBone(_rightHandBone, _weaponL.entity);
				//enviamos un mensaje para avisar al arma enganchada cual es su brazo
				std::shared_ptr<SET_ARMTOWEAPON> mWeapon (new SET_ARMTOWEAPON());
				mWeapon->setBool(true);
				mWeapon->setArm("Right");
				_weaponL.entity->emitMessage(mWeapon, this);

			
			}

			if (_weaponR.entity)
			{
				_animEntity->deattachObjectFromBone(_rightHandBone, _weaponR.entity);
				//avisamos al arma de que va a ser soltada de esta mano
				std::shared_ptr<SET_ARMTOWEAPON> mSoltar (new SET_ARMTOWEAPON());
				mSoltar->setBool(false);
				mSoltar->setArm("Right");
				_weaponR.entity->emitMessage(mSoltar, this);

				_animEntity->attachObjectToBone(_leftHandBone, _weaponR.entity);

				//enviamos un mensaje para avisar al arma enganchada cual es su brazo
				std::shared_ptr<SET_ARMTOWEAPON> mWeapon (new SET_ARMTOWEAPON());
				mWeapon->setBool(true);
				mWeapon->setArm("Left");
				_weaponR.entity->emitMessage(mWeapon);
			}
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr","swapWeapon");
			//LUA::CScriptManager::GetPtrSingleton()->executeProcedure("HUD","swapArms");
			// Ahora intercambiamos la información del struct de un brazo al otro
			WeaponInArm aux;
			aux = _weaponL;
			_weaponL = _weaponR;
			_weaponR = aux;

			//Cambiamos las animaciones el brazo derecho y del brazo izquierdo
			SendAnimationMessage("Right");
			SendAnimationMessage("Left");


			///mandamos la orden de ataque de parada a ambas armas
			attackOrder("Right", false);
			attackOrder("Left", false);
			specialAttackOrder("Right", false);
			specialAttackOrder("Left", false);
		}
	}

	void CArms::chooseArmsToRotate()
	{
		/*std::string boneR = "Bip01 R UpperArm";
		std::string boneL = "Bip01 L UpperArm";*/

		// Estas comprobaciones comprueban si existe un arma en su brazo correspondiente.
		// Si es cierto, rotarán el brazo, y si no anularán el control manual de su hueso
		if (_weaponR.entity && _weaponR.attackType == "Ranged")
		{
			rotateArm(_rightArmBone);
		}

		else
		{
			_animEntity->setBoneManuallyControlled(_rightArmBone, false);
		}

		if (_weaponL.entity && _weaponL.attackType == "Ranged")
		{
			rotateArm(_leftArmBone);
		}

		else
		{
			_animEntity->setBoneManuallyControlled(_leftArmBone, false);
		}
	} // rotateArms


	void CArms::rotateArm(std::string boneName)
	{
		//Codigo para rotar el brazo con la palma apuntando al suelo
		//Cogemos del servidor del GUI la dirección en la que mira el player
		int lookingDirection = GUI::CServer::getSingletonPtr()->getPlayerController()->getPlayerLookingDirection();

		// Establecemos a true para poder mover libremente el hueso y
		// aplicamos una orientación por defecto de la cual partir
		_animEntity->setBoneManuallyControlled(boneName, true);

		//partimos del quaternion identity para girar los hombros del player
		Quaternion q = Quaternion::IDENTITY;
		Quaternion qx;
		Quaternion qy;

		// según a donde mira el giro en el eje y será distinto
		qy = Quaternion(sqrt(0.5), 0, lookingDirection * -sqrt(0.5), 0);
		q = q * qy;

		//el giro en el eje x del mundo, z del bone será dependiente del hueso
		if(boneName == _rightArmBone)
		{
			qx = Quaternion(sqrt(0.5), 0, 0, -sqrt(0.5));
		}
		else if(boneName == _leftArmBone)
		{
			qx = Quaternion(sqrt(0.5), 0, 0, sqrt(0.5));
		}

		q = q * qx;
		
		_animEntity->setBoneWorldOrientation(boneName, q);

		// Sacamos los vectores necesarios para determinar el ángulo. 
		// dirOri = distancia desde el hueso al ratón. dirFin = eje X
		Vector3 dirOri =  (_posMouse - _animEntity->getBoneWorldPosition(boneName)) * Vector3(1,1,0); 
		Vector3 dirFin =  Vector3(1,0,0);

		dirOri.normalise(); 

		// Para sacar el ángulo necesitamos hacer el arcoseno del producto escalar de los dos vectores
		float dot = lookingDirection * dirOri.dotProduct(dirFin);
		
		float angulo(acos(dot));

		// Para saber la dirección a la que apuntar sacamos el producto vectorial de los dos vectores
		// y hacemos el producto escalar de dicho producto vectorial con el eje sobre el que queremos rotar (Z)
		Vector3 cross = dirOri.crossProduct(dirFin);

		//Calculamos la dirección de giro, según el brazo y hacia donde mire
		float dir;

		if(boneName == _rightArmBone)
		{
			dir = -cross.dotProduct(Vector3(0,0,1));
		}
		else if(boneName == _leftArmBone)
		{
			dir = cross.dotProduct(Vector3(0,0,1));
		}

		// Como el producto escalar solo devuelve de 0 a 180º, invertimos si la dirección es negativa
		if (dir < 0)
			angulo = -angulo;

		// Giramos el hueso con el ángulo final
		_animEntity->yawBone(boneName,angulo);
	}

	//-----------------------------------------------------------

	void CArms::weaponPermanency()
	{
		if(is_weapon)
		{
			std::string typeWeapon;

			//Comprobamos si hay un arma en el brazo izquierdo
			LUA::CScriptManager::GetPtrSingleton()->executeFunction("GetWeapon", "LeftArm", typeWeapon);

			if(typeWeapon.compare("") != 0)
			{
				_weaponL.entity = CEntityFactory::getSingletonPtr()->createEntityByType(typeWeapon, _entity->getPosition(), _entity->getMap());
				_weaponL.attached = true;
				// Comprobamos y asignamos datos según el brazo en que vaya a estar
				std::string bone = _leftHandBone;
			
				// Guardamos los datos del nuevo arma a acoplar
				_weaponL.weaponType = _weaponL.entity->getType();
			
				//Cogemos el usable object
				LUA::CScriptManager::GetPtrSingleton()->executeFunction("GetUsableWeapon", "LeftArm", typeWeapon);

				_weaponL.usableObjectType = typeWeapon;
				_weaponL.attackType = _weaponL.entity->getEntityInfo()->getStringAttribute("attackType");

				// Enviamos mensaje para enganchar el arma al hueso
				_animEntity->attachObjectToBone(bone, _weaponL.entity);

				std::shared_ptr<SET_ARMTOWEAPON> mWeapon (new SET_ARMTOWEAPON());
				mWeapon->setBool(true);
				mWeapon->setArm("Left");
				_weaponL.entity->emitMessage(mWeapon, this);

				/*std::shared_ptr<SET_ARMTOWEAPONHUD> mWeaponhud2 (new SET_ARMTOWEAPONHUD());
				mWeaponhud2->setArm("Left");
				_weaponL.entity->emitMessage(mWeaponhud2, this);*/

				std::shared_ptr<NOTIFY_ARM> mWeaponhud (new NOTIFY_ARM());
				mWeaponhud->setArm("Left");
				_weaponL.entity->emitMessage(mWeaponhud, this);
			}

			//Enviamos el mensaje para setear la animacion
			SendAnimationMessage("Left");

			LUA::CScriptManager::GetPtrSingleton()->executeFunction("GetWeapon", "RightArm", typeWeapon);

			if(typeWeapon.compare("") != 0)
			{
				// Creamos la entidad del arma
				_weaponR.entity = CEntityFactory::getSingletonPtr()->createEntityByType(typeWeapon, _entity->getPosition(), _entity->getMap());
				_weaponR.attached=true;
				std::string bone = _rightHandBone;

				// Guardamos los datos del nuevo arma a acoplar
				_weaponR.weaponType = _weaponR.entity->getType();

				//Cogemos el usable object
				LUA::CScriptManager::GetPtrSingleton()->executeFunction("GetUsableWeapon", "RightArm", typeWeapon);

				_weaponR.usableObjectType = typeWeapon;
				_weaponR.attackType = _weaponR.entity->getEntityInfo()->getStringAttribute("attackType");
				
				// Enviamos mensaje para enganchar el arma al hueso
				_animEntity->attachObjectToBone(bone, _weaponR.entity);

				std::shared_ptr<SET_ARMTOWEAPON> mWeapon (new SET_ARMTOWEAPON());
				mWeapon->setBool(true);
				mWeapon->setArm("Right");
				_weaponR.entity->emitMessage(mWeapon);

				/*std::shared_ptr<SET_ARMTOWEAPONHUD> mWeaponhud2 (new SET_ARMTOWEAPONHUD());
				mWeaponhud2->setArm("Right");
				_weaponR.entity->emitMessage(mWeaponhud2, this);*/

				std::shared_ptr<NOTIFY_ARM> mWeaponhud (new NOTIFY_ARM());
				mWeaponhud->setArm("Right");
				_weaponR.entity->emitMessage(mWeaponhud);
			}

			//Enviamos el mensaje para setear la animacion
			SendAnimationMessage("Right");

			is_weapon = false;
		}
		else
		{
			if(_weaponR.entity == NULL)
			{
				//Enviamos el mensaje para setear la animacion
				SendAnimationMessage("Right");
			}
			if(_weaponL.entity == NULL)
			{
				//Enviamos el mensaje para setear la animacion
				SendAnimationMessage("Left");
			}
		}
	}

	//-----------------------------------------------------------

	void CArms::SendAnimationMessage(std::string name)
	{
		CAnimatedGraphics* cmp = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");
		if(name.compare("Left") == 0)
		{
			if(_weaponL.attackType.compare("Melee") == 0)
			{
				//Colocamos la animación de la espada en el brazo izquierdo

				cmp->setAnimation("IdleSword1L", "LeftArm", true, true);
			}
			else if(_weaponL.attackType.compare("Ranged") == 0)
			{
				std::shared_ptr<STOP_ANIMATION> m(new STOP_ANIMATION());
				m->setAnimation("IdleR");
				m->setAnimationSet("LeftArm");
				_entity->emitMessage(m);
			}
			else
			{
				//Colocamos la animación de idle en el brazo izquierdo
				cmp->setAnimation("IdleL", "LeftArm", true, true);
			}
		}
		else
		{
			if(_weaponR.attackType.compare("Melee") == 0)
			{
				//Colocamos la animación de la espada en el brazo derecho
				cmp->setAnimation("IdleSword1R", "RightArm", true, true);
			}
			else if(_weaponR.attackType.compare("Ranged") == 0)
			{
				//Colocamos la animación de la espada en el brazo derecho
				std::shared_ptr<STOP_ANIMATION> m(new STOP_ANIMATION());
				m->setAnimation("IdleR");
				m->setAnimationSet("RightArm");
				_entity->emitMessage(m);
			}
			else
			{
				//Colocamos la animación de idle en el brazo izquierdo
				cmp->setAnimation("IdleR", "RightArm", true, true);
			}
		}
	}


	void CArms::attackOrder(const std::string &arm, bool attack)
	{

		if (arm == "Right" && _weaponR.entity && _weaponR.weaponComponent)
		{	
			_weaponR.weaponComponent->attackOrder(attack, _baseDamage, _baseMainDelay, _baseSpecialDelay);

			//forzamos la animación de idle en el componente de animación
			//Enviamos el mensaje de control si hay un arma en el brazo
			SendControl("attackR", attack);
		}
		else if(arm == "Left" && _weaponL.entity && _weaponL.weaponComponent)
		{
			_weaponL.weaponComponent->attackOrder(attack, _baseDamage, _baseMainDelay, _baseSpecialDelay);

			//forzamos la animación de idle en el componente de animación
			//Enviamos el mensaje de control si hay un arma en el brazo
			SendControl("attackL", attack);
		}
	}///attackOrder

	//-------------------------------------------------------------


	void CArms::specialAttackOrder(const std::string &arm, bool attack)
	{

		if (arm == "Right" && _weaponR.entity && _weaponR.weaponComponent)
		{
			_weaponR.weaponComponent->specialAttackOrder(attack, _baseDamage, _baseMainDelay, _baseSpecialDelay);
			
			//forzamos la animación de idle en el componente de animación
			//Enviamos el mensaje de control si hay un arma en el brazo
			SendControl("attackR", attack);
		}
		else if(arm == "Left" && _weaponL.entity && _weaponL.weaponComponent)
		{
			_weaponL.weaponComponent->specialAttackOrder(attack, _baseDamage, _baseMainDelay, _baseSpecialDelay);

			//forzamos la animación de idle en el componente de animación
			//Enviamos el mensaje de control si hay un arma en el brazo
			SendControl("attackL", attack);
		}
	}//specialAttackOrder

	//------------------------------------------------------------


	void const CArms::setWeaponComponent(const std::string &arm, Weapon* component)
	{
		if (arm == "RightArm" && _weaponR.entity)
		{
			_weaponR.weaponComponent = component;
		}
		else if(arm == "LeftArm" && _weaponL.entity)
		{
			_weaponL.weaponComponent = component;
		}
	}//specialAttackOrder

	//------------------------------------------------------------

	void CArms::removeWeapons()
	{
		if (_weaponL.entity)
		{
			// Enviamos mensaje para quitar el arma del hashmap de observadores
			_animEntity->deattachObjectFromBone(_leftHandBone, _weaponL.entity);

			std::shared_ptr<SET_ARMTOWEAPON> mSoltar (new SET_ARMTOWEAPON());
			mSoltar->setBool(false);
			mSoltar->setArm("Left");
			_weaponL.entity->emitMessage(mSoltar, this);

			//paramos la animación
			std::shared_ptr<STOP_ANIMATION> m(new STOP_ANIMATION());
			m->setAnimation("IdleR");
			m->setAnimationSet("LeftArm");
			_entity->emitMessage(m);

			//quitamos los giros al hombro y a la mano
			Quaternion q = Quaternion::IDENTITY;
			_animEntity->setBoneWorldOrientation(_leftHandBone, q);
			_animEntity->setBoneWorldOrientation(_leftArmBone, q);

		}

		if (_weaponR.entity)
		{
			// Enviamos mensaje para quitar el arma del hashmap de observadores
			_animEntity->deattachObjectFromBone(_rightHandBone, _weaponR.entity);

			std::shared_ptr<SET_ARMTOWEAPON> mSoltar (new SET_ARMTOWEAPON());
			mSoltar->setBool(false);
			mSoltar->setArm("Right");
			_weaponR.entity->emitMessage(mSoltar, this);

			//Paramos la animación
			std::shared_ptr<STOP_ANIMATION> m(new STOP_ANIMATION());
			m->setAnimation("IdleR");
			m->setAnimationSet("RightArm");
			_entity->emitMessage(m);

			//quitamos los giros al hombro y a la mano
			Quaternion q = Quaternion::IDENTITY;
			_animEntity->setBoneWorldOrientation(_rightHandBone, q);
			_animEntity->setBoneWorldOrientation(_rightArmBone, q);
		}
	}

	//------------------------------------------------------------

	void CArms::InitializeHandsBones(bool simetric)
	{
		//rotamos el hueso de la mano
		Quaternion q = Quaternion::IDENTITY;
		Quaternion qx = Quaternion(sqrt(0.5), -sqrt(0.5), 0, 0);
		Quaternion qy = Quaternion (sqrt(0.5), 0, -sqrt(0.5), 0);

		//al brazo derecho se le aplica un giro en el eje y y un giro en el eje x
		q = q * qy; //giro en el eje y del mundo y del bone
		q = q * qx; //giro en el eje x del mundo, z del bone
		
		_animEntity->setBoneWorldOrientation(_leftHandBone, q);

		if(simetric)
		{
			Quaternion qz = Quaternion (0, 0, 0, 1);
			//al brazo izquierdo además le alicamos un giro en el eje z
			q = q * qz; //giro en el eje z del mundo, x del bone
		}
		
		_animEntity->setBoneWorldOrientation(_rightHandBone, q);

		

	}

	void CArms::changeHandBoneSimetry(const std::string &hand, const Quaternion &qGiro)
	{
		//rotamos el hueso de la mano
		Quaternion q = Quaternion::IDENTITY;
		
		q = q * qGiro;
		if(hand == "Left")
		{
			_animEntity->setBoneLocalOrientation(_leftHandBone, q);
		}
		else
		{
			_animEntity->setBoneLocalOrientation(_rightHandBone, q);
		}
	}

	void CArms::setWeaponVisible(const std::string &arm, bool visible)
	{
		WeaponInArm *w;
		std::string hand = "";
		if(arm == "RightArm")
		{
			w = &_weaponR;
			hand = _rightHandBone;
		}
		else if(arm == "LeftArm")
		{
			w = &_weaponL;
			hand = _leftHandBone;
		}

		if(visible && !w->attached)//si no esta attachada y nos indican que la hagamos visible, la atachamos
		{
			_animEntity->attachObjectToBone(hand, w->entity);
			w->attached = true;
		}
		else if(!visible && w->attached)
		{
			_animEntity->deattachObjectFromBone(hand, w->entity);
			w->attached = false;
		}
	}

	//--------------------------------------------------------------------------------------

	void CArms::SendControl(std::string message, bool active)
	{
		if(active)
		{
			std::shared_ptr<Logic::CONTROL> m(new Logic::CONTROL());

			//enviamos un mensaje para forzar la animación de idle
			m->setString(message);
			_entity->emitMessage(m);
		}
		else
		{
			//enviamos el mensaje para que pare de contar
			std::shared_ptr<Logic::START_COUNT> m(new Logic::START_COUNT());
			m->setString(message);
			_entity->emitMessage(m);
		}
	}

} // namespace Logic