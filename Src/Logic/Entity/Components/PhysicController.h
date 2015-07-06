/**
@file PhysicController.h

Contiene la declaraci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico usando character controllers.

@see Logic::PhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Logic_PhysicController_H
#define __Logic_PhysicController_H

#include "Physics.h"

// Predeclaraci�n de tipos
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
	Componente que se utiliza para representar jugadores y enemigos en el mundo f�sico usando 
	character controllers.
	<p>
	Este componente recibe mensajes de tipo AVATAR_WALK indicando el movimiento que se quiere 
	realizar. A continuaci�n se le indica al motor de f�sica el movimiento deseado, y en el
	siguiente tick se actualiza la posici�n de la entidad l�gica. Debemos tener en cuenta que 
	la posici�n final la determina el motor de f�sica, ya que durante el movimiento podemos 
	colisionar con otra entidades, resbalar sobre paredes, etc.
	<p>
	Este componente tambi�n simula el efecto de la gravedad sobre el jugador (los controllers
	de PhysX no se ven afectados por la gravedad).
	
    @ingroup logicGroup

	@author Antonio S�nchez Ruiz-Granados
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
		Este componente s�lo acepta mensajes de tipo AVATAR_WALK.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);
		
		/**
		Cuando se recibe un mensaje de tipo AVATAR_WALK, se almacena su vector de 
		desplazamiento para mover el character controller en el pr�ximo tick.
		De esta forma, si en un ciclo se reciben varios mensaje de tipo AVATAR_WALK 
		s�lo tendr� efecto el �ltimo.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y hace lo siguiente:
		<ul>
		<li> Actualiza la posici�n de la entidad l�gica usando la informaci�n porporcionada por
		     el motor de f�sica. </li>
		<li> Mueve el character controller de acuerdo al �ltimo mensaje AVATAR_WALK recibido. </li>
		</ul>
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
		*/
		void  onTrigger (IPhysics *otherComponent, bool enter);

		void onContact(IPhysics *otherComponent, bool enter) {};

		/**
		Se invoca cuando se produce una colisi�n entre un character controller y una entidad f�sica.
		*/
		void onShapeHit (const physx::PxControllerShapeHit &hit);

		/**
		Se invoca cuando se produce una colisi�n entre dos character controllers.
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
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		La implementaci�n registrar� al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cron�metro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementaci�n eliminar� al componente de algunos observers en los 
		que pueda estar registrado (como el cron�metro del sistema, etc.).m
		*/
		virtual void deactivate();


		/*
		Contiene la l�gica para la resoluci�n de los filtros de colisi�n de CCT -vs- CCT.
		La l�gica es bastante sencilla, en caso de ser el player devolver� un valor que 
		depende de si puedo ser da�ado o no.

		@param other Componente de controlador f�sico de la otra entidad
		@return True si nos chocamos, false en caso contrario
		*/
		//bool filterControllerColision(const CPhysicController *other);


		void avatarWalk(const Vector3 &movement);

	private:

		/**
		Crea el character controller de PhysX que representa la entidad f�sica a partir de la
		informaci�n del mapa.
		*/
		Physics::CCapsuleController* createController(const Map::CEntity *entityInfo);

		// Character controller que representa la entidad f�sica en PhysX
		Physics::CCapsuleController *_controller;

		// Vector de desplazamiento recibido en el �ltimo mensaje de tipo AVATAR_WALK. 
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
