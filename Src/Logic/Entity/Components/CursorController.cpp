/**
@file CursorController.cpp

Contiene la implementación del componente que controla el giro del personaje y el movimiento del ratón
 
@see Logic::CursorController
@see Logic::IComponent

@author Bárbara Domínguez
@date Enero, 2015
*/

#include "CursorController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

#include "Graphics/Server.h"
#include "Physics/Server.h"

namespace Logic 
{
	IMP_FACTORY(CCursorController);
	
	//---------------------------------------------------------

	bool CCursorController::spawn(CEntity *entity, CMap *map, const Map::CEntity *entityInfo) 
	{
		if(!IComponent::spawn(entity,map,entityInfo))
			return false;

		if(entityInfo->hasAttribute("cursorGroupCollision"))
			GROUP_COLLISION = entityInfo->getIntAttribute("cursorGroupCollision");

		if(entityInfo->hasAttribute("lookingDirection"))
			lookingDirection = entityInfo->getIntAttribute("lookingDirection");
		
		posRel = Vector2();

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CCursorController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "MOUSE_ACTION";

	} // accept
	
	//---------------------------------------------------------

	void CCursorController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("MOUSE_ACTION") == 0)
		{
			SetRelativePos(dynamic_cast<MOUSE_ACTION*>(message.get())->getVector2());
		}
	} // process

	//---------------------------------------------------------

	void CCursorController::turn(int look)  
	{
		

		//emitimos un mensaje para que cambie la animación de andar al girar
		std::shared_ptr<Logic::CONTROL> m(new Logic::CONTROL());
		m->setString("turn");
		m->setInt(look);
		_entity->emitMessage(m);
	} // turn

	//--------------------------------------------------------

	void CCursorController::SetRelativePos(Vector2 relativepos)
	{
		posRel = relativepos;
	}

	//---------------------------------------------------------------------

	void CCursorController::tick(unsigned int msecs)
	{
		IComponent::tick(msecs);

		//en cada tick comprobamos la posición real del ratón y si es necesario giramos al personaje
		GetRealPos(posRel.x, posRel.y);
	}
} // namespace Logic

