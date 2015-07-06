/**
@file SoulController.cpp

@author Alberto Martínez
@date Mayo, 2015
*/

#include "SoulController.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace Logic;

IMP_FACTORY(CSoulController);

CSoulController::~CSoulController() 
{
}

bool CSoulController::OnSpawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("max_speed"))
	{
		_maxSpeed = entityInfo->getFloatAttribute("max_speed");
	}

	if(entityInfo->hasAttribute("time_cicle"))
	{
		_timeCicle = entityInfo->getFloatAttribute("time_cicle") * 1000;
	}

	if(entityInfo->hasAttribute("accel"))
	{
		_accel = entityInfo->getFloatAttribute("accel");
	}
	return true;
} 

//---------------------------------------------------------

bool CSoulController::respawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("max_speed"))
	{
		_maxSpeed = entityInfo->getFloatAttribute("max_speed");
	}

	if(entityInfo->hasAttribute("time_cicle"))
	{
		_timeCicle = entityInfo->getFloatAttribute("time_cicle") * 1000;
	}

	if(entityInfo->hasAttribute("accel"))
	{
		_accel = entityInfo->getFloatAttribute("accel");
	}
	return true;
}

//---------------------------------------------------------

bool CSoulController::activate()
{
	
	_actualSpeed = Vector3(0, _maxSpeed, 0);

	_actualAccel = Vector3::ZERO;

	_timeAcum = 0;

	_sqrMaxSpeed = _maxSpeed * _maxSpeed;

	return true;
	
}

//---------------------------------------------------------


//---------------------------------------------------------

void CSoulController::onTick(unsigned int msecs) 
{
	_timeAcum += msecs;

	float angle = (2 * _timeAcum * Math::PI) / _timeCicle;

	//actualizamos la aceleración
	_actualAccel = Vector3(sin(angle), 1, 0);
	_actualAccel.normalise();
	_actualAccel *= _accel;

	_actualSpeed += _actualAccel * msecs;

	//limitamos a velocidad máxima
	if(_actualSpeed.squaredLength() > _sqrMaxSpeed)
	{
		_actualSpeed.normalise();
		_actualSpeed *= _maxSpeed;
	}


	//calculamos la nueva posicion
	Vector3 newPos = _entity->getPosition() + _actualSpeed * msecs;

	_entity->setPosition(newPos);
	//BaseSubsystems::Log::Debug("sin: " + std::to_string(sin(angle)));
	//BaseSubsystems::Log::Debug("Vel: " + std::to_string(_actualSpeed.x) + ", " + std::to_string(_actualSpeed.y));
	//BaseSubsystems::Log::Debug("Pos: " + std::to_string(newPos.x) + ", " + std::to_string(newPos.y));

}

//---------------------------------------------------------






