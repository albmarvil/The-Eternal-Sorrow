/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "particulaChupaAlmasController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "LUA/ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CParticulaChupaAlmasController);
	
	//---------------------------------------------------------

	CParticulaChupaAlmasController::~CParticulaChupaAlmasController()
	{

	}

	bool CParticulaChupaAlmasController::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_moveTo = false;

		bool ok =	entityInfo->hasAttribute("speed") &&
					entityInfo->hasAttribute("distanceToDisappear");

		assert(ok && "Error en el onspawn del componente particulaChupaAlmasController");

		_speed = entityInfo->getFloatAttribute("speed");

		_distanceToDisappear = entityInfo->getFloatAttribute("distanceToDisappear");

		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CParticulaChupaAlmasController::respawn(const Map::CEntity *entityInfo)
	{
		_moveTo = false;
		
		bool ok =	entityInfo->hasAttribute("speed") &&
					entityInfo->hasAttribute("distanceToDisappear");

		assert(ok && "Error en el onspawn del componente particulaChupaAlmasController");

		_speed = entityInfo->getFloatAttribute("speed");

		_distanceToDisappear = entityInfo->getFloatAttribute("distanceToDisappear");

		return true;
	}

	bool CParticulaChupaAlmasController::activate()
	{
		return true;
	}

	void CParticulaChupaAlmasController::deactivate()
	{

	}

	void CParticulaChupaAlmasController::onTick(unsigned int msecs)
	{
		//Si tiene donde moverse
		if(_moveTo && _destino)
		{
			Vector3 dir = _destino->getCenterPosition() - _entity->getPosition();

			dir.normalise();

			Vector3 pos = _entity->getPosition();

			pos += dir * msecs * _speed;

			_entity->setPosition(pos);

			if(_entity->getPosition().distance(_destino->getCenterPosition()) < _distanceToDisappear)
			{
				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			}
		}
		else if(_moveTo && !_destino)
		{
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}
	}

	void CParticulaChupaAlmasController::moveTo(CEntity *destino)
	{
		_moveTo = true;
		_destino = destino;
	}

} // namespace Logic

