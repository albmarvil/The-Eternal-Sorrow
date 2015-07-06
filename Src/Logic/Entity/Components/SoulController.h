/**
@file SoulController.h

@author Alberto Martínez Villarán
@date Mayo, 2015
*/

#ifndef __Logic_SoulController_H
#define __Logic_SoulController_H

#include "Logic/Entity/Component.h"


// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	
	class CSoulController : public IComponent
	{
		DEC_FACTORY(CSoulController);
	public:
		
		//constructor
		CSoulController() : _maxSpeed(0.1f), _accel(0.002f), _actualSpeed(Vector3(0, _maxSpeed, 0)), _actualAccel(Vector3::ZERO), _timeAcum(0), _timeCicle(1000) {};

		///destructor
		virtual ~CSoulController();
		

		virtual bool OnSpawn(const Map::CEntity *entityInfo);
		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

	protected:
		
		float _maxSpeed;

		float _accel;

		float _sqrMaxSpeed;

		Vector3 _actualSpeed;

		Vector3 _actualAccel;

		unsigned int _timeAcum;

		unsigned int _timeCicle;

	}; // class CSoulController

	REG_FACTORY(CSoulController);

} // namespace Logic

#endif // __Logic_SoulController_H
