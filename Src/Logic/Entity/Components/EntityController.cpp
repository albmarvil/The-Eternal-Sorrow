
#include "EntityController.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CEntityController);
	
	//---------------------------------------------------------

	bool CEntityController::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_volador = false;

		if(entityInfo->hasAttribute("maxFallingSpeed"))
		{
			_maxFallingSpeed = entityInfo->getFloatAttribute("maxFallingSpeed");
		}
		if(entityInfo->hasAttribute("initialJumpSpeed"))
		{
			_initialJumpSpeed = entityInfo->getFloatAttribute("initialJumpSpeed");
		}
		if(entityInfo->hasAttribute("jumpHeight"))
		{
			_jumpHeight = entityInfo->getFloatAttribute("jumpHeight");
		}
		if(entityInfo->hasAttribute("jump_number"))
		{
			_numJumpMax = entityInfo->getIntAttribute("jump_number");
			_numJump = _numJumpMax;
		}
		if(entityInfo->hasAttribute("gravityAcceleration"))
		{
			_gravityAcceleration = entityInfo->getFloatAttribute("gravityAcceleration");
		}
		if(entityInfo->hasAttribute("minJumpSpeed"))
		{
			_minJumpSpeed = entityInfo->getFloatAttribute("minJumpSpeed");
			_actualSpeed.x = _minJumpSpeed;
			_actualSpeed.y = _minJumpSpeed;
		}
		if(entityInfo->hasAttribute("lookingDirection"))
		{
			_lookingDirection = entityInfo->getIntAttribute("lookingDirection");
		}

		if(entityInfo->hasAttribute("volador"))
		{
			_volador = entityInfo->getBoolAttribute("volador");
		}

		//El giro dura 1 segundo
		_msecondsAnimationTurn = 1000;
		_degreeToTurn = 0;
		_turning = false;
		_acumDegree = 0;

		//Guardamos la orientiacion inicial, porque asi despues al hacerle respawn vuelve a su orientacion inicial
		_transformInicial = _entity->getOrientation();

		return true;

	} // spawn
	

	//---------------------------------------------------------

	bool CEntityController::respawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("maxFallingSpeed"))
		{
			_maxFallingSpeed = entityInfo->getFloatAttribute("maxFallingSpeed");
		}
		if(entityInfo->hasAttribute("initialJumpSpeed"))
		{
			_initialJumpSpeed = entityInfo->getFloatAttribute("initialJumpSpeed");
		}
		if(entityInfo->hasAttribute("jumpHeight"))
		{
			_jumpHeight = entityInfo->getFloatAttribute("jumpHeight");
		}
		if(entityInfo->hasAttribute("numJumpMax"))
		{
			_numJumpMax = entityInfo->getIntAttribute("numJumpMax");
			_numJump = _numJumpMax;
		}
		if(entityInfo->hasAttribute("gravityAcceleration"))
		{
			_gravityAcceleration = entityInfo->getFloatAttribute("gravityAcceleration");
		}
		if(entityInfo->hasAttribute("minJumpSpeed"))
		{
			_minJumpSpeed = entityInfo->getFloatAttribute("minJumpSpeed");
			_actualSpeed.x = _minJumpSpeed;
			_actualSpeed.y = _minJumpSpeed;
		}

		if(entityInfo->hasAttribute("lookingDirection"))
		{
			_lookingDirection = entityInfo->getIntAttribute("lookingDirection");
		}
		
		_turning = false;
		_acumDegree = 0;

		_entity->setOrientation(_transformInicial);

		return true;

	} // spawn
	

	//---------------------------------------------------------

	bool CEntityController::activate()
	{	
		return true;
	} // activate
	
	//---------------------------------------------------------

	void CEntityController::deactivate()
	{
		turn(1);  //hacemos esto para reestablecer la orientacion del enemigo antes de morir
	} // deactivate
	
	//---------------------------------------------------------

	bool CEntityController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "FALLING" || 
			   message->getType() == "TURN"    ||
			   message->getType() == "ENEMY_JUMP" ||
			   message->getType() == "UP_COLLISION";

	} // accept
	
	//---------------------------------------------------------

	void CEntityController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("FALLING") == 0)
		{
			falling(dynamic_cast<FALLING*>(message.get())->getBool());
		}
		else if(message->getType().compare("TURN") == 0)
		{
			turn(dynamic_cast<TURN*>(message.get())->getInt());
		}
		else if(message->getType() == "ENEMY_JUMP")
		{
			jump();
		}
		else if( message->getType().compare("UP_COLLISION") == 0)
		{
			upCollision( dynamic_cast<UP_COLLISION*>(message.get())->getBool());
		}

	} // process

	//---------------------------------------------------------

	void CEntityController::falling(bool falling)
	{
		_falling = falling;

		if(!_jump && falling)//Si no esta saltando, entonces esta cayendo
		{
			//Ponemos animación de correr al no tener caida
			//sendSET_ANIMATION("Run");
		}
		else if(!_jump && !falling)//al caer sobre el suelo cambiamos la animacion
		{
			_numJump = _numJumpMax; //ponemos el contador a tope de nuevo
			_direction.y = 0;
		}
	}

	void CEntityController::jump()
	{
		if (canJump()) // Si está en el aire no saltará
		{
			_direction.y = 1; // direccion del salto

			_maxJumpHeight = _entity->getPosition().y + _jumpHeight;
			_jump = true;
			--_numJump; //actualizamos el numero de saltos

			//Al iniciar el salto debemos setear la velocidad
			//se hace de forma instantánea ya que el personaje lleva una "fuerza" de impulso
			//Con el impulso llegará a una altura objetivo, a partir del cual decelera
			//(se resuelve en el tick)
			_actualSpeed.y = _initialJumpSpeed;

			//Ponemos animación de correr al no tener salto
			//sendSET_ANIMATION("Run");
		}
	}//jump

	//----------------------------------------------------------------

	void CEntityController::upCollision(bool up)
	{
		//cuando nos llega un mensaje de este tipo nos indica que colisionamos por la parte de arriba
		//Por lo tanto si estabamos saltando debemos interrumpir el salto
		_jump = false;
	}//upCollision

	//-------------------------------------------------------

	bool CEntityController::canJump()
	{
		return _numJump > 0;
	}//canJump

	//---------------------------------------------

	void CEntityController::turn(int look)
	{
		if(_lookingDirection != look)
		{
			//Si no es volador, giro como antes
			if(!_volador)
			{
				float rad = Math::fromDegreesToRadians(180);
				_entity->yaw(rad);
			}
			//Si es volador y no esta girando lo que hago es calcular los angulos de giro 
			else if(!_turning)
			{
				_degreeFinal = -120*_lookingDirection;
				_degreeToTurn = _degreeFinal / _msecondsAnimationTurn; //Calculamos cuantos grados debe girar cada msec
				_turning = true;
			}
			else if(_turning)
			{
				//Si hay cambio de giro mientras estaba girando, giramos por completo al personaje y comenzamos el giro nuevo
				_entity->yaw(Math::fromDegreesToRadians( _degreeFinal - _acumDegree));
				_degreeFinal = -120*_lookingDirection;
				_degreeToTurn = _degreeFinal / _msecondsAnimationTurn; //Calculamos cuantos grados debe girar cada msec
				_acumDegree = 0;
			}

			_lookingDirection = look;
		}
	}

	//---------------------------------------------------------

	void CEntityController::onTick(unsigned int msecs)
	{
		//Si esta girando
		if(_turning)
		{
			_acumDegree += msecs*_degreeToTurn; 

			//Si es mayor, paramos de girar, 
			if(abs(_acumDegree) >= abs(_degreeFinal))
			{
				_acumDegree -= msecs*_degreeToTurn; //Vuelvo al mismo valor de antes para obtener la diferencia con la final
				_turning = false;	
				_entity->yaw(Math::fromDegreesToRadians(_degreeFinal - _acumDegree));
				_acumDegree = 0;
			}		
			else
			{
				_entity->yaw(Math::fromDegreesToRadians(msecs*_degreeToTurn));
			}
		}

		#pragma region Actualizacion De Consecucion Del Salto Y Velociad
		if(_jump)
		{
			//Seguimos saltando, direccion positiva en y
			_direction.y = 1;
			//Si la entidad ha pasado la altura de salto, aplicamos deceleracion
			if(_entity->getPosition().y >= _maxJumpHeight)
			{
				//deceleramos
				_actualSpeed.y -= _gravityAcceleration * msecs;
				if(_actualSpeed.y <= _minJumpSpeed)//cuando deceleramos hasta estar quietos, finalizamos el salto
				{
					_actualSpeed.y = _minJumpSpeed;
					_jump = false;
				}
			}
		}
		else 
		{
			_maxJumpHeight = 0.0f;
			_direction.y = -1;
		}
		#pragma endregion

		//Aplicamos las reglas de velocidad de la caida
		#pragma region Aplicar Aceleracion A Velocidad En Caida
		
		if(_falling && !_jump)
		{
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



		//vector desplazamiento. La direccion ya viene normalizada	
		Vector3 desplazamiento = _direction * _actualSpeed * msecs;

		std::shared_ptr<AVATAR_WALK> m(new AVATAR_WALK());
		m->setVector3(desplazamiento);
		_entity->emitMessage(m);

	
	} // tick
	
	//--------------------------------------------------------------

} // namespace Logic

