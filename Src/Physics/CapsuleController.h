
/**
@file CapsuleController.h

Contiene la declaraci�n de la encapsulaci�n del CapsuleController de f�sica

Un controller de f�sica no es necesariamente un Actor. La relaci�on que tiene es que un objeto Controller dentro
de physX tiene como atributo miembro de la clase un actor.

Los controladores tienen un tratamiento bastante especial y estan hechos para mandar callbakcs bastante especificos sobre colsiones y el estado
de las mismas

@see Physics::CCapsuleController

@author Alberto Mart�nez Villar�n
@date Enero, 2015
@author Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#ifndef __Physics_CapsuleController_H
#define __Physics_CapsuleController_H

#include "BaseSubsystems/Math.h"
#include "Scene.h"
#include <PxFiltering.h>
// Predeclaraci�n de tipos
namespace physx {
	class PxMaterial;
	class PxPhysics;
	class PxCapsuleController;
	class PxScene;
	class PxControllerManager;
	class PxRigidDynamic;
};

namespace Physics
{
	class CCollisionManager;
	class CServer;
};

namespace Logic
{
	class CPhysicController;
}


// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	descripcion de la clase CCapsuleController
	@ingroup physicGroup

	@author Alberto Mart�nez Villar�n
	@date Enero, 2015
	*/
	class CCapsuleController 
	{
	public:

		/*
		Constructor

		@param position Posicion donde se crea el controlador
		@param radius Radio de la cpasula del controllador
		@param height Altura de la capsula del controlador
		@param group  Grupo de colision del ACTOR del controlador (NO FUNCIONA POR AHORA)
		@param component Componente donde enviar los callbacks
		@param material Material de comportamiento
		@param controllerManager Manager de control (Mas Info  en Physicis::CServer)
		@param collisionManager Manager de colisiones, clase que implementa todos los callbacks
		@param physics Elemento principal de physX
		@param scene Escena f�sica
		*/
		CCapsuleController(const Vector3 &position, 
			float radius,
			float height,
			Vector3 offset,
			int group,
			const Logic::CPhysicController *component, 
			physx::PxMaterial *material = Physics::CScene::getSingletonPtr()->getDefaultMaterial());

		/*
		Destructor. Es el servidor de fisica el que se encarga de sacar de escena y destruir CapsuleControlleres

		*/
		~CCapsuleController();

		/*
		Al igual que un actor kinem�tico los controladores se mueven de forma especial
		Estos a su vez aplican dos par�metros en su movimiento
		FILTROS y OBSTACULOS

		Ambos configurados correctamente (mediante callbacks) definen el comportamiento de un controller
		al moverse, con que se choca y como se choca. Actualmente se usa un filtro implementado en collisionManager
		que permite que los controladores no se choquen entre s�

		@param movement Incremento de desplazamiento 
		@param msecs Tiempo en milisegundos
		@return Flags de colision de PhysX
		*/
		unsigned moveController(const Vector3 &movement, unsigned int msecs);

		/*
		Devuelve la posici�n del controlador f�sico

		@param Posicion con las coordenadas transformadas de PhysX a Galeon
		*/
		Vector3 getControllerPosition();

		/*
		Setea la posicion del controlador. Esto se usa para teletransportarlo.
		Ya que no aporta informaci�on de colision en el trayecto que realiza

		@param position Posicion en la cual se ubica al controller
		*/
		void setControllerPosition(const Vector3 &position);

		/*
		DEfine si el controlador interactua con el resto de la escena o no

		Lo hace a trav�s del "Shape" del actor del controller
		Es un poco rebuscado pero es efectivo. SAcamos el actor asociado al controller y desde
		el accedemos a su Shape. Suponiendo que solo es una desactivamos el flag que hace que se puedan mandar
		callbacks de colision desde el controlador y hacia el controlador

		@param True si se quiere participar de la simulacion, false para sacar al controlador de la simulacion
		*/
		void setSimulation(bool simulate);

		physx::PxRigidDynamic *getActor(){ return _actor;}

		int getCollisionGroup();
		void setCollisionGroup(int group);

		Physics::CScene *getScene() { return _scene;}

	protected:

		/*
		CapsuleController de PhysX
		*/
		physx::PxCapsuleController *_controller;

		float _height;

		float _radius;

		Vector3 _offset;

		physx::PxRigidDynamic *_actor;

		CCollisionManager *_collisionManager;

		Physics::CServer *_server;

		Physics::CScene *_scene;

		physx::PxControllerManager *_controllerManager;

		physx::PxFilterData _data;

	}; // class CCapsuleController

}; // namespace Physics

#endif // __Physics_CapsuleController_H
