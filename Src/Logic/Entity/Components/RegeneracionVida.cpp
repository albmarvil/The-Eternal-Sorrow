/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "RegeneracionVida.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"

#include "LUA/ScriptManager.h"
#include "LifePlayer.h"

namespace Logic 
{
	IMP_FACTORY(CRegeneracionVida);
	
	//---------------------------------------------------------

	CRegeneracionVida::~CRegeneracionVida()
	{

	}

	bool CRegeneracionVida::OnSpawn(const Map::CEntity *entityInfo) 
	{

		//Esto hay que leerlo desde dataPlayer
		//_lifeToAument = LUA::CScriptManager::GetPtrSingleton()->getField("player_skills","vidaRegenerar",0);
		//la vida a regenerar será el contador de la habilidad CRegeneracionVida
		std::string nombre = LUA::CScriptManager::GetPtrSingleton()->getField("Nombre_Habilidad", "CRegeneracionVida", "");
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("HadHability", nombre, _lifeToAument);

		_timeToAument = 0;

		_timeStamp = _totalTimeToCiclo = 2000;

		if(_lifeToAument > 0)
		{
			_isSleeping = false;
		}

		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CRegeneracionVida::respawn(const Map::CEntity *entityInfo)
	{
		//la vida a regenerar será el contador de la habilidad CRegeneracionVida
		std::string nombre = LUA::CScriptManager::GetPtrSingleton()->getField("Nombre_Habilidad", "CRegeneracionVida", "");
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("HadHability", nombre, _lifeToAument);

		_timeToAument = 0;

		_timeStamp = _totalTimeToCiclo = 2000;

		if(_lifeToAument > 0)
		{
			_isSleeping = false;
		}

		return true;
	}

	bool CRegeneracionVida::activate()
	{

		return true;
	}

	void CRegeneracionVida::deactivate()
	{

	}

	bool CRegeneracionVida::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Recibimos un mensaje de tipo HABILIDAD
		return message->getType() == "SET_HABILIDAD";
	} // accept
	
	//---------------------------------------------------------

	void CRegeneracionVida::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_HABILIDAD") == 0)
		{
			if(!dynamic_cast<SET_HABILIDAD*>(message.get())->getString().compare("RegeneracionVida"))
			{
				//Despertamos el componente, si no estaba despierto antes
				if(_isSleeping)
				{
					_isSleeping = false;
				}

				_lifeToAument += 1;
				
				//Apuntamos en lua la habilidad que se ha recogido
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("AddHability", _name);
			}		
		}
	} // process

	void CRegeneracionVida::onTick(unsigned int msecs)
	{
		_timeStamp += msecs;

		_timeToAument = _totalTimeToCiclo / _lifeToAument;

		if(_timeStamp >= _timeToAument)
		{
			_timeStamp = 0;
			
			float life = _entity->getComponent<Logic::CLifePlayer*>()->getLife()+1;
			int lifeMax = 0;
			LUA::CScriptManager::GetPtrSingleton()->executeFunction("Data_Player","getLifeMax",lifeMax);
			if(lifeMax >= life)
			{
				_entity->getComponent<Logic::CLifePlayer*>()->setLife(life);
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "updateLifePj", life);
			}
		}
	}

} // namespace Logic

