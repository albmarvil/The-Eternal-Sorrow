/**
@file PhysicWeapon.cpp

Contiene la implementación del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicWeapon
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicWeapon.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Physics/DynamicActor.h"

#include "WeaponController.h"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace Logic;

IMP_FACTORY(CPhysicWeapon);

//---------------------------------------------------------

CPhysicWeapon::CPhysicWeapon() : CPhysicEntity()
{

}

//---------------------------------------------------------

CPhysicWeapon::~CPhysicWeapon() 
{
	

} 


//---------------------------------------------------------

bool CPhysicWeapon::activate()
{
	if(!CPhysicEntity::activate())
	{
		return false;
	}

	_weaponController =  _entity->getComponent<CWeaponController*>();

	//nos registramos como oyentes de la posicion lógica de la entidad
	_entity->addEntityTransformListener(this);

	return true;
}


void CPhysicWeapon::deactivate()
{
	CPhysicEntity::activate();

	_entity->removeEntityTransformListener(this);

}


//---------------------------------------------------------
//void CPhysicWeapon::onTick(unsigned int msecs) 
//{
//	_transform = _weaponController->getPivotTransform();
//
//	CPhysicEntity::onTick(msecs);
//
//	
//}

void CPhysicWeapon::OnEntitySetTransform(const Matrix4 &transform)
{
	_transform = transform;
	
	if(_actor->isRigidDynamic())
	{
		Physics::CDynamicActor* dinActor = (Physics::CDynamicActor*) _actor;

		// Si es una entidad estática no hacemos nada
		if(!dinActor) 
			return;

		// Si el objeto físico es cinemático intentamos moverlo de acuerdo 
		// a los mensajes KINEMATIC_MOVE recibidos 
		if (dinActor->isKinematic()) 
		{
			if(_transform != Matrix4::ZERO)
			{
				dinActor->moveKinematicActor(_transform);
			}
			_transform = Matrix4::ZERO;
		}
	}
}




