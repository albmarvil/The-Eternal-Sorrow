/**
@file AvatarController.cpp

Contiene la implementación del componente que controla el movimiento 
de la entidad.
 
@see Logic::CAvatarController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/


#include "AvatarController.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Map/TipoTile.h"
#include "LUA/ScriptManager.h"
#include "Logic/Maps/Map.h"
#include "Logic/Maps/EntityFactory.h"

#include "AnimatedGraphics.h"
#include "PhysicController.h"

namespace Logic 
{
	IMP_FACTORY(CAvatarController);
	
	//---------------------------------------------------------

	bool CAvatarController::OnSpawn(const Map::CEntity *entityInfo) 
	{	
		if(entityInfo->hasAttribute("initialJumpSpeed"))
		{
			_initialJumpSpeed = entityInfo->getFloatAttribute("initialJumpSpeed");
		}
		if(entityInfo->hasAttribute("maxFallingSpeed"))
		{
			_maxFallingSpeed = entityInfo->getFloatAttribute("maxFallingSpeed");
		}
		if(entityInfo->hasAttribute("jumpHeight"))
		{
			_jumpHeight = entityInfo->getFloatAttribute("jumpHeight");
		}
		if(entityInfo->hasAttribute("numJumpMax"))
		{
			//cogemos el número de saltos máximo, que corresponderá al contador de la habilidad CAumentoUnSalto + 2
			std::string nombre = LUA::CScriptManager::GetPtrSingleton()->getField("Nombre_Habilidad", "CAumentoUnSalto", "");
			LUA::CScriptManager::GetPtrSingleton()->executeFunction("HadHability", nombre, _numJumpMax);

			_numJumpMax += entityInfo->getIntAttribute("numJumpMax");
			//Si el número máximo de saltos es menor que el número total de saltos recogidos modificamos su valor

			_numJump = _numJumpMax;
		}
		if(entityInfo->hasAttribute("turnRight"))
		{
			_lookingDirection = entityInfo->getIntAttribute("lookingDirection");
		}
		if(entityInfo->hasAttribute("minJumpSpeed"))
		{
			_minJumpSpeed = entityInfo->getFloatAttribute("minJumpSpeed");
			_actualSpeed.x = 0;
			_actualSpeed.y = _minJumpSpeed;
		}
		if(entityInfo->hasAttribute("horizontalAcceleration"))
		{
			_baseHorizAccel = entityInfo->getFloatAttribute("horizontalAcceleration");
			_horizontalAcceleration = _baseHorizAccel;
		}
		if(entityInfo->hasAttribute("gravityAcceleration"))
		{
			_gravityAcceleration = entityInfo->getFloatAttribute("gravityAcceleration");
		}
		if(entityInfo->hasAttribute("animation_config"))
		{
			_animationConfig = entityInfo->getStringAttribute("animation_config");
		}

		return true;

	} // spawn
	

	//---------------------------------------------------------

	bool CAvatarController::activate()
	{	
		CAnimatedGraphics* componente = (CAnimatedGraphics*)_entity->getComponent("CAnimatedGraphics");
		if(componente != NULL)
		{
			componente->getAnimatedEntity()->setObserver(this, "Jump");
			componente->getAnimatedEntity()->setObserver(this, "Fall");
			//componente->getAnimatedEntity()->setObserver(this, "Air");
		}

		_physicController = (CPhysicController*) _entity->getComponent("CPhysicController");

		assert(_physicController);

		_logicMap = CServer::getSingletonPtr()->getMap();

		_fallingTicks = 0;
		_horizontalAcceleration = _baseHorizAccel;
		_maxHorizontalSpeed = _baseMaxHorizSpeed;
		_allowJump = false;

		return true;
	} // activate
	
	//---------------------------------------------------------

	void CAvatarController::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool CAvatarController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CONTROL" ||
			   message->getType() == "FALLING" ||
			   message->getType() == "UP_COLLISION"||
			   message->getType() == "SIDE_COLLISION"||
			   message->getType() == "AUMENTAR_SALTO" ||
			   message->getType() == "SET_STAT" ||
			   message->getType() == "SET_VEL_MULT";
	} // accept
	
	//---------------------------------------------------------

	void CAvatarController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("CONTROL") == 0)
		{
			std::string control = dynamic_cast<CONTROL*>(message.get())->getString();
			if(!control.compare("walk"))
				walk(dynamic_cast<CONTROL*>(message.get())->getInt());
			else if(!control.compare("stopWalk"))
				stopWalk();
			else if(!control.compare("turn"))
				turn(dynamic_cast<CONTROL*>(message.get())->getInt());
			else if(!control.compare("jump"))
				jump(dynamic_cast<CONTROL*>(message.get())->getBool());
		}
		else if(message->getType().compare("FALLING") == 0)
		{
			falling(dynamic_cast<FALLING*>(message.get())->getBool());
		}
		else if( message->getType().compare("UP_COLLISION") == 0)
		{
			upCollision( dynamic_cast<UP_COLLISION*>(message.get())->getBool());
		}
		else if( message->getType().compare("SIDE_COLLISION") == 0)
		{
			sideCollision( dynamic_cast<SIDE_COLLISION*>(message.get())->getBool(), dynamic_cast<SIDE_COLLISION*>(message.get())->getNormalCollision());
		}
		else if(message->getType().compare("AUMENTAR_SALTO") == 0)
		{
			_numJumpMax += dynamic_cast<AUMENTAR_SALTO*>(message.get())->getInt();
		}
		else if(message->getType().compare("SET_STAT") == 0)
		{
			std::string typeHability = dynamic_cast<SET_STAT*>(message.get())->getString();

			if(typeHability.compare("velocidad_movimiento") == 0)
			{
				_baseMaxHorizSpeed = dynamic_cast<SET_STAT*>(message.get())->getFloat();
				_maxHorizontalSpeed = _baseMaxHorizSpeed;
			}
		}

		else if(message->getType().compare("SET_VEL_MULT") == 0)
		{
			_velMult = dynamic_cast<SET_VEL_MULT*>(message.get())->getFloat();
		}

	} // process
	
	//---------------------------------------------------------

	void CAvatarController::turn(int look)
	{
		if(_lookingDirection != look && !_isSleeping)
		{
			float rad = Math::fromDegreesToRadians(180);
			_entity->yaw(rad);
			_lookingDirection = look;

			if(_walking && !_sideCollision)
			{
				if(_animActual.compare("Walk")==0)
				{
					sendSET_ANIMATION("Walkback", true, false,  (_baseMaxHorizSpeed * 1.0f) * 5);
				}
				else if(_animActual.compare("Walkback") == 0)
				{
					sendSET_ANIMATION("Walk", true, false, (_baseMaxHorizSpeed * 2.0f) * 5);
				}
			}

			//enviamos el mensaje para que pare de contar
			std::shared_ptr<Logic::START_COUNT> m(new Logic::START_COUNT());
			m->setString("turn");
			_entity->emitMessage(m);
		}
	}

	//---------------------------------------------------------

	int CAvatarController::getLookingDirection()
	{
		return _lookingDirection;
	}
	//---------------------------------------------------------

	void CAvatarController::walk(int dir) 
	{
		_walking = true;

		_direction.x = dir;

		//std::string nAnim;
		char *nAnim;

		float speed;

		//OJO!! Si estamos en el aire o saltando y nos movemos, esto machaca las animaciones
		//de salto o de caida. por eso dejamos decidir el giro, pero no la animación. 
		//Prevalece siempre la animación de salto o de caida

		//Si mira y avanza hacia la misma direccion entonces animacion "walk"
		if (dir == _lookingDirection)
		{	
			nAnim = "Walk";

			//El speed de la animación de walk dependerá de la velocidad de movimiento del player.
			//La velocidad de movimiento base es 0.2 y su speed correspondiente es 2.0f. Para calcular
			//el speed de la animación en relación a la velocidad de movimiento hacemos una regla de 3
			speed = (_baseMaxHorizSpeed * 2.2f) * 5; //1/0.2 = 5. Se hace la multiplicación por ser más eficiente que la resta
		}
		else
		{
			nAnim = "Walkback";

			//De la misma forma calculamos el speed correspondiente para la animación de andar hacia atrás
			speed = (_baseMaxHorizSpeed * 1.7f) * 5;
		}

		// Cambiamos la animación
		if(!_jump && !_falling && !_sideCollision)
		{
			sendSET_ANIMATION(nAnim, true, false, speed);
		}

	} // walk
	
	//---------------------------------------------------------

	void CAvatarController::stopWalk() 
	{
		_walking = false;

	} // stopWalk
	
	//---------------------------------------------------------

	void CAvatarController::jump(bool salto)
	{
		// Si le quedan saltos y ha pulsado la tecla de Salto, lo iniciará
		if (_numJump > 0 && salto)
		{
			_maxJumpHeight = _entity->getPosition().y + _jumpHeight;

			_direction.y = 1; // Direccion hacia arriba
			_jump = true;
			--_numJump;

			if (_falling)
				CEntityFactory::getSingletonPtr()->createEntityByType("SaltoAire",_entity->getPosition(), _entity->getMap());

			// Al iniciar el salto debemos setear la velocidad de salto inicial,
			// es decir, la fuerza que impulsa inicialmente al player
			_actualSpeed.y = _initialJumpSpeed;

			sendSET_ANIMATION("Jump", false, true, 1.0);

			//Activamos el trail de salto
			std::shared_ptr<SET_TRAIL_LENGTH> mt(new SET_TRAIL_LENGTH());
			mt->setLength(60);
			_entity->emitMessage(mt, this);

			std::shared_ptr<SET_TRAIL_VISIBLE> m(new SET_TRAIL_VISIBLE());
			m->setVisible(true);
			_entity->emitMessage(m, this);
		}

		// Si ya ha saltado y suelta la tecla de saltar, interrumpiremos el salto
		else if(_jump && !salto) 
		{
			_maxJumpHeight = _entity->getPosition().y;
		}
	}

	//---------------------------------------------------------

	void CAvatarController::falling(bool falling)
	{
		_falling = falling;

		if(!_jump && falling)//Si no esta saltando, entonces esta cayendo
		{
			//Ponemos animación de caida en el aire
			++_fallingTicks;
	
			if(_fallingTicks >= 2)
			{
				sendSET_ANIMATION("Fall", true, false, 1.0);

				std::shared_ptr<SET_TRAIL_LENGTH> mt(new SET_TRAIL_LENGTH());
				mt->setLength(60);
				_entity->emitMessage(mt, this);

				std::shared_ptr<SET_TRAIL_VISIBLE> m(new SET_TRAIL_VISIBLE());
				m->setVisible(true);
				_entity->emitMessage(m, this);
			}
		}
		else if(!_jump && !falling && _allowJump) //Si no estamos cayendo, estamos en el suelo
		{
			_numJump = _numJumpMax; //ponemos el contador a tope de nuevo
			_direction.y = 0;

			if(_walking)
			{
				walk(_direction.x);
			}
			else
			{
				stopWalk();
			}

			//tambien seteamos la velocidad al minimo
			_actualSpeed.y = _minJumpSpeed;

			//FIN DE SALTO. Apagamos el trail

			std::shared_ptr<SET_TRAIL_LENGTH> mt(new SET_TRAIL_LENGTH());
			mt->setLength(0);
			_entity->emitMessage(mt, this);

			std::shared_ptr<SET_TRAIL_VISIBLE> m(new SET_TRAIL_VISIBLE());
			m->setVisible(false);
			_entity->emitMessage(m, this);
		}
	}

	//---------------------------------------------------------

	bool CAvatarController::canJump()
	{
		return _numJump > 0;
	}

	//---------------------------------------------------------

	void CAvatarController::upCollision(bool up)
	{
		//cuando nos llega un mensaje de este tipo nos indica que colisionamos por la parte de arriba
		//Por lo tanto si estabamos saltando debemos interrumpir el salto
		//BaseSubsystems::Log::Debug("Choco contra el techo");
		_jump = false;
		_actualSpeed.y = 0;
	}

	//---------------------------------------------------------

	void CAvatarController::sideCollision(bool side, int normal)
	{
		//cuando nos llega un mensaje de este tipo nos indica que colisionamos por uno de los laterales
		//Por lo tanto si tenemos algun tipo de velocidad horizontal debemos cancelarla y cancelar el movimiento
		
		//BaseSubsystems::Log::Debug("Normal: "+std::to_string(normal)+" Direction.x: "+std::to_string(_direction.x));

		if (!_falling && side) //si no estamos en caida paramos el movimiento
		{
			_actualSpeed.x = 0;

			if(normal != _direction.x)
			{
				_sideCollision = true;
				sendSET_ANIMATION("Idle", true, false, 1.0);
			}
		}
		else
		{
			_sideCollision = false;
		}
	}

	//---------------------------------------------------------

	void CAvatarController::onTick(unsigned int msecs)
	{		_horizontalAcceleration = _baseHorizAccel;
		_maxHorizontalSpeed = _baseMaxHorizSpeed;
		_allowJump = true;

		if (!_falling)
		{
			int idTile = _logicMap->getTileColision(_entity->getPosition() - Vector3(0,20,0));

			if (idTile == Map::TipoTile::TILE_ICE)
			{
				_horizontalAcceleration = _baseHorizAccel * 0.27;
				_maxHorizontalSpeed = _baseMaxHorizSpeed * 1.5;
			}

			else if (idTile == Map::TipoTile::TILE_GUM)
			{
				_maxHorizontalSpeed = _baseMaxHorizSpeed * 0.25;
				_allowJump = false;
				_numJump = 0;
			}
			
		}

		#pragma region Movimiento Horizontal

		if(_walking) 
		{
			//Si es la misma direccion que la anterior
			if(_lastDir == _direction.x)
			{
				//Incrementamos la velocidad solo si no hemos llegado al límite
				if(abs(_actualSpeed.x) < _maxHorizontalSpeed) 
				{
					_actualSpeed.x += _horizontalAcceleration * _direction.x * msecs;
				}
				else // Si no, establecemos la actual como la máxima
				{
					_actualSpeed.x = _direction.x * _maxHorizontalSpeed;
				}
			}
			//Si no es la misma direccion, entonces vamos frenando segun la direccion anterior y no la actual
			else
			{
				if(abs(_actualSpeed.x) > 0)
				{
					_actualSpeed.x -= _horizontalAcceleration * _lastDir* msecs;

					// Cuando frenemos, cambiamos el sentido de movimiento
					if(_actualSpeed.x * _lastDir < 0) 
					{
						_actualSpeed.x = 0;
					}
				}

				_lastDir = _direction.x;
			}
		}
		else 
		{
			if(abs(_actualSpeed.x) > 0)
			{
				_actualSpeed.x -= _horizontalAcceleration * _direction.x* msecs;

				// Cuando frenemos, cambiamos el sentido de movimiento
				if(_actualSpeed.x * _direction.x < 0) 
				{
					_direction.x *= -1;
					_actualSpeed.x = 0;
				}
			}
			else
			{
				//En este caso estamos quietos
				if(!_jump && !_falling /*&& _animActual != "Falling"*/)
				{
					sendSET_ANIMATION("Idle", true, false, 1.0);
				}
			}
		}

		
		#pragma endregion

		#pragma region Salto
		/*
		Aqui resolvemos si hemos completado un salto y que hacer con la velocidad del mismo.
		Si hemos llegado a la altura máxima, aplicaremos la aceleracion negativa de la gravedad.
		Cuando el personaje se quede "quieto" consideraremos que finaliza el salto.
		Entonces se le aplican las reglas de aceleracion y velocidad respectivas a "caer"
		*/
		
		if(_jump)
		{
			//Si la entidad ha pasado la altura de salto, aplicamos deceleracion
			if(_entity->getPosition().y >= _maxJumpHeight)
			{
				_actualSpeed.y -= _gravityAcceleration * msecs; //Deceleramos con cierta gravedad

				// Cuando deceleremos hasta un mínimo, nos paramos
				if(_actualSpeed.y <= _minJumpSpeed) 
				{
					_actualSpeed.y = _minJumpSpeed;
					_jump = false;

					//FIN DE SALTO. Apagamos el trail
					/*std::shared_ptr<SET_TRAIL_VISIBLE> m(new SET_TRAIL_VISIBLE());
					m->setVisible(false);
					_entity->emitMessage(m, this);*/
				}
			}
		}
		else 
		{
			_maxJumpHeight = 0.0f;
			_direction.y = -1;
		}
		#pragma endregion

		#pragma region Caída

		//Aplicamos las reglas de velocidad de la caida
		if(_falling && !_jump)
		{
			// Si la velocidad que llevamos es menor que la máxima de caída, aceleramos
			if(_actualSpeed.y < _maxFallingSpeed)
			{
				_actualSpeed.y += _gravityAcceleration * msecs;
			}
			else
			{
				_actualSpeed.y = _maxFallingSpeed;
			}
		}

		#pragma endregion

		#pragma region Cálculo de movimiento resultante

		// En el caso de las X la velocidad ya lleva su signo, no debe ser multiplicada por la direcciñon
		// En el caso de las Y SI debe ser multiplicada por la dirección, para evitar la vibración de caída constante.
		// Z = 0
		Vector3 desplazamiento = Vector3( _actualSpeed.x * msecs, _direction.y * _actualSpeed.y * msecs, 0 ) * _velMult;
	
		//La  direccion ya viene normalizada		
		/*std::shared_ptr<AVATAR_WALK> m(new AVATAR_WALK());
		m->setVector3(desplazamiento);
		_entity->emitMessage(m);*/
		_physicController->avatarWalk(desplazamiento);
		#pragma endregion

	} // tick

	void CAvatarController::sendSET_ANIMATION(char *animacion, bool loop, bool restart, float speed)
	{
		//Solo mandamos el mensaje si la animacion es diferente
		if(_animActual.compare(animacion) != 0 /*|| animacion == "Jump"*/)
		{
			float duration = 0;

			if(!LUA::CScriptManager::GetPtrSingleton()->executeFunction("getTransitionTime", _animationConfig, _animActual, animacion, duration))
			{
				duration = 0;
			}

			//std::shared_ptr<SET_ANIMATION> m(new SET_ANIMATION());
			//m->setAnimation(animacion);
			//m->setLoop(loop);
			//m->setDuration(duration);
			//m->setRestartFlag(restart);
			//m->setSpeed(speed);
			////no seteamos el conjunto, optamos por el por defecto
			//_entity->emitMessage(m,this);

			CAnimatedGraphics* cmp = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");
			cmp->setAnimation(animacion, "default", loop, restart, duration, speed);

			_animActual = animacion;
		}
	}//sendSET_ANIMACION
	
	//--------------------------------------------------------------

	void CAvatarController::animationFinished(const std::string &animation)
	{
		if (animation == "Jump")
		{
			sendSET_ANIMATION("Fall", true, true, 1.0);
		}
		
		/*if(animation == "Fall")
		{
			if(_walking)
			{
				if (_direction.x == _lookingDirection)
				{	
					sendSET_ANIMATION("Walk", true, false, 1.0);
				}
				else
				{
					sendSET_ANIMATION("Walkback", true, false, 1.0);
			
				}
			}
			else
			{
				sendSET_ANIMATION("Idle", true, false, 1.0);
			}
		}*/
	}


	void CAvatarController::movePlayerAtDoor()
	{
		_actualSpeed = Vector3::ZERO;
		Vector3 desp = Vector3(-_lastDir*2,10,0);
		//BaseSubsystems::Log::Error("Desplazamiento al abrir puerta: " + std::to_string(desp.x) +", "+ std::to_string(desp.y));
		falling(true);
		_physicController->avatarWalk(desp);
	}

} // namespace Logic

