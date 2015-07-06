/*
author Cesar Manuel Paz Guzman
date Marzo 2015
*/

#include "ScriptEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Component.h"
#include "Logic/Entity/Components/MovableText.h"
#include "Logic/Entity/components/StaticText.h"

namespace Logic 
{

	//---------------------------------------------------------

	CScriptEntity::~CScriptEntity() 
	{

	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CScriptEntity::spawn(const Map::CEntity *entityInfo) 
	{

		return true;
	} // spawn
	
	void CScriptEntity::tick(unsigned int msecs)
	{

	}

	void CScriptEntity::stopAnim()
	{
		//Cuando esta función sea llamada desde LUA, se encargara de enviar el mensaje de stopAnim
	}

	bool CScriptEntity::activate()
	{
		return true;
	}

	void CScriptEntity::deactivate()
	{

	}

	bool CScriptEntity::respawn(const Map::CEntity *entityInfo)
	{
		return true;
	}

	void CScriptEntity::setSleep(std::string nameComponent, bool sleep)
	{
 		IComponent* comp = _entity->getComponent(nameComponent);
 		comp->setSleeping(sleep);
	}

	void CScriptEntity::sendMessageSETHABILIDAD(const std::string &nombreHabilidad,Logic::CEntity* other)
	{
		std::shared_ptr<SET_HABILIDAD> n(new SET_HABILIDAD());
		n->setString(nombreHabilidad);
		n->setInt(0);
		other->emitMessage(n);
	}

	void CScriptEntity::sendMessageMovableText(const std::string &text, Logic::CEntity *movableText, bool _movable)
	{
		_movable 
		?
			movableText->getComponent<CMovableText2 *>()->setText(text)
		:
			movableText->getComponent<CStaticText*>()->setText(text);
	}

	void CScriptEntity::sendMessageSetColorMovableText(const std::string &color, Logic::CEntity *movableText, bool _movable)
	{
		_movable
		? 
			movableText->getComponent<CMovableText2 *>()->setColor(color)
		:
			movableText->getComponent<CStaticText*>()->setColor(color);
	}

	void CScriptEntity::setVisibleText(CEntity *entity, const std::string &texto, const std::string &color, bool visible)
	{
		CStaticText *comp = entity->getComponent<CStaticText*>();
		comp->setText(texto);
		comp->setColor(color);
		comp->setVisible(visible);
	}
	///FUNCIONES DE LOS MENSAJES

	void CScriptEntity::touched(Logic::CEntity* other)
	{
	}

	void CScriptEntity::untouched(Logic::CEntity* other)
	{

	}

	void CScriptEntity::laserToLaserBall()
	{

	}

	void CScriptEntity::openChest()
	{

	}

	void CScriptEntity::cancelOpen()
	{

	}

	void CScriptEntity::use (bool pressed)
	{

	}

	void CScriptEntity::broken ()
	{

	}

	void CScriptEntity::damaged(float damage)
	{

	}
	
} // namespace Logic
