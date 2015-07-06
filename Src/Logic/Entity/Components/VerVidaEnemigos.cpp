/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "VerVidaEnemigos.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"

#include "LUA/ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CVerVidaEnemigos);
	
	//---------------------------------------------------------

	CVerVidaEnemigos::~CVerVidaEnemigos()
	{

	}

	bool CVerVidaEnemigos::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CVerVidaEnemigos::respawn(const Map::CEntity *entityInfo)
	{
		return true;
	}

	bool CVerVidaEnemigos::activate()
	{

		return true;
	}

	void CVerVidaEnemigos::deactivate()
	{

	}

	bool CVerVidaEnemigos::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Recibimos un mensaje de tipo HABILIDAD
		return message->getType() == "SET_HABILIDAD";
	} // accept
	
	//---------------------------------------------------------

	void CVerVidaEnemigos::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_HABILIDAD") == 0)
		{
			if(!dynamic_cast<SET_HABILIDAD*>(message.get())->getString().compare("BarraVidaEnemigo"))
			{
				//Despertamos el componente, si no estaba despierto antes
				if(_isSleeping)
				{
					_isSleeping = false;
				}

				//Apuntamos en lua la habilidad que se ha recogido
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("AddHability", _name);
			}		
		}
	} // process

	void CVerVidaEnemigos::onTick(unsigned int msecs)
	{

	}

} // namespace Logic

