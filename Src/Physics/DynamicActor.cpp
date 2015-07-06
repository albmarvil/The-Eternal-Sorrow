/**
@file DynamicActor.cpp

Contiene la implementación de la encapsulación del actor estático de física

@see Physics::CDynamicActor

@author Alberto Martínez Villarán
@date Enero, 2015
*/

#include "DynamicActor.h"
#include "Server.h"
#include "Conversions.h"

#include <PxPhysicsAPI.h>
#include <PxConstraint.h>
#include <PxConstraintDesc.h>

using namespace physx;

using namespace Logic;



namespace Physics
{
	CDynamicActor::~CDynamicActor()
	{
		_joint->release();
	}

	CDynamicActor::CDynamicActor(const Vector3 &position, const Vector3 &dimensions, float mass, bool kinematic, bool trigger, int group, const IPhysics *component, Vector3 &offset, const std::string &material)
	{
		physx::PxMaterial *PhysicMaterial = Physics::CScene::getSingletonPtr()->getPhysicMaterialByName(material);
		if (PhysicMaterial ==  NULL)
		{
			PhysicMaterial = Physics::CScene::getSingletonPtr()->getDefaultMaterial();
		}
		PxBoxGeometry geom(Vector3ToPxVec3(dimensions));

		float density = mass / (dimensions.x * dimensions.y * dimensions.z); // Densidad = masa / volumen

		createDynamicActor(position, geom, density, dimensions, kinematic, trigger, group,  component, offset, PhysicMaterial);
		
	}//CDynamicActor

	CDynamicActor::CDynamicActor(const Vector3 &position, float radius, float mass, bool kinematic, bool trigger, int group,  const IPhysics *component, Vector3 &offset, const std::string &material)
	{
		physx::PxMaterial *PhysicMaterial = Physics::CScene::getSingletonPtr()->getPhysicMaterialByName(material);
		if (PhysicMaterial ==  NULL)
		{
			PhysicMaterial = Physics::CScene::getSingletonPtr()->getDefaultMaterial();
		}
		PxSphereGeometry geom(radius);

		Vector3 dimensions = Vector3(radius, radius, radius);

		float density = mass / ((4 * 3.1416 * radius * radius * radius) / 3); //masa / volumen

		createDynamicActor(position, geom, density, dimensions, kinematic, trigger, group, component, offset, PhysicMaterial);
		
	}//CDynamicActor

	CDynamicActor::CDynamicActor(const Vector3 &position, float radius, float height, float mass, bool kinematic, bool trigger, int group, const IPhysics *component, Vector3 &offset, const std::string &material)
	{
		physx::PxMaterial *PhysicMaterial = Physics::CScene::getSingletonPtr()->getPhysicMaterialByName(material);
		if (PhysicMaterial ==  NULL)
		{
			PhysicMaterial = Physics::CScene::getSingletonPtr()->getDefaultMaterial();
		}
		PxCapsuleGeometry geom(radius, height/2);

		Vector3 dimensions = Vector3(radius, height, radius);

		float density = mass / (3.1416 * radius * radius * height); // Densidad = masa / volumen

		createDynamicActor(position, geom, density, dimensions, kinematic, trigger, group, component,offset ,PhysicMaterial);

	}//CDynamicActor

	//-------------------------------------------------------------

	void CDynamicActor::createDynamicActor(const Vector3 &position, const PxGeometry &geom, float density, const Vector3 &dimensions, bool kinematic, bool trigger, int group, const Logic::IPhysics *component, Vector3 &offset, physx::PxMaterial *material)
	{
		_scene = _server->getScene()->getScene();

		_dynamic = true;
		// Nota: PhysX coloca el sistema de coordenadas local en el centro de la caja, mientras
		// que la lógica asume que el origen del sistema de coordenadas está en el centro de la 
		// cara inferior. Para unificar necesitamos realizar una traslación en el eje Y.
		// Afortunadamente, el descriptor que se usa para crear el actor permite definir esta 
		// transformación local, por lo que la conversión entre sistemas de coordenadas es transparente. 

		// Crear un cubo dinámico
		PxTransform pose(Vector3ToPxVec3(position));

		PxTransform localPose(Vector3ToPxVec3(offset)); // Transformación de coordenadas lógicas a coodenadas de PhysX

		_actor = PxCreateDynamic(*_server->getSdk(), pose, geom, *material, density, localPose);
		
		((PxRigidDynamic*)_actor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, kinematic);

		// Transformarlo en trigger si es necesario
		setTrigger(trigger);
		
		// Anotar el componente lógico asociado a la entidad física
		_actor->userData = (void *) component;

		//Indicamos al actor con quien debe colisionar
		_server->setupFiltering(_actor, CollisionGroup(group));

		// Añadir el actor a la escena
		//_scene->addActor(*_actor);
		
		_joint = PxD6JointCreate(*_server->getSdk(), NULL, _actor->getGlobalPose(), _actor, PxTransform(PxVec3(0,0,0)));
		_joint->setMotion(PxD6Axis::eZ, PxD6Motion::eFREE);
		_joint->setMotion(PxD6Axis::eX,     PxD6Motion::eFREE);
		_joint->setMotion(PxD6Axis::eY,     PxD6Motion::eFREE);
		_joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
		_joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);
		_joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
		
		
	}

	CDynamicActor::CDynamicActor(PxRigidDynamic *dynActor)
	{
		_actor = dynActor;
		_dynamic = true;
		_scene = _server->getScene()->getScene();
	}

	bool CDynamicActor::isKinematic()
	{
		//hacemos casting
		return ((PxRigidDynamic*)_actor)->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC;
	}//isKinematic

	//-------------------------------------------------------------

	void CDynamicActor::moveKinematicActor(const Matrix4 &transform)
	{
		assert(_actor);
		assert(isKinematic());

		PxTransform pose = Matrix4ToPxTransform(transform);
		pose.q.normalize();

		// Mover el actor tras transformar el destino a coordenadas lógicas
		((PxRigidDynamic*)_actor)->setKinematicTarget(Matrix4ToPxTransform(transform));
	}//moveKinematicActor

	//-------------------------------------------------------------


	void CDynamicActor::moveKinematicActor(const Vector3 &displ)
	{
		assert(_actor);
		assert(isKinematic());

		// Mover el actor tras transformar el destino a coordenadas lógicas
		PxTransform transform = _actor->getGlobalPose();
		transform.p += Vector3ToPxVec3(displ);
		((PxRigidDynamic*)_actor)->setKinematicTarget(transform);
	}//moveKinematicActor

	//------------------------------------------------------------
	
	
	
	void CDynamicActor::setSimulation(bool simulate)
	{
		CActor::setSimulation(simulate);
	}

	void CDynamicActor::addVerticalConstraint()
	{
		_joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
		_joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
	}

	void CDynamicActor::addZ0Constraint()
	{
		_joint->setMotion(PxD6Axis::eZ, PxD6Motion::eLOCKED);
	}

	void CDynamicActor::addForce(const Vector3 &force, TipoFuerza tipofuerza)
	{
		switch (tipofuerza)
		{
		case Physics::IMPULSO:
			((PxRigidDynamic*)_actor)->addForce(Vector3ToPxVec3(force), PxForceMode::eIMPULSE);
			break;
		case Physics::FUERZA:
			((PxRigidDynamic*)_actor)->addForce(Vector3ToPxVec3(force));
			break;
		}
	}


	void CDynamicActor::addTorque(const Vector3 &torque, TipoFuerza tipofuerza)
	{
		switch (tipofuerza)
		{
		case Physics::IMPULSO:
			((PxRigidDynamic*)_actor)->addTorque(Vector3ToPxVec3(torque), PxForceMode::eIMPULSE);
			break;
		case Physics::FUERZA:
			((PxRigidDynamic*)_actor)->addTorque(Vector3ToPxVec3(torque));
			break;
		}
	}

}
