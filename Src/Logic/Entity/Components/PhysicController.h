/**
@file PhysicController.h

Contiene la declaración del componente que se utiliza para representar jugadores y enemigos en
el mundo físico usando character controllers.

@see Logic::PhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Logic_PhysicController_H
#define __Logic_PhysicController_H

#include "Physics.h"

// Predeclaración de tipos
namespace physx {
//	class PxCapsuleController;
	struct PxControllersHit;
	struct PxControllerShapeHit;
};

namespace Physics {
	class CCapsuleController;
	class CScene;
};

// Los componentes se definen dentro del namespace Logica
namespace Logic 
{
	/**
	Componente que se utiliza para representar jugadores y enemigos en el mundo físico usando 
	character controllers.
	<p>
	Este componente recibe mensajes de tipo AVATAR_WALK indicando el movimiento que se quiere 
	realizar. A continuación se le indica al motor de física el movimiento deseado, y en el
	siguiente tick se actualiza la posición de la entidad lógica. Debemos tener en cuenta que 
	la posición final la determina el motor de física, ya que durante el movimiento podemos 
	colisionar con otra entidades, resbalar sobre paredes, etc.
	<p>
	Este componente también simula el efecto de la gravedad sobre el jugador (los controllers
	de PhysX no se ven afectados por la gravedad).
	
    @ingroup logicGroup

	@author Antonio Sánchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class CPhysicController : public IPhysics
	{
		DEC_FACTORY(CPhysicController);
	public:

		/**
		Constructor por defecto.
		*/
		CPhysicController();

		/**
		Destructor.
		*/
		virtual ~CPhysicController();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo AVATAR_WALK.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);
		
		/**
		Cuando se recibe un mensaje de tipo AVATAR_WALK, se almacena su vector de 
		desplazamiento para mover el character controller en el próximo tick.
		De esta forma, si en un ciclo se reciben varios mensaje de tipo AVATAR_WALK 
		sólo tendrá efecto el último.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Este método se invoca en cada ciclo de la simulación y hace lo siguiente:
		<ul>
		<li> Actualiza la posición de la entidad lógica usando la información porporcionada por
		     el motor de física. </li>
		<li> Mueve el character controller de acuerdo al último mensaje AVATAR_WALK recibido. </li>
		</ul>
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		*/
		void  onTrigger (IPhysics *otherComponent, bool enter);

		void onContact(IPhysics *otherComponent, bool enter) {};

		/**
		Se invoca cuando se produce una colisión entre un character controller y una entidad física.
		*/
		void onShapeHit (const physx::PxControllerShapeHit &hit);

		/**
		Se invoca cuando se produce una colisión entre dos character controllers.
		*/
		void onControllerHit (CEntity* other);
		
		/**
		Metodo que sirve para setear el entityInfo y el map en donde sera respawneada. No pongo solo la posicion, sino mas bien
		el entityInfo entero, porque puede ocurrir que queramos setear por ejemplo, la vida que tenga un enemigo, dado
		que los enemigos se haran mas fuertes. 

		@param map Mapa Logic en el que se registrara la entidad
		@param entity Informacion de construccion de la entidad leida del fichero
		@return Cierto si el respawn ha sido satisfatorio
		**/
		virtual bool respawn(const Map::CEntity *entity);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		La implementación registrará al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cronómetro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementación eliminará al componente de algunos observers en los 
		que pueda estar registrado (como el cronómetro del sistema, etc.).m
		*/
		virtual void deactivate();


		/*
		Contiene la lógica para la resolución de los filtros de colisión de CCT -vs- CCT.
		La lógica es bastante sencilla, en caso de ser el player devolveré un valor que 
		depende de si puedo ser dañado o no.

		@param other Componente de controlador físico de la otra entidad
		@return True si nos chocamos, false en caso contrario
		*/
		//bool filterControllerColision(const CPhysicController *other);


		void avatarWalk(const Vector3 &movement);

	private:

		/**
		Crea el character controller de PhysX que representa la entidad física a partir de la
		información del mapa.
		*/
		Physics::CCapsuleController* createController(const Map::CEntity *entityInfo);

		// Character controller que representa la entidad física en PhysX
		Physics::CCapsuleController *_controller;

		// Vector de desplazamiento recibido en el último mensaje de tipo AVATAR_WALK. 
		Vector3 _movement;

		// Indica si el character controller esta apoyado sobre una superficie o cayendo.
		bool _falling;

		//Indica si el character controller esta apoyado en alguna superficie por la parte de arriba
		bool _upCollision;

		//indica si el character controller esta colisionando con alguna superficie de forma lateral
		bool _sideCollision;

		unsigned int _numTicksNoSideCollision;

		Physics::CScene *_scene;
		//Map::CEntity _entityInfo;
	}; // class CPhysicController

	REG_FACTORY(CPhysicController);

} // namespace Logic

#endif // __Logic_PhysicController_H
