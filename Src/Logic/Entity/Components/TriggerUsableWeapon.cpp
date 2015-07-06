/**
@file TriggerUsableWeapon.cpp

@see Logic::CTriggerUsableWeapon
@see Logic::IComponent
@see Logic::CPhysicController

@author Alberto Martínez
@date Abril, 2015
*/

#include "TriggerUsableWeapon.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Physics/DynamicActor.h"

using namespace Logic;
using namespace Physics;



IMP_FACTORY(CTriggerUsableWeapon);


bool CTriggerUsableWeapon::OnSpawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("cointrigger_dimensions"))
	{
		physicDimensions = entityInfo->getVector3Attribute("cointrigger_dimensions");
	}

	int group = 0;
	if(entityInfo->hasAttribute("cointrigger_group"))
	{
		group = entityInfo->getIntAttribute("cointrigger_group");
	}
	createDynamicBox(_entity->getPosition(), physicDimensions, 1.0, true, true, group, Vector3(0,0,0), "");

	_scene = _actor->getScene();

	_simulation = false;
	_actor->setSimulation(false);

	return true;
}

//---------------------------------------------------------

bool CTriggerUsableWeapon::respawn(const Map::CEntity *entityInfo)
{
	_movement = Vector3::ZERO;
	_transform = Matrix4::ZERO;
	//Seteo su posicion
	_actor->setPosition(_entity->getPosition());
	
	return true;
	
}

//---------------------------------------------------------

bool CTriggerUsableWeapon::activate()
{
	return CPhysicEntity::activate();
}

//---------------------------------------------------------
void CTriggerUsableWeapon::onTick(unsigned int msecs) 
{
	if(_simulation)
	{
		// Comprobamos si es la entidad es estática o dinámica
		CDynamicActor *dinActor = NULL;
		if(_actor->isRigidDynamic())
			dinActor = (CDynamicActor*) _actor;

		// Si es una entidad estática no hacemos nada
		if (!dinActor) 
			return;

		_transform = _entity->getTransform();

		// Si el objeto físico es cinemático intentamos moverlo de acuerdo 
		// a los mensajes KINEMATIC_MOVE recibidos 
		if (dinActor->isKinematic()) 
		{
			dinActor->moveKinematicActor(_transform);
			_transform = Matrix4::ZERO;
		}
	}
	
}


void CTriggerUsableWeapon::onTrigger(IPhysics *otherComponent, bool enter)
{
	CPhysicEntity::onTrigger(otherComponent, enter);
}




