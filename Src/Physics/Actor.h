
/**
@file Actor.h

Contiene la declaraci�n de la encapsulaci�n del actor de f�sica. 
B�sicamente envuelve la funcionalidad de la clase PxRigidActor.
Ser� la clase padre y punto de acceso al resto del encapsulado de PhysX

La clase contiene un constructor por defecto vac�o, pero el que se usa
es el constructor a partir de un fichero repX.

@see Physics::CActor

@author Alberto Mart�nez Villar�n
@date Enero, 2015
*/

#ifndef __Physics_Actor_H
#define __Physics_Actor_H

#include "BaseSubsystems/Math.h"
#include "Scene.h"
#include "Server.h"

// Predeclaraci�n de tipos
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

// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	descripcion de la clase CActor
	@ingroup physicGroup

	@author Alberto Mart�nez Villar�n
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
		Si el actor esta en una escena f�sica lo saca de ella, despues liberar recursos
		*/
		virtual ~CActor();


		/*
		Devuelve la matriz de transformaci�n del actor f�sico en la escena f�sica
		Hace falta transformar el tipo de datos de Galeon a PhysX

		@param Matriz de transformaci�n del actor f�sico
		*/
		Matrix4 getTransform();

		/*
		Setea la matriz de transformaci�on del actor f�sico en la escena f�sica
		Hace falta transformar el tipo de datos de Galeon a PhysX

		@param transform Matriz de transformaci�n a setear
		*/
		void setTransform( const Matrix4 &transform);


		/*
		Setea la posici�n del actor f�sico en la escena f�sica
		Hace falta transformar el tipo de datos de Galeon a PhysX

		@param position Posicion en la cual se setea al actor f�sico
		*/
		void setPosition(const Vector3 &position);

		/*
		Permite guardar informaci�n en el actor en forma de void*
		ACtualmente  se usa para guardar un puntero a un componente de tipo IPhysics
		el cual es el usado para recibir los callbacks de colisi�n

		@param data Informac��n para guardar en el actor f�sico
		*/

		void setUserData(void * data);

		/*
		Devuelve la informaci�n guardada en el actor f�sico en forma de void*

		ACtualmente  se usa para guardar un puntero a un componente de tipo IPhysics
		el cual es el usado para recibir los callbacks de colisi�n

		@return Datos guardados en el actor f�sico
		*/
		void* getUserData();


		/*
		Predicado que indica si un actor esta configurado como trigger o no.
		Hace uso de la "forma" (PxShape) del actor. Esta suponiendo que por defecto
		un actor tiene una �nica forma y esta es la que contiene la informaci�n de si es trigger o no

		@return True si el actor es un trigger
		*/
		bool isTrigger();

		/*
		Configura si el actor funciona como un trigger o no.
		Hace uso de la "forma" (PxShape) del actor. Esta suponiendo que por defecto
		un actor tiene una �nica forma y esta es la que contiene la informaci�n de si es trigger o no

		@param trigger True para configurar el actor como trigger
		*/
		void setTrigger(bool trigger);

		/*
		Este m�todo activa o desactiva la simulaci�n del objeto f�sico en la escena.
		Esto no significa que el actor siga estando en la escena f�sica. Simplemente nos previene de
		recibir callbakcs desde el (callbacks de colision). B�sicamente dejamos de chocar con �l.
		
		Para ello se usa un Flag del propio actor PxActorFlag::DISABLE_SIMULATION

		@param simulate True = Actor en simulacion, False = Actor fuera de simulacion
		*/
		void setSimulation(bool simulate);

		/*
		Devuelve el objeto encapsulado por la clase en caso de querer hacer un procesado m�s preciso

		@return Actor de PhysX (PxRigidActor)
		*/
		physx::PxRigidActor* getActor() { return _actor; }


		/*
		Este predicado nos dice si la instancia del actor que estamos controlando es un actor din�mico

		@return True si es un actor din�mico
		*/
		bool isRigidDynamic();

		/*
		Predicado antag�nico del anterior. Devuelve la informacion relativa a si el actor es est�tico o no
		Si un actor es est�tico no ser� din�mico y si no es din�mico entonces ser� est�tico

		@return True si es un actor est�tico
		*/
		bool isRigidStatic();

		Physics::CScene *getScene();

		void setCollisionGroup(CollisionGroup group);

	protected:
		/*
		El servidor de f�sica es clase amiga ya que podr� acceder a par�metros como el actor o a m�todos privados como 
		los de gesti�n del actor en la escena
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
		Flag que nos sirve para saber si el actor encapsula a un actor din�mico o no
		*/
		bool _dynamic;

		bool _isTrigger;
		
	}; // class CActor

}; // namespace Physics

#endif // __Physics_Actor_H
