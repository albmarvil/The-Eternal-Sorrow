/**
@file LaserBeamController.h

@author Alberto Martínez Villarán
@date Mayo, 2015
*/

#ifndef __Logic_LaserBeamController_H
#define __Logic_LaserBeamController_H

#include "Logic/Entity/Component.h"

namespace Logic
{
	class CRibbonTrailComponent;
};

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	
	enum LaserState
	{
		NONE, //no hace nada
		INIT,  //la entidad del proyectil está en la pos del arma
		OBJECTIVE, //la entidad del proyectil esta en la pos de impacto
		EFFECT, //se esta aplicando el efecto
		END, //la entidad del proyectil muere en el siguiente tick
	};

	class CLaserBeamController : public IComponent
	{
		DEC_FACTORY(CLaserBeamController);
	public:
		
		//constructor
		CLaserBeamController() : _trailComponent(0), _state(INIT), _orig(Vector3::ZERO), _objective(Vector3::ZERO){};

		///destructor
		virtual ~CLaserBeamController();
		

		virtual bool OnSpawn(const Map::CEntity *entityInfo);
		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		virtual void deactivate();


		void setLaserConfig(const Vector3 &orig, const Vector3 &objective, float delay, float delayPercentage);

		void setNewObjective(const Vector3 &orig, const Vector3 &objective);

		void forceEndEffect() { _state = END; };

	protected:
		
		LaserState _state;

		Vector3 _orig;

		Vector3 _objective;

		CRibbonTrailComponent *_trailComponent;

		float _trailWidthChange;

		float _objectiveWidthChange;

		float _objectiveLength;

		unsigned int _timeEffect;

		unsigned int _timeAcum;

		int _timeAliveTrail;

		int _particlesBeamOffset;

	}; // class CLaserBeamController

	REG_FACTORY(CLaserBeamController);

} // namespace Logic

#endif // __Logic_LaserBeamController_H
