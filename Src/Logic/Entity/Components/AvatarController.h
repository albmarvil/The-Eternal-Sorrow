/**
@file AvatarController.h

Contiene la declaraci�n del componente que controla el movimiento 
de la entidad.

@see Logic::CAvatarController
@see Logic::IComponent

@author David Llans�
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

//declaraci�n de la clase
namespace Logic 
{


/**
	Este componente es el encargado de mover a una entidad animada. Tiene
	diferentes m�todos que permiten avances o giros. El uso de este componente
	es un poco at�pico ya que se puede registrar en otro controlador externo
	(i.e. GUI::CPlayerController) que sea el que de las �rdenes que se deben
	llevar a cabo mediante llamadas a m�todos p�blicos del componente. Puede
	no obstante ampliarse este componente para aceptar mensajes tradicionales
	con las �rdenes, sin embargo de momento as� es suficiente.
	
	@ingroup logicGroup

	@author David Llans� Garc�a
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
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		speed que indica a la velocidad a la que se mover� el jugador.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se registra as� mismo en el controlador del GUI para que las ordenes 
		se reciban a partir de los eventos de teclado y rat�n.

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Si el componente pertenece a la entidad del jugador, el componente
		se deregistra as� mismo en el controlador del GUI para dejar de
		recibir las ordenes dadas a partir de los eventos de teclado y rat�n.
		*/
		virtual void deactivate();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		CONTROL.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		M�todo virtual que procesa un mensaje.

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
		Setea la animaci�n deseada. Lo hace comparando primero con la animacion actual, solo la cambia si son diferentes
		Optimizado para no enviar un mensaje innecesario en cada tick

		@param animacion Cadena de texto que indica a que animacion cambiar
		*/
		void sendSET_ANIMATION(char* animacion, bool loop, bool restart, float speed);

		/**
		M�todo que ser� invocado siempre que se termine una animaci�n.
		Las animaciones en c�clicas no invocar�n nunca este m�todo.

		@param animation Nombre de la animaci�n terminada.
		*/
		void animationFinished(const std::string &animation);

		/* 
		Devuelve si est� tocando un cofre o no
		*/
		bool getTouchingChest();

		/**
		Funci�n que devuelve el valor de giro del personaje
		*/
		int getLookingDirection();

		Vector3 getActualSpeed() const { return _actualSpeed;}

		void movePlayerAtDoor();
	protected:

		void frenar(unsigned int msecs);
		/**
		metodo que cambia la direcci�n en la que est� mirando el personaje para que la animaci�n de andar sea la correcta
		*/
		void turn (int look);

		/* 
		Atributo que permite cambiar la velocidad de movimiento teniendo en cuenta un multiplicador
		*/ 
		float _velMult;
		/**
		Atributo para saber si la entidad est� avanzando.
		*/
		bool _walking;

		/**
		Atributo que almacena la �ltima posici�n del rat�n
		*/
		Vector2 moveMouse;

		/**
		Atributo para saber si la entidad est� saltando.
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
		Indica el numero de ticks consecutivos en falling, si hay mas de 2 ticks entonces es que s� estamos en el aire
		*/
		int _fallingTicks;

		/**
		Atributo que indica la magnitud de velocidad de la entidad.
		*/
		float _maxHorizontalSpeed;

		/*
		Atributo que modifica la m�xima velocidad que puede obtener seg�n se de la situaci�n
		*/
		float _baseMaxHorizSpeed;

		/*
		Atributo que indica la aceleracion horizontal de la entidad
		*/
		float _horizontalAcceleration;

		/*
		Atributo que modifica la aceleracion horizontal seg�n se de la situaci�n
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
		Altura de salto m�xima
		*/
		float _jumpHeight;

		/*
		altura objetivo desde la �ltima vez que se ha saltado
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
		Atributo que permite controlar la animaci�n actual
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
