

/**
@file PlayerLightController.cpp


 
@see Logic::CPlayerLightController
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "PlayerLightController.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CPlayerLightController);
	
	//---------------------------------------------------------

	bool CPlayerLightController::OnSpawn(const Map::CEntity *entityInfo)
	{
		

		if(entityInfo->hasAttribute("player_offset"))
		{
			_offset = entityInfo->getVector3Attribute("player_offset");
		}

		return true;

	} // spawn

	bool CPlayerLightController::respawn(const Map::CEntity* entityInfo)
	{
		if(entityInfo->hasAttribute("player_offset"))
		{
			_offset = entityInfo->getVector3Attribute("player_offset");
		}
		return true;
	}

	//---------------------------------------------------------

	bool CPlayerLightController::activate()
	{
		
		return true;

	} // activate
	
	//---------------------------------------------------------

	void CPlayerLightController::deactivate()
	{
		_player = 0;

	} // deactivate
	
	//---------------------------------------------------------

	void CPlayerLightController::onTick(unsigned int msecs)
	{
		_player = CServer::getSingletonPtr()->getPlayer();
		if(_player)
		{
			_entity->setPosition(_player->getPosition() + _offset);
		}

	} // tick

} // namespace Logic

