/**
@file CoinTorqueController.h

@author Alberto Martínez Villarán
@date Abril, 2015
*/

#ifndef __Logic_CoinTorqueController_H
#define __Logic_CoinTorqueController_H

#include "Logic/Entity/Component.h"

// Predeclaración de tipos
namespace Physics {
	class CDynamicActor;
};


// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	
	class CCoinTorqueController : public IComponent
	{
		DEC_FACTORY(CCoinTorqueController);
	public:
	
		CCoinTorqueController() : _torque(0.0f), _startForce(10), _dispersionAngle(20), _forceDeviation(1.0f) {};

		
		virtual ~CCoinTorqueController() {};
		
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

	
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		virtual void deactivate();

	protected:

		float _torque;

		float _forceDeviation;

		int _startForce;

		int _dispersionAngle;

		Physics::CDynamicActor* _actor;

	}; // class CCoinTorqueController

	REG_FACTORY(CCoinTorqueController);

} // namespace Logic

#endif // __Logic_CoinTorqueController_H
