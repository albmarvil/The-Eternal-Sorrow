/**
@file PhysicEntity.h

Contiene la declaración del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
@refactor Alberto Martínez Villarán
@date Enero, 2015
*/

#ifndef __Logic_PhysicEntity_H
#define __Logic_PhysicEntity_H

#include "Physics.h"

// Predeclaración de tipos
namespace Physics {
	class CActor;
	class CStaticActor;
	class CScene;
};

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	/**
	Componente encargardo de la representación física de una entidad simple. Entendemos por 
	entidades simples aquellas que pueden ser representadas mediante un único actor de PhysX.
	Esta clase no sirve para representar character controllers ni entidades compuestas como
	ragdolls.
	<p>
	En PhysX existen dos tipos de entidades físicas: estáticas y dinámicas. Las entidades 
	estáticas no se pueden mover una vez creadas y sólo sirven como volúmenes de colisión.
	Las entidades dinámicas tienen masa y se mueven aplicándoles fuerzas (por ejemplo la gravedad). 
	También existe un tipo especial de entidades dinámicas llamadas cinemáticas, que están pensadas 
	para representar objetos cuyo movimiento viene dictado desde la lógica del juego. Las entidades 
	cinemáticas se pueden mover mediante desplazamientos en lugar de fuerzas.
	<p>
	En cada tick, este componente actualiza la posición y rotación de las entidades lógicas usando
	la información que proporciona el motor de física (sólo entidades dinámicas). 
	<p>
	Cuando este componente se utiliza para representar una entidad cinemática, acepta mensajes de 
	tipo KINEMATIC_MOVE indicando el movimiento que se quiere realizar. Y en cada tick el componente
	intenta mover la entidad física de acuerdo a los mensajes recibidos. 
	
    @ingroup logicGroup

	@author Antonio Sánchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class CPhysicEntity : public IPhysics
	{
		DEC_FACTORY(CPhysicEntity);
	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicEntity();

		/**
		Destructor. Elimina el objeto físico de la escena y lo destruye. 
		*/
		virtual ~CPhysicEntity();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo KINEMATIC_MOVE. Estos mensajes  
		sólo se utilizan para mover entidades de tipo cinemático.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Cuando recibe mensajes de tipo KINEMATIC_MOVE almacena los movimientos para aplicarlos 
		en el próximo tick sobre la entidad cinemática. Si en un ciclo se reciben varios 
		mensajes KINEMATIC_MOVE se acumulan. 
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Este método se invoca en cada ciclo de la simulación y hace lo siguiente:
		<ul>
		<li> Si la entidad física es de tipo estático no hace nada. </li>
		<li> Si la entidad física es de tipo dinámico actualiza la posición y rotación de 
		     la entidad lógica usando la información proporcionada por el motor de física. </li>
		<li> Si la entidad física es de tipo cinemático, además solicita al motor de física
		     que mueva la entidad de acuerdo al último mensaje KINEMATIC_MOVE recibido. </li>
		</ul>
		*/
		virtual void onTick(unsigned int msecs);
 
		/**
		Se invoca cuando se produce una colisión entre una entidad física y un trigger.
		*/
		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		virtual void onContact (IPhysics *otherComponent, bool enter);

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

		Physics::CActor* getActor() { return _actor; };

		std::string getPhysicType() { return _physicType; };

		void kinematicMove(Vector3 &movement, const Matrix4 &transform = Matrix4::ZERO);

		void changePhysicsGroup(int group);
	protected:

		/**
		Crea el actor de PhysX que representa la entidad física a partir de la
		información del mapa.
		*/
		Physics::CActor* createActor(const Map::CEntity *entityInfo);

		/**
		Crea un plano estático a partir de la información de mapa. 
		*/
		Physics::CStaticActor* createPlane(const Map::CEntity *entityInfo);
				
		/**
		Crea una entidad rígida (estática, dinámica o cinemática) a partir de la información de mapa. 
		*/
		Physics::CActor* createRigid(const Map::CEntity *entityInfo);
		
		/**
		Crea una entidad rígida (estática, dinámica o cinemática) a partir de un fichero RepX
		exportando previamente con el plug-in the PhysX para 3ds Max.
		*/
		Physics::CActor* createFromFile(const Map::CEntity *entityInfo);

		// Actor que representa la entidad física en PhysX
		Physics::CActor *_actor;

		/*
		Grupo de colision del actor de esta entidad
		*/
		int _collisionGroup;

		/*
		Flag de simulacion
		*/
		bool _simulation;

		// Vector de deplazamiento recibido en el último mensaje de tipo KINEMATIC_MOVE. Sirve
		// para mover entidades físicas cinemáticas.
		Vector3 _movement;

		// Matriz de trasnformación recibida en el último mensaje de tipo KINEMATIC_MOVE. Sirve
		// para mover entidades físicas cinemáticas.
		Matrix4 _transform;

		//Para almacenar las dimensiones de la caja
		Vector3 physicDimensions;

		//el tipo de actor fisico que es
		std::string _physicType;

		//Escena fisica a la que pertenece la entidad fisica
		Physics::CScene *_scene;

		std::string _physicMaterial;

		bool _z0Constraint;

		bool _verticalConstraint;

		void createDynamicBox(Vector3 &position, Vector3 &dimensions, float mass, bool kinematic, bool trigger, int group, Vector3 &offset, const std::string &material);
		void createStaticBox(Vector3 &position, Vector3 &dimensions, float mass, bool kinematic, bool trigger, int group, Vector3 &offset);

	}; // class CPhysicEntity

	REG_FACTORY(CPhysicEntity);

} // namespace Logic

#endif // __Logic_PhysicEntity_H
