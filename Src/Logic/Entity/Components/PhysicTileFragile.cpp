
#include "PhysicTileFragile.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Physics/DynamicActor.h"

using namespace Logic;
using namespace Physics;



IMP_FACTORY(CPhysicTileFragile);

//---------------------------------------------------------

CPhysicTileFragile::CPhysicTileFragile() : CPhysicEntity()
{
}

//---------------------------------------------------------

CPhysicTileFragile::~CPhysicTileFragile() 
{
	

} 


bool CPhysicTileFragile::OnSpawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("tiletrigger_dimensions"))
	{
		physicDimensions = entityInfo->getVector3Attribute("tiletrigger_dimensions");
	}
	
	Vector3 offset = Vector3::ZERO;

	if(entityInfo->hasAttribute("offsetTrigger"))
	{
		offset = entityInfo->getVector3Attribute("offsetTrigger");
	}

	int group = 0;

	if(entityInfo->hasAttribute("tiletrigger_group"))
	{
		group = entityInfo->getIntAttribute("tiletrigger_group");
	}

	createStaticBox(_entity->getPosition(), physicDimensions, 1.0, false, true, group, offset);

	_scene = _actor->getScene();

	return true;
}

//---------------------------------------------------------

bool CPhysicTileFragile::respawn(const Map::CEntity *entityInfo)
{
	_movement = Vector3::ZERO;
	_transform = Matrix4::ZERO;
	//Seteo su posicion
	_actor->setPosition(_entity->getPosition());
	return true;
	
}

//---------------------------------------------------------

bool CPhysicTileFragile::activate()
{
	return CPhysicEntity::activate();
}

//---------------------------------------------------------

void CPhysicTileFragile::onTrigger(IPhysics *otherComponent, bool enter)
{
	CPhysicEntity::onTrigger(otherComponent, enter);
}




