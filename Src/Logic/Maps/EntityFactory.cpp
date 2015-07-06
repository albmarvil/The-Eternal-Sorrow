/**
@file EntityFactory.cpp

Contiene la implementación de la clase factoría de entidades
del juego.

@see Logic::CEntityFactory

@author David Llansó García, Marco Antonio Gómez Martín
@date Agosto, 2010
*/
#include "EntityFactory.h"
#include "ComponentFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Managers/PoolManager.h"
#include "Logic/Entity/Component.h"

#include "Map.h"
#include "Map/MapEntity.h"
#include "Map/MapParser.h"

#include <iostream>
#include <fstream>

#include "LUA/ScriptManager.h"


// HACK. Debería leerse de algún fichero de configuración
#define BLUEPRINTS_FILE_PATH LUA::CScriptManager::GetPtrSingleton()->getField("Paths","pathMaps", ".")

namespace Logic
{

	CEntityFactory *CEntityFactory::_instance = 0;
	
	//---------------------------------------------------------

	CEntityFactory::CEntityFactory()
	{
		_instance = this;

	} // CEntityFactory
	
	//---------------------------------------------------------

	CEntityFactory::~CEntityFactory()
	{
		_instance = 0;

	} // ~CEntityFactory
	
	//---------------------------------------------------------

	bool CEntityFactory::Init() 
	{
		assert(!_instance && "Segunda inicialización de Logic::CEntityFactory no permitida!");

		new CEntityFactory();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init
	
	//---------------------------------------------------------

	void CEntityFactory::Release() 
	{
		assert(_instance && "Logic::CEntityFactory no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CEntityFactory::open()
	{
		//_archetypes  = new ArchetypeMap;

		return true;

	} // open

	//--------------------------------------------------------

	void CEntityFactory::close() 
	{
		unloadBluePrints();
		unloadArchetypes();
		//delete _archetypes;

	} // close
	
	//---------------------------------------------------------

	//typedef std::pair<std::string,CEntityFactory::TBluePrint> TStringBluePrintPair;

	bool CEntityFactory::loadBluePrints(const std::string &filename)
	{
		//Obtenemos el punto de entrada al interprete
		LUA::CScriptManager *sm = LUA::CScriptManager::GetPtrSingleton();

		int ok;

		//Ejecutamos la funcion de LUA llamada loadBlueprints y le pasamos como parametro el blueprints
		ok = sm->executeProcedure("loadBlueprints", filename);

		// Comprobamos que la carga fue correcta
		if (!ok) {
			BaseSubsystems::Log::Error("Error durante la carga del blueprints: "+filename);
			return false;
		}

		return true;

	} // loadBluePrints
	
	//---------------------------------------------------------

	void CEntityFactory::unloadBluePrints()
	{
		_mapBlueprints.clear();

	} // unloadBluePrints
	
	//---------------------------------------------------------

	//TODO VER QUE PASA CUANDO SE CREA _archetypes Y SE PASA A CREATEeNTITY
	bool CEntityFactory::loadArchetypes(const std::string &filename)
	{

		BaseSubsystems::Log::Debug("Cargando arquetipo: " + filename);
		if(!Map::CMapParser::getSingletonPtr()->parseFile(filename))
		{
			assert(!"No se ha podido parsear los arquetipos.");
			return false;
		}

		// Extraemos los arquetipos del parseo.
		Map::CMapParser::TEntityList entityList = Map::CMapParser::getSingletonPtr()->getEntityList();

		Map::CMapParser::TEntityList::iterator it = entityList.begin();
		Map::CMapParser::TEntityList::iterator end = entityList.end();

		//Construimos la informacion del mapa de arquetipos
		for(; it != end; ++it)
		{
			ArchetypeMap::iterator find = _archetypes.find((*it)->getType());
			if(find != _archetypes.end())//Tenemos el arquetipo ya en el mapa
			{
				//eliminamos primero el que ya estaba
				delete (find->second);
				//clonamos la entidad obtenida y nos guardamos nuestro puntero, ya que se desreferencia
				//cuando el parser se vuelve a usar
				find->second = new Map::CEntity((*it)); //actualizamos el arquetipo
			}
			else //si no estaba el arquetipo en el mapa lo añadimos
			{
				//añadimos la pareja <tipo, entityInfo>
				_archetypes.insert(std::pair<std::string, Map::CEntity*>((*it)->getType(),new Map::CEntity((*it))));
			}
		}
		return true;
	}//loadArchetypes

	//---------------------------------------------------------------------------

	void CEntityFactory::unloadArchetypes()
	{
		BaseSubsystems::Log::Debug("Descargando arquetipos");
		ArchetypeMap::iterator it = _archetypes.begin();
		for(; it != _archetypes.end(); ++it)
		{
			delete (it->second);
		}
		_archetypes.clear();

	} // unloadArchetypes
	
	//---------------------------------------------------------

	typedef std::map<std::string,std::vector<std::pair<std::string, bool>>> mapB;
	typedef std::vector<std::pair<std::string, bool>> vecComponentes;
	Logic::CEntity *CEntityFactory::assembleEntity(const std::string &type) 
	{
		mapB::const_iterator it = _mapBlueprints.find(type);

		// si el tipo se encuentra registrado.
		if (it != _mapBlueprints.end()) 
		{
			CEntity* ent = new CEntity(EntityID::NextID());

			std::string nombreComponente;
			bool dormido = false;

			// Añadimos todos sus componentes.
			IComponent* comp;

			vecComponentes::const_iterator itc = (*it).second.begin();

			for(; itc !=(*it).second.end(); ++itc)
			{
				nombreComponente = (*itc).first;
				dormido = !(*itc).second;
				
				if(CComponentFactory::getSingletonPtr()->has(nombreComponente))
				{
					comp = CComponentFactory::getSingletonPtr()->create(nombreComponente);
					comp->setName(nombreComponente);
				}
				else
				{
					assert(!"Nombre erroneo de un componente, Mira a ver si están todos bien escritos en el fichero de blueprints");
					delete ent;
					return 0;
				}
				if(comp)
					ent->addComponent(comp,dormido);

			}

			return ent;
		}
		return 0;

	} // assembleEntity
	
	//---------------------------------------------------------

	void CEntityFactory::setMapBlueprints(const std::string &type, const std::string &component, bool value)
	{
		//Si el tipo de entidad no esta añadido 
		if(_mapBlueprints.find(type) == _mapBlueprints.end())
		{
			//Lo añado, con un vector de pares de componentes/de si esta despierto o no
			_mapBlueprints[type] = std::vector<std::pair<std::string, bool>>();
		}

		//Obtengo el vector de componentes que se ha leido hasta el momento del tipo de entidad indicado
		std::vector<std::pair<std::string, bool>> vec = _mapBlueprints[type];
		//Creammos el par, componente y estado (despierto o dormido)
		std::pair<std::string, bool> couple(component, value);
		//Lo metemos en el map
		_mapBlueprints[type].push_back(couple);
	}

	Logic::CEntity *CEntityFactory::createEntity(/*const*/ Map::CEntity *entityInfo, Logic::CMap *map)
	{
		/**
		En este punto tenemos en entityInfo la información para construir la entidad. Dependiendo del tipo se ensanblará
		con unos componentes u otros (assembleEntity usa blueprints). Nosotros debemos tratar el objeto entityInfo para que use
		siempre y cuando pueda la información de _archetypes. Por ello el entityInfo nuevo tendra todo lo que venga en _archetypes + 
		los atributos extras del entityInfo (sobreescribiendo si es necesario). El type de entrada y salida han de ser el mismo.
		
		TODO ESTO SE REALIZA A NIVEL DE MAP::CENTITY
		*/
		//buscamos el arquetipo que coincida con la entidad que se esta creando
		//En caso de no haber arquetipo coincidente el entityInfo no se modifica

		//entityInfo->setName(entityInfo->getName()+std::to_string(EntityID::_nextId));

		ArchetypeMap::iterator find = _archetypes.find(entityInfo->getType());
		
		if(find != _archetypes.end())//Hay coincidencia de arquetipo, por lo tanto hacemos un merge de la informacion
		{
			entityInfo->mergeArchetype(find->second);
		}

		//Llamo al metodo instanciate del spawner, para ver si hay alguna entidad desactivada en el pull y activarla
		CEntity *ret = Logic::CPoolManager::getSingletonPtr()->instanciate(entityInfo);
		//Si no encontro ninguna entidad del tipo en el pull
		
		if(ret == 0)
		{
			//llamo al assembleEntity para añadirle sus componentes y crear la entidad
			ret = assembleEntity(entityInfo->getType());
		}

		//Si en el pull no ha encontrado ninguna entidad del tipo, y el assembleEntity no fue capaz de crear la entidad, devolvemos null
		if (!ret)
			return 0;

		// Añadimos la nueva entidad en el mapa.
		map->addEntity(ret);

		
		// Y lo inicializamos
		if (ret->spawn(map, entityInfo))
		{
			if(!map->activarEntidad(ret))
			{
				BaseSubsystems::Log::Debug("Spawneo de la entidad "+entityInfo->getName()+" fallido");
				return 0;
			}
			return ret;
		}
		else {
			map->removeEntity(ret);
			delete ret;
			return 0;
		}	

		

	} // createEntity
	
	//---------------------------------------------------------

	CEntity* CEntityFactory::createEntityByType(const std::string &type, Vector3 position, CMap *map)
	{
		//solo hacemos creamos algo a partir de un arquetipo así que nos aseguramos de que exista
		ArchetypeMap::iterator find = _archetypes.find(type);
		if( find == _archetypes.end())
		{
			return NULL;
		}

		//nos aseguramos de construir un nombre unico
		//int id = EntityID::_nextId;
		
		std::string uniqueName = type;

		//Hacer un new para que no se desreferencie en el futuro
		Map::CEntity *entityInfo = new Map::CEntity(uniqueName);


		entityInfo->setType(type);
		std::string pos = std::to_string(position.x)+','+std::to_string(position.y)+','+std::to_string(position.z);
		entityInfo->setAttribute("position", pos );
		entityInfo->setName(entityInfo->getName()+std::to_string(EntityID::_nextId));
		//meterlo en la cola de entidades del parser para que se borren al recargar mapas
		Map::CMapParser::getSingletonPtr()->addEntityInfo(entityInfo);

		return createEntity(entityInfo, map);

	}

	CEntity* CEntityFactory::createEntityByTypeTransform(const std::string &type, Matrix4 transform, CMap *map)
	{
		//solo hacemos creamos algo a partir de un arquetipo así que nos aseguramos de que exista
		ArchetypeMap::iterator find = _archetypes.find(type);
		if( find == _archetypes.end())
		{
			return NULL;
		}

		//nos aseguramos de construir un nombre unico
		//int id = EntityID::_nextId;
		
		std::string uniqueName = type;

		//Hacer un new para que no se desreferencie en el futuro
		Map::CEntity *entityInfo = new Map::CEntity(uniqueName);

		//El tipo
		entityInfo->setType(type);

		//La posicion
		Vector3 pos;
		Vector3 scale;
		Quaternion quat;

		transform.decomposition(pos, scale, quat);

		std::string string = std::to_string(pos.x)+','+std::to_string(pos.y)+','+std::to_string(pos.z);
		entityInfo->setAttribute("position", string );
		
		//El nombre unico
		entityInfo->setName(entityInfo->getName()+std::to_string(EntityID::_nextId));

		//La orientacion
		Vector3 axis;
		Ogre::Degree angle;
		quat.ToAngleAxis(angle, axis);

		string = std::to_string(axis.x) +","+std::to_string(axis.y)+","+std::to_string(axis.z);
		entityInfo->setAttribute("orientation",string);
		//std::cout<<string<<std::endl;
		string = std::to_string(angle.valueDegrees());
		//std::cout<<string<<std::endl;
		entityInfo->setAttribute("orientation_angle", string);

		//meterlo en la cola de entidades del parser para que se borren al recargar mapas
		Map::CMapParser::getSingletonPtr()->addEntityInfo(entityInfo);

		return createEntity(entityInfo, map);

	}

	void CEntityFactory::deleteEntity(Logic::CEntity *entity)
	{
		assert(entity);

		// Si la entidad estaba activada se desactiva al sacarla
		// del mapa.
		entity->getMap()->removeEntity(entity);
		delete entity;

	} // deleteEntity
	
	//---------------------------------------------------------

	//CEntity * CEntityFactory::createObject(const std::string &type, const std::string &family, const std::string &subtype, const std::string &model, 
	//									   const std::string &material, const Vector3 &position, CMap *map)
	//{
	//	//solo creamos algo a partir de un arquetipo así que nos aseguramos de que exista
	//	ArchetypeMap::iterator find = _archetypes.find(type);
	//	if( find == _archetypes.end())
	//	{
	//		return NULL;
	//	}

	//	std::string uniqueName = type + std::to_string(EntityID::_nextId);

	//	//Hacer un new para que no se desreferencie en el futuro
	//	Map::CEntity *entityInfo = new Map::CEntity(uniqueName);

	//	entityInfo->setType(type);

	//	std::string pos = std::to_string(position.x)+' '+std::to_string(position.y)+' '+std::to_string(position.z);
	//	entityInfo->setAttribute("position", pos );
	//	entityInfo->setAttribute("family", family);
	//	entityInfo->setAttribute("subtype", subtype);
	//	entityInfo->setAttribute("model", model);
	//	entityInfo->setAttribute("material", material);
	//	
	//	//meterlo en la cola de entidades del parser para que se borren al recargar mapas
	//	Map::CMapParser::getSingletonPtr()->addEntityInfo(entityInfo);

	//	return createEntity(entityInfo, map);
	//}

	void CEntityFactory::deferredDeleteEntity(Logic::CEntity *entity)
	{
		assert(entity);

		if(!entity->getIsDead())
		{
			_pendingEntities.push_back(entity);
			entity->setDead(true);
		}

	} // deferredDeleteEntity
	
	//---------------------------------------------------------

	void CEntityFactory::deleteDefferedEntities()
	{
		TEntityList::iterator it(_pendingEntities.begin());

		for(; it != _pendingEntities.end(); ++it)
		{
			// Si la entidad estaba activada se desactiva al sacarla del mapa.
			(*it)->getMap()->removeEntity((*it));
	
			//llevo la entidad al pool del spawner
			Logic::CPoolManager::getSingletonPtr()->addEntityDeactivateToPool((*it));

			//Esto habra que hacerlo al final del juego, bien cuando el jugador cierre el juego, bien cuando el jugador pierda
			//deleteEntity(entity);
		}

		if (!_pendingEntities.empty())
			_pendingEntities.clear();

	} // deleteDefferedObjects

	Map::CEntity* CEntityFactory::getArchetype(std::string &type)
	{
		ArchetypeMap::iterator find = _archetypes.find(type);

		if(find != _archetypes.end())//lo encontramos
		{
			return find->second;
		}
		else
		{
			return NULL;
		}
	}

} // namespace Logic
