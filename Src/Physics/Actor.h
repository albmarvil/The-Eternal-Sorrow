
/**
@file Actor.h

Contiene la declaración de la encapsulación del actor de física. 
Básicamente envuelve la funcionalidad de la clase PxRigidActor.
Será la clase padre y punto de acceso al resto del encapsulado de PhysX

La clase contiene un constructor por defecto vacío, pero el que se usa
es el constructor a partir de un fichero repX.

@see Physics::CActor

@author Alberto Martínez Villarán
@date Enero, 2015
*/

#ifndef __Physics_Actor_H
#define __Physics_Actor_H

#include "BaseSubsystems/Math.h"
#include "Scene.h"
#include "Server.h"

// Predeclaración de tipos
namespace physx {
	class PxCooking;
	class PxPhysics;
	class PxRigidActor;
	class PxScene;
	class PxGeometry;
	class PxVec3;
};

namespace Logic
{
	class IPhysics;
	
}

// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	descripcion de la clase CActor
	@ingroup physicGroup

	@author Alberto Martínez Villarán
	@date Enero, 2015
	*/
	class CActor 
	{
	public:

		CActor(const std::string &file, int group, bool trigger, const Logic::IPhysics *component);

		/*
		Constructor por defecto
		*/
		CActor();

		/*
		Destructor.
		Si el actor esta en una escena física lo saca de ella, despues liberar recursos
		*/
		virtual ~CActor();


		/*
		Devuelve la matriz de transformación del actor físico en la escena física
		Hace falta transformar el tipo de datos de Galeon a PhysX

		@param Matriz de transformación del actor físico
		*/
		Matrix4 getTransform();

		/*
		Setea la matriz de transformaciñon del actor físico en la escena física
		Hace falta transformar el tipo de datos de Galeon a PhysX

		@param transform Matriz de transformación a setear
		*/
		void setTransform( const Matrix4 &transform);


		/*
		Setea la posición del actor físico en la escena física
		Hace falta transformar el tipo de datos de Galeon a PhysX

		@param position Posicion en la cual se setea al actor físico
		*/
		void setPosition(const Vector3 &position);

		/*
		Permite guardar información en el actor en forma de void*
		ACtualmente  se usa para guardar un puntero a un componente de tipo IPhysics
		el cual es el usado para recibir los callbacks de colisión

		@param data Informacíón para guardar en el actor físico
		*/

		void setUserData(void * data);

		/*
		Devuelve la información guardada en el actor físico en forma de void*

		ACtualmente  se usa para guardar un puntero a un componente de tipo IPhysics
		el cual es el usado para recibir los callbacks de colisión

		@return Datos guardados en el actor físico
		*/
		void* getUserData();


		/*
		Predicado que indica si un actor esta configurado como trigger o no.
		Hace uso de la "forma" (PxShape) del actor. Esta suponiendo que por defecto
		un actor tiene una única forma y esta es la que contiene la información de si es trigger o no

		@return True si el actor es un trigger
		*/
		bool isTrigger();

		/*
		Configura si el actor funciona como un trigger o no.
		Hace uso de la "forma" (PxShape) del actor. Esta suponiendo que por defecto
		un actor tiene una única forma y esta es la que contiene la información de si es trigger o no

		@param trigger True para configurar el actor como trigger
		*/
		void setTrigger(bool trigger);

		/*
		Este método activa o desactiva la simulación del objeto físico en la escena.
		Esto no significa que el actor siga estando en la escena física. Simplemente nos previene de
		recibir callbakcs desde el (callbacks de colision). Básicamente dejamos de chocar con él.
		
		Para ello se usa un Flag del propio actor PxActorFlag::DISABLE_SIMULATION

		@param simulate True = Actor en simulacion, False = Actor fuera de simulacion
		*/
		void setSimulation(bool simulate);

		/*
		Devuelve el objeto encapsulado por la clase en caso de querer hacer un procesado más preciso

		@return Actor de PhysX (PxRigidActor)
		*/
		physx::PxRigidActor* getActor() { return _actor; }


		/*
		Este predicado nos dice si la instancia del actor que estamos controlando es un actor dinámico

		@return True si es un actor dinámico
		*/
		bool isRigidDynamic();

		/*
		Predicado antagónico del anterior. Devuelve la informacion relativa a si el actor es estático o no
		Si un actor es estático no será dinámico y si no es dinámico entonces será estático

		@return True si es un actor estático
		*/
		bool isRigidStatic();

		Physics::CScene *getScene();

		void setCollisionGroup(CollisionGroup group);

	protected:
		/*
		El servidor de física es clase amiga ya que podrá acceder a parámetros como el actor o a métodos privados como 
		los de gestión del actor en la escena
		*/
		Physics::CServer *_server;

		/*
		Escena en donde pertenece el actor
		*/
		physx::PxScene *_scene;
		/*
		Actor de PhysX
		*/
		physx::PxRigidActor *_actor;

		/*
		Flag que nos sirve para saber si el actor encapsula a un actor dinámico o no
		*/
		bool _dynamic;

		bool _isTrigger;
		
	}; // class CActor

}; // namespace Physics

#endif // __Physics_Actor_H
