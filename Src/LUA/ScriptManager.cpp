/**
@author Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#include "ScriptManager.h"

#include <cassert>
#include <iostream>
#include <sstream>
// Incluímos las cabedceras de Lua.
// Como es código C (no C++), hay que indicarselo al
// compilador para que asuma el convenio de nombres
// de C en el código objeto.
extern "C" {
	#include <lua.h>
	//Contiene la funcion para cargar los script: loadfile
	#include <lauxlib.h>
	//Funciones relacionadas con la libreria de lua (libreria del lenguaje de programacion LUA).
	#include <lualib.h>
}

#include <luabind/luabind.hpp>
#include "luabind/class_info.hpp"		// Para RTTI desde LUA

using namespace luabind;

namespace LUA {

// Única instancia de la clase.
CScriptManager *CScriptManager::_instance = 0;

CScriptManager::CScriptManager() : _lua(NULL) {

	_instance = this;

} // Constructor

//---------------------------------------------------------

CScriptManager::~CScriptManager() {

	_instance = NULL;

} // Destructor

//---------------------------------------------------------

bool CScriptManager::Init() {

	assert(_instance == NULL);

	new CScriptManager();
	
	if (!_instance->open()) {
		// ¡¡Vaya!!
		Release();
		return false;
	}

	return true;

} // Init

//---------------------------------------------------------

void CScriptManager::Release() {

	assert(_instance && "ScriptManager no está inicializado!");

	if(_instance)
	{
		_instance->close();
		delete _instance;
	}

} // Release

//---------------------------------------------------------

bool CScriptManager::open() {

	_lua = lua_open();

	if(!_lua)
		return false;

	//Tenemos que activar y registrar la libreria basica, para que me funcionen las funciones de lua en el script
	//Le pedimos que integre en este interprete todas las funciones de la libreria basica
	//luaopen_base(_lua);

	//luaopen_math(_lua);
	
	luaL_openlibs(_lua);

	//esta instrucción carga todas las librerías de LUA
	luaL_openlibs(_lua);
	
	// Y luabind
	luabind::open(_lua);
	luabind::bind_class_info(_lua);	// RTTI de luabind

	// Registramos la función de error
	luabind::set_pcall_callback(CScriptManager::luaError);

	return true;

} // open

//---------------------------------------------------------

void CScriptManager::close() {

	if(_lua)
		lua_close(_lua);

	_lua = nullptr;

} // close

//---------------------------------------------------------

bool CScriptManager::loadScript(const char *script) {

	assert(_lua);
	try
	{
		luaL_dofile(_lua, script);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua: "+std::string(ex.what())+" En el fichero: "+script);
		return false;
	}

	return true;
} // loadScript

//---------------------------------------------------------

bool CScriptManager::executeScript(const char *script) {
	 
	if(luaL_dostring(_lua, script))
	{
		BaseSubsystems::Log::Error("Error sintactico al cargar el fichero lua: "+std::string(script));
		return false;
	}

	return true;

} // executeScript

//---------------------------------------------------------

int CScriptManager::getField(const char *table,const char *field, int defaultValue)
{
		//lE PIDO A LUA BIND CUAL ES EL OBJETO DE LA TABLA
		luabind::object obj = luabind::globals(_lua)[table];

		//Si el objeto no es una table, entonces falla
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		{
			return defaultValue;
		}

		//Y SI ES UNA TABLA, entonces obtengo el valor correspondiente a field dentro de la tabla
		luabind::object tablefield = obj[field];

		//Si el objeto no es un numero, entonces falla
		if(!tablefield.is_valid() || (luabind::type(tablefield) != LUA_TNUMBER))
		{
			return defaultValue;
		}

		return luabind::object_cast<int>(tablefield);
}

bool CScriptManager::getField(const char *table,const char *field, bool defaultValue)
{
		//lE PIDO A LUA BIND CUAL ES EL OBJETO DE LA TABLA
		luabind::object obj = luabind::globals(_lua)[table];

		//Si el objeto no es una table, entonces falla
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		{
			return defaultValue;
		}

		//Y SI ES UNA TABLA, entonces obtengo el valor correspondiente a field dentro de la tabla
		luabind::object tablefield = obj[field];

		//Si el objeto no es un numero, entonces falla
		if(!tablefield.is_valid() || (luabind::type(tablefield) != LUA_TBOOLEAN))
		{
			return defaultValue;
		}

		return luabind::object_cast<bool>(tablefield);
}

std::string CScriptManager::getField(const char *table,const char *field, const char * defaultValue)
{
		//lE PIDO A LUA BIND CUAL ES EL OBJETO DE LA TABLA
		luabind::object obj = luabind::globals(_lua)[table];

		//Si el objeto no es una table, entonces falla
		if(!obj.is_valid() || (luabind::type(obj) != LUA_TTABLE))
		{
			return defaultValue;
		}

		//Y SI ES UNA TABLA, entonces obtengo el valor correspondiente a field dentro de la tabla
		luabind::object tablefield = obj[field];

		//Si el objeto no es un numero, entonces falla
		if(!tablefield.is_valid() || (luabind::type(tablefield) != LUA_TSTRING))
		{
			return defaultValue;
		}

		return luabind::object_cast<std::string>(tablefield);
}

//---------------------------------------------------------

int CScriptManager::getGlobal(const char *name, int defaultValue)
{
	assert(_lua);

	//Me devuelve la variable global llamada name
	luabind::object obj = luabind::globals(_lua)[name];

	//Si no es un valor valida o no es de tipo numero, devuelvo el valor por defecto
	if(!obj.is_valid() || (luabind::type(obj) != LUA_TNUMBER))
	{
		return defaultValue;
	}

	//Si todo ha ido correctamente devuelvo el objeto entero
	return luabind::object_cast<int>(obj);
}

bool CScriptManager::getGlobal(const char *name, bool defaultValue)
{
	assert(_lua);

	//Me devuelve la variable global llamada name
	luabind::object obj = luabind::globals(_lua)[name];

	//Si no es un valor valida o no es de tipo numero, devuelvo el valor por defecto
	if(!obj.is_valid() || (luabind::type(obj) != LUA_TBOOLEAN))
	{
		return defaultValue;
	}

	//Si todo ha ido correctamente devuelvo el objeto entero
	return luabind::object_cast<bool>(obj);
}

std::string CScriptManager::getGlobal(const char *name, const char * defaultValue)
{
	assert(_lua);

	//Me devuelve la variable global llamada name
	luabind::object obj = luabind::globals(_lua)[name];

	//Si no es un valor valida o no es de tipo numero, devuelvo el valor por defecto
	if(!obj.is_valid() || (luabind::type(obj) != LUA_TSTRING))
	{
		return defaultValue;
	}

	//Si todo ha ido correctamente devuelvo el objeto entero
	return luabind::object_cast<std::string>(obj);
}

//---------------------------------------------------------

bool CScriptManager::setGlobal(const char *name, const int &value) {

	assert(_lua);

	luabind::globals(_lua)[name] = value;

	return true;
}

bool CScriptManager::setGlobal(const char *name, const bool &value)
{
	assert(_lua);

	luabind::globals(_lua)[name] = value;

	return true;
}

bool CScriptManager::setGlobal(const char *name, const std::string &value)
{
	assert(_lua);

	luabind::globals(_lua)[name] = value;

	return true;
}


//---------------------------------------------------------

bool CScriptManager::executeProcedure(const char *subroutineName) {

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName]();
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua: "+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

} // executeProcedure

bool CScriptManager::executeProcedure(const char *subroutineName, int &param1)
{
		assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}


bool CScriptManager::executeProcedure(const char *subroutineName, int &param1, int &param2)
{
		assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1, param2);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName, const std::string &param1, const std::string &param2, const int &param3, const int &param4, const Vector3 &param5)
{
	assert(_lua);

	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](param1, param2, param3, param4, param5);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *subroutineName, Vector3 &param1)
{
	assert(_lua);

	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *subroutineName, std::string &param1, int &param2)
{
		assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName];

		//Llamos a la funcion
		obj(param1, param2);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *subroutineName, bool &param1)
{
		assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *subroutineName, float &param1)
{
	assert(_lua);

	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *subroutineName, const std::string &param1)
{
		assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}


bool CScriptManager::executeProcedure(const char *subroutineName, const std::string &param1, const std::string &param2)
{
		assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1, param2);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Debug("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *subroutineName, const std::string &param1, const std::string &param2, const std::string &param3, const float &param4)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1, param2, param3, param4);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

//---------------------------------------------------------

bool CScriptManager::executeProcedure(const char *subroutineName, const Vector3 &param1)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeFunction(const char *subroutineName, int param1, int &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<int>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

}

bool CScriptManager::executeFunction(const char *subroutineName, float param1, float param2, bool &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1, param2);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TBOOLEAN))
			return false;

		result = luabind::object_cast<bool>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

}

bool CScriptManager::executeFunction(const char *subroutineName, float param1, float &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<float>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

}

bool CScriptManager::executeFunction(const char *subroutineName, float &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName]();

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<float>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

}

//-------------------------------------

bool CScriptManager::executeFunction(const char *subroutineName, const std::string &param1, int &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<int>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Debug("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

}

bool CScriptManager::executeFunction(const char *subroutineName, const std::string &param1, unsigned int &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<unsigned int>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Debug("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

}

//-------------------------------------

bool CScriptManager::executeFunction(const char *subroutineName, const std::string &param1, float &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<float>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Debug("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

}

//-----------------------------------------------------------

bool CScriptManager::executeFunction(const char *subroutineName, const std::string &param1, std::string &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TSTRING))
			return false;

		result = luabind::object_cast<std::string>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

} 

//---------------------------------------------------------

bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName, const Vector3& _vector3, int param1, int param2)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](_vector3, param1, param2);

		if(!obj.is_valid())
			return false;
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName, const Vector3& _vector3)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](_vector3);

		if(!obj.is_valid())
			return false;
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName, const std::string &param1, int &param2, Vector3 &param3)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](param1, param2, param3);

		if(!obj.is_valid())
			return false;
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName, int &param1, int &param2)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](param1, param2);

		if(!obj.is_valid())
			return false;
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName, int &param1, int &param2, int &param3, Vector3 &param4)
{
	assert(_lua);

	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](param1, param2, param3, param4);

		if(!obj.is_valid())
			return false;
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName, int &param1)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](param1);

		if(!obj.is_valid())
			return false;
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName, float &param1)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](param1);

		if(!obj.is_valid())
			return false;
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}


bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName, const std::string &_string)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](_string);

		if(!obj.is_valid())
			return false;
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Debug("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeProcedure(const char *tabla, const char *subroutineName)
{
	
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName]();

		if(!obj.is_valid())
			return false;
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}


const char* CScriptManager::executeFunction(const char *tabla, const char *subroutineName)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName]();

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TSTRING))
			return false;

		return luabind::object_cast<const char*>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

}

bool CScriptManager::executeFunction(const char *tabla, const char *subroutineName, Vector3 &param1, int &result)
{
	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName](param1);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<int>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}
}

bool CScriptManager::executeFunction(const char *tabla, const char *subroutineName, int&result)
{
	assert(_lua);

	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName]();

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<int>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeFunction(const char *tabla, const char *subroutineName, std::string &result, int valor)
{
	assert(_lua);

	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[tabla][subroutineName]();

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TSTRING))
			return false;

		result = luabind::object_cast<std::string>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;
}

bool CScriptManager::executeFunction(const char *subroutineName, const std::string &param1, const std::string &param2, const std::string &param3, float &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1, param2, param3);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<float>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

} 


bool CScriptManager::executeFunction(const char *subroutineName, const std::string &param1, const std::string &param2, int &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1, param2);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TNUMBER))
			return false;

		result = luabind::object_cast<int>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

} 


bool CScriptManager::executeFunction(const char *subroutineName, const std::string &param1, const std::string &param2, std::string &result) 
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1, param2);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TSTRING))
			return false;

		result = luabind::object_cast<std::string>(obj);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

} 

bool CScriptManager::executeFunction(const char *subroutineName, const std::string &param1, const std::string &param2, int &param3, float &result, float &result2)
{

	assert(_lua);
	
	try{

		//Darme el valor de lua que hay detras de ese nombre como variable global
		luabind::object obj = luabind::globals(_lua)[subroutineName](param1, param2, param3);

		if(!obj.is_valid() && (luabind::type(obj) != LUA_TTABLE))
			return false;

		result = luabind::object_cast<float>(obj[1]);
		result2 = luabind::object_cast<float>(obj[2]);
	}
	catch (luabind::error &ex)
	{
		BaseSubsystems::Log::Error("Error ejecutando lua"+std::string(ex.what())+ "En la funcion: "+subroutineName);
		return false;
	}

	return true;

}

//---------------------------------------------------------

int CScriptManager::luaError(lua_State *L) {
		// Error en LUA
	std::stringstream msg;

	// Cambiamos el mensaje, para meter la información
	// de la pila de LUA.
	msg << "Error en script LUA: " << lua_tostring(L, -1) << std::endl;
	msg << "Pila de LUA: " << std::endl;
	int level = 0;
	lua_Debug d;

	while (lua_getstack(L, level, &d) == 1) {
		lua_getinfo(L, "Sln", &d);

		// Nivel dentro de la pila
		msg << level << ":: ";

		// ¿Es una llamada de C?
		if (strcmp(d.what, "C") == 0) {
			msg << "Native";
		} else {
			msg << d.short_src << ":" << d.currentline;
			//msg << std::endl << d.source << std::endl;
		}
		// Si tiene un nombre, lo ponemos
		if (d.name != 0)
			msg << " (" << d.namewhat << " " << d.name << ")";
		msg << std::endl;
		level++;
	}

	// Sacamos la cadena de error vieja
	lua_pop(L, 1);
	// Y metemos la nueva mejorada con la información de la pila, etc.
	lua_pushstring(L, msg.str().c_str());

	BaseSubsystems::Log::Error(msg.str());
	return 1;
}



} // namespace ScriptManager