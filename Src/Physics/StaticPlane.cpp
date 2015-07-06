/**
@file Actor.cpp

Contiene la implementaci�n de la encapsulaci�n del actor est�tico de f�sica

@see Physics::CStaticActor

@author Alberto Mart�nez Villar�n
@date Enero, 2015
*/

#include "StaticPlane.h"
#include "Server.h"
#include "Conversions.h"

#include <PxPhysicsAPI.h>

using namespace physx;

using namespace Logic;



namespace Physics
{
	CStaticPlane::CStaticPlane(const Vector3 &point, const Vector3 &normal, int group, const IPhysics *component, PxMaterial *material)
	{
		_scene = Physics::CScene::getSingletonPtr()->getScene();

		_dynamic = false;
		// Crear un plano est�tico
		PxPlane plane(Vector3ToPxVec3(point), Vector3ToPxVec3(normal));
		PxRigidStatic *actor = PxCreatePlane(*_server->getSdk(), plane, *material);
		// Anotar el componente l�gico asociado a la entidad f�sica
		_actor->userData = (void *) component;

		//Indicamos al actor con quien debe colisionar
		_server->setupFiltering(_actor, CollisionGroup(group));

		// A�adir el actor a la escena
		_scene->addActor(*actor);
	}
}
