/*
CLase encargada de publicar las clases necesarias con sus metodos en LUA. 

/**
@author Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#ifndef __LUA_ClassToLUA_H
#define __LUA_ClassToLUA_H

struct lua_State;

namespace LUA
{
	class CClassToLUA
	{

	public:

		/*
		Metodo que llama a todas las funciones de publicar
		*/
		bool publishAllClass();

		/*
		Metodo que publica funciones del singleton ScriptManager
		*/
		void publicarScriptManager(lua_State *_lua);

		/*
		Metodo que publica funciones del singleton Log.
		*/
		void publicarLog(lua_State *_lua);

		void publicarRandomUtils(lua_State *_lua);

		/*
		Metodo que publica funciones del Singleton MapParser
		*/
		void publicarMapParser(lua_State *_lua);

		/*
		Metodo que publica funciones del GaleonApplication
		*/
		void publicarGaleonApplication(lua_State *_lua);

		void publicarScriptEntity(lua_State *_lua);

		void publicarVector3(lua_State *_lua);

		void publicarLogicEntity(lua_State *_lua);

		/*
		clase que publica el vector de std
		*/
		void publicarVectorStd(lua_State *_lua);

		/*
		clase que publica la estructura de puntos
		*/
		void publicarInterestingPoint(lua_State *_lua);

		/*
		clase que publica la estructura de puntos de interés para el altar
		*/
		void publicarAltarPoint(lua_State *_lua);

		/*
		clase que publica el servidor lógico
		*/
		void publicarLogicServer(lua_State *_lua);

		void publicarSoundServer(lua_State *_lua);

		void publicarEntityFactory(lua_State *_lua);

		void publicarLogicMap(lua_State *_lua);

		/*
		Publicar clase MapManager
		*/
		void publicarMapManager(lua_State *_lua);

		void publicarConfigurationManager(lua_State *_lua);

		void publicarMusicManager();

		void publicarGUISoundsManager();

		/*
		Constructor
		*/
		CClassToLUA(){}

		/*
		Destructor
		*/
		virtual ~CClassToLUA(){}
		
	};
}
#endif