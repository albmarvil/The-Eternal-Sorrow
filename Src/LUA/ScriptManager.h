/*
INTERPRETE DE LUA. Contiene las funciones que se comunicaran de C++ a LUA y viceversa. 

/**
@author Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#ifndef __ScriptManager_ScriptManager_H
#define __ScriptManager_ScriptManager_H

#include <string>
#include <map>
#include <cassert>
#include "BaseSubsystems/Log.h"
#include "BaseSubsystems/Math.h"

// Estructura con el contexto (estado) del intérprete de Lua.
struct lua_State;

typedef int (*lua_CFunction) (lua_State *L);

namespace LUA {

class CScriptManager {
public:

	/**
	Inicializa la instancia
	@return Devuelve false si no se ha podido inicializar.
	*/
	static bool Init();

	/**
	Libera la instancia de CServer. Debe llamarse al finalizar la aplicación.
	*/
	static void Release();

	/**
	Devuelve la única instancia de la clase CServer.
	@return Única instancia de la clase CServer.
	*/

	static CScriptManager &GetSingleton(){ return *_instance;}

	/**
	Devuelve la única instancia de la clase CServer.
	@return Única instancia de la clase CServer.
	*/
	static CScriptManager *GetPtrSingleton(){ return _instance;}

	/*
	Metodo que me carga un script.lua para su posterior uso. Es necesario llamar a este método cada vez que creamos un
	nuevo fichero.lua. Esta función es llamada desde el fichero CargadorFiles.lua. 
	*/
	bool loadScript(const char *script);

	/*
	Metodo que se encarga de ejecutar un script de c++ en LUA. 
	*/
	bool executeScript(const char *script);

	//---------------------------------------------------------

	/*
	Metodos que extraen el valor de una tabla de LUA, apartir de su clave
	*/
	int getField(const char *table,const char *field, int defaultValue);
	bool getField(const char *table,const char *field, bool defaultValue);
	std::string getField(const char *table,const char *field, const char * defaultValue);

	//---------------------------------------------------------

	/*
	Metodos que devuelven el valor de una variable global de LUA
	*/
	int getGlobal(const char *name, int defaultValue);
	bool getGlobal(const char *name, bool defaultValue);
	std::string getGlobal(const char *name, const char * defaultValue);

	//---------------------------------------------------------

	/*
	Metodos que setean las variables globales de LUA desde C++
	*/
	bool setGlobal(const char *name, const int &value);
	bool setGlobal(const char *name, const bool &value);
	bool setGlobal(const char *name, const std::string &value);

	//---------------------------------------------------------
	
	/*
	Metodo que me ejecuta una funcion de LUA, con una llamada desde C++
	*/
	bool executeProcedure(const char *subroutineName);
	bool executeProcedure(const char *subroutineName, int &param1);
	bool executeProcedure(const char *subroutineName, int &param1, int &param2);
	bool executeProcedure(const char *subroutineName, std::string &param1, int &param2);
	bool executeProcedure(const char *subroutineName, bool &param1);
	bool executeProcedure(const char *subroutineName, float &param1);
	bool executeProcedure(const char *subroutineName, Vector3 &param1);
	bool executeProcedure(const char *subroutineName, const std::string &param1);
	
	bool executeProcedure(const char *subroutineName, const std::string &param1, const std::string &param2);
	bool executeProcedure(const char *subroutineName, const std::string &param1, const std::string &param2, const std::string &param3, const float &param4);
	bool executeProcedure(const char *subroutineName, const Vector3 &position);
	
	bool executeProcedure(const char *tabla, const char *subroutineName);
	bool executeProcedure(const char *tabla, const char *subroutineName, const std::string &_string);
	bool executeProcedure(const char *tabla, const char *subroutineName, const std::string &param1, const std::string &param2, const int &param3, const int &param4, const Vector3 &param5);
	bool executeProcedure(const char *tabla, const char *subroutineName, const Vector3& _vector3, int param1, int param2);
	bool executeProcedure(const char *tabla, const char *subroutineName, const Vector3& _vector3);
	bool executeProcedure(const char *tabla, const char *subroutineName, const std::string &param1, int &param2, Vector3 &param3);
	bool executeProcedure(const char *tabla, const char *subroutineName, int &param1, int &param2);
	bool executeProcedure(const char *tabla, const char *subroutineName, int &param1, int &param2, int &param3, Vector3 &param4);
	bool executeProcedure(const char *tabla, const char *subroutineName, int &param1);
	bool executeProcedure(const char *tabla, const char *subroutineName, float &param1);
	const char* executeFunction(const char *tabla, const char *subroutineName);
	//---------------------------------------------------------

	/*
	Metodo que me ejecuta una funcion de LUA, con una llamada desde C++. A diferencia del anterior, este me devuelve un resultado. 
	*/
	bool executeFunction(const char *tabla, const char *subroutineName, int&result);
	bool executeFunction(const char *tabla, const char *subroutineName, std::string &result, int valor);
	bool executeFunction(const char *tabla, const char *subroutineName, Vector3 &param1, int &result);
	bool executeFunction(const char *subroutineName, int param1, int &result);
	bool executeFunction(const char *subroutineName, float param1, float &result);
	bool executeFunction(const char *subroutineName, float &result);
	bool executeFunction(const char *subroutineName, const std::string &param1, int &result);
	bool executeFunction(const char *subroutineName, const std::string &param1, unsigned int &result);
	bool executeFunction(const char *subroutineName, const std::string &param1, float &result);
	bool executeFunction(const char *subroutineName, const std::string &param1, std::string &result);
	bool executeFunction(const char *subroutineName, float param1, float param2, bool &result) ;
	bool executeFunction(const char *subroutineName, const std::string &param1, const std::string &param2, const std::string &param3, float &result);
	bool executeFunction(const char *subroutineName, const std::string &param1, const std::string &param2, int &result);
	bool executeFunction(const char *subroutineName, const std::string &param1, const std::string &param2, std::string &result);
	
	/*
	Esta función llama a una función en LUA que devuelve una TABLA. Aquí procesamos dicha tabla y devolvemos múltiples valores en result y result2
	*/
	bool executeFunction(const char *subroutineName, const std::string &param1, const std::string &param2, int &param3, float &result, float &result2);
	//---------------------------------------------------------

	/*
	Metodo que devuelve el interprete de LUA
	*/
	lua_State *getNativeInterpreter() { return _lua; }

	
protected:

	/*
	Constructor
	*/
	CScriptManager();

	/*
	Destructor
	*/
	~CScriptManager();

	/**
	Segunda fase de la construcción del objeto. Sirve para hacer
	inicializaciones de la propia instancia en vez de inicializaciones 
	estáticas.

	@return true si todo fue correctamente.
	*/
	bool open();

	/**
	Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
	los recursos de la propia instancia, la liberación de los recursos 
	estáticos se hace en Release().
	*/
	void close();


	static int luaError(lua_State *s);

	lua_State *_lua;

private:
	/*
	Instancia de la clase
	*/
	static CScriptManager *_instance;
};

}

#endif