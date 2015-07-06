/**
@file CoinTorqueController.cpp

@author Alberto Martínez
@date Abril, 2015
*/

#include "CoinTorqueController.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Logic/Entity/Components/PhysicEntity.h"
#include "Physics/DynamicActor.h"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace Logic;
using namespace Physics;

IMP_FACTORY(CCoinTorqueController);


bool CCoinTorqueController::OnSpawn(const Map::CEntity *entityInfo)
{
	_torque = -1;

	if(entityInfo->hasAttribute("torque_speed"))
	{
		_torque = entityInfo->getFloatAttribute("torque_speed");
	}

	if(entityInfo->hasAttribute("start_impulse"))
	{
		_startForce = entityInfo->getIntAttribute("start_impulse");
	}

	if(entityInfo->hasAttribute("impulse_deviation"))
	{
		_forceDeviation = entityInfo->getFloatAttribute("impulse_deviation");
	}

	if(entityInfo->hasAttribute("dispersion_angle"))
	{
		_dispersionAngle = entityInfo->getIntAttribute("dispersion_angle");
	}

	return true;
} 

//---------------------------------------------------------

bool CCoinTorqueController::respawn(const Map::CEntity *entityInfo)
{
	_torque = -1;

	if(entityInfo->hasAttribute("torque_speed"))
	{
		_torque = entityInfo->getFloatAttribute("torque_speed");
	}

	if(entityInfo->hasAttribute("start_impulse"))
	{
		_startForce = entityInfo->getIntAttribute("start_impulse");
	}

	if(entityInfo->hasAttribute("impulse_deviation"))
	{
		_forceDeviation = entityInfo->getFloatAttribute("impulse_deviation");
	}

	if(entityInfo->hasAttribute("dispersion_angle"))
	{
		_dispersionAngle = entityInfo->getIntAttribute("dispersion_angle");
	}
	return true;
}

//---------------------------------------------------------

bool CCoinTorqueController::activate()
{
	CPhysicEntity *physComp = (CPhysicEntity*)_entity->getComponent("CPhysicEntity");

	if(physComp != NULL)
	{
		if(physComp->getPhysicType() == "dynamic")
		{
			_actor = dynamic_cast<CDynamicActor*>(physComp->getActor());
			_actor->setSimulation(true);

			int angle = rand() % _dispersionAngle;
			int rnd = rand() % 2;
			//BaseSubsystems::Log::Debug(""+std::to_string(rnd));
			if (rnd == 0)
			{
				angle *= -1;
			}
			angle += 90;
			//BaseSubsystems::Log::Debug("angle offset: "+std::to_string(angle));

			float Radangle = Math::fromDegreesToRadians(angle);
			Vector3 direction = Vector3(cos(Radangle),sin(Radangle),0);
			direction.normalise();
			//BaseSubsystems::Log::Debug(std::to_string(direction.x)+", "+std::to_string(direction.y)+", "+std::to_string(direction.z));

			//direction.normalise();

			int midForce = (_startForce * _forceDeviation); //un 25% de la fuerza como desviación
			int lowForce = _startForce - midForce;
			int finalForce = rand() % midForce + lowForce;

			_actor->addForce(direction * finalForce, Physics::TipoFuerza::IMPULSO);
			//BaseSubsystems::Log::Debug(_entity->getName()+" Impulso: "+std::to_string(finalForce));

			//int midTorque = (_torque * 0.5); //un 50% del torque como desviación
			//int lowTorque = _torque - midTorque;
			//int finalTorque = rand() % midTorque + lowTorque;
			//_torque = finalTorque;
			//BaseSubsystems::Log::Debug(_entity->getName()+" Torque: "+std::to_string(finalTorque));

			/*_actor->addTorque(Vector3(0,_torque, 0)); */

			return true;
		}
		else
		{
			return false;
		}
		
	}
	else
	{
		return false;
	}
	
}

//---------------------------------------------------------

void CCoinTorqueController::deactivate()
{

}

//---------------------------------------------------------

bool CCoinTorqueController::accept(const std::shared_ptr<Logic::IMessage> &message)
{
	return false;
}

//---------------------------------------------------------

void CCoinTorqueController::process(const std::shared_ptr<Logic::IMessage> &message)
{
	
}

//---------------------------------------------------------

void CCoinTorqueController::onTick(unsigned int msecs) 
{
	if (_torque != -1)
		_actor->addTorque(Vector3(0,_torque, 0)); 
}

//---------------------------------------------------------






