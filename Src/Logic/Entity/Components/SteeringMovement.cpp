/**
@file SteeringMovement.cpp

Contiene la declaraci�n de la clase CSteringMovement. Se encarga de
mover un avatar utilizando movimientos cin�ticos o din�micos.

@author Gonzalo Fl�rez
@date Diciembre, 2010
*/
#include "SteeringMovement.h"

#include "Map/MapEntity.h"

#include "AI/Movement.h"

#include "LUA/ScriptManager.h"

#include "AnimatedGraphics.h"

namespace Logic 
{

	IMP_FACTORY(CSteeringMovement);

	//---------------------------------------------------------
	/**
	Destructor
	*/
	CSteeringMovement::~CSteeringMovement()
	{
		if (_currentMovement != 0)
			delete _currentMovement;
		if (_yaw != 0)
			delete _yaw;
		if (_evade != NULL)
			delete _evade;
	}

	//---------------------------------------------------------
	/**
	Inicializaci�n del componente, utilizando la informaci�n extra�da de
	la entidad le�da del mapa (Maps::CEntity). Toma del mapa los atributos
	speed, angularSpeed, accel y angularAccel, que indican los valores m�ximos 
	de velocidad y aceleraci�n lineales y angulares. El atributo tolerance indica
	la distancia a la que se considera que se ha alcanzado una posici�n.

	@param entity Entidad a la que pertenece el componente.
	@param map Mapa L�gico en el que se registrar� el objeto.
	@param entityInfo Informaci�n de construcci�n del objeto le�do del
	fichero de disco.
	@return Cierto si la inicializaci�n ha sido satisfactoria.
	*/
	bool CSteeringMovement::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("speed"))
			_maxLinearSpeed = entityInfo->getFloatAttribute("speed");
		if(entityInfo->hasAttribute("angularSpeed"))
			_maxAngularSpeed = entityInfo->getFloatAttribute("angularSpeed");
		if(entityInfo->hasAttribute("accel"))
			_maxLinearAccel = entityInfo->getFloatAttribute("accel");
		if(entityInfo->hasAttribute("angularAccel"))
			_maxAngularAccel = entityInfo->getFloatAttribute("angularAccel");
		if (entityInfo->hasAttribute("movementTolerance"))
			_currentProperties.tolerance =  entityInfo->getFloatAttribute("movementTolerance");
		if (entityInfo->hasAttribute("slowdownMovement"))
			_currentProperties.slowdownFactor =  entityInfo->getFloatAttribute("slowdownMovement");

		// En yaw guardamos el movimiento correspondiente a la rotaci�n de la entidad.
		// En este caso suponemos que la entidad siempre se mueve hacia adelante, 
		// as� que hacemos que siempre est� alineado con su vector de velocidad.

		_yaw = AI::IMovement::getMovement(AI::IMovement::MOVEMENT_KINEMATIC_ALIGN_TO_SPEED, 
			_maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel); 

		_yaw->setEntity(_entity);

		// Creaci�n del comportamiento para esquivar la torreta. 
		// OJO: todo esto est� muy cableado. En condiciones normales habr�a que decidir junto a la percepci�n
		// qu� entidades hay que esquivar
		_evade = new AI::CDynamicFlee(_maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel);

		if (_evade != NULL) 
		{
			_evade->setEntity(_entity);
			// Ponemos el target a esquivar
			_evade->setTarget(Vector3(0, 0, 100));
			_evadeDistance = 15;
		}
		return true;

	} // spawn

	//---------------------------------------------------------
	/**
	M�todo llamado en cada frame que actualiza el estado del componente.
	<p>
	Se encarga de mover la entidad en cada vuelta de ciclo cuando es necesario.

	@param msecs Milisegundos transcurridos desde el �ltimo tick.
	*/
	void CSteeringMovement::onTick(unsigned int msecs)
	{
		// En este m�todo se realiza un movimiento b�sico usando un steering movement
		// El steering movement calcula las velocidades y aceleraciones lineales y angulares y,
		// con esta informaci�n se env�an mensajes a la f�sica, el avatar controller y las animaciones
		
		// Variables �tiles:
		// _maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel: velocidades y aceleraciones lineales y angulares.
		// _tolerance: distancia a la que se considera que se ha llegado al punto de destino.
		// _arrived: �ha llegado al destino?
		// _currentTarget: punto de destino del movimiento actual.
		// _target: nuevo punto de destino que se ha establecido en el m�todo process, pero que a�n no es el actual.
		// _currentMovementType, _movType: lo mismo con el tipo de movimiento. Son valores sacados del enumerado IMovement::EnmMovementType 
		// _currentMovement: instancia de IMovement correspondiente al movimiento actual.
		// _yaw: instancia de IMovement correspondiente al movimiento de giro actual. De momento s�lo hay un posible valor
		//		(MOVEMENT_KINEMATIC_ALIGN_TO_SPEED) que establecemos en el spawn.
		// _currentProperties: esta variable sirve de entrada y de salida para los steering behaviours. En ella se almacena la informaci�n
		//		de velocidad entre los ticks.

		// En cada tick tenemos que:
		// Comprobar si hay que cambiar de movimiento o destino
		//		Sacar el nuevo movimiento de la factor�a (puede ser NULL ==> parado)
		//		Si el movimiento es distinto de null (que ser�a parar)
		//			Asignarle el nuevo destino y la entidad al movimiento
		//			Actualizar la variable _arrived para indicar que no hemos llegado a destino
		//			Enviar un mensaje para cambiar la animaci�n a "Walk"
		//		Si el movimiento es null cambiar la animaci�n a "Idle"

		// Comprobamos si hemos llegado al destino
		// Podemos usar positionEquals (IMPORTANTE: usar la _tolerance)
		//		Si es as�, enviar un mensaje FINISHED_MOVE para indicar que hemos llegado

		// Si tenemos que movernos (no hemos llegado al destino y nuestro movimiento no es NULL)
		//		Invocar al m�todo move del movimiento actual. 
		//		Le pasamos la variable _currentProperties, que contiene las velocidades actuales y en la que recogemos las nuevas velocidades y aceleraciones
		//		Calcular la nueva posici�n y notificarla a la f�sica para que nos mueva (mensaje AVATAR_WALK)
		//		Calcular la nueva rotaci�n y notificarla al AvatarController para que nos gire (mensaje CONTROL)
		//		Actualizar las velocidades usando la aceleraci�n que hemos obtenido en _currentProperties
		//		Las velocidades que tenemos que actualizar tambi�n est�n en _currentProperties para pas�rselas en el siguiente tick al movimiento


		// Comprobamos si hay que cambiar de movimiento o destino
		if (_movType != _currentMovementType || _currentTarget != _target) 
		{
			if (_currentMovement != NULL) 
			{
				delete _currentMovement;
			}

			_currentMovementType = _movType;
			_currentTarget = _target;
			_currentMovement = AI::IMovement::getMovement(_currentMovementType, _maxLinearSpeed, _maxAngularSpeed, _maxLinearAccel, _maxAngularAccel);
		
			// Si hay un nuevo movimiento
			if (_currentMovement != NULL) 
			{
				// Ajustamos el target
				_arrived = false;
				_currentMovement->setEntity(_entity);
				_currentMovement->setTarget(_currentTarget);
				// Y la animaci�n
				sendAnimationMessage("Walk", true, 0);
			} 
			else 
			{
				// Si no hay movimiento paramos la animaci�n
				sendAnimationMessage("Walk", true, 0);
			}
		}

		bool justArrived = (!_arrived && _entity->getPosition().positionEquals(_target, _currentProperties.tolerance)) || 
						   (_currentMovementType == AI::IMovement::MOVEMENT_KINEMATIC_STOP && _currentProperties.linearSpeed.positionEquals(Vector3::ZERO, _currentProperties.tolerance));

		//std::cout << "JustArrived: "<< justArrived <<  ", TipoMov: " << _currentMovementType << std::endl;
		//std::cout << "Vel: " << _currentProperties.linearSpeed << std::endl;
		//std::cout << "----------------" << std::endl;

		if (justArrived) 
		{
			// Enviar un mensaje para informar de que hemos llegado a destino
			_arrived = true;
			std::shared_ptr<Logic::FINISHED_MOVE> m(new Logic::FINISHED_MOVE());
			m->setBool(true);
			_entity->emitMessage(m);
		}

		// Si nos estamos desplazando calculamos la pr�xima posici�n
		if (!_arrived && _currentMovement != NULL) 
		{
			_currentMovement->move(msecs, _currentProperties);
			
			//// Calculamos la evitaci�n
			//if (_evade != NULL && _evadeDistance > 0)
			//{
			//	AI::IMovement::DynamicMovement evadeProperties;
			//	_evade->move(msecs, evadeProperties);
			//	// Distancia al objeto a evadir
			//	Vector3 position = this->getEntity()->getPosition();
			//	Vector3 distance = _evade->getTarget() - position;
			//	float distanceMagnitude = distance.length();
			//	if (distanceMagnitude <= _evadeDistance)
			//	{
			//		_currentProperties.linearAccel += 0.15f * evadeProperties.linearAccel;
			//	}
			//}
	
			// Calculamos la rotaci�n
			_yaw->move(msecs, _currentProperties);

			//_currentProperties.linearSpeed.y -= 0.05;

			// Enviar un mensaje para que el componente f�sico mueva el personaje
			std::shared_ptr<Logic::AVATAR_WALK> m(new Logic::AVATAR_WALK());
			m->setVector3(_currentProperties.linearSpeed * msecs);
			_entity->emitMessage(m);
			
			// Aplicar la rotaci�n 
			/*std::shared_ptr<Logic::CONTROL2> m2(new Logic::CONTROL2());
			m2->setString("turn");
			m2->setFloat(_currentProperties.angularSpeed * msecs);
			_entity->emitMessage(m2);*/

			// Acelerar
			_currentProperties.linearSpeed += _currentProperties.linearAccel * msecs;

			// Clipping
			double speedValue = _currentProperties.linearSpeed.length();
			if (speedValue > _maxLinearSpeed) 
			{
				_currentProperties.linearSpeed *= (_maxLinearSpeed / speedValue);
			}

			_currentProperties.angularSpeed += _currentProperties.angularAccel * msecs;

			if (_currentProperties.angularSpeed > _maxAngularSpeed) 
			{
				_currentProperties.angularSpeed = Ogre::Math::Sign(_currentProperties.angularSpeed) * _maxAngularSpeed;
			}
		}

	} // tick

	//---------------------------------------------------------
	/**
	Env�a un mensaje para cambiar la animaci�n actual.

	@param animation Nueva animaci�n.
	*/
	void CSteeringMovement::sendAnimationMessage(std::string animation, bool loop, float duration) 
	{
		// Creamos y enviamos un mensaje indicando que cambie de animaci�n y ponga o no loop
		/*std::shared_ptr<Logic::SET_ANIMATION> m(new Logic::SET_ANIMATION());
		m->setAnimation(animation);
		m->setLoop(loop);
		m->setDuration(duration);
		_entity->emitMessage(m, this);*/

		CAnimatedGraphics* cmp = (CAnimatedGraphics*)_entity->getComponent("CAnimatedGraphics");
		cmp->setAnimation(animation, "default", loop, false, duration);

	}

	//---------------------------------------------------------
	/**
	Este componente s�lo acepta mensajes de tipo MOVE_TO.
	*/
	bool CSteeringMovement::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "MOVE_TO";

	} // accept
	
	//---------------------------------------------------------

	/**
	Si se recibe un mensaje de tipo MOVE_TO, almacena el vector como 
	destino del movimiento y el tipo de movimiento.
	*/
	void CSteeringMovement::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("MOVE_TO") == 0)
		{
			// Anotamos el vector de desplazamiento para usarlo posteriormente en 
			// el m�todo tick. De esa forma, si recibimos varios mensajes AVATAR_MOVE
			// en el mismo ciclo s�lo tendremos en cuenta el �ltimo.
			Logic::MOVE_TO* m = dynamic_cast<MOVE_TO*>(message.get()); // Alias para ahorrar operaciones

			_target = m->getVector3();
			_movType = m->getInt();
		}
	} // process

	//---------------------------------------------------------

}