
#ifndef __Logic_CEntityController_H
#define __Logic_CEntityController_H

#include "Logic/Entity/Component.h"

namespace Logic 
{

	class CEntityController : public IComponent
	{
		DEC_FACTORY(CEntityController);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CEntityController() : IComponent(), _falling(false), _jump(false),	_maxFallingSpeed(0.7), _actualSpeed(Vector3(0.0,0.0,0)),
			_direction(Vector3::ZERO), _gravityAcceleration(0.009), _lookingDirection(1), _minJumpSpeed(0.01),
			_jumpHeight(10), _numJump(1), _numJumpMax(1), _initialJumpSpeed(0.09){}
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se moverá el jugador.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);
		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra así mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y ratón.

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra así mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y ratón.
		*/
		virtual void deactivate();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Método virtual que elige que mensajes son aceptados. Son válidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Maneja el mensaje falling

		@param falling Booleano del mensaje que nos indica si esta en el aire o no
		*/
		void falling(bool falling);

		/*
		Setea la animación deseada. Lo hace comparando primero con la animacion actual, solo la cambia si son diferentes
		Optimizado para no enviar un mensaje innecesario en cada tick

		@param animacion Cadena de texto que indica a que animacion cambiar
		*/
		void sendSET_ANIMATION(char* animacion);

		/*
		Este método devuelve la dirección de movimiento actual
		*/
		int getDirActual() { return _lookingDirection; };

		/*
		Este método devuelve la velocidad de movimiento actual
		*/
		Vector3 getActualSpeed() { return _actualSpeed;};

		/*
		Este método setea una nueva velocidad de movimiento
		*/
		void setActualSpeed(Vector3 speed) { _actualSpeed = speed;};

		/*
		Método que controla la lógica de salto
		*/
		void jump();

		/*
		Prediado que nos indica cuando podemos saltar o no
		*/

		bool canJump();

		/*
		Método que controla las colisiones con el techo
		*/
		void upCollision(bool collision);

	protected:

		/**
		Metodo que cambia la dirección en la que está mirando la entidad para que la animación de andar sea la correcta
		*/
		void turn (int look);


		/**
		Atributo que almacena la última posición del ratón
		*/
		Vector2 moveMouse;

		/*
		Atributo que indica la magnitud de la velocidad en caida libre
		*/
		float _maxFallingSpeed;
				
		/*
		Atributo que indica la velocidad minima. _actualSpeed se inicializa con este parametro
		*/
		float _minJumpSpeed;
		/*
		Atributo que indica la velocidad instantanea de la entidad
		*/
		Vector3 _actualSpeed;
		/*

		/*
		Atributo que indica la direccion de la entidad
		*/
		Vector3 _direction;
		/*

		/**
		Atributo que indica la aceleracion de la gravedad de la entidad
		*/
		float _gravityAcceleration;

		/*
		Atributo para saber si la entidad esta en el aire
		true-> Esta en el aire 
		false-> Esta en tierra
		*/
		bool _falling;

		/**
		Variable que nos indica hacia donde esta mirando la entidad
			   *Izquierda= -1
			   *Derecha = 1
		*/
		int _lookingDirection;

		/*
		Variables para control de salto
		*/
		bool _jump;

		float _initialJumpSpeed;

		float _jumpHeight;

		float _maxJumpHeight;

		int _numJumpMax;

		int _numJump;

		int _msecondsAnimationTurn;
		float _degreeToTurn;
		bool _turning;
		float _acumDegree;
		float _degreeFinal;
		bool _volador;

		Matrix3 _transformInicial;
	}; // class CAvatarController

	REG_FACTORY(CEntityController);

} // namespace Logic

#endif // __Logic_AvatarController_H
