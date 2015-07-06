/**
@file DoorTrigger.cpp

Contiene la declaración del componente que provoca la carga
de un nuevo mapa llamando al MapManager
cuando recibe un mensaje TOUCHED / UNTOUCHED.
 
@see Logic::DoorTrigger
@see Logic::IComponent

@author Alberto Martínez
@date Octubre, 2015
*/

#include "DoorTrigger.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Managers/MapManager.h"

#include "LUA/ScriptManager.h"

#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(DoorTrigger);
	
	//---------------------------------------------------------

	bool DoorTrigger::OnSpawn(const Map::CEntity *entityInfo)
	{
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool DoorTrigger::activate()
	{
		return true;

	} // activate
	
	//---------------------------------------------------------

	void DoorTrigger::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool DoorTrigger::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "TOUCHED" /*||
			message->getType() == "UNTOUCHED"*/;

	} // accept
	
	//---------------------------------------------------------

	void DoorTrigger::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Cargamos el siguiente mapa
		if((message->getType().compare("TOUCHED") == 0))
		{
			if(dynamic_cast<TOUCHED*>(message.get())->getEntidad()->getType().compare("Player") == 0)
			{
				//marcamos que hemos superado la sala
				LUA::CScriptManager::GetPtrSingleton()->executeScript("GameMgr.FinSala()");
				Logic::MapManager::getSingletonPtr()->nextMap();
			}
		}
		
	} // process


} // namespace Logic

