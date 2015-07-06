/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "AumentarTiempoReto.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"

#include "LUA/ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CAumentarTiempoReto);
	
	//---------------------------------------------------------

	CAumentarTiempoReto::~CAumentarTiempoReto()
	{

	}

	bool CAumentarTiempoReto::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CAumentarTiempoReto::respawn(const Map::CEntity *entityInfo)
	{

		return true;
	}

	bool CAumentarTiempoReto::activate()
	{
		return true;
	}

	void CAumentarTiempoReto::deactivate()
	{

	}

	bool CAumentarTiempoReto::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Recibimos un mensaje de tipo HABILIDAD
		return message->getType() == "SET_HABILIDAD";
	} // accept
	
	//---------------------------------------------------------

	void CAumentarTiempoReto::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_HABILIDAD") == 0)
		{
			if(!dynamic_cast<SET_HABILIDAD*>(message.get())->getString().compare("AumentarTiempoReto"))
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

	void CAumentarTiempoReto::onTick(unsigned int msecs)
	{

	}

} // namespace Logic

