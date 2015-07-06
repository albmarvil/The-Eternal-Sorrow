/**
@author Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#include "Server.h"
#include <cassert>
#include "ScriptManager.h"
#include "FunctionsToLUA.h"
#include "ClassToLUA.h"
namespace LUA
{
	CServer* CServer::_instance = 0;
	//--------------------------------------------------------
	CServer::CServer()
	{
		_instance = this;

	} // CServer
	//--------------------------------------------------------
	CServer::~CServer()
	{
		_instance = 0;

	} // ~CServer
	//--------------------------------------------------------
	bool CServer::Init()
	{
		//assert(!_instance && "Segunda inicialización de GUI::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init
	//--------------------------------------------------------
	void CServer::Release()
	{
		assert(_instance && "GUI::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release
	//--------------------------------------------------------
	bool CServer::open()
	{
		return LUA::CScriptManager::Init();
	}
	//--------------------------------------------------------
	void CServer::close()
	{
		LUA::CScriptManager::Release();
	}
	//--------------------------------------------------------
	bool CServer::publishAll()
	{
		//Esto no lo hago con new, ya que al salir de este ambito me dan igual. 
		LUA::CClassToLUA ctl;
		LUA::CFunctionsToLUA ftl;

		if(!ctl.publishAllClass())
			return false;

		if(!ftl.publishAllFunctions())
			return false;

		return true;
	}
}