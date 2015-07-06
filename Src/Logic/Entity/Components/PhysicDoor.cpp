/**
@file PhysicDoor.cpp

@see Logic::CPhysicDoor
@see Logic::IComponent
@see Logic::CPhysicController

@author Bárbara Domínguez de la Torre González
@date Abril, 2015
*/

#include "PhysicDoor.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Physics/DynamicActor.h"

using namespace Logic;
using namespace Physics;



IMP_FACTORY(CPhysicDoor);

//---------------------------------------------------------

CPhysicDoor::CPhysicDoor() : CPhysicEntity()
{
}

//---------------------------------------------------------

CPhysicDoor::~CPhysicDoor() 
{
	

} 


bool CPhysicDoor::OnSpawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("doortrigger_dimensions"))
	{
		physicDimensions = entityInfo->getVector3Attribute("doortrigger_dimensions");
	}

	int group = 0;
	if(entityInfo->hasAttribute("doortrigger_group"))
	{
		group = entityInfo->getIntAttribute("doortrigger_group");
	}

	Vector3 offset = Vector3::ZERO;
	if(entityInfo->hasAttribute("offset"))
	{
		offset = entityInfo->getVector3Attribute("offset");
	}
	createStaticBox(_entity->getPosition(), physicDimensions, 1.0, false, true, group, offset);

	_scene = _actor->getScene();

	return true;
}

//---------------------------------------------------------

bool CPhysicDoor::respawn(const Map::CEntity *entityInfo)
{
	_movement = Vector3::ZERO;
	_transform = Matrix4::ZERO;
	//Seteo su posicion
	_actor->setPosition(_entity->getPosition());
	return true;
	
}

//---------------------------------------------------------

bool CPhysicDoor::activate()
{
	return CPhysicEntity::activate();
}

//---------------------------------------------------------

void CPhysicDoor::onTrigger(IPhysics *otherComponent, bool enter)
{
	CPhysicEntity::onTrigger(otherComponent, enter);
}




