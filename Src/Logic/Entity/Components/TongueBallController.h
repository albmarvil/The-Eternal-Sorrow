
#ifndef __Logic_TongueBallController_H
#define __Logic_TongueBallController_H

#include "Logic/Entity/Component.h"

// Predeclaración de tipos
namespace Physics {
	class CDynamicActor;
};


// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	
	class CTongueBallController : public IComponent
	{
		DEC_FACTORY(CTongueBallController);
	public:
	
		CTongueBallController(){};

		
		virtual ~CTongueBallController() {};
		
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

	
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		virtual void deactivate();

		void launch(Vector3 dir);
		
	protected:

		float _forceDeviation;

		int _startForce;

		Physics::CDynamicActor* _actor;

	}; // class CCoinTorqueController

	REG_FACTORY(CTongueBallController);

} // namespace Logic

#endif // __Logic_CoinTorqueController_H
