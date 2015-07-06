/**
@file PhysicCoin.cpp

@see Logic::CPhysicCoin
@see Logic::IComponent
@see Logic::CPhysicController

@author Alberto Martínez
@date Abril, 2015
*/

#include "PhysicCoin.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Physics/DynamicActor.h"

using namespace Logic;
using namespace Physics;



IMP_FACTORY(CPhysicCoin);

//---------------------------------------------------------

CPhysicCoin::CPhysicCoin() : CPhysicEntity()
{
	_timeToActivateTrigger = 1000.0f;
	_timeAcum = 0.0f;
}

//---------------------------------------------------------

CPhysicCoin::~CPhysicCoin() 
{
	

} 


bool CPhysicCoin::OnSpawn(const Map::CEntity *entityInfo)
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

	Vector3 offset = entityInfo->hasAttribute("coin_offset") ? entityInfo->getVector3Attribute("coin_offset") : 
		Vector3::ZERO;


	createDynamicBox(_entity->getPosition() + offset, physicDimensions, 1.0, true, true, group, Vector3(0,0,0), "");

	_scene = _actor->getScene();

	if(entityInfo->hasAttribute("time_to_activate_trigger"))
	{
		_timeToActivateTrigger = entityInfo->getFloatAttribute("time_to_activate_trigger");
	}

	_simulation = false;
	_actor->setSimulation(false);

	return true;
}

//---------------------------------------------------------

bool CPhysicCoin::respawn(const Map::CEntity *entityInfo)
{
	_movement = Vector3::ZERO;
	_transform = Matrix4::ZERO;

	Vector3 offset = entityInfo->hasAttribute("coin_offset") ? entityInfo->getVector3Attribute("coin_offset") : 
		Vector3::ZERO;

	//Seteo su posicion
	_actor->setPosition(_entity->getPosition() + offset);

	if(entityInfo->hasAttribute("time_to_activate_trigger"))
	{
		_timeToActivateTrigger = entityInfo->getFloatAttribute("time_to_activate_trigger");
	}
	return true;
	
}

//---------------------------------------------------------

bool CPhysicCoin::activate()
{
	_timeAcum = 0.0f;
	return CPhysicEntity::activate();
}

//---------------------------------------------------------
void CPhysicCoin::onTick(unsigned int msecs) 
{
	if(_timeAcum < _timeToActivateTrigger)
	{
		_timeAcum += msecs;
	}

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


void CPhysicCoin::onTrigger(IPhysics *otherComponent, bool enter)
{
	if( _timeAcum >= _timeToActivateTrigger)
	{
		CPhysicEntity::onTrigger(otherComponent, enter);
	}
}




