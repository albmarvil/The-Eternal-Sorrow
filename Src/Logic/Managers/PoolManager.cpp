/**
@file PoolManager.cpp

Contiene la implementación de la clase CPoolManager, Singleton que se encarga de
la gestión del spawner de entidades.

@see Logic::CPoolManager

@author Cesar Manuel Paz Guzman
@date Mayo, 2015

*/

#include "PoolManager.h"

#include <cassert>
#include "BaseSubsystems/Log.h"

#include "Logic/Entity/Entity.h"

//Para poder acceder a la info del entityInfo
#include "Map/MapEntity.h"

namespace Logic
{

	CPoolManager* CPoolManager::_instance = 0;

	//--------------------------------------------------------

	CPoolManager::CPoolManager()
	{
		_instance = this;
	}

	//--------------------------------------------------------

	CPoolManager::~CPoolManager()
	{
		_instance = 0;
	}

	//--------------------------------------------------------

	bool CPoolManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CServer no permitida!");

		new CPoolManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;
	} 

	//--------------------------------------------------------

	void CPoolManager::Release()
	{
		assert(_instance && "Logic::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}
	} 

	//--------------------------------------------------------

	bool CPoolManager::open()
	{
		return true;
	} 

	//--------------------------------------------------------

	void CPoolManager::close() 
	{
		destroyColaActual();
	}

	//---------------------------------------------------------------------------------------

	void CPoolManager::destroyColaActual()
	{
		//Dado que las entidades no fueron destruidas en ningun lugar, las destruiremos aqui
		TColaMultiMap::const_iterator it = _colaEntidades.begin();
		std::list<CEntity*>::const_iterator it2;

		for(; it!= _colaEntidades.end(); ++it)
		{
			it2 = (*it).second.begin();
			for(; it2 != (*it).second.end(); ++it2)
			{
				delete (*it2);
			}
		}
		_colaEntidades.clear();
	}

	//--------------------------------------------------------

	void CPoolManager::addEntityDeactivateToPool(CEntity *_entity)
	{
		_colaEntidades[_entity->getType()].push_back(_entity);
	}

	//--------------------------------------------------------

	CEntity * CPoolManager::instanciate(Map::CEntity *entityInfo)
	{

		//Si hay alguno que estea en el pull de entidades desactivadas
		if(!_colaEntidades[entityInfo->getType()].empty())
		{
			//BaseSubsystems::Log::Debug("Numero de entidades de tipo "+entityInfo->getType()+" es "+std::to_string(_colaEntidades[entityInfo->getType()].size()));
			CEntity *entity = _colaEntidades[entityInfo->getType()].back();
			//La quito de la cola
			_colaEntidades[entityInfo->getType()].pop_back();
			//La devuelvo
			return entity;
		}
		
		return 0;
	}

	//--------------------------------------------------------
}
