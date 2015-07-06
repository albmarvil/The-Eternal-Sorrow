/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "DamageMeleeEnemy.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"

#include "LUA/ScriptManager.h"
#include "LifePlayer.h"

namespace Logic 
{
	IMP_FACTORY(CDamageMeleeEnemy);
	
	//---------------------------------------------------------

	CDamageMeleeEnemy::~CDamageMeleeEnemy()
	{

	}

	bool CDamageMeleeEnemy::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CDamageMeleeEnemy::respawn(const Map::CEntity *entityInfo)
	{

		return true;
	}

	bool CDamageMeleeEnemy::activate()
	{
		return true;
	}

	void CDamageMeleeEnemy::deactivate()
	{

	}

	bool CDamageMeleeEnemy::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Recibimos un mensaje de tipo HABILIDAD
		return message->getType() == "SET_HABILIDAD";
	} // accept
	
	//---------------------------------------------------------

	void CDamageMeleeEnemy::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_HABILIDAD") == 0)
		{
			if(!dynamic_cast<SET_HABILIDAD*>(message.get())->getString().compare("DamageEnemigoMelee"))
			{
				//Despertamos el componente, si no estaba despierto antes
				if(_isSleeping)
				{
					_isSleeping = false;
				}

				//LUA::CScriptManager::GetPtrSingleton()->executeScript(("player_skills.damageEnemigoMelee =player_skills.damageEnemigoMelee+"+std::to_string(1)).c_str());

				//Apuntamos en lua la habilidad que se ha recogido
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("AddHability", _name);
			}		
		}
	} // process

	void CDamageMeleeEnemy::onTick(unsigned int msecs)
	{

	}

} // namespace Logic

