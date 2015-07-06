
#include "TongueBallController.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Logic/Entity/Components/PhysicEntity.h"
#include "Physics/DynamicActor.h"

#include "Logic/Maps/EntityFactory.h"

using namespace Logic;
using namespace Physics;

IMP_FACTORY(CTongueBallController);

bool CTongueBallController::OnSpawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("start_impulse"))
	{
		_startForce = entityInfo->getIntAttribute("start_impulse");
	}

	if(entityInfo->hasAttribute("impulse_deviation"))
	{
		_forceDeviation = entityInfo->getFloatAttribute("impulse_deviation");
	}

	return true;
} 

//---------------------------------------------------------

bool CTongueBallController::respawn(const Map::CEntity *entityInfo)
{
	
	if(entityInfo->hasAttribute("start_impulse"))
	{
		_startForce = entityInfo->getIntAttribute("start_impulse");
	}

	if(entityInfo->hasAttribute("impulse_deviation"))
	{
		_forceDeviation = entityInfo->getFloatAttribute("impulse_deviation");
	}

	return true;
}

//---------------------------------------------------------

bool CTongueBallController::activate()
{
	return true;
}

//---------------------------------------------------------

void CTongueBallController::deactivate()
{
}

//---------------------------------------------------------

bool CTongueBallController::accept(const std::shared_ptr<Logic::IMessage> &message)
{
	return false;
}

//---------------------------------------------------------

void CTongueBallController::process(const std::shared_ptr<Logic::IMessage> &message)
{
	
}

//---------------------------------------------------------

void CTongueBallController::onTick(unsigned int msecs) 
{
	
}

void CTongueBallController::launch(Vector3 dir)
{
	CPhysicEntity *physComp = (CPhysicEntity*)_entity->getComponent("CPhysicEntity");
	_actor = dynamic_cast<CDynamicActor*>(physComp->getActor());
	_actor->setSimulation(true);

	int midForce = (_startForce * _forceDeviation); //un 25% de la fuerza como desviación
	int lowForce = _startForce - midForce;
	int finalForce = rand() % midForce + lowForce;

	Vector3 finalDir = (dir - _entity->getPosition()) * Vector3(1,1,0);
	finalDir.normalise();

	_actor->addForce(finalDir * finalForce , Physics::TipoFuerza::IMPULSO);
}

//---------------------------------------------------------






