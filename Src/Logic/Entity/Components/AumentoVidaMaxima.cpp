/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/


#include "AumentoVidaMaxima.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"

#include "LUA/ScriptManager.h"
#include "LifePlayer.h"
#include "Logic/Maps/EntityFactory.h"
#include "MovableText.h"

namespace Logic 
{
	IMP_FACTORY(CAumentoVidaMaxima);
	
	//---------------------------------------------------------

	CAumentoVidaMaxima::~CAumentoVidaMaxima()
	{

	}

	bool CAumentoVidaMaxima::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CAumentoVidaMaxima::respawn(const Map::CEntity *entityInfo)
	{

		return true;
	}

	bool CAumentoVidaMaxima::activate()
	{
		//Seteamos la estadistica de la vida máxima con la vida máxima correspondiente al número de habilidades cogidas.
		//Por cada habilidad obtenida suma 1 a la vida máxima al salir de la sala
		int lifeToAument;
		std::string nombre = LUA::CScriptManager::GetPtrSingleton()->getField("Nombre_Habilidad", "CAumentoVidaMaxima", "");
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("HadHability", nombre, lifeToAument);
		
		LUA::CScriptManager::GetPtrSingleton()->executeScript(("Data_Player.player_stats.vida = Data_Player.player_stats.vida+"+std::to_string(lifeToAument)).c_str());

		if(lifeToAument >0)
		{
			CEntity *movableText = Logic::CEntityFactory::getSingletonPtr()->createEntityByType("MovableText", _entity->getCenterPosition() + 
				Vector3(0,-5,40), _entity->getMap());

			movableText->getComponent<CMovableText2 *>()->setText("+"+std::to_string(lifeToAument)+"V");
			movableText->getComponent<CMovableText2 *>()->setColor("green");
		}

		LUA::CScriptManager::GetPtrSingleton()->executeScript("LifeHUD.setLife(0)");
		return true;
	}

	void CAumentoVidaMaxima::deactivate()
	{

	}

	bool CAumentoVidaMaxima::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Recibimos un mensaje de tipo HABILIDAD
		return message->getType() == "SET_HABILIDAD";
	} // accept
	
	//---------------------------------------------------------

	void CAumentoVidaMaxima::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_HABILIDAD") == 0)
		{
			if(!dynamic_cast<SET_HABILIDAD*>(message.get())->getString().compare("AumentoVidaMaxima"))
			{
				//Despertamos el componente, si no estaba despierto antes
				if(_isSleeping)
				{
					_isSleeping = false;
				}

				//LUA::CScriptManager::GetPtrSingleton()->executeScript(("player_skills.lifeMaxToAument =player_skills.lifeMaxToAument+"+std::to_string(1)).c_str());

				//Apuntamos en lua la habilidad que se ha recogido
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("AddHability", _name);
			}		
		}
	} // process

	void CAumentoVidaMaxima::onTick(unsigned int msecs)
	{

	}

} // namespace Logic

