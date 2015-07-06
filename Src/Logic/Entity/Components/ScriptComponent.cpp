/*
author Cesar Manuel Paz Guzman
date Marzo 2015
*/

#include "ScriptComponent.h"

#include "Logic/Entity/ScriptEntity.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "LUA/ScriptManager.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <luabind/luabind.hpp>
#include <luabind/adopt_policy.hpp>

#define LUAPATH LUA::CScriptManager::GetPtrSingleton()->getField("Paths","luaScriptsPath", ".")
namespace Logic 
{
	IMP_FACTORY(CScriptComponent);
	
	//---------------------------------------------------------

	CScriptComponent::~CScriptComponent()
	{
		std::vector<CScriptEntity*>::iterator it= _VectorScriptEntity.begin();

		for(; it!=_VectorScriptEntity.end();++it)
		{
			delete (*it);
		}
		_VectorScriptEntity.clear();
	}

	bool CScriptComponent::OnSpawn(const Map::CEntity *entityInfo) 
	{
		
		#pragma region Carga de scripts de lua en el interprete
		std::vector<std::string> vecScripts;
		std::string nombre;
		std::vector<std::string>::const_iterator it;
		//Si tiene el atributo scriptsLUA, meto en el vector, el nombre de cada script.
		if(entityInfo->hasAttribute("scriptsLUA"))
		{
			vecScripts = entityInfo->getVectorSTLAttribute("scriptsLUA");
			it = vecScripts.begin();
			for(; it!= vecScripts.end(); ++it)
			{
				nombre = std::string(LUAPATH) + (*it) + ".lua";
				LUA::CScriptManager::GetPtrSingleton()->loadScript(nombre.c_str());
			}
		}
		else
		{
			//Si no tiene ese atributo, meto simplemente el nombre de la entidad
			vecScripts.push_back(_entity->getType());
			nombre = std::string(LUAPATH)+ _entity->getType() + ".lua";
			LUA::CScriptManager::GetPtrSingleton()->loadScript(nombre.c_str());
		}

		#pragma endregion

		//Creamos cada uno de los scriptEntity, segun el nombre obtenido del arquetipo. 
		it = vecScripts.begin();
		for(; it != vecScripts.end(); ++it)
		{
			if(!createScriptEntity((*it)))
			{
				return false;
			}
		}

		
		std::vector<CScriptEntity*>::const_iterator it2 = _VectorScriptEntity.begin();
		for(;it2 != _VectorScriptEntity.end(); ++it2)
		{
			//Al scriptEntity seteamos el setOwner para que sepa quien es la entidad, de esta forma podremos acceder al entity lógico desde LUA
			(*it2)->setOwner(_entity);

			//scriptEntity es la representacion de entity en LUA, hacemos su spawn. Como es virtual, hará el spawn de la clase del script de LUA
			if(!(*it2)->spawn(entityInfo))
				return false;
		}	

		return true;

	} // spawn


	bool CScriptComponent::respawn(const Map::CEntity *entity)
	{
		std::vector<CScriptEntity*>::const_iterator it = _VectorScriptEntity.begin();
		
		for(; it != _VectorScriptEntity.end(); ++it)
		{
			(*it)->respawn(entity);
		}
		return true;
	}

	bool CScriptComponent::activate()
	{
		std::vector<CScriptEntity*>::const_iterator it = _VectorScriptEntity.begin();
		
		for(; it != _VectorScriptEntity.end(); ++it)
		{
			(*it)->activate();
		}
		return true;
	}

	void CScriptComponent::deactivate()
	{
		std::vector<CScriptEntity*>::const_iterator it = _VectorScriptEntity.begin();
		
		for(; it != _VectorScriptEntity.end(); ++it)
		{
			(*it)->deactivate();
		}
	}

	void CScriptComponent::onTick(unsigned int msecs)
	{
		//Hacemos el tick de cada uno de los scriptEntity asociado a la entidad logica del scriptComponent
		std::vector<CScriptEntity*>::const_iterator it = _VectorScriptEntity.begin();

		for(; it!=_VectorScriptEntity.end(); ++it)
		{
			(*it)->tick(msecs);
		}
	}


	bool CScriptComponent::createScriptEntity(const std::string &name)
	{
		//Obtenemos una referencia del interprete de LUA 
		lua_State *_lua = LUA::CScriptManager::GetPtrSingleton()->getNativeInterpreter();
		CScriptEntity * _scriptEntity;

		try {
			//Llamamos al constructor de la clase en lua. 
			luabind::object ret = luabind::globals(_lua)[name.c_str()]();

			//Con lo de adopt, le indico que lo adopto yo, es decir, que el recolector de basura no lo destruira, hasta que
			//lo destruya yo, y convertimos el objeto devuelto a ScriptEntity. Tipo estatico ScriptEntity, tipo dinamico la clase 
			//que representa al script de LUA.
			_scriptEntity = luabind::object_cast<Logic::CScriptEntity*>(ret,luabind::adopt(luabind::result));

		} catch (luabind::error &ex) {
			std::cerr << "Error ejecutando el procedimiento " << name.c_str() <<std::endl;
			std::cerr << ex.what() << std::endl;
			return false;
		}

		//Metemos en el vector de scriptEntity la clase creada de LUA. 
		_VectorScriptEntity.push_back(_scriptEntity);

		return true;
	}
	bool CScriptComponent::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "TOUCHED" || 
				message->getType() == "UNTOUCHED" ||
				message->getType() == "OPEN_CHEST" ||
				message->getType() == "LASER_TO_LASERBALL" ||
				message->getType() == "USE"||
				message->getType() == "SET_ARMTOWEAPONHUD"|| 
				message->getType() == "SPECIAL_ATTACK_LAUNCHED" ||
				message->getType() == "NOTIFY_ARM" ||
				message->getType() == "BROKEN" ||
				message->getType() == "CANCEL_OPEN" ||
				message->getType() == "DAMAGED";
	}

	void CScriptComponent::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		std::vector<CScriptEntity *>::const_iterator it;

		if(message->getType().compare("TOUCHED") == 0)
		{
			CEntity *other = dynamic_cast<TOUCHED*>(message.get())->getEntidad();
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->touched(other);
			}
		}
		else if(message->getType().compare("LASER_TO_LASERBALL") == 0)
		{
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->laserToLaserBall();
			}
		}
		else if(message->getType().compare("SET_ARMTOWEAPONHUD") == 0)
		{
			std::string _arm = dynamic_cast<SET_ARMTOWEAPONHUD*>(message.get())->getArm();
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->arm(_arm);
			}
		}
		else if(message->getType().compare("NOTIFY_ARM") == 0)
		{
			std::string _arm = dynamic_cast<NOTIFY_ARM*>(message.get())->getArm();
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->setArm(_arm);
			}
		}
		else if(message->getType().compare("SPECIAL_ATTACK_LAUNCHED") == 0)
		{
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->specialAtkLaunched();
			}
		}
		else if(message->getType().compare("UNTOUCHED") == 0)
		{
			CEntity *other = dynamic_cast<UNTOUCHED*>(message.get())->getEntidad();
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->untouched(other);
			}
		}
		else if(message->getType().compare("OPEN_CHEST") == 0)
		{
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->openChest();
			}
		}
		else if(message->getType().compare("CANCEL_OPEN") == 0)
		{
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->cancelOpen();
			}
		}
		else if(message->getType().compare("USE") == 0)
		{
			USE* m = dynamic_cast<USE*>(message.get());
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->use(m->getBool());
			}
		}
		else if(message->getType().compare("BROKEN") == 0)
		{
			BROKEN* m = dynamic_cast<BROKEN*>(message.get());
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->broken();
			}
		}
		else if(message->getType().compare("DAMAGED") == 0 )
		{
			float damage = dynamic_cast<DAMAGED*>(message.get())->getFloat();
			for(it = _VectorScriptEntity.begin(); it != _VectorScriptEntity.end(); ++it)
			{
				(*it)->damaged(damage);
			}
		}
	}
} 

