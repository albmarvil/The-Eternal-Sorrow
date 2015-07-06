/**
@file SoulLightEffect.h

@author Alberto Martínez Villarán
@date Mayo, 2015
*/

#ifndef __Logic_SoulLightEffect_H
#define __Logic_SoulLightEffect_H

#include "Logic/Entity/Component.h"


// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	
	class CSoulLightEffect : public IComponent
	{
		DEC_FACTORY(CSoulLightEffect);
	public:
		
		//constructor
		CSoulLightEffect() {};

		///destructor
		virtual ~CSoulLightEffect();
		

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		
		//virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

	
		//virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		//virtual void deactivate();

	protected:
		
		unsigned int _timeAcum;

		unsigned int _timeEffectAcum;

		float _linear;

		float _constant;

		float _range;

		float _quadratic;

		unsigned int _timeCicle;

		unsigned int _waitEffect;

		bool _wait;

	}; // class CSoulLightEffect

	REG_FACTORY(CSoulLightEffect);

} // namespace Logic

#endif // __Logic_SoulLightEffect_H
