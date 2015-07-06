/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "PinchosController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"

#include "LUA/ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CPinchosController);
	
	//---------------------------------------------------------

	CPinchosController::~CPinchosController()
	{

	}

	bool CPinchosController::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CPinchosController::respawn(const Map::CEntity *entityInfo)
	{

		return true;
	}

	bool CPinchosController::activate()
	{
		_down = _up = false;

		_valorArriba = _entity->getPosition() + Vector3(0,10,0);
		_valorAbajo = _entity->getPosition();
		return true;
	}

	void CPinchosController::deactivate()
	{

	}

	void CPinchosController::onTick(unsigned int msecs)
	{
		if(_up)
		{
			_entity->setPosition(_entity->getPosition() + Vector3(0,2.5,0));	

			if (_entity->getPosition().y >= _valorArriba.y)
			{
				_up = false;
			}
			
		}
		else if(_down)
		{
			_entity->setPosition(_entity->getPosition() + Vector3(0,-0.5,0));

			if (_entity->getPosition().y <= _valorAbajo.y)
			{
				_down = false;
			}
		}
	}

} // namespace Logic

