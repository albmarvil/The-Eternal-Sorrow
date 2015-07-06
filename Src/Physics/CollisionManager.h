/**
@file CollisionManager.h

Contiene la declaraci�n del gestor de colisiones.

@see Physics::CCollisionManager

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __PHYSICS_COLLISION_MANAGER_H_
#define __PHYSICS_COLLISION_MANAGER_H_

#include <PxSimulationEventCallback.h> 
#include <characterkinematic/PxController.h>

//#include "PxFiltering.h"
//#include "PxPhysxConfig.h"

// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	Clase que gestiona las colisiones entre entidades f�sicas. Cuando se produce una
	colisi�n notifica a los componentes l�gicos correspondientes. 

	 @ingroup physicGroup

	 @author Antonio S�nchez Ruiz-Granados
	 @date Noviembre, 2012
	*/
	class CCollisionManager : public physx::PxSimulationEventCallback,
							  public physx::PxUserControllerHitReport,
							  public physx::PxControllerFilterCallback
	{
	public:
		/**
		Constructor por defecto.
		*/
		CCollisionManager();

		/**
		Destructor.
		*/
		virtual ~CCollisionManager();

		/**
		M�todo invocado cuando se rompe un constraint (ver documentaci�n PhysX).
		*/
		void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count); 
 
		/**
		M�todo invocado cuando se despiertan actores f�sicos (ver documentaci�n PhysX).
		*/
		void onWake(physx::PxActor **actors, physx::PxU32 count);
  
		/**
		M�todo invocado cuando se duermen actores f�sicos (ver documentaci�n PhysX).
		*/
		void onSleep(physx::PxActor **actors, physx::PxU32 count); 
   
		/**
		M�todo invocado cuando se produce un contacto entre dos shapes f�sicos 
		(ver documentaci�n PhysX).
		*/
		void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
  
		/**
		M�todo invocado cuando una entidad f�sica entra o sale de un trigger 
		(ver documentaci�n PhysX).
		*/
		void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);

		/**
		M�todo invocado cuando un character controller golpea una shape 
		(ver documentaci�n PhysX).
		*/ 
		void onShapeHit(const physx::PxControllerShapeHit &hit);
  
		/**
		M�todo invocado cuando un character controller golpea otro character controller 
		(ver documentaci�n PhysX).
		*/ 
		void onControllerHit(const physx::PxControllersHit &hit); 
   
		/**
		M�todo invocado cuando un character controller golpea un obst�culo definido por 
		el usuario (ver documentaci�n PhysX).
		*/ 
		void onObstacleHit(const physx::PxControllerObstacleHit &hit);

		/*
		M�todo invocado cuando dos CCTs (CapsuleController) se chocan. Si se devuelve true habr� colisi�n entre ellos
		false en caso contrario. si queremos hacer una distincion especial entre ellos habr� que pasar informaci�n a la hora
		de crearlos y luego leerla en este callback y ahcer nuestra l�gica

		@param a CCT que participa en la colsion
		@param b CCT que participa en la colsion
		@return True si los CCTs colisionan entre ellos, False en caso contrario
		*/
		bool filter(const physx::PxController& a, const physx::PxController& b); 

		/*
		@param attributes0 Informaci�n general sobre el primer objeto.
		@param filterData0 Datos de filtro del primer objeto.
		@param attributes1 Informaci�n general sobre el segundo objeto.
		@param filterData1 Datos de filtro del segundo objeto.
		@param pairFlags Flags de la pareja.
		@param constantBlock Puntero al bloque de datos.
		@param constantBlockSize Tama�o del bloque de datos.
		@return Los flags que indican que tipo de notificaci�n debe dispararse.
		*/
		static physx::PxFilterFlags filterShader(
								physx::PxFilterObjectAttributes attributes0,
								physx::PxFilterData filterData0,
								physx::PxFilterObjectAttributes attributes1,
								physx::PxFilterData filterData1,
								physx::PxPairFlags& pairFlags,
								const void* constantBlock,
								physx::PxU32 constantBlockSize
						);
   
	}; // CCollisionManager

}; // namespace Physics

#endif // __PHYSICS_COLLISION_MANAGER_H_