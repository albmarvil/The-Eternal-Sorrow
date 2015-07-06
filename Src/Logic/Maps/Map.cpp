/**
@file Map.cpp

Contiene la implementación de la clase CMap, Un mapa lógico.

@see Logic::Map

@author David Llansó
@date Agosto, 2010
*/

#include "Map.h"

#include "Logic/Entity/Entity.h"
#include "EntityFactory.h"
#include "ComponentFactory.h"
#include "Logic/Entity/Component.h"
#include "Logic/Managers/PoolManager.h"

#include "Map/MapParser.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"

#include "Physics/Server.h"
#include "Physics/Actor.h"
#include "Physics/StaticActor.h"


#include "Map/ParserTile.h"
#include "Map/MapEntity.h"

#include "LUA/ScriptManager.h"

#include <ctime>
#include <cassert>

// HACK. Debería leerse de algún fichero de configuración
#define MAP_FILE_PATH LUA::CScriptManager::GetPtrSingleton()->getField("Paths","pathMaps", ".")

namespace Logic {
		
	CMap* CMap::createMapFromFile(const std::string &filename)
	{
		srand (time(NULL));
		// Completamos la ruta con el nombre proporcionado
		std::string completePath = "map" + filename + ".lua";

		// Parseamos el fichero
		if(!Map::CMapParser::getSingletonPtr()->parseFile(completePath))
		{
			assert(!"No se ha podido parsear el mapa.");
			return false;
		}
		
		//PARSEAMOS EL ESCENARIO
		std::string XMLCompletePath(MAP_FILE_PATH);
		XMLCompletePath = XMLCompletePath + "map" + filename + ".tmx";

		//Realizamos el parser del fichero xml
		if(!Map::CParserTile::getSingletonPtr()->parserFile(XMLCompletePath))
		{
			assert(!"No se ha podido parsear el mapa.");
			return false;
		}

		// Si se ha realizado con éxito el parseo creamos el mapa.
		CMap *map = new CMap(filename);


		#pragma region Puntos de Interes
		
		//Llamamos al DropMgr y le pasamos la lista de puntos para que cree los entityInfo de las entidades
		//LUA::CScriptManager::GetPtrSingleton()->executeScript("Drop.CreateObjects()");
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("Drop", "CreateObjects");
		#pragma endregion

		// Extraemos las entidades del parseo.
		Map::CMapParser::TEntityList entityList = 
			Map::CMapParser::getSingletonPtr()->getEntityList();

		//Extramos las entidades lógicas de los parseos de los tiles del escenario
		Map::CParserTile::TEntityList logicEntityListEscenario = Map::CParserTile::getSingletonPtr()->getLogicEntityList();

		#pragma region Eleccion de la puerta
		//Obtenemos todas las posibles puertas. 
		Map::CParserTile::TMatriz doors = Map::CParserTile::getSingletonPtr()->getDoors();

		int dado = 0;
		std::vector<Map::CEntity*>::const_iterator itDoor;
		//Si hay puertas
		if(doors.size() != 0)
		{
			//Elegimos la puerta a poner
			dado = rand() % doors.size();
		
			//Ponemos la puerta en la logica
			logicEntityListEscenario.push_back(doors.at(dado)[0]);

			//Elimino la puerta logica, dado que ya no la usare mas, de la matriz doors. El itDoor sera usado posteriormente. 
			itDoor = doors.at(dado).begin();
			doors.at(dado).erase(itDoor);
		}

		#pragma endregion

		//MOVEMOS a entitList la lista de entidades de tile, y al mismo dejamos vacia logicEntityListEscenario
		entityList.splice(entityList.end(), logicEntityListEscenario);

		//Extramos las entidades correspondientes a cada uno de los tiles del escenario
		Map::CParserTile::TMatrizMatriz entityMatrixEscenario = Map::CParserTile::getSingletonPtr()->getEntityList();

		#pragma region Meter las puertas no seleccionada, como tiles TILE_DEFAULT en la matriz de tiles del escenario, antes del preprocesado
		/*
		Lo que hago es cambiar los entityInfo de las puertas no escogidos por el de tile_Default, y lo meto en la matriz de tiles 
		del escenario para ser tenida en cuenta para el preprocesado
		*/
		//Si hay puertas
		if(doors.size()!= 0)
		{
			//Declaracion de variables a ser usadas en el for
			std::string tipoTile;
			Map::CParserTile::TMatriz::iterator itPuertas = doors.begin();
			Map::CEntity *ent;
			//Necesito la proporcion para saber donde meterlo en la matriz de tiles del escenario
			Vector3 proporcion = Vector3(Map::ANCHO_TILE, Map::ALTO_TILE, Map::PROFUNDO_TILE);
	
			//Recorro la matriz de las puertas
			for(; itPuertas != doors.end(); ++itPuertas)
			{
				//Obtengo el grupo que corresponde a la itPuertas puerta. Por ahora el conjunto es 13-13-13-14/15
				itDoor = (*itPuertas).begin();

				//Y lo recorro
				for(; itDoor != (*itPuertas).end(); ++itDoor)
				{
					
					//Si NO se corresponde con el grupo asociado a la puerta escogida, ponemos su tipo tile_default
					if((*itPuertas) != doors.at(dado))
					{
						tipoTile = "TILE_DEFAULT";

						//Creamos el nuevo entityInfo con su nuevo tipo, actualizado mediante el arquetipo del tipo, y añadido a la matriz para el preprocesado
						ent = (*itDoor);
						ent->setType(tipoTile);
						ent->mergeArchetype(Logic::CEntityFactory::getSingletonPtr()->getArchetype(tipoTile));
						entityMatrixEscenario[Map::TipoCapa::Colision][-ent->getVector3Attribute("position").y/proporcion.y][ent->getVector3Attribute("position").x/proporcion.x] = ent;
					}
					
					//Si se corresponde con el grupo asociado a la puerta escogida, ponemos su tipo tile_door
					/*if((*itPuertas) == doors.at(dado))
					{
						tipoTile = "TILE_DOOR";
					}
					else
					{
						tipoTile = "TILE_DEFAULT";
					}

					//Creamos el nuevo entityInfo con su nuevo tipo, actualizado mediante el arquetipo del tipo, y añadido a la matriz para el preprocesado
					ent = (*itDoor);
					ent->setType(tipoTile);
					ent->mergeArchetype(Logic::CEntityFactory::getSingletonPtr()->getArchetype(tipoTile));
					entityMatrixEscenario[Map::TipoCapa::Colision][-ent->getVector3Attribute("position").y/proporcion.y][ent->getVector3Attribute("position").x/proporcion.x] = ent;*/
				}
			}
		}
		doors.clear();

		#pragma endregion

		
		
		#pragma region procesado independiente de los tiles
		Map::CEntity* anterior = NULL;

		std::vector<Map::CEntity*> group;

		Map::CParserTile::TMatrizMatriz::iterator itTilesCapas = entityMatrixEscenario.begin();
		Map::CParserTile::TMatriz::iterator itTiles;
		std::vector<Map::CEntity*>::const_iterator itTiles2;
		for(; itTilesCapas != entityMatrixEscenario.end(); ++itTilesCapas)
		{
			itTiles = (*itTilesCapas).begin();
			for(; itTiles != (*itTilesCapas).end(); ++itTiles)
			{
				itTiles2 = (*itTiles).begin();

				for(; itTiles2 != (*itTiles).end(); ++itTiles2)
				{
					if(!(*itTiles2))
					{
						//group.clear();
						continue;
					}
					//construimos las entidades gráficas
					map->createTileGraphicEntity((*itTiles2));

					//preprocesado de la parte física
					if((*itTiles2)->hasAttribute("physic_entity"))
					{
						#pragma region preprocesado Horizontal
						Map::CEntity* actual = (*itTiles2);
				
						if(anterior == NULL)//estamos ante el primer caso
						{
							group.push_back(actual);
						}
						else if(itTiles2 == --(*itTiles).end() && itTiles == --(*itTilesCapas).end())//estamos ante el último caso
						{
							group.push_back(actual);
							Map::CEntity agrupado = map->agrupaTileFisico(group);
							map->createTilePhysicEntity(&agrupado);
							group.clear();
						}
						else if(map->tileFisicoContiguo(anterior, actual))
						{
							group.push_back(actual);
						}
						//Si ya no hay mas que agrupar, creamos la malla fisica total correspondiente
						else if (group.size() > 0)
						{
							//creamos entidad fisica
							Map::CEntity agrupado = map->agrupaTileFisico(group);
							map->createTilePhysicEntity(&agrupado);
							group.clear();
							//empiezo a agrupar con actual que era diferente al grupo
							group.push_back(actual);
						}

						anterior = actual;
						#pragma endregion
					}
				}
			}
		}
		#pragma endregion


		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();


		Map::CMapParser::TEntityList::const_iterator it, end;
		it = entityList.begin();
		end = entityList.end();

		#pragma region Procesado del entityInfo de configuración de la escena
		Map::CEntity* sceneConfig;

		for(; it != end; ++it)
		{
			if((*it)->getType().compare("SceneConfiguration")==0)
			{
				sceneConfig = (*it);
				map->setSceneConfiguration(sceneConfig);
				entityList.erase(it);
				break;
			}
		}
		#pragma endregion

		/*#pragma region Poner al player de primero, para que sea el primero en ser procesado
		Map::CEntity *player;

		it = entityList.begin();
		for(;it != end; ++it)
		{
			if((*it)->getType().compare("Player")==0)
			{
				player = (*it);
				entityList.erase(it);
				entityList.push_front(player);
				break;
			}
		}
		#pragma endregion*/

		it = entityList.begin();
		// Creamos todas las entidades lógicas.
		for(; it != end; ++it)
		{
			// La propia factoría se encarga de añadir la entidad al mapa.
			CEntity *entity = entityFactory->createEntity((*it),map);
			assert(entity && "No se pudo crear una entidad del mapa");
		}

		return map;

	} // createMapFromFile

	//--------------------------------------------------------

	CMap::CMap(const std::string &name)
	{
		_mapaCreado = false;
		//_archetypeChanged = false;
		_name = name;
		_scene = Graphics::CServer::getSingletonPtr()->createScene(name);

	} // CMap

	//--------------------------------------------------------

	CMap::~CMap()
	{
		destroyAllEntities();
		if(Graphics::CServer::getSingletonPtr())
			Graphics::CServer::getSingletonPtr()->removeScene(_scene);
		

	} // ~CMap

	//--------------------------------------------------------

	bool CMap::activate()
	{
		Graphics::CServer::getSingletonPtr()->setScene(_scene);

		//Flag que indica que se han hecho todos los spawn/respawn
		_mapaCreado = true;

		TEntityMap::const_iterator it,end;
		it = _entityMap.begin();
		end = _entityMap.end();
		bool correct = true;

		// Activamos todas las entidades registradas en el mapa.
		for(; it != end; it++)
		{
			correct = (*it).second->activate() && correct;
		}
			

		return correct;

	} // getEntity

	//--------------------------------------------------------

	void CMap::deactivate()
	{
		TEntityMap::const_iterator it, end;
		end = _entityMap.end();
		it = _entityMap.begin();

		// Desactivamos todas las entidades activas registradas en el mapa.
		for(; it != end; it++)
			if((*it).second->isActivated())
				(*it).second->deactivate();

		Graphics::CServer::getSingletonPtr()->setScene(0);

	} // getEntity

	//---------------------------------------------------------
	void CMap::tick(unsigned int msecs) 
	{
		TEntityMap::const_iterator it;
		for( it = _entityMap.begin(); it != _entityMap.end(); ++it )
			(*it).second->tick(msecs);

	} // tick

	//--------------------------------------------------------

	typedef std::pair<TEntityID,CEntity*> TEntityPair;


	void CMap::addEntity(CEntity *entity)
	{
		//Si no existe metemos la entidad en la lista de entidades activas.
		if(_entityMap.count(entity->getEntityID()) == 0)
		{
			TEntityPair elem(entity->getEntityID(),entity);
			_entityMap.insert(elem);
		}

	} // addEntity

	bool CMap::activarEntidad(CEntity *entity)
	{
		//Si la escena actual es la escena del mapa, entonces significa que todas las entidades iniciales de este mapa se han activado.
		//Por que en el activate de esta clase establece la escena actual a _scene.
		//Por tanto si queremos añadir una entidad estando ya en la escena, tendremos que activarla desde aqui. 
		if(_mapaCreado)
		{
			return entity->activate();
		}
		return true;
	}
	//-------------------------------------------------------

	void CMap::removeEntity(CEntity *entity)
	{
		if(_entityMap.count(entity->getEntityID()) != 0)
		{
			if(entity->isActivated())
				entity->deactivate();
			entity->_map = 0;
			_entityMap.erase(entity->getEntityID());
		}

	} // removeEntity

	//--------------------------------------------------------

	void CMap::destroyAllEntities()
	{
		CEntityFactory* entityFactory = CEntityFactory::getSingletonPtr();

		TEntityMap::const_iterator it, end;
		it = _entityMap.begin();
		end = _entityMap.end();

		// Eliminamos todas las entidades. La factoría se encarga de
		// desactivarlas y sacarlas previamente del mapa.
		while(it != end)
		{
			CEntity* entity = (*it).second;
			it++;
			if(entity->getType().compare("Camera")==0 || entity->getType().compare("Player")==0)
				entityFactory->deleteEntity(entity);
			else
				entityFactory->deferredDeleteEntity(entity);
			//En lugar de eliminarlas para siempre, lo que hago es eliminarlas del mapa, pero no del juego
			
		}
		entityFactory->deleteDefferedEntities();
		_entityMap.clear();

		#pragma region Vaciado de pool
		//si hemos cambiado de arquetipo durante este mapa vaciamos el pool
		/*if(_archetypeChanged)
		{
			Logic::CPoolManager::getSingletonPtr()->destroyColaActual();
			_archetypeChanged = false;
		}*/
		#pragma endregion

		#pragma region destruccion de tiles graficos
		std::vector<Graphics::CEntity*>::iterator git = _graphicEntities.begin();
		for(; git != _graphicEntities.end(); ++git)
		{
			_scene->removeEntity((*git));
			delete (*git);
		}
		_graphicEntities.clear();
		#pragma endregion

		#pragma region destruccion de tiles fisicos
		std::vector<Physics::CActor*>::iterator fit = _physicActors.begin();
		for(; fit != _physicActors.end(); ++fit)
		{
			//(*fit)->getScene()->removeActor((*fit)->getActor());
			(*fit)->setSimulation(false);
			delete (*fit);
		}
		_physicActors.clear();
		#pragma endregion

	} // removeEntity

	//--------------------------------------------------------

	CEntity* CMap::getEntityByID(TEntityID entityID)
	{
		if(_entityMap.count(entityID) == 0)
			return 0;
		return (*_entityMap.find(entityID)).second;

	} // getEntityByID

	//--------------------------------------------------------

	CEntity* CMap::getEntityByName(const std::string &name, CEntity *start)
	{
		TEntityMap::const_iterator it, end;
		end = _entityMap.end();

		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		if (start)
		{
			it = _entityMap.find(start->getEntityID());
			// si la entidad no existe devolvemos NULL.
			if(it == end)
				return 0;
			it++;
		}
		else
			it = _entityMap.begin();

		for(; it != end; it++)
		{
			// si hay coincidencia de nombres devolvemos la entidad.
			if (!(*it).second->getName().compare(name))
				return (*it).second;
		}
		// si no se encontró la entidad devolvemos NULL.
		return 0;

	} // getEntityByName

	//--------------------------------------------------------

	CEntity* CMap::getEntityByType(const std::string &type, CEntity *start)
	{
		TEntityMap::const_iterator it, end;
		end = _entityMap.end();

		// Si se definió entidad desde la que comenzar la búsqueda 
		// cogemos su posición y empezamos desde la siguiente.
		if (start)
		{
			it = _entityMap.find(start->getEntityID());
			// si la entidad no existe devolvemos NULL.
			if(it == end)
				return 0;
			it++;
		}
		else
			it = _entityMap.begin();

		for(; it != end; it++)
		{
			// si hay coincidencia de nombres devolvemos la entidad.
			if (!(*it).second->getType().compare(type))
				return (*it).second;
		}
		// si no se encontró la entidad devolvemos NULL.
		return 0;

	} // getEntityByType


	bool CMap::createTileGraphicEntity(Map::CEntity* entityInfo)
	{
		std::string type = "";
		std::string model = "";
		bool isPrefab = false;
		Graphics::CEntity* graphicsEntity = NULL;

		std::string name = "";
		Matrix4 transform = Matrix4::ZERO;

		#pragma region Info General
		if(entityInfo->hasAttribute("name"))
			name = entityInfo->getStringAttribute("name");

		if(entityInfo->hasAttribute("position"))
		{
			Vector3 position = entityInfo->getVector3Attribute("position");
			transform.setTrans(position);
		}

		// Por comodidad en el mapa escribimos los ángulos en grados.
		if(entityInfo->hasAttribute("orientation"))
		{
			Vector3 orientation = entityInfo->getVector3Attribute("orientation");
			orientation.normalise();
			float angle = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("orientation_angle"));
			Math::rotate(orientation, angle, transform);
		}
		#pragma endregion

		#pragma region Info relacionada con la parte grafica
		if(entityInfo->hasAttribute("basic_shape"))
		{
			std::string shape = entityInfo->getStringAttribute("basic_shape");
			if(shape.compare("Prefab_Cube") == 0)
			{
				type = shape;
				isPrefab = true;
			}
		}
		else if(entityInfo->hasAttribute("model"))
		{
			model = entityInfo->getStringAttribute("model");
			isPrefab = false;
		}
		


		if(!isPrefab)
		{
			#pragma region Creacion de la entidad si no es prefab
			//graphicsEntity = createGraphicsEntity(entityInfo);
			bool isStatic = false;
			if(entityInfo->hasAttribute("static"))
				isStatic = entityInfo->getBoolAttribute("static");

			if(isStatic)
			{
				graphicsEntity = new Graphics::CStaticEntity(name, model);
				if(!_scene->addStaticEntity((Graphics::CStaticEntity*)graphicsEntity))
					return false;
			}
			else
			{
				graphicsEntity = new Graphics::CEntity(name, model);
				if(!_scene->addEntity(graphicsEntity))
					return false;
			}

			#pragma endregion
		}
		else
		{
			#pragma region Creacion de la entidad si es un prefab
			//graphicsEntity = createPrefabGraphicsEntity(entityInfo);

			bool isStatic = false;
			if(entityInfo->hasAttribute("static"))
				isStatic = entityInfo->getBoolAttribute("static");

			if(isStatic)
			{
				graphicsEntity = new Graphics::CStaticEntity(name,type);
				if(!_scene->addStaticEntity((Graphics::CStaticEntity*)graphicsEntity))
					return false;
			}
			else
			{
				graphicsEntity = new Graphics::CEntity(name,type);
				if(!_scene->addEntity(graphicsEntity))
					return false;
			}
			#pragma endregion
		}

		//Seteamos el material en caso de tenerlo
		if(entityInfo->hasAttribute("material"))
		{
			std::string  material = entityInfo->getStringAttribute("material");
			graphicsEntity->setMaterial(material);
		}

		//Seteamos la escala en caso de tener
		if(entityInfo->hasAttribute("scale"))
		{
			Vector3 scale = entityInfo->getVector3Attribute("scale");
			graphicsEntity->setScale(scale);
		}

		//seteamos la posicion
		graphicsEntity->setTransform(transform);
		#pragma endregion

		if(!graphicsEntity)
			return false;

		_graphicEntities.push_back(graphicsEntity);
		return true;

	}//createTileGraphicEntity
	//-----------------------------------------------------------------------------

	bool CMap::createTilePhysicEntity(Map::CEntity* entityInfo)
	{

		Physics::CActor* actor = NULL;
		Physics::CServer *server = Physics::CServer::getSingletonPtr();

		// Leer el tipo de entidad
		assert(entityInfo->hasAttribute("physic_entity"));
		const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");

		// Leer la posición de la entidad
		assert(entityInfo->hasAttribute("position"));
		const Vector3 position = entityInfo->getVector3Attribute("position");

		// Leer el grupo de colisión (por defecto 0)
		int group = 0;
		if (entityInfo->hasAttribute("physic_group"))
			group = entityInfo->getIntAttribute("physic_group");

		// Leer si es un trigger (por defecto no)
		bool trigger = false;
		if (entityInfo->hasAttribute("physic_trigger"))
			trigger = entityInfo->getBoolAttribute("physic_trigger");



		#pragma region rigid
		if (physicEntity == "rigid") 
		{
			// Leer el tipo de entidad: estáticos, dinámico o cinemático
			assert(entityInfo->hasAttribute("physic_type"));
			const std::string physicType = entityInfo->getStringAttribute("physic_type");
			assert((physicType == "static") || (physicType == "dynamic") || (physicType == "kinematic"));

			// Leer la forma (shape)
			assert(entityInfo->hasAttribute("physic_shape"));
			const std::string physicShape = entityInfo->getStringAttribute("physic_shape");

			if (physicType == "static") 
			{
				if (physicShape == "box") 
				{
					// Leer las dimensiones de la caja
					assert(entityInfo->hasAttribute("physic_dimensions"));
					Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");

					// Crear una caja estática, proporcionando la posicion de la entidad, las dimensiones del cubo, si es un triger, el grupo de colision y una referencia al componente fisico
					actor = new Physics::CStaticActor(position, physicDimensions, trigger, group,  NULL);//pasamos un NULL como componente
				}
			} 
		}
		#pragma endregion

		#pragma region fromFile
		if (physicEntity == "fromFile")
		{
			//return createFromFile(entityInfo);

			// Leer la ruta del fichero RepX
			assert(entityInfo->hasAttribute("physic_file"));
			const std::string file = entityInfo->getStringAttribute("physic_file");

			// Crear el actor a partir del fichero RepX
			actor = new Physics::CActor(file, group, trigger, NULL);//le pasamos un NULL como componente

			Matrix4 transform = Matrix4::ZERO;
			Vector3 orientation = Vector3::ZERO;
			float angle = 0;
			if(entityInfo->hasAttribute("physic_orientation"))
			{
				orientation = entityInfo->getVector3Attribute("physic_orientation");
				angle = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("physic_orientation_angle"));
			}
			Math::rotate(orientation, angle,transform);
			transform.setTrans(position);
			actor->setTransform(transform);
		}
		#pragma endregion


		if(actor == NULL)
		{
			return false;
		}

		_physicActors.push_back(actor);

		return true;
	}



	bool CMap::tileFisicoContiguo(Map::CEntity* anterior, Map::CEntity* actual)
	{
		//si alguno de los dos es null devolvemos false
		if(anterior == NULL || actual == NULL)
		{
			return false;
		}

		//También tenemos que asegurarnos de que estan usando el mismo arquetipo
		if(anterior->getType() != actual->getType())
		{
			return false;
		}

		/*
		Una forma fácil de saber si podemos agrupar o no es si el tile es un simple cuadrado que se podrá estirar
		o necesita de un fichero repX. En el caso de que alguno de los dos necesite un fichero ya no podremos agruparlos
		*/

		if(anterior->getStringAttribute("physic_entity") == "fromFile" || actual->getStringAttribute("physic_entity") == "fromFile")
		{
			return false;
		}

		//En este punto sabemos que los dos son tiles cuadrados estriables

		/*
		Para saber si son contiguas o no en la misma fila vamos a sacar su tile posicion y vamos a ver si estan
		en la misma fila de la matriz de tiles y después si se diferencian en menos de 1 unidad en la columna.
		*/
		assert(anterior->hasAttribute("position"));
		assert(actual->hasAttribute("position"));
		Vector3 anteriorPosition = anterior->getVector3Attribute("position");
		Vector3 actualPosition = actual->getVector3Attribute("position");

		assert(anterior->hasAttribute("physic_dimensions"));
		assert(actual->hasAttribute("physic_dimensions"));
		Vector3 anteriorDimensions = anterior->getVector3Attribute("physic_dimensions");
		Vector3 actualDimensions = actual->getVector3Attribute("physic_dimensions");
		
		anteriorDimensions *= 2;
		actualDimensions *= 2;

		//transformamos a tile posicion;
		anteriorPosition /= anteriorDimensions;
		actualPosition /= actualDimensions;

		if(anteriorPosition.y != actualPosition.y)//esta en filas diferentes
		{
			return false;
		}

		return abs(anteriorPosition.x - actualPosition.x) <= 1;///si la diferencia de ambos en la columna es menor o igual a 1, entonces estan consecutivos
	}//tileFisicoContiguo
	//-----------------------------------------------------------------------------------------------

	Map::CEntity CMap::agrupaTileFisico(std::vector<Map::CEntity*> group)
	{
		Map::CEntity* muestra = group[0];
		if(muestra->getStringAttribute("physic_entity") == "fromFile" )
		{
			return muestra;
		}

		assert(muestra->hasAttribute("physic_dimensions"));
		Vector3 dimensions = muestra->getVector3Attribute("physic_dimensions");

		//obtenemos la nueva dimension. Como agrupamos horizontalmente tan solo trasteamos en la x
		dimensions.x *= group.size();

		Map::CEntity* ultimo = group[group.size()-1];

		Vector3 firstPos = muestra->getVector3Attribute("position");
		Vector3 lastPos = ultimo->getVector3Attribute("position");
		float diff = (lastPos.x - firstPos.x)/2;
		
		//calculamos la posicion final
		firstPos.x += diff;

		Map::CEntity res (muestra->getName());
		res.setType(muestra->getType());

		std::string pos = std::to_string(firstPos.x)+','+std::to_string(firstPos.y)+','+std::to_string(firstPos.z);
		res.setAttribute("position", pos );

		std::string dim = std::to_string(dimensions.x)+','+std::to_string(dimensions.y)+','+std::to_string(dimensions.z);
		res.setAttribute("physic_dimensions", dim );
		
		res.mergeArchetype(muestra);
		return res;

	}//agrupaTileFisico
	//------------------------------------------------------------------------------------------------
	
	
	
	void CMap::setSceneConfiguration(Map::CEntity* sceneConfiguration)
	{
		//este método puede crecer bastante
		//seteamos la luz ambiente que venga si no la por defecto
		assert(_scene);//necesitamos la escena grafica para hacer esto
		Vector3 ambient = Vector3(0.9, 0.9, 0.9);
		if(sceneConfiguration->hasAttribute("ambient_light"))
		{
			ambient = sceneConfiguration->getVector3Attribute("ambient_light");
		}

		_scene->setAmbientLight(ambient);


		///Continuará
	}//

	///----------------------------


	/*void CMap::archetypeChangeNotification()
	{
		_archetypeChanged = true;
	}*/


	int CMap::getTileColision(Vector3 &position)
	{
		int x = position.x / Map::ANCHO_TILE;
		int y = position.y / Map::ALTO_TILE;

		return getTileColisionTP(x, -y);
	}

	int CMap::getTileColisionTP(int x, int y)
	{
		if(x>getMapTileWidth()-1)
		{
			return 0;
		}
		return Map::CParserTile::getSingletonPtr()->getCapa(Map::TipoCapa::Colision)->getMatrizIDs()[abs(y)][x].first;
	}

	int CMap::getMapTileWidth()
	{
		return Map::CParserTile::getSingletonPtr()->getCapa(Map::TipoCapa::Colision)->getMatrizIDs()[0].size();
	}

	int CMap::getMapTileHeight()
	{
		return Map::CParserTile::getSingletonPtr()->getCapa(Map::TipoCapa::Colision)->getMatrizIDs().size();
	}

} // namespace Logic
