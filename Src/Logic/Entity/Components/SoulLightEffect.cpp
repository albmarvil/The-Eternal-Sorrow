/**
@file SoulLightEffect.cpp

@author Alberto Martínez
@date Mayo, 2015
*/

#include "SoulLightEffect.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace Logic;

IMP_FACTORY(CSoulLightEffect);

CSoulLightEffect::~CSoulLightEffect() 
{
}

bool CSoulLightEffect::OnSpawn(const Map::CEntity *entityInfo)
{

	if(entityInfo->hasAttribute("attenuation_range"))//si tiene uno de los parámetros suponemos el resto
	{
		_range = entityInfo->getFloatAttribute("attenuation_range");
		assert(entityInfo->hasAttribute("attenuation_constant"));
		_constant = entityInfo->getFloatAttribute("attenuation_constant");
		assert(entityInfo->hasAttribute("attenuation_linear"));
		_linear = entityInfo->getFloatAttribute("attenuation_linear");
		assert(entityInfo->hasAttribute("attenuation_quadratic"));
		_quadratic = entityInfo->getFloatAttribute("attenuation_quadratic");
	}

	if(entityInfo->hasAttribute("light_time_cicle"))
	{
		_timeCicle = entityInfo->getFloatAttribute("light_time_cicle") * 1000;
	}

	if(entityInfo->hasAttribute("light_effect_wait"))
	{
		_waitEffect = entityInfo->getFloatAttribute("light_effect_wait") * 1000;
	}

	return true;
} 

//---------------------------------------------------------

bool CSoulLightEffect::respawn(const Map::CEntity *entityInfo)
{

	if(entityInfo->hasAttribute("attenuation_range"))//si tiene uno de los parámetros suponemos el resto
	{
		_range = entityInfo->getFloatAttribute("attenuation_range");
		assert(entityInfo->hasAttribute("attenuation_constant"));
		_constant = entityInfo->getFloatAttribute("attenuation_constant");
		assert(entityInfo->hasAttribute("attenuation_linear"));
		_linear = entityInfo->getFloatAttribute("attenuation_linear");
		assert(entityInfo->hasAttribute("attenuation_quadratic"));
		_quadratic = entityInfo->getFloatAttribute("attenuation_quadratic");
	}

	if(entityInfo->hasAttribute("light_time_cicle"))
	{
		_timeCicle = entityInfo->getFloatAttribute("light_time_cicle") * 1000;
	}

	if(entityInfo->hasAttribute("light_effect_wait"))
	{
		_waitEffect = entityInfo->getFloatAttribute("light_effect_wait") * 1000;
	}
	return true;
}

//---------------------------------------------------------

bool CSoulLightEffect::activate()
{
	_timeAcum = 0;

	_timeEffectAcum = 0;
	return true;
	
}

//---------------------------------------------------------

//---------------------------------------------------------

void CSoulLightEffect::onTick(unsigned int msecs) 
{
	_timeAcum += msecs;

	if(_timeAcum >= _waitEffect && _wait)
	{
		//std::cout<<"end wait"<<std::endl;
		_wait = false;
		_timeAcum = _timeCicle * 0.75;
		_timeEffectAcum = 0;
	}

	if(!_wait)
	{
		_timeEffectAcum += msecs;

		float angle = (2 * _timeAcum * Math::PI) / _timeCicle;

		/*float r = std::max(sin(angle) * _range, _range*0.25f);
		float l = std::max(1/sin(angle) * _linear, _linear*0.25f);
		float q = std::max(1/sin(angle) * _quadratic, _quadratic * 0.1f);*/

		float l = (1+sin(angle)) * 0.5f * _linear + _linear;
		float q = (1+sin(angle)) * 0.5f *_quadratic + _linear;

		//std::cout<<l<<", "<<q<<std::endl;
		std::shared_ptr<SET_ATTENUATION> m (new SET_ATTENUATION());
		m->setAttenuation(Vector4(_range, _constant, l, q));
		_entity->emitMessage(m, this);

		if(_timeEffectAcum >= _timeCicle)
		{
			_timeEffectAcum = 0;
			//std::cout<<"wait"<<std::endl;
			_timeAcum = 0;
			_wait = true;

			std::shared_ptr<SET_ATTENUATION> m (new SET_ATTENUATION());
			m->setAttenuation(Vector4(_range, _constant, _linear, _quadratic));
			_entity->emitMessage(m, this);
		}
	}
	

}

//---------------------------------------------------------






