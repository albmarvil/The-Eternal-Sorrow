/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "ReduceTimeChest.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"

#include "LUA/ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CReduceTimeChest);
	
	//---------------------------------------------------------

	CReduceTimeChest::~CReduceTimeChest()
	{

	}

	bool CReduceTimeChest::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CReduceTimeChest::respawn(const Map::CEntity *entityInfo)
	{

		return true;
	}

	bool CReduceTimeChest::activate()
	{
		return true;
	}

	void CReduceTimeChest::deactivate()
	{

	}

	bool CReduceTimeChest::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Recibimos un mensaje de tipo HABILIDAD
		return message->getType() == "SET_HABILIDAD";
	} // accept
	
	//---------------------------------------------------------

	void CReduceTimeChest::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_HABILIDAD") == 0)
		{
			if(!dynamic_cast<SET_HABILIDAD*>(message.get())->getString().compare("ReduceTimeChest"))
			{
				//Despertamos el componente, si no estaba despierto antes
				if(_isSleeping)
				{
					_isSleeping = false;
				}

				//LUA::CScriptManager::GetPtrSingleton()->executeScript(("player_skills.porcentajeSoulToAument =player_skills.porcentajeSoulToAument+"+std::to_string(1)).c_str());

				//Apuntamos en lua la habilidad que se ha recogido
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("AddHability", _name);
			}		
		}
	} // process

	void CReduceTimeChest::onTick(unsigned int msecs)
	{

	}

} // namespace Logic

