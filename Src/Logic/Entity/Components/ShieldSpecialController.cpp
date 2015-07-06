#include "ShieldSpecialController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "MeleeShield.h"

namespace Logic 
{
	IMP_FACTORY(CShieldSpecialController);
	 
	bool CShieldSpecialController::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxVelPong"))
			_maxVelPongOri = _maxVelPongX = _maxVelPongY = entityInfo->getFloatAttribute("maxVelPong");

		if(entityInfo->hasAttribute("accelMovApproach"))
		{
			_accelMovAppr = entityInfo->getFloatAttribute("accelMovApproach");
		}

		if(entityInfo->hasAttribute("maxVelApproach"))
		{
			_maxVelMovAppr = entityInfo->getFloatAttribute("maxVelApproach");
		}

		if(entityInfo->hasAttribute("minDistStopAppr"))
		{
			_minDistStopAppr = entityInfo->getIntAttribute("minDistStopAppr");
		}

		if(entityInfo->hasAttribute("maxHitsPong"))
		{
			_maxHitsPong = entityInfo->getIntAttribute("maxHitsPong");
		}

		if(entityInfo->hasAttribute("maxTimePong"))
		{
			_maxTimePong = entityInfo->getIntAttribute("maxTimePong");
		}

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

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CShieldSpecialController::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxVelPong"))
			_maxVelPongOri = _maxVelPongX = _maxVelPongY = entityInfo->getFloatAttribute("maxVelPong");

		if(entityInfo->hasAttribute("accelMovApproach"))
		{
			_accelMovAppr = entityInfo->getFloatAttribute("accelMovApproach");
		}

		if(entityInfo->hasAttribute("maxVelApproach"))
		{
			_maxVelMovAppr = entityInfo->getFloatAttribute("maxVelApproach");
		}

		if(entityInfo->hasAttribute("minDistStopAppr"))
		{
			_minDistStopAppr = entityInfo->getIntAttribute("minDistStopAppr");
		}

		if(entityInfo->hasAttribute("maxHitsPong"))
		{
			_maxHitsPong = entityInfo->getIntAttribute("maxHitsPong");
		}

		if(entityInfo->hasAttribute("maxTimePong"))
		{
			_maxTimePong = entityInfo->getIntAttribute("maxTimePong");
		}

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

		return true;
	}

	//---------------------------------------------------------

	bool CShieldSpecialController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "FALLING" ||
			   message->getType() == "UP_COLLISION" ||
			   message->getType() == "SIDE_COLLISION" ||
			   message->getType() == "TOUCHED";
	} // accept

	//---------------------------------------------------------

	void CShieldSpecialController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		// Solo realizará el cambio de dirección si el anterior cambio que hizo no fue el
		// mismo que va a realizar (de modo que no esté entrando constántemente al mismo y "tiemble)
		if (message->getType() == "FALLING" && _lastCollision != "DOWN")
		{
			bool isFalling = dynamic_cast<FALLING*>(message.get())->getBool();

			if (!isFalling)
			{
				_dirPong.y *= -1;
				_lastCollision = "DOWN";
				_numHitsPong++;

				///Lanzamos sonido de colision
				std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
				sound->setSound(std::string("/Armas/Escudo/ReboteEscudo"));
				_entity->emitMessage(sound, this);
			}
		}

		else if (message->getType() == "UP_COLLISION" && _lastCollision != "UP")
		{
			_dirPong.y *= -1;
			_lastCollision = "UP";
			_numHitsPong++;
			//BaseSubsystems::Log::Debug("UP");

			///Lanzamos sonido de colision
			std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
			sound->setSound(std::string("/Armas/Escudo/ReboteEscudo"));
			_entity->emitMessage(sound, this);
		}
		else if (message->getType() == "SIDE_COLLISION")
		{
			std::string thisCollision;

			int normal = dynamic_cast<SIDE_COLLISION*>(message.get())->getNormalCollision();
			//BaseSubsystems::Log::Debug("SIDE : " + std::to_string(normal));
			if (normal != 0)
			{
				if (normal == 1)
					thisCollision = "RIGHT";
				else if (normal == -1)
					thisCollision = "LEFT";

				if (thisCollision != _lastCollision)
				{
					_lastCollision = thisCollision;
					_dirPong.x *= -1;
					_numHitsPong++;
					///Lanzamos sonido de colision
					std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
					sound->setSound(std::string("/Armas/Escudo/ReboteEscudo"));
					_entity->emitMessage(sound, this);
				}
			}
		}

		if(message->getType().compare("TOUCHED") == 0)
		{
			CEntity* otherEntity = dynamic_cast<TOUCHED*>(message.get())->getEntidad();
			if( otherEntity != NULL)
			{
				//Si golpeo a un enemigo genero chispa roja, si golpeo la puerta la normal
				if(otherEntity->getTag() == "enemy")
				{
					CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoEnemy",_entity->getPosition(), _entity->getMap());
					
					std::shared_ptr<DAMAGED>m(new DAMAGED());
					m->setFloat(_damage);
					m->setString(_entity->getType());
					otherEntity->emitMessage(m, this);

					std::shared_ptr<PUSH> push (new PUSH());
					push->setDirection(_dirPong);
					push->setSpeed(0.6f);
					otherEntity->emitMessage(push, this);

					///Lanzamos sonido de colision
					std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
					sound->setSound(std::string("/Armas/Escudo/ReboteEscudo"));
					_entity->emitMessage(sound, this);

					sendSET_MATERIAL(_materialSangre);
					_timeAcum = 0;
					_countToClean = true;
				}
				else if( otherEntity->getTag() == "objetoRompible")
				{
					CEntityFactory::getSingletonPtr()->createEntityByType("Chispa",_entity->getPosition(), _entity->getMap());
					
					std::shared_ptr<DAMAGED>m(new DAMAGED());
					m->setFloat(_damage);
					m->setString(_entity->getType());
					otherEntity->emitMessage(m, this);

					///Lanzamos sonido de colision
					std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
					sound->setSound(std::string("/Armas/Escudo/ReboteEscudo"));
					_entity->emitMessage(sound, this);
				}
				
			}
			//Si golpeo un tile genera chispa normal
			else
			{
				CEntityFactory::getSingletonPtr()->createEntityByType("Chispa",_entity->getPosition(),_entity->getMap());
			}
		}

	} // process

	//---------------------------------------------------------

	CShieldSpecialController::~CShieldSpecialController()
	{
		
	} // ~Controller

	bool CShieldSpecialController::activate()
	{
		_angle = 0;
		_velPong = 0;
		_dirPong = 1;
		_numHitsPong = 0;
		_timePong = 0;
		_canPong = false;
		_canAppr = false;
		_lastCollision = "";
		_player = CServer::getSingletonPtr()->getPlayer();

		///seteamos el material por defecto
		sendSET_MATERIAL(_material);

		_timeAcum = 0;
		_countToClean = false;
	 
		return true;

	} // activate

	void CShieldSpecialController::deactivate()
	{
	} // deactivate

	void CShieldSpecialController::setShieldControllerConfiguration(Vector3 mousePos, float damage, CMeleeShield* shieldComponent)
	{
		_shieldComponent = shieldComponent;
		_damage = damage;
		_dirPong = (mousePos - _entity->getCenterPosition()) * Vector3(1,1,0);
		_dirPong.normalise();
		_canPong = true;
	
		// Reinicializamos los valores necesarios
		_numHitsPong = 0;
		_angle= 0 ;
		_timePong = 0;
		_canAppr = false;
		_lastCollision = "";

	}

	void CShieldSpecialController::onTick(unsigned int msecs)
	{
		if (_canPong)
		{
			_velPong = Vector3(_maxVelPongX, _maxVelPongY, 0)  * _dirPong * msecs;

			std::shared_ptr<AVATAR_WALK> m(new Logic::AVATAR_WALK());
			m->setVector3(_velPong);
			_entity->emitMessage(m);

			_timePong += msecs;

			if (_timePong >= _maxTimePong || _numHitsPong >= _maxHitsPong)
			{
				_canPong = false;
				_canAppr = true;
				std::shared_ptr<SET_PHYSIC_GROUP>m(new SET_PHYSIC_GROUP());
				m->setGroup(8);
				_entity->emitMessage(m);
			}
		}


		if (_canAppr)
		{
			_dirAppr = (_player->getCenterPosition() - _entity->getCenterPosition()) * Vector3(1,1,0); // No nos interesa el z
			
			float distance = _dirAppr.length();

			if (distance <= _minDistStopAppr)
			{
				_shieldComponent->setShieldVisible(true);
				_canAppr = false;
				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			}

			_dirAppr.normalise();
			
			_velFinalAppr = _maxVelMovAppr * _dirAppr * msecs; 

			std::shared_ptr<AVATAR_WALK> m(new AVATAR_WALK());
			m->setVector3(_velFinalAppr);
			_entity->emitMessage(m);
		}

		if(_countToClean)
		{
			_timeAcum += msecs;

			if(_timeAcum >= _cleanUpTime)
			{
				_countToClean = false;
				_timeAcum = 0;
				sendSET_MATERIAL(_material);
			}
		}

	} // tick


	void CShieldSpecialController::sendSET_MATERIAL(const std::string &material)
	{
		std::shared_ptr<SET_MATERIAL> m(new SET_MATERIAL());
		m->setString(material);
		_entity->emitMessage(m, this);
	}

} // namespace Logic

