/**
@file AvatarController.h

Contiene la declaración del componente que controla el movimiento 
de la entidad.

@see Logic::CAvatarController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#ifndef __Logic_AvatarController_H
#define __Logic_AvatarController_H

#include "Logic/Entity/Component.h"
#include "Graphics/AnimatedEntity.h"


namespace Logic
{
	class CPhysicController;
};

//declaración de la clase
namespace Logic 
{


/**
	Este componente es el encargado de mover a una entidad animada. Tiene
	diferentes métodos que permiten avances o giros. El uso de este componente
	es un poco atípico ya que se puede registrar en otro controlador externo
	(i.e. GUI::CPlayerController) que sea el que de las órdenes que se deben
	llevar a cabo mediante llamadas a métodos públicos del componente. Puede
	no obstante ampliarse este componente para aceptar mensajes tradicionales
	con las órdenes, sin embargo de momento así es suficiente.
	
	@ingroup logicGroup

	@author David Llansó García
	@date Agosto, 2010
*/
	class CAvatarController : public IComponent, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CAvatarController);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAvatarController() : IComponent(),_physicController(0), _walking(false), _jump(false), _falling(false), _allowJump(false),
			_jumpHeight(10), _numJump(1), _numJumpMax(1), _maxHorizontalSpeed(0.07), _initialJumpSpeed(0.09),
			_maxFallingSpeed(0.7), 
			_actualSpeed(Vector3(0.01,0.01,0)), _minJumpSpeed(0.01), _horizontalAcceleration(0.001),
			_gravityAcceleration(0.009),
			_animActual (""), _animationConfig(""),_direction(Vector3::ZERO), _lookingDirection(-1), _fallingTicks(0),_lastDir(0),
			_sideCollision(false), _velMult(1.f){}
		
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
		Provoca que la entidad avance.
		*/
		void walk(int dir);

		/**
		Provoca que la entidad retroceda.
		*/
		//void walkBack();

		/**
		Provoca que la entidad cese el avance.
		*/
		void stopWalk();

		/**
		Provoca que la entidad se desplace lateralmente a la izquierda.
		*/
		//void strafeLeft();

		/**
		Provoca que la entidad se desplace lateralmente a la derecha.
		*/
		//void strafeRight();

		/**
		Provoca que la entidad cese el desplazamiento lateral.
		*/
		//void stopStrafe();
		
		/**
		Maneja el salto del jugador.
		*/
		void jump(bool salto);

		/**
		Maneja el mensaje falling

		@param falling Booleano del mensaje que nos indica si esta en el aire o no
		*/
		void falling(bool falling);

		/*
		Maneja el mensaje UP_COLLISION desde el PhysicController

		@param up Booleano del mensaje que nos indica si esta colisionando con un techo o no
		*/
		void upCollision(bool up);

		/*
		Maneja el mensaje SIDE_COLLISION desde el PhysicContoller
		@param side Booleano del mensaje que nos indica si estamos colisionando por un lateral o no
		@param normal Normal de colision. -1 si es por la derecha y 1 si es por la izquierda (Es la normal de colision por eso esta invertido)
		*/
		void sideCollision(bool side, int normal);

		/*
		Predicado que nos indica en todo momento si podemos saltar o no

		@return True si se puede saltar. False en caso contrario
		*/
		bool canJump();

		/*
		Setea la animación deseada. Lo hace comparando primero con la animacion actual, solo la cambia si son diferentes
		Optimizado para no enviar un mensaje innecesario en cada tick

		@param animacion Cadena de texto que indica a que animacion cambiar
		*/
		void sendSET_ANIMATION(char* animacion, bool loop, bool restart, float speed);

		/**
		Método que será invocado siempre que se termine una animación.
		Las animaciones en cíclicas no invocarán nunca este método.

		@param animation Nombre de la animación terminada.
		*/
		void animationFinished(const std::string &animation);

		/* 
		Devuelve si está tocando un cofre o no
		*/
		bool getTouchingChest();

		/**
		Función que devuelve el valor de giro del personaje
		*/
		int getLookingDirection();

		Vector3 getActualSpeed() const { return _actualSpeed;}

		void movePlayerAtDoor();
	protected:

		void frenar(unsigned int msecs);
		/**
		metodo que cambia la dirección en la que está mirando el personaje para que la animación de andar sea la correcta
		*/
		void turn (int look);

		/* 
		Atributo que permite cambiar la velocidad de movimiento teniendo en cuenta un multiplicador
		*/ 
		float _velMult;
		/**
		Atributo para saber si la entidad está avanzando.
		*/
		bool _walking;

		/**
		Atributo que almacena la última posición del ratón
		*/
		Vector2 moveMouse;

		/**
		Atributo para saber si la entidad está saltando.
		*/
		bool _jump;

		/*
		Atributo para saber si la entidad esta en el aire
		true-> Esta en el aire 
		false-> Esta en tierra
		*/
		bool _falling;

		/* Indica si puede saltar o no */
		bool _allowJump;

		/*
		Indica el numero de ticks consecutivos en falling, si hay mas de 2 ticks entonces es que sí estamos en el aire
		*/
		int _fallingTicks;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _maxHorizontalSpeed;

		/*
		Atributo que modifica la máxima velocidad que puede obtener según se de la situación
		*/
		float _baseMaxHorizSpeed;

		/*
		Atributo que indica la aceleracion horizontal de la entidad
		*/
		float _horizontalAcceleration;

		/*
		Atributo que modifica la aceleracion horizontal según se de la situación
		*/
		float _baseHorizAccel;

		/*
		Atributo que indica la magnitud de la velocidad del salto de la entidad antes de la deceleracion
		*/
		float _initialJumpSpeed;

		/*
		Atributo que indica la magnitud de la velocidad en caida libre
		*/
		float _maxFallingSpeed;

		/*
		Atributo que indica la velocidad minima. _actualSpeed se inicializa con este parametro
		*/
		float _minJumpSpeed;
		
		/*
		Altura de salto máxima
		*/
		float _jumpHeight;

		/*
		altura objetivo desde la última vez que se ha saltado
		*/
		float _maxJumpHeight;

		/**
		Atributo que indica la aceleracion de la gravedad de la entidad
		*/
		float _gravityAcceleration;

		/*
		Numero de saltos consecutivos que puede hacer antes de caer al suelo
		Contador ACTUAL
		*/
		unsigned int _numJump;

		/*
		Numero de saltos consecutivos que puede hacer antes de caer al suelo
		*/
		unsigned int _numJumpMax;

		/*
		Atributo que indica la velocidad instantanea de la entidad
		*/
		Vector3 _actualSpeed;

		/*
		Vector de direccion
		*/
		Vector3 _direction;

		/**
		Variable que nos indica hacia donde esta mirando la entidad
			   *Izquierda= 1
			   *Derecha = -1
		*/
		int _lookingDirection;

		/* Mapa logico actual*/
		CMap* _logicMap;


		/*
		Atributo que permite controlar la animación actual
		*/
		std::string _animActual;

		/*
		Atributo que indica el nombre del configurador de animaciones
		Usado para calcular el tiempo de las transiciones
		*/
		std::string _animationConfig;


		CPhysicController *_physicController;

		int _lastDir;

		bool _sideCollision;
	}; // class CAvatarController

	REG_FACTORY(CAvatarController);

} // namespace Logic

#endif // __Logic_AvatarController_H
