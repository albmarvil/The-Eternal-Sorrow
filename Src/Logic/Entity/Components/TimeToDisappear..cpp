/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "TimeToDisappear.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Logic/Maps/EntityFactory.h"
#include "Graphics.h"
#include "Graphics/Entity.h"

namespace Logic
{
	IMP_FACTORY(CTimeToDisappear);

	//---------------------------------------------------------

	bool CTimeToDisappear::OnSpawn(const Map::CEntity *entityInfo)
	{
		_timeStamp =  _timeStampParpadeo = _timeStampInvisible = 0;

		bool ok =	entityInfo->hasAttribute("time_to_start_to_disappear") &&
					entityInfo->hasAttribute("time_between_parpadeo") &&
					entityInfo->hasAttribute("proporcion") &&
					entityInfo->hasAttribute("timeInvisible");

		assert(ok && "Error en el onspawn del componente timetodisappear");

		_time_to_start_to_disappear = entityInfo->getFloatAttribute("time_to_start_to_disappear") * 1000;

		_time_between_parpadeo = entityInfo->getFloatAttribute("time_between_parpadeo")*1000;

		_proporcion = entityInfo->getFloatAttribute("proporcion");
	
		_timeInvisible = entityInfo->getFloatAttribute("timeInvisible") * 1000;
		

		return true;
	}

	bool CTimeToDisappear::respawn(const Map::CEntity *entityInfo)
	{
		_timeStamp =  _timeStampParpadeo = _timeStampInvisible = 0;

		bool ok =	entityInfo->hasAttribute("time_to_start_to_disappear") &&
					entityInfo->hasAttribute("time_between_parpadeo") &&
					entityInfo->hasAttribute("proporcion") &&
					entityInfo->hasAttribute("timeInvisible");

		assert(ok && "Error en el onspawn del componente timetodisappear");

		_time_to_start_to_disappear = entityInfo->getFloatAttribute("time_to_start_to_disappear") * 1000;

		_time_between_parpadeo = entityInfo->getFloatAttribute("time_between_parpadeo")*1000;

		_proporcion = entityInfo->getFloatAttribute("proporcion");
	
		_timeInvisible = entityInfo->getFloatAttribute("timeInvisible") * 1000;
		

		return true;
	}

	bool CTimeToDisappear::activate()
	{
		return true;
	}

	void CTimeToDisappear::deactivate()
	{

	}

	void CTimeToDisappear::onTick(unsigned int msecs)
	{
		_timeStamp += msecs;
		
		//Si hemos agotado el tiempo sin parpadear, empezamos a parpadear
		if(_timeStamp >= _time_to_start_to_disappear)
		{
			if(_isVisible)
			{
				_timeStampParpadeo += msecs;
				//Miramos si agotamos el tiempo entre parpadeo
				if(_timeStampParpadeo >= _time_between_parpadeo)
				{
					_timeStampParpadeo = 0;
					_time_between_parpadeo /= _proporcion;
					//Hacemos unvisible el sceneNode de la entidad grafica
					_entity->getComponent<CGraphics*>()->getGraphicsEntity()->setVisible(false);
					_isVisible = false;

					//Si el tiempo entre parpadeo es menor a 50 milisegundos, destruimos la entidad
					if(_time_between_parpadeo <= 5)
					{
						Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
					}
				}
			
			}
			else
			{
				_timeStampInvisible += msecs;
				
				if(_timeStampInvisible >= _timeInvisible)
				{
					_timeStampInvisible = 0;
					_isVisible = true;
					//Hacemos visible de nuevo al sceneNode de la entidad grafica
					_entity->getComponent<CGraphics*>()->getGraphicsEntity()->setVisible(true);
				}
			}
		}
		
	}

}