/**
@file LaserBeamController.cpp

@author Alberto Martínez
@date Mayo, 2015
*/

#include "LaserBeamController.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "RibbonTrailComponent.h"
#include "Logic/Maps/EntityFactory.h"



#define _USE_MATH_DEFINES
#include <math.h>


using namespace Logic;

IMP_FACTORY(CLaserBeamController);

CLaserBeamController::~CLaserBeamController() 
{
}

bool CLaserBeamController::OnSpawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("trail_width_change"))
	{
		_trailWidthChange = entityInfo->getFloatAttribute("trail_width_change");
	}

	if(entityInfo->hasAttribute("numParticlesBeam"))
	{
		_particlesBeamOffset = entityInfo->getFloatAttribute("numParticlesBeam");
	}
	return true;
} 

//---------------------------------------------------------

bool CLaserBeamController::respawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("trail_width_change"))
	{
		_trailWidthChange = entityInfo->getFloatAttribute("trail_width_change");
	}

	if(entityInfo->hasAttribute("particlesBeamOffset"))
	{
		_particlesBeamOffset = entityInfo->getFloatAttribute("particlesBeamOffset");
	}
	return true;
}

//---------------------------------------------------------

bool CLaserBeamController::activate()
{
	_trailComponent = (CRibbonTrailComponent*) _entity->getComponent("CRibbonTrailComponent");
	assert(_trailComponent);

	_timeAcum = 0;
	_timeAliveTrail = 500;
	_state = INIT;

	return true;
	
}

//---------------------------------------------------------

void CLaserBeamController::deactivate()
{
	_trailComponent = 0;
	_timeAcum = 0;
	_state = INIT;
	

	_orig = Vector3::ZERO;
	_objective = Vector3::ZERO;
}

//---------------------------------------------------------

//bool CLaserBeamController::accept(const std::shared_ptr<Logic::IMessage> &message)
//{
//
//	return false;
//}

//---------------------------------------------------------

//void CLaserBeamController::process(const std::shared_ptr<Logic::IMessage> &message)
//{
//	
//}

//---------------------------------------------------------

void CLaserBeamController::onTick(unsigned int msecs) 
{
	//BaseSubsystems::Log::Debug("Acum: " + std::to_string(_timeAcum));
	switch (_state)
	{
	case Logic::INIT:
		//colocamos la pos lógica de la entidad en el transform del arma
		//BaseSubsystems::Log::Debug("INIT");
		//_trailComponent->setTrailLength(0.1f);
		//_trailComponent->setTrailVisible(false);

		_entity->setPosition(_orig);
		_state = OBJECTIVE;
		break;
	case Logic::OBJECTIVE:
		//BaseSubsystems::Log::Debug("OBJECTIVE");

		_timeAcum += msecs;

		_trailComponent->setTrailLength(_objectiveLength);
		_trailComponent->setTrailWidthChange(_objectiveWidthChange);
		_trailComponent->setTrailVisible(true);
		//_timeAcum = 0;

		_entity->setPosition(_objective);
		_state = EFFECT;
		break;
	case Logic::EFFECT:
		//BaseSubsystems::Log::Debug("EFFECT");
		_timeAcum += msecs;

		if(_timeAcum >= _timeEffect)
		{
			_trailComponent->setTrailLength(0.1f);
			_trailComponent->setTrailVisible(false);

			_entity->setPosition(_orig);

			_state = END;
		}
		break;
	case Logic::END:
		//BaseSubsystems::Log::Debug("END");
		CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		break;
	}
}

//---------------------------------------------------------


void CLaserBeamController::setLaserConfig(const Vector3 &orig, const Vector3 &objective, float delay, float delayPercentage)
{
	_trailComponent->setTrailLength(0.1f);
	_trailComponent->setTrailVisible(false);

	float d = std::max(delay, 1.0f);
	_objective = objective;
	_objective.z = 0;

	_orig = orig;
	_orig.z = 0;

	_timeAcum = 0;

	_timeEffect = std::max(d - d * delayPercentage, 15.0f);

	_objectiveWidthChange =  _trailWidthChange * 1/(d * 0.001);

	float dist = objective.distance(orig);
	_objectiveLength = dist*1.12;

	Vector3 dirBeam = (objective - orig) * Vector3(1,1,0);
	dirBeam.normalise();
	// Particulas dentro del laser
	int numParticles = dist / _particlesBeamOffset;

	for (int i = 1; i <= numParticles; ++i)
	{
		//int random = rand() % 101;

		float distParticle = _particlesBeamOffset  * i;

		Vector3 posParticle = orig + dirBeam * distParticle;

		CEntityFactory::getSingletonPtr()->createEntityByType("LaserBeamParticle", posParticle, _entity->getMap());
	}

	_state = INIT;
}

void CLaserBeamController::setNewObjective(const Vector3 &orig, const Vector3 &objective)
{
	if(_state == EFFECT)
	{
		//BaseSubsystems::Log::Debug("New Objective");
		_objective = objective;
		_objective.z = 0;

		_orig = orig;
		_orig.z = 0;

		_objectiveLength = _objective.distance(_orig) + 15;

		//_timeAcum = 0;

		_state = INIT;
	}

}





