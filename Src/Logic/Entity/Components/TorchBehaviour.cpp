/**
@file TorchBehaviour.cpp

Contiene la implementación del componente que controla la luz de la escena
 
@see Logic::CTorchBehaviour
@see Logic::IComponent

@author Bárbara Domínguez
@date Enero, 2015
*/

#include "TorchBehaviour.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

namespace Logic 
{
	IMP_FACTORY(CTorchBehaviour);
	
	//---------------------------------------------------------

	CTorchBehaviour::~CTorchBehaviour() 
	{
		
	} 

	bool CTorchBehaviour::OnSpawn(const Map::CEntity *entityInfo)
	{
	
		assert(entityInfo->hasAttribute("attenuation_range"));
		_baseRange = entityInfo->getFloatAttribute("attenuation_range");
		assert(entityInfo->hasAttribute("attenuation_constant"));
		_baseConstant = entityInfo->getFloatAttribute("attenuation_constant");
		assert(entityInfo->hasAttribute("attenuation_linear"));
		_baseLinear = entityInfo->getFloatAttribute("attenuation_linear");
		assert(entityInfo->hasAttribute("attenuation_quadratic"));
		_baseQuadratic = entityInfo->getFloatAttribute("attenuation_quadratic");

		assert(entityInfo->hasAttribute("torch_flickering"));
		_flickering = entityInfo->getFloatAttribute("torch_flickering");

		assert(entityInfo->hasAttribute("torch_min_range"));
		_minRangeMultiplier = entityInfo->getFloatAttribute("torch_min_range");
		assert(entityInfo->hasAttribute("torch_max_range"));
		_maxRangeMultiplier = entityInfo->getFloatAttribute("torch_max_range");

		assert(entityInfo->hasAttribute("torch_min_linear"));
		_minLinearMultiplier = entityInfo->getFloatAttribute("torch_min_linear");
		assert(entityInfo->hasAttribute("torch_max_linear"));
		_maxLinearMultiplier = entityInfo->getFloatAttribute("torch_max_linear");
		

		assert(entityInfo->hasAttribute("torch_min_quadratic"));
		_minQuadraticMultiplier = entityInfo->getFloatAttribute("torch_min_quadratic");
		assert(entityInfo->hasAttribute("torch_max_quadratic"));
		_maxQuadraticMultiplier = entityInfo->getFloatAttribute("torch_max_quadratic");
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CTorchBehaviour::respawn(const Map::CEntity* entityInfo)
	{
		if(entityInfo->hasAttribute("attenuation_range"))//si tiene uno de los parámetros suponemos el resto
		{
			float range = entityInfo->getFloatAttribute("attenuation_range");
			assert(entityInfo->hasAttribute("attenuation_constant"));
			float constant = entityInfo->getFloatAttribute("attenuation_constant");
			assert(entityInfo->hasAttribute("attenuation_linear"));
			float linear = entityInfo->getFloatAttribute("attenuation_linear");
			assert(entityInfo->hasAttribute("attenuation_quadratic"));
			float quadratic = entityInfo->getFloatAttribute("attenuation_quadratic");
			assert(entityInfo->hasAttribute("torch_flickering"));
			_flickering = entityInfo->getFloatAttribute("torch_flickering");

			assert(entityInfo->hasAttribute("torch_min_range"));
			_minRangeMultiplier = entityInfo->getFloatAttribute("torch_min_range");
			assert(entityInfo->hasAttribute("torch_max_range"));
			_maxRangeMultiplier = entityInfo->getFloatAttribute("torch_max_range");

			assert(entityInfo->hasAttribute("torch_min_linear"));
			_minLinearMultiplier = entityInfo->getFloatAttribute("torch_min_linear");
			assert(entityInfo->hasAttribute("torch_max_linear"));
			_maxLinearMultiplier = entityInfo->getFloatAttribute("torch_max_linear");
		

			assert(entityInfo->hasAttribute("torch_min_quadratic"));
			_minQuadraticMultiplier = entityInfo->getFloatAttribute("torch_min_quadratic");
			assert(entityInfo->hasAttribute("torch_max_quadratic"));
			_maxQuadraticMultiplier = entityInfo->getFloatAttribute("torch_max_quadratic");
		}
		return true;
	}

	bool CTorchBehaviour::activate()
	{
		_timeAcum = 0.0f;
		
		return true;
	}//activate

	//-----------------------------------------------------

	void CTorchBehaviour::deactivate()
	{

	}//deactivate

	//-----------------------------------------------------

	void CTorchBehaviour::onTick(unsigned int msecs)
	{
		//float timeInSeconds = msecs * 0.001;


		_timeAcum += msecs;

		if(_timeAcum < _flickering && _timeAcum >= (_flickering*0.5))
		{
			//añadimos aleatoriedad y un X% de probabilidad de cambiar la iluminacion antes del tiempo de flicker
			int rnd = rand()%100;
			if(rnd < 10)
			{
				_changeTorch = true;
			}
		}

		//cambio
		if(_timeAcum >= _flickering || _changeTorch)
		{
			_changeTorch = false;
			_timeAcum = 0.0f;

			#pragma region parametro Linear

			float maxlinear = _baseLinear * _maxLinearMultiplier;
			float minLinear = _baseLinear * _minLinearMultiplier;
			
			int rnd = rand() % 3;
			float destLinear = _baseLinear;
			switch (rnd)
			{
			case 0:
				destLinear = minLinear;
				break;
			case 1:
				destLinear = _baseLinear;
				break;
			case 2:
				destLinear = maxlinear;
				break;
			}
			#pragma endregion




			#pragma region parametro Quadratic

			float maxQuadratic = _baseQuadratic * _maxQuadraticMultiplier;
			float minQuadratic = _baseQuadratic * _minQuadraticMultiplier;
			
			int rndQuadratic = rand() % 3;
			float destQuadratic = _baseQuadratic;
			switch (rndQuadratic)
			{
			case 0:
				destQuadratic = minQuadratic;
				break;
			case 1:
				destQuadratic = _baseQuadratic;
				break;
			case 2:
				destQuadratic = maxQuadratic;
				break;
			}

			#pragma endregion


			
			#pragma region parametro range

			float maxRange = _baseRange * _maxRangeMultiplier;
			float minRange = _baseRange * _minRangeMultiplier;
			
			int rndRange = rand() % 3;
			float destRange = _baseRange;
			switch (rndRange)
			{
			case 0:
				destRange = minRange;
				break;
			case 1:
				destRange = _baseRange;
				break;
			case 2:
				destRange = maxRange;
				break;
			}

			#pragma endregion
			//mando el mensaje
			std::shared_ptr<SET_ATTENUATION> m(new SET_ATTENUATION());
			m->setAttenuation(Vector4(destRange, _baseConstant, destLinear, destQuadratic));
			_entity->emitMessage(m);

		}

	}

	//-----------------------------------------------------

	bool CTorchBehaviour::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return false;
	}

	void CTorchBehaviour::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		
	}

} // namespace Logic

