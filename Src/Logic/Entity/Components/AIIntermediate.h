/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __AI_INTERMEDIATE__
#define __AI_INTERMEDIATE__

#include "Logic/Entity/Component.h"
#include <map>
#include <vector>
namespace Map
{
	class CEntity;
}

namespace Logic
{
	class CIAAvoid;
	class CPhysicController;
}

namespace Logic
{
	enum SteeringMovement
	{
		Avoid = 0,
		SeekMultiDir = 1,
		SeekSingleDir = 2,
		SeekMultiPos = 3,
		Wander = 4,

	};

	class CAIIntermediate: public IComponent
	{
		DEC_FACTORY(CAIIntermediate);
	public:

		CAIIntermediate():IComponent(), _controller(0){}
		virtual ~CAIIntermediate(){}

		bool OnSpawn(const Map::CEntity* entityInfo);
		bool activate();
		void deactivate();

		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		virtual void process(const std::shared_ptr<Logic::IMessage> &message);


		void onTick(unsigned int msecs);

		void addNewMovement(int idMovement, Logic::IComponent* funcionCalculoVelocidad);

		void setVelocityMultiplier(float velmult) {_velocityMult = velmult;}

	private:

		std::map<int, Logic::IComponent* > _vectorSteeringMovement;
		Vector3 _velocity;
		float _velocityMult;
		Vector3 _aceleration;

		//componente físico del enemigo
		CPhysicController* _controller;
	};

	REG_FACTORY(CAIIntermediate);
}

#endif