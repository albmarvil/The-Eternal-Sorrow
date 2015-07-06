/**
@file Actor.cpp

Contiene la implementación de la encapsulación del actor de física

@see Physics::CActor

@author Alberto Martínez Villarán
@date Enero, 2015
@refactor Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#include "Actor.h"
//#include "Server.h"
#include "Conversions.h"

#include <PxPhysicsAPI.h>

using namespace physx;

using namespace Logic;



namespace Physics
{
	CActor::CActor() 
	{
		_server = Physics::CServer::getSingletonPtr();
	}

	CActor::CActor(const std::string &file, int group, bool trigger, const Logic::IPhysics *component)
	{
		_server = Physics::CServer::getSingletonPtr();

		_scene = _server->getScene()->getScene();

		// Preparar parámetros para deserializar
		PxSerializationRegistry* registry = PxSerialization::createSerializationRegistry(*_server->getSdk()); 
		PxDefaultFileInputData data(file.c_str());
		PxCollection* collection;

		// Deserializar a partir del fichero RepX
		collection = PxSerialization::createCollectionFromXml(data, *_server->getCooking(), *registry);
	
		// Añadir entidades físicas a la escena
		_scene->addCollection(*collection); 
	
		// Buscar una entidad de tipo PxRigidActor. Asumimos que hay exactamente 1 en el fichero.
		_actor = NULL;
		for (unsigned int i=0; (i<collection->getNbObjects()) && !_actor; i++)
		{
			_actor = collection->getObject(i).is<PxRigidActor>();		
		}

		assert(_actor);

		setTrigger(trigger);

		// Anotar el componente lógico asociado a la entidad física
		_actor->userData = (void *) component;

		//Indicamos al actor con quien debe colisionar
		_server->setupFiltering(_actor, CollisionGroup(group));

		// Liberar recursos
		collection->release();
		registry->release();

	}//CActor

	//------------------------------------------------------------------------------

	CActor::~CActor()
	{
		//_server->getScene()->removeActor(_actor);
		_scene = 0;
		_actor->release();
	}//~CActor

	//------------------------------------------------------------------------------


	Matrix4 CActor::getTransform()
	{
		assert(_actor);
		return Physics::PxTransformToMatrix4(_actor->getGlobalPose());
	}//getTransform

	//------------------------------------------------------------------------------

	void CActor::setTransform(const Matrix4 &transform)
	{
		PxTransform pose (Physics::Matrix4ToPxTransform(transform));
		_actor->setGlobalPose(pose);
	}//setTransform

	//------------------------------------------------------------------------------

	void CActor::setPosition(const Vector3 &position)
	{
		PxTransform pose (Physics::Vector3ToPxVec3(position));
		_actor->setGlobalPose(pose);
	}//setPosition

	//------------------------------------------------------------------------------


	void* CActor::getUserData()
	{
		return _actor->userData;
	}//getUserData

	//------------------------------------------------------------------------------


	void CActor::setUserData(void* data)
	{
		_actor->userData = data;
	}//setUserData

	//------------------------------------------------------------------------------


	//------------------------------------------------------------------------------

	bool CActor::isTrigger()
	{
		/*PxShape *shape;
		_actor->getShapes(&shape, 1, 0);
		return shape->getFlags() & PxShapeFlag::eTRIGGER_SHAPE;
		*/
		return _isTrigger;
	}//isTrigger

	//-------------------------------------------------------------

	void CActor::setTrigger(bool trigger)
	{
		PxShape *shape;
		_actor->getShapes(&shape, 1, 0);

		shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !trigger);
		shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, trigger);
	
		_isTrigger = trigger;
	}//setTrigger

	//-------------------------------------------------------------


	void CActor::setSimulation(bool simulate)
	{
		#pragma region Segunda opcion
		//_actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !simulate);
		if(simulate && _actor->getScene() == NULL)
		{
			_scene->addActor(*_actor);
		}

		#pragma endregion
		PxShape *shape;
		_actor->getShapes(&shape,1,0);

		if(_isTrigger)
		{
			shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, simulate);
		}
		else
		{
			shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, simulate);
		}

		shape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, simulate);

		_actor->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !simulate);
	}//setSimulation

	//-------------------------------------------------------------

	bool CActor::isRigidDynamic()
	{
		return _dynamic;
	}//isRigidDynamic

	//-------------------------------------------------------------

	bool CActor::isRigidStatic()
	{
		return !_dynamic;
	}//isRigidDynamic

	//------------------------------------------------------------


	Physics::CScene *CActor::getScene()
	{
		return _server->getScene();
	}


	void CActor::setCollisionGroup(CollisionGroup group)
	{
		_server->setupFiltering(_actor, group);
	}
}
