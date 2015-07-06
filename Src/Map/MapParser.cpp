/**
@file MapParser.cpp

Contiene la implementación de la clase que encapsula el parseo de mapas.

@see Map::CMapParser

@author David Llansó García
@date Agosto, 2010
*/

#include <cassert>

#include "MapEntity.h"
#include "MapParser.h"
#include "LUA\ScriptManager.h"

namespace Map {

	CMapParser* CMapParser::_instance = 0;

	//--------------------------------------------------------

	CMapParser::CMapParser(): _entityInProgress(0)
	{
		_entityList = std::list<Map::CEntity*>();
		_instance = this;

	} // CMapParser

	//--------------------------------------------------------

	CMapParser::~CMapParser()
	{
		releaseEntityList();
		_instance = 0;

	} // ~CMapParser
	
	//--------------------------------------------------------

	bool CMapParser::Init()
	{
		assert(!_instance && "Segunda inicialización de Map::CMapParser no permitida!");

		new CMapParser();
		return true;

	} // Init

	//--------------------------------------------------------

	void CMapParser::Release()
	{
		assert(_instance && "Map::CMapParser no está inicializado!");
		if(_instance)
		{
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CMapParser::parseFile(const std::string &filename)
	{
		//Obtenemos el punto de entrada al interprete de LUA
		LUA::CScriptManager *sm = LUA::CScriptManager::GetPtrSingleton();

		// Borramos la lista de entidades anterior
		releaseEntityList();

		bool ok;
		//Ejecutamos el metodo loadMap de LUA, para cargar el mapa o arquetipo que le indiquemos en el filename
		ok = sm->executeProcedure("loadMap", filename);

		// Comprobamos que la carga fue correcta
		if (!ok) {
			BaseSubsystems::Log::Error("Error durante la carga del mapa.");
			return false;
		} 

		return true;
	} // parseFile

	//--------------------------------------------------------
		
	void CMapParser::releaseEntityList()
	{
		while(!_entityList.empty())
		{
			_entityInProgress = _entityList.back();
			_entityList.pop_back();
			delete _entityInProgress;
		}
	}

	void CMapParser::addEntityInfo(CEntity* entityInfo)
	{
		//Comprobamos si la entidad que acabamos de recibir es un enemigo
		if(entityInfo->hasAttribute("tag"))
		{
			std::string tag = entityInfo->getStringAttribute("tag");
		}
		_entityList.push_back(entityInfo);
	}

} // namespace Map
