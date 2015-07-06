/**
@file CapsuleController.cpp

Contiene la implementación de la encapsulación del CapsuleController de física

@see Physics::CCapsuleController

@author Alberto Martínez Villarán
@date Enero, 2015

@refactor Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#include "CapsuleController.h"
#include "Server.h"
#include "CollisionManager.h"
#include "Conversions.h"

#include <PxPhysicsAPI.h>


using namespace physx;

using namespace Logic;



namespace Physics
{
	CCapsuleController::CCapsuleController(const Vector3 &position, float radius, float height, Vector3 offset, int group, const Logic::CPhysicController *component, PxMaterial *material)
	{
		_server = Physics::CServer::getSingletonPtr();

		_scene = _server->getScene();

		_radius = radius;
		_height = height;

		_collisionManager = _server->getCollisionManager();

		_controllerManager = _scene->getControllerManager();
		// Nota: PhysX coloca el sistema de coordenadas local en el centro de la cápsula, mientras
		// que la lógica asume que el origen del sistema de coordenadas está en los piés del 
		// jugador. Para unificar necesitamos realizar una traslación en el eje Y.
		// Desafortunadamente, el descriptor que se usa para crear los controllers no permite
		// definir esta transformación local (que sí permite al crear un actor), por lo que
		// tendremos que realizar la traslación nosotros cada vez. 

		_offset = offset;
		// Transformación entre el sistema de coordenadas lógico y el de PhysX
		float offsetY = height / 2.0f + radius;
		PxVec3 pos = Vector3ToPxVec3(position + Vector3(0, offsetY, 0) + offset);
	
		// Crear descriptor del controller
		PxCapsuleControllerDesc desc;
		desc.position = PxExtendedVec3(pos.x, pos.y, pos.z);
		desc.height = height;
		desc.radius = radius;
		desc.material = material;
		desc.climbingMode = PxCapsuleClimbingMode::eEASY; 
		//desc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
		//desc.slopeLimit = 0.707f;

		desc.reportCallback = _collisionManager;   // Establecer gestor de colisiones
		desc.userData = (void *) component;  // Anotar el componente lógico asociado al controller

		_controller = (PxCapsuleController *) _controllerManager->createController(desc);

		_actor = _controller->getActor();
		// Anotar el componente lógico asociado al actor dentro del controller (No es automático)
		_actor->userData = (void *) component;

		_server->setupFiltering(_actor, CollisionGroup(group));

		//PARTE IMPORTANTE TANTO PARA QUE FUNCIONE EL TRIGGER, COMO LOS GRUPOS DE COLISION
		PxFilterData data;
		PxShape *shape;
		//obtengo la malla fisica del actor
 		_controller->getActor()->getShapes(&shape, 1, 0);
		//Obtengo el filterData del actor
		data = shape->getSimulationFilterData();

		_scene->getScene()->removeActor(*_controller->getActor());

		//Quito el trigger, porque sino el controller hace cosas raras y choca contra el. 
		_server->UpdateGroup(data.word1, CollisionGroup::eTriggers, false);
		_server->UpdateGroup(data.word1, CollisionGroup::eBulletEnemClip, false);
		_server->UpdateGroup(data.word1, CollisionGroup::eBulletEnemNoClip, false);
		_server->UpdateGroup(data.word1, CollisionGroup::eBulletPlayerNoClip, false);
		_server->UpdateGroup(data.word1, CollisionGroup::eBulletPlayerClip, false);
		
		//Igualo a word0 al nuevo FilterData del capsuleController a los grupos de colision del player
		(_data).word0 = data.word1;

	}//CCapsuleController
	//-----------------------------------------------------------------------

	CCapsuleController::~CCapsuleController()
	{
		if (_controller) {
			_controller->release();
			_controller = NULL;
		}
		
	}//~CCapsuleController
	//-----------------------------------------------------------

	unsigned CCapsuleController::moveController(const Vector3 &movement, unsigned int msecs)
	{
		// Mover el character controller y devolver los flags de colisión
		PxVec3 disp = Vector3ToPxVec3(movement);
		float minDist = 0.01f;
		float elapsedTime = msecs * 0.001f;
		/*construimos el filtro de colision a partir de los callbacks implementados en nuestro collision Manager*/

		PxControllerFilters filters(&_data,NULL, _collisionManager);

		//_controllerManager->computeInteractions(msecs, _collisionManager);
		return _controller->move(disp, minDist, elapsedTime, filters, NULL);
	}

	Vector3 CCapsuleController::getControllerPosition()
	{
		// Antes de devolver la posición del controller debemos transformar entre el 
		// sistema de coordenadas de PhysX y el de la lógica
		float offsetY = _controller->getHeight() / 2.0f + _controller->getRadius();
		Vector3 pos = PxExtendedVec3ToVector3(_controller->getPosition());
		return pos - Vector3(0, offsetY, 0) - _offset;
	}

	void CCapsuleController::setControllerPosition(const Vector3 &position)
	{
		// Mover el character controller y devolver los flags de colisión
		float offsetY = _controller->getHeight() / 2.0f + _controller->getRadius();
		PxExtendedVec3 pos = Vector3ToPxExtendedVec3(position+Vector3(0,offsetY,0) + _offset);
		_controller->setPosition(pos);

		PxTransform pose (Physics::Vector3ToPxVec3(position));
		_controller->getActor()->setGlobalPose(pose);
	}

	void CCapsuleController::setSimulation(bool simulate)
	{
		
		if(simulate)
		{
			_scene->getScene()->addActor(*_controller->getActor());
		}

		#pragma region Primera opcion para quitar la fisica del actor
		PxShape *shape;

		//suponemos que tiene una sola shape
		_actor->getShapes(&shape, 1, 0);
		shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, simulate);
		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, simulate);
		_actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !simulate);

		#pragma endregion

	}


	int CCapsuleController::getCollisionGroup()
	{
		return PxGetGroup(*_actor);
	}


	void CCapsuleController::setCollisionGroup(int group)
	{
		_server->setupFiltering(_actor, CollisionGroup(group));

		//PARTE IMPORTANTE TANTO PARA QUE FUNCIONE EL TRIGGER, COMO LOS GRUPOS DE COLISION
		PxFilterData data;
		PxShape *shape;
		//obtengo la malla fisica del actor
 		_controller->getActor()->getShapes(&shape, 1, 0);
		//Obtengo el filterData del actor
		data = shape->getSimulationFilterData();

		//_scene->getScene()->removeActor(*_controller->getActor());

		//Quito el trigger, porque sino el controller hace cosas raras y choca contra el. 
		_server->UpdateGroup(data.word1, CollisionGroup::eTriggers, false);
		_server->UpdateGroup(data.word1, CollisionGroup::eBulletEnemClip, false);
		_server->UpdateGroup(data.word1, CollisionGroup::eBulletEnemNoClip, false);
		_server->UpdateGroup(data.word1, CollisionGroup::eBulletPlayerNoClip, false);
		_server->UpdateGroup(data.word1, CollisionGroup::eBulletPlayerClip, false);
		
		//Igualo a word0 al nuevo FilterData del capsuleController a los grupos de colision del player
		(_data).word0 = data.word1;
	}
}
