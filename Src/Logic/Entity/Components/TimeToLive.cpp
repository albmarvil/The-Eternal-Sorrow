/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "TimeToLive.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Logic/Maps/EntityFactory.h"

namespace Logic
{
	IMP_FACTORY(CTimeToLive);

	//---------------------------------------------------------

	bool CTimeToLive::OnSpawn(const Map::CEntity *entityInfo)
	{
		_timeStamp = _time_to_live = 0;

		assert(entityInfo->hasAttribute("time_to_live") && "Error en el onspawn del componente timetolive");

		_time_to_live = entityInfo->getFloatAttribute("time_to_live") * 1000;
		

		return true;
	}

	bool CTimeToLive::respawn(const Map::CEntity *entityInfo)
	{
		_timeStamp = _time_to_live = 0;

		assert(entityInfo->hasAttribute("time_to_live") && "Error en el onspawn del componente timetolive");

		_time_to_live = entityInfo->getFloatAttribute("time_to_live") * 1000;

		return true;
	}
	
	bool CTimeToLive::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CAN_TIME_TO_LIVE";
	} // accept

	//---------------------------------------------------------

	void CTimeToLive::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CAN_TIME_TO_LIVE")
		{
			_canCountTime = dynamic_cast<CAN_TIME_TO_LIVE*>(message.get())->getBool();
		}
	}

	bool CTimeToLive::activate()
	{
		_canCountTime = true;

		return true;
	}

	void CTimeToLive::deactivate()
	{

	}

	void CTimeToLive::onTick(unsigned int msecs)
	{
		if (_canCountTime)
		{
			_timeStamp += msecs;

			if(_timeStamp >= _time_to_live)
			{
				_timeStamp = 0;
				Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			}
		}
		
	}

	void CTimeToLive::setTimeToLive(int _time)
	{
		_time_to_live = _time * 1000;
	}
}