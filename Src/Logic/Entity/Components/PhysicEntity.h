/**
@file PhysicEntity.h

Contiene la declaraci�n del componente encargado de representar entidades f�sicas simples,
que son aquellas representadas mediante un �nico actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
@refactor Alberto Mart�nez Villar�n
@date Enero, 2015
*/

#ifndef __Logic_PhysicEntity_H
#define __Logic_PhysicEntity_H

#include "Physics.h"

// Predeclaraci�n de tipos
namespace Physics {
	class CActor;
	class CStaticActor;
	class CScene;
};

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	/**
	Componente encargardo de la representaci�n f�sica de una entidad simple. Entendemos por 
	entidades simples aquellas que pueden ser representadas mediante un �nico actor de PhysX.
	Esta clase no sirve para representar character controllers ni entidades compuestas como
	ragdolls.
	<p>
	En PhysX existen dos tipos de entidades f�sicas: est�ticas y din�micas. Las entidades 
	est�ticas no se pueden mover una vez creadas y s�lo sirven como vol�menes de colisi�n.
	Las entidades din�micas tienen masa y se mueven aplic�ndoles fuerzas (por ejemplo la gravedad). 
	Tambi�n existe un tipo especial de entidades din�micas llamadas cinem�ticas, que est�n pensadas 
	para representar objetos cuyo movimiento viene dictado desde la l�gica del juego. Las entidades 
	cinem�ticas se pueden mover mediante desplazamientos en lugar de fuerzas.
	<p>
	En cada tick, este componente actualiza la posici�n y rotaci�n de las entidades l�gicas usando
	la informaci�n que proporciona el motor de f�sica (s�lo entidades din�micas). 
	<p>
	Cuando este componente se utiliza para representar una entidad cinem�tica, acepta mensajes de 
	tipo KINEMATIC_MOVE indicando el movimiento que se quiere realizar. Y en cada tick el componente
	intenta mover la entidad f�sica de acuerdo a los mensajes recibidos. 
	
    @ingroup logicGroup

	@author Antonio S�nchez Ruiz-Granados
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
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysicEntity();
		
		/**
		Inicializa el componente usando los atributos definidos en el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Este componente s�lo acepta mensajes de tipo KINEMATIC_MOVE. Estos mensajes  
		s�lo se utilizan para mover entidades de tipo cinem�tico.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Cuando recibe mensajes de tipo KINEMATIC_MOVE almacena los movimientos para aplicarlos 
		en el pr�ximo tick sobre la entidad cinem�tica. Si en un ciclo se reciben varios 
		mensajes KINEMATIC_MOVE se acumulan. 
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y hace lo siguiente:
		<ul>
		<li> Si la entidad f�sica es de tipo est�tico no hace nada. </li>
		<li> Si la entidad f�sica es de tipo din�mico actualiza la posici�n y rotaci�n de 
		     la entidad l�gica usando la informaci�n proporcionada por el motor de f�sica. </li>
		<li> Si la entidad f�sica es de tipo cinem�tico, adem�s solicita al motor de f�sica
		     que mueva la entidad de acuerdo al �ltimo mensaje KINEMATIC_MOVE recibido. </li>
		</ul>
		*/
		virtual void onTick(unsigned int msecs);
 
		/**
		Se invoca cuando se produce una colisi�n entre una entidad f�sica y un trigger.
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

		Physics::CActor* getActor() { return _actor; };

		std::string getPhysicType() { return _physicType; };

		void kinematicMove(Vector3 &movement, const Matrix4 &transform = Matrix4::ZERO);

		void changePhysicsGroup(int group);
	protected:

		/**
		Crea el actor de PhysX que representa la entidad f�sica a partir de la
		informaci�n del mapa.
		*/
		Physics::CActor* createActor(const Map::CEntity *entityInfo);

		/**
		Crea un plano est�tico a partir de la informaci�n de mapa. 
		*/
		Physics::CStaticActor* createPlane(const Map::CEntity *entityInfo);
				
		/**
		Crea una entidad r�gida (est�tica, din�mica o cinem�tica) a partir de la informaci�n de mapa. 
		*/
		Physics::CActor* createRigid(const Map::CEntity *entityInfo);
		
		/**
		Crea una entidad r�gida (est�tica, din�mica o cinem�tica) a partir de un fichero RepX
		exportando previamente con el plug-in the PhysX para 3ds Max.
		*/
		Physics::CActor* createFromFile(const Map::CEntity *entityInfo);

		// Actor que representa la entidad f�sica en PhysX
		Physics::CActor *_actor;

		/*
		Grupo de colision del actor de esta entidad
		*/
		int _collisionGroup;

		/*
		Flag de simulacion
		*/
		bool _simulation;

		// Vector de deplazamiento recibido en el �ltimo mensaje de tipo KINEMATIC_MOVE. Sirve
		// para mover entidades f�sicas cinem�ticas.
		Vector3 _movement;

		// Matriz de trasnformaci�n recibida en el �ltimo mensaje de tipo KINEMATIC_MOVE. Sirve
		// para mover entidades f�sicas cinem�ticas.
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
