/**
@author Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#include "ClassToLUA.h"
#include "ScriptManager.h"
#include "Map\MapEntity.h"
#include "Application\BaseApplication.h"
#include "Logic\Entity\ScriptEntity.h"
#include "Logic\Entity\Entity.h"
#include "BaseSubsystems\Math.h"
#include "BaseSubsystems\RandomUtils.h"
#include "Logic\Server.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Maps\Map.h"
#include "Map\ParserTile.h"
#include "Sounds\Server.h"
#include "Sounds/MusicManager.h"
#include "Sounds/GUISoundsManager.h"
#include "Logic\Maps\Map.h"
#include "Logic\Managers\MapManager.h"
#include "BaseSubsystems\ConfigurationManager.h"


extern "C" {
#include "lua.h"
#include <lauxlib.h>
#include <lualib.h>
}
 
#include <luabind/luabind.hpp>

using namespace luabind;

namespace LUA
{

	bool CClassToLUA::publishAllClass()
	{

		lua_State *_lua = LUA::CScriptManager::GetPtrSingleton()->getNativeInterpreter();

		publicarScriptManager(_lua);

		publicarLog(_lua);

		publicarRandomUtils(_lua);

		publicarMapParser(_lua);

		publicarGaleonApplication(_lua);

		publicarLogicEntity(_lua);
		
		publicarVector3(_lua);

		publicarScriptEntity(_lua);

		publicarLogicServer(_lua);

		publicarInterestingPoint(_lua);

		publicarAltarPoint(_lua);

		publicarVectorStd(_lua);

		publicarSoundServer(_lua);

		publicarEntityFactory(_lua);

		publicarLogicMap(_lua);

		publicarMapManager(_lua);

		publicarConfigurationManager(_lua);


		return true;
	}

	void CClassToLUA::publicarScriptManager(lua_State *_lua)
	{
		module(_lua)
			[
				class_<LUA::CScriptManager>("__ScriptManager")
					.def("loadScript",&LUA::CScriptManager::loadScript)
					.def("executeProcedureFromTable", (bool (LUA::CScriptManager::*)(const char*, const char*)) &LUA::CScriptManager::executeProcedure)
					.def("executeFunction1", (const char* (LUA::CScriptManager::*)(const char*, const char*)) &LUA::CScriptManager::executeFunction)
					.def("executeScript", &LUA::CScriptManager::executeScript)
			];
		luabind::globals(_lua)["scriptMgr"] = LUA::CScriptManager::GetPtrSingleton();
	}

	void CClassToLUA::publicarLog(lua_State *_lua)
	{
		module(_lua)
			[
				class_<BaseSubsystems::Log>("Log").scope[luabind::def("Debug",&BaseSubsystems::Log::Debug)
				,luabind::def("Warning",&BaseSubsystems::Log::Warning)
				,luabind::def("Error",&BaseSubsystems::Log::Error)]
			];
	}

	void CClassToLUA::publicarRandomUtils(lua_State *_lua)
	{
		module(_lua)
			[
				class_<BaseSubsystems::RandomUtils>("RandomUtils").scope[
				luabind::def("setSeed",&BaseSubsystems::RandomUtils::setRandomSeed)
				,luabind::def("Random",&BaseSubsystems::RandomUtils::Random)
				,luabind::def("RandomRange",&BaseSubsystems::RandomUtils::RandomRange)]
			];
	}

	void CClassToLUA::publicarMapParser(lua_State *_lua)
	{
		luabind::module(_lua)
			[
				luabind::class_<Map::CEntity>("MapEntity")
					.def(luabind::constructor<std::string>())
					.def("SetType", &Map::CEntity::setType)
					.def("SetAttrib", &Map::CEntity::setAttribute)
					.def("hasAttribute", &Map::CEntity::hasAttribute)
					.def("getStringAttribute", &Map::CEntity::getStringAttribute)
					.def("getIntAttribute", &Map::CEntity::getIntAttribute)
					.def("getFloatAttribute", &Map::CEntity::getFloatAttribute)
					.def("getBoolAttribute", &Map::CEntity::getFloatAttribute)
					.def("getVector3Attribute", &Map::CEntity::getVector3Attribute)
					.def("getVectorSTLAttribute", &Map::CEntity::getVectorSTLAttribute)
					.def("getType", &Map::CEntity::getType)
					.def("getName", &Map::CEntity::getName)
			];
	}

	void CClassToLUA::publicarGaleonApplication(lua_State *_lua)
	{
		luabind::module(_lua) [
			luabind::class_<Application::CBaseApplication>("__APPLICATION")
					.def("ChangeToState", &Application::CBaseApplication::setState)
					.def("GetTime", &Application::CBaseApplication::getAppTime)
					.def("exitRequest", &Application::CBaseApplication::exitRequest)
					.def("push", &Application::CBaseApplication::push)
					.def("pop", &Application::CBaseApplication::pop)
					.def("clearStates", &Application::CBaseApplication::clearStates)
		];

		//Una buena forma de definir la clase, sin usar el singleton desde LUA es esta. Y le indicamos que el nombre
		//de la clase de C++ es un nombre privado para LUA, por lo que la unica forma de acceder a la intancia es mediante
		//app:getTime por ejemplo. 
		luabind::globals(_lua)["app"] = Application::CBaseApplication::getSingletonPtr();
	}

	void CClassToLUA::publicarScriptEntity(lua_State *_lua)
	{
		//Dadp que la clase scriptEntity es padre de todas las clases que estan en los scripts de LUA referentes
		//a la entidad, es necesario implementar un wrapper, que sirve para que en el script me ejecute el metodo sobrecargado
		//en el hijo, y no el del padre. 

		struct CScriptEntity_wrapper: Logic::CScriptEntity, luabind::wrap_base
		{
			//Constructor
			CScriptEntity_wrapper(): Logic::CScriptEntity(){}

			//METODOS VIRTUALES, con su declaracion y el default_spawn, que es el llamado cuando se quiere llamar al del padre de forma explicita
			virtual bool spawn(const Map::CEntity *entityInfo)
			{
				return call<bool>("spawn",entityInfo);
			}

			static bool default_spawn(Logic::CScriptEntity* ptr, const Map::CEntity *entityInfo)
			{
				return ptr->Logic::CScriptEntity::spawn(entityInfo);
			}

			virtual void tick(unsigned int msecs)
			{
				call<void>("tick", msecs);
			}

			static void default_tick(Logic::CScriptEntity* ptr, unsigned int msecs)
			{
				return ptr->Logic::CScriptEntity::tick(msecs);
			}

			virtual bool activate()
			{
				return call<bool>("activate");
			}

			static bool default_activate(Logic::CScriptEntity* ptr)
			{
				return ptr->Logic::CScriptEntity::activate();
			}

			virtual void deactivate()
			{
				call<void>("deactivate");
			}

			static void default_deactivate(Logic::CScriptEntity* ptr)
			{
				return ptr->Logic::CScriptEntity::deactivate();
			}

			virtual bool respawn(const Map::CEntity *entityInfo)
			{
				return call<bool>("respawn", entityInfo);
			}

			static bool default_respawn(Logic::CScriptEntity* ptr, const Map::CEntity *entityInfo)
			{
				return ptr->Logic::CScriptEntity::respawn(entityInfo);
			}

			virtual void laserToLaserBall()
			{
				call<void>("laserToLaserBall");
			}

			static void default_laserToLaserBall(Logic::CScriptEntity* ptr)
			{
				ptr->Logic::CScriptEntity::laserToLaserBall();
			}

			virtual void touched(Logic::CEntity*other)
			{
				call<void>("touched", other);
			}

			static void default_touched(Logic::CScriptEntity* ptr, Logic::CEntity* other)
			{
				ptr->Logic::CScriptEntity::touched(other);
			}

			virtual void damaged(float damage)
			{
				call<void>("damaged", damage);
			}

			static void default_damaged(Logic::CScriptEntity* ptr, float damage)
			{
				ptr->Logic::CScriptEntity::damaged(damage);
			}

			virtual void arm(const std::string &brazo)
			{
				call<void>("arm", brazo);
			}

			static void default_arm(Logic::CScriptEntity* ptr, const std::string &brazo)
			{
				return ptr->Logic::CScriptEntity::arm(brazo);
			}

			virtual void specialAtkLaunched()
			{
				call<void>("specialAtkLaunched");
			}

			static void default_specialAtkLaunched(Logic::CScriptEntity* ptr)
			{
				return ptr->Logic::CScriptEntity::specialAtkLaunched();
			}

			virtual void untouched(Logic::CEntity*other)
			{
				call<void>("untouched", other);
			}

			static void default_untouched(Logic::CScriptEntity* ptr, Logic::CEntity* other)
			{
				ptr->Logic::CScriptEntity::untouched(other);
			}

			virtual void use(bool pressed)
			{
				call<void>("use", pressed);
			}

			static void default_use(Logic::CScriptEntity* ptr, bool pressed)
			{
				ptr->Logic::CScriptEntity::use(pressed);
			}

			virtual void broken()
			{
				call<void>("broken");
			}

			static void default_broken(Logic::CScriptEntity* ptr)
			{
				return ptr->Logic::CScriptEntity::broken();
			}

			virtual void openChest()
			{
				call<void>("openChest");
			}

			static void default_openChest(Logic::CScriptEntity* ptr)
			{
				ptr->Logic::CScriptEntity::openChest();
			}

			virtual void cancelOpen()
			{
				call<void>("cancelOpen");
			}

			static void default_cancelOpen(Logic::CScriptEntity* ptr)
			{
				ptr->Logic::CScriptEntity::cancelOpen();
			}

			virtual void setArm(const std::string &brazo)
			{
				call<void>("setArm", brazo);
			}

			static void default_setArm(Logic::CScriptEntity* ptr, const std::string &brazo )
			{
				ptr->Logic::CScriptEntity::setArm(brazo);
			}
		};

		//Ahora ademas, le indicamos el wrapper a publicar
		module(_lua) [
			luabind::class_<Logic::CScriptEntity, CScriptEntity_wrapper>("CScriptEntity")
				.def(luabind::constructor<>())
				.def("spawn",&Logic::CScriptEntity::spawn, &CScriptEntity_wrapper::default_spawn)
				.def("tick", &Logic::CScriptEntity::tick, &CScriptEntity_wrapper::default_tick)
				.def("activate", &Logic::CScriptEntity::activate, &CScriptEntity_wrapper::default_activate)
				.def("deactivate", &Logic::CScriptEntity::deactivate, &CScriptEntity_wrapper::default_deactivate)
				.def("respawn", &Logic::CScriptEntity::respawn, &CScriptEntity_wrapper::default_respawn)
				.def("getOwner", &Logic::CScriptEntity::getOwner)
				.def("setSleep", &Logic::CScriptEntity::setSleep)
				.def("sendMessageSETHABILIDAD", &Logic::CScriptEntity::sendMessageSETHABILIDAD)
				.def("arm",&Logic::CScriptEntity::arm, &CScriptEntity_wrapper::default_arm)
				.def("specialAtkLaunched", &Logic::CScriptEntity::specialAtkLaunched, &CScriptEntity_wrapper::default_specialAtkLaunched)
				.def("touched", &Logic::CScriptEntity::touched, &CScriptEntity_wrapper::default_touched)
				.def("laserToLaserBall", &Logic::CScriptEntity::laserToLaserBall, &CScriptEntity_wrapper::default_laserToLaserBall)
				.def("untouched", &Logic::CScriptEntity::untouched, &CScriptEntity_wrapper::default_untouched)
				.def("use", &Logic::CScriptEntity::use, &CScriptEntity_wrapper::default_use)
				.def("broken", &Logic::CScriptEntity::broken, &CScriptEntity_wrapper::default_broken)
				.def("openChest", &Logic::CScriptEntity::openChest, &CScriptEntity_wrapper::default_openChest)
				.def("setArm", &Logic::CScriptEntity::setArm, &CScriptEntity_wrapper::default_setArm)
				.def("sendMessageMovableText", &Logic::CScriptEntity::sendMessageMovableText)
				.def("sendMessageSetColorMovableText", &Logic::CScriptEntity::sendMessageSetColorMovableText)
				.def("setVisibleText", &Logic::CScriptEntity::setVisibleText)
				.def("cancelOpen",&Logic::CScriptEntity::cancelOpen, &CScriptEntity_wrapper::default_cancelOpen)
				.def("damaged", &Logic::CScriptEntity::damaged, &CScriptEntity_wrapper::default_damaged)
		];
	}

	void CClassToLUA::publicarVector3(lua_State *_lua)
	{
		 luabind::module(_lua)
		[
		luabind::class_<Ogre::Vector3>("Vector3")
			.def(luabind::constructor<float,float,float>())
			.def_readwrite("x",&Ogre::Vector3::x)
			.def_readwrite("y",&Ogre::Vector3::y)
			.def_readwrite("z",&Ogre::Vector3::z)
		 ];
	}

	void CClassToLUA::publicarLogicEntity(lua_State *_lua)
	{
		luabind::module(_lua)
		[
			class_<Logic::CEntity>("LogicEntity")
			.def("getPosition", &Logic::CEntity::getPosition)
			.def("getID", &Logic::CEntity::getEntityID)
			.def("getName", &Logic::CEntity::getName)
			.def("getEntityInfo", &Logic::CEntity::getEntityInfo)
			.def("getType",&Logic::CEntity::getType)
			.def("getTag", &Logic::CEntity::getTag)
			.def("setPosition", &Logic::CEntity::setPosition)
		];

		//publicamos el EntityID de rebote
		luabind::module(_lua)
		[
			class_<Logic::EntityID>("EntityID")
		];

		
	}

	void CClassToLUA::publicarLogicServer(lua_State *_lua)
	{
		module(_lua)
		[
			class_<Logic::CServer>("LogicServer")
			.def("getMap", &Logic::CServer::getMap)
			.def("getPlayer", &Logic::CServer::getPlayer)
		];
		luabind::globals(_lua)["logicserver"] = Logic::CServer::getSingletonPtr();
	}

	void CClassToLUA::publicarInterestingPoint(lua_State *_lua)
	{
		module(_lua)
		[
			class_<Map::InterestingPointInfo>("PointInfo")
			.def("pos", &Map::InterestingPointInfo::GetPos)
			.def("weight", &Map::InterestingPointInfo::GetWeight)
		];
	}

	void CClassToLUA::publicarAltarPoint(lua_State *_lua)
	{
		module(_lua)
		[
			class_<Map::AltarPointInfo>("AltarPoint")
			.def("pos", &Map::AltarPointInfo::GetPos)
			.def("weight", &Map::AltarPointInfo::GetWeight)
			.def("radio", &Map::AltarPointInfo::GetRadio)
		];
	}

	void CClassToLUA::publicarVectorStd(lua_State *_lua)
	{
		module(_lua)
		[
			class_<std::vector<Map::InterestingPointInfo>>("vector")
			.def("size", &std::vector<Map::InterestingPointInfo>::size)
			//publicación del método sobrecargado at.
			//.def("nombre metodo", ((valor que devuelve el método)(valor que recibe por parámetro) &función a publicar)
			//en este caso será
			//"nombre metodo" = "at"
			//(valor que devuelve el método) = std::vector<Map::InterestingPointInfo>::reference (std::vector<Map::InterestingPointInfo>::*)
			//(valor que recibe por parámetro = (std::vector<Map::InterestingPointInfo>::size_type)
			//función a publicar = &std::vector<Map::InterestingPointInfo>::at
			.def("at", (std::vector<Map::InterestingPointInfo>::reference (std::vector<Map::InterestingPointInfo>::*)(std::vector<Map::InterestingPointInfo>::size_type))
			&std::vector<Map::InterestingPointInfo>::at)
		];

		module(_lua)
		[
			class_<std::vector<Map::AltarPointInfo>>("vectorAltar")
			.def("size", &std::vector<Map::AltarPointInfo>::size)
			.def("at", (std::vector<Map::AltarPointInfo>::reference (std::vector<Map::AltarPointInfo>::*)(std::vector<Map::AltarPointInfo>::size_type))
			&std::vector<Map::AltarPointInfo>::at)
		];

		module(_lua)
		[
			class_<std::vector<std::string>>("vectorString")
			.def("size", &std::vector<std::string>::size)
			.def("at", (std::vector<std::string>::reference (std::vector<std::string>::*)(std::vector<std::string>::size_type))
			&std::vector<std::string>::at)
		];
	}

	void CClassToLUA::publicarSoundServer(lua_State *_lua)
	{
		module(_lua)
		[
			class_<Sounds::CServer>("__SOUNDSERVER")
			.def("AddSoundBank", &Sounds::CServer::AddBank)
			.def("InitManagers", &Sounds::CServer::InitManagers)
		];
		//publicamos la instancia del singleton
		luabind::globals(_lua)["soundServer"] = Sounds::CServer::getSingletonPtr();
	}

	void CClassToLUA::publicarMapManager(lua_State *_lua)
	{
		/*module(_lua)
		[
			class_<Logic::MapManager>("__MAPMANAGER")
			.def("nextMap", &Logic::MapManager::nextMap)
		];

		//publicamos la instancia del singleton
		luabind::globals(_lua)["logicMapMgr"] = Logic::MapManager::getSingletonPtr();
		*/
		module(_lua)
		[
			class_<Logic::MapManager>("LogicMapManager").scope
			[
				luabind::def("getInstance", &Logic::MapManager::getSingletonPtr)
			]
			.def("nextMap", &Logic::MapManager::nextMap)
			.def("changeAmbientacion", &Logic::MapManager::nextAmbientation)
		];
	}


	void CClassToLUA::publicarEntityFactory(lua_State *_lua)
	{
		
		module (_lua)
		[
			class_<Logic::CEntityFactory>("__ENTITYFACTORY")
			.def("createEntityByType", &Logic::CEntityFactory::createEntityByType)
			.def("deferredDeleteEntity", &Logic::CEntityFactory::deferredDeleteEntity)
			.def("createEntity", &Logic::CEntityFactory::createEntity)
		];

		luabind::globals(_lua)["entityfactory"] = Logic::CEntityFactory::getSingletonPtr();
	}

	void CClassToLUA::publicarLogicMap(lua_State *_lua)
	{
		module(_lua)
		[
			class_<Logic::CMap>("LogicMap")
			.def("getTileColisionTP", &Logic::CMap::getTileColisionTP)
			.def("getMapTileWidth", &Logic::CMap::getMapTileWidth)
			.def("getMapTileHeight", &Logic::CMap::getMapTileHeight)
			.def("activate", &Logic::CMap::activate)
			.def("deactivate", &Logic::CMap::deactivate)
		];
	}

	void CClassToLUA::publicarConfigurationManager(lua_State *_lua)
	{
		module(_lua)
		[
			class_<BaseSubsystems::CConfigurationManager>("configurationManager")
			.def("saveConfig", &BaseSubsystems::CConfigurationManager::saveConfig)
			.def("updateResolution", &BaseSubsystems::CConfigurationManager::updateResolution)
			.def("changeFullMode", &BaseSubsystems::CConfigurationManager::changeFullMode)
			.def("vsyncActive", &BaseSubsystems::CConfigurationManager::vsyncActive)
			.def("updateVolumeMusic", &BaseSubsystems::CConfigurationManager::updateVolumeMusic)
			.def("updateVolumeEffects", &BaseSubsystems::CConfigurationManager::updateVolumeEffects)
			.def("updateFSAA", &BaseSubsystems::CConfigurationManager::updateFsaa)
			.def("setValueConfiguration",&BaseSubsystems::CConfigurationManager::setValueConfiguration)
		];
		luabind::globals(_lua)["configurationManager"] = BaseSubsystems::CConfigurationManager::getSingletonPtr();
	}

	void CClassToLUA::publicarMusicManager()
	{
		lua_State *lua = LUA::CScriptManager::GetPtrSingleton()->getNativeInterpreter();

		module(lua)
			[
				class_<Sounds::CMusicManager>("__MUSICMANAGER")
				.def("playSound", &Sounds::CMusicManager::playSound)
				.def("stopSound", &Sounds::CMusicManager::stopSound)
				.def("stopAllSounds", &Sounds::CMusicManager::stopAllSounds)
				.def("setSoundParameter", &Sounds::CMusicManager::setSoundParameter)
			];
		luabind::globals(lua)["musicManager"] = Sounds::CMusicManager::getSingletonPtr();
	}

	void CClassToLUA::publicarGUISoundsManager()
	{
		lua_State *lua = LUA::CScriptManager::GetPtrSingleton()->getNativeInterpreter();

		module(lua)
			[
				class_<Sounds::CGUISoundsManager>("__GUISOUNDS")
				.def("playSound", &Sounds::CGUISoundsManager::playSound)
				.def("stopSound", &Sounds::CGUISoundsManager::stopSound)
				.def("stopAllSounds", &Sounds::CGUISoundsManager::stopAllSounds)
				.def("setSoundParameter", &Sounds::CGUISoundsManager::setSoundParameter)
			];
		luabind::globals(lua)["guiSounds"] = Sounds::CGUISoundsManager::getSingletonPtr();
	}

}