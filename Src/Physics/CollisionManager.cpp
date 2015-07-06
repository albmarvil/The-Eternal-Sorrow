/**
@file CollisionManager.cpp

Contiene la implementación del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "CollisionManager.h"
#include "Logic/Entity/Components/PhysicController.h"
#include "Logic/Entity/Components/PhysicEntity.h"
#include "Conversions.h"

#include <PxRigidActor.h>
#include <PxShape.h> 
#include <PxSimulationEventCallback.h> 
//#include <PxPhysicsAPI.h>

using namespace Physics;
using namespace Logic;
using namespace physx;

//--------------------------------------------------

CCollisionManager::CCollisionManager()
{

}

//--------------------------------------------------

CCollisionManager::~CCollisionManager()
{

}

//--------------------------------------------------

void CCollisionManager::onConstraintBreak(PxConstraintInfo *constraints, PxU32 count)
{
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onWake(PxActor **actors, PxU32 count)
{
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onSleep(PxActor **actors, PxU32 count)
{
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------

void CCollisionManager::onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs) 
{
	BaseSubsystems::Log::Debug("JAI");

	// Recorrer el array de colisiones
	//for (unsigned int i=0; i<nbPairs; i++) {
		
		//// Ignoramos los pares en los que alguna de las shapes (del trigger o de la otra entidad) ha sido borrada
		//if (pairs[i].flags & (PxContactPairFlag::eDELETED_SHAPE_0 | PxContactPairFlag::eDELETED_SHAPE_1))
		//	continue;

		//// Comprobamos si estamos saliendo o entrando en el trigger
  // 		bool enter = pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND;
		//bool exit = pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_LOST;

		//// Sólo tenemos en cuenta los eventos de entrada y salida del trigger. En PhysX 2.8 y anteriores
		//// también se notificada si el objeto permanecía dentro del trigger. La documentación no deja muy
		//// claro si este comportamiento se ha eliminado de manera definitiva.
		//if (!enter && !exit)
		//	continue;

		//
		//void* firstUData = pairHeader.actors[0];
		//IPhysics *firstComponent = NULL;
		//if(firstUData != NULL)//significa que hay un componente detrás de ello
		//{
		//	// Obtener el componente lógico asociado al trigger físico
		//	firstComponent = (IPhysics *) firstUData;
		//	assert(firstComponent);
		//}
		//
		//

		//// Obtener el componente lógico asociado a la otra entidad física
		//IPhysics *otherComponent = NULL;

		//void* otherUData = pairHeader.actors[1];
		//if(otherUData != NULL)//significa que hay un componente detras de ello
		//{
		//	otherComponent = (IPhysics *) otherUData;
		//	assert(otherComponent);
		//}
		//

		//// Notificar a ambos componentes la colisión
		///*
		//Estamos notificando los unos a los otros. En el caso de que un se haya chocado con algo que no tenga componente
		//se le enviará un NULL
		//*/
		//if(firstComponent != NULL)
		//{
		//	firstComponent->onContact(otherComponent, enter);
		//}

		//if(otherComponent != NULL)
		//{
		//	otherComponent->onContact(firstComponent, enter);
		//}
		
	//}
}

//--------------------------------------------------

void CCollisionManager::onTrigger(PxTriggerPair *pairs, PxU32 count)
{
	// Recorrer el array de colisiones
	for (unsigned int i=0; i<count; i++) {
		
		// Ignoramos los pares en los que alguna de las shapes (del trigger o de la otra entidad) ha sido borrada
		if (pairs[i].flags & (PxTriggerPairFlag::eDELETED_SHAPE_TRIGGER | PxTriggerPairFlag::eDELETED_SHAPE_OTHER))
			continue;

		// Comprobamos si estamos saliendo o entrando en el trigger
   		bool enter = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND;
		bool exit = pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_LOST;

		// Sólo tenemos en cuenta los eventos de entrada y salida del trigger. En PhysX 2.8 y anteriores
		// también se notificada si el objeto permanecía dentro del trigger. La documentación no deja muy
		// claro si este comportamiento se ha eliminado de manera definitiva.
		if (!enter && !exit)
			continue;

		//Lo de abajo es lo mismo que pairs[i].triggerActor->userData;
		void* triggerUData = pairs[i].triggerShape->getActor()->userData;
		IPhysics *triggerComponent = NULL;
		if(triggerUData != NULL)//significa que hay un componente detrás de ello
		{
			// Obtener el componente lógico asociado al trigger físico
			triggerComponent = (IPhysics *) triggerUData;
			assert(triggerComponent);
		}
		
		

		// Obtener el componente lógico asociado a la otra entidad física
		IPhysics *otherComponent = NULL;

		//Lo de abajo es lo mismo que pairs[i].otherActor->userData;
		void* otherUData = pairs[i].otherShape->getActor()->userData;
		if(otherUData != NULL)//significa que hay un componente detras de ello
		{
			otherComponent = (IPhysics *) otherUData;
			assert(otherComponent);
		}
		

		// Notificar a ambos componentes la colisión
		/*
		Estamos notificando los unos a los otros. En el caso de que un se haya chocado con algo que no tenga componente
		se le enviará un NULL
		*/
		if(triggerComponent != NULL)
		{
			triggerComponent->onTrigger(otherComponent, enter);
		}

		if(otherComponent != NULL)
		{
			otherComponent->onTrigger(triggerComponent, enter);
		}
		
	}	
}

//--------------------------------------------------
void CCollisionManager::onShapeHit(const PxControllerShapeHit &hit)
{

	// Obtener el componente lógico asociado al controller
	CPhysicController *component = (CPhysicController *) hit.controller->getUserData();

	assert(component);

	// Notificar la colisión
	component->onShapeHit(hit);

}

//--------------------------------------------------

void CCollisionManager::onControllerHit(const PxControllersHit &hit) 
{
	
	// Obtener el componente lógico asociado al controller
	CPhysicController *component = (CPhysicController *) hit.controller->getUserData();
	assert(component);


	CPhysicController *otherComponent = (CPhysicController *) hit.other->getUserData();
	assert(otherComponent);

	// Notificar la colisión
	component->onControllerHit(otherComponent->getEntity());
}

//--------------------------------------------------

void CCollisionManager::onObstacleHit(const PxControllerObstacleHit &hit)
{
	// Por ahora ignoramos estos mensajes	
}

//--------------------------------------------------


bool CCollisionManager::filter(const PxController& a, const PxController& b)
{
	//// Obtener el componente lógico asociado al controller
	//CPhysicController *componentA = (CPhysicController *) a.getUserData();
	//assert(componentA);

	//// Obtener el componente lógico asociado al controller
	//CPhysicController *componentB = (CPhysicController *) b.getUserData();
	//assert(componentB);


	//// resolver preguntando a la logica
	//bool ba = componentA->filterControllerColision(componentB);
	//// resolver preguntando a la logica
	//bool bb = componentB->filterControllerColision(componentA);
	//
	//return ba || bb;
	return false;
}

physx::PxFilterFlags CCollisionManager::filterShader(
								PxFilterObjectAttributes attributes0,
								PxFilterData filterData0,
								PxFilterObjectAttributes attributes1,
								PxFilterData filterData1,
								PxPairFlags& pairFlags,
								const void* constantBlock,
								PxU32 constantBlockSize)
{
		
		
		//Si en ambos actores se han indicado que pueden chocar 
		if( (filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1) ) {
			//std::cout << "Entra: "<<std::endl;//
			//Si es un trigger
			if(PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
			{
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT; //= eNOTIFY_TOUCH_FOUND | eNOTIFY_TOUCH_LOST 
				return PxFilterFlag::eDEFAULT;
			}

			//Activamos el flag de notify found O notifyLost
			pairFlags =  PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST;

			// Si ademas el objeto NO es un trigger, queremos resolver la colision y obtener los
			// puntos de contacto
			if( !PxFilterObjectIsTrigger(attributes0) && !PxFilterObjectIsTrigger(attributes1) ) {
				pairFlags = PxPairFlag::eRESOLVE_CONTACTS | PxPairFlag::eNOTIFY_CONTACT_POINTS;
			}
		}
		else
		{
			//Si no pueden chocar, entonces ignoro la colision
			return PxFilterFlag::eKILL;
		}

        return PxFilterFlag::eDEFAULT;
}
//--------------------------------------------------

