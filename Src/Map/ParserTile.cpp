#include "Map\ParserTile.h"

#include "BaseSubsystems/Log.h"
#include <assert.h>

#include "pugixml.hpp"

#include "MapParser.h"
#include "MapEntity.h"

#include "Logic/Maps/EntityFactory.h"

namespace Map{

	//Al ser static, la instancia es necesario declararla de esta manera
	CParserTile* CParserTile::_instance = 0;
	//------------------------------------------------------------------------------------------------

	//Constructor del parser. Instanciamos el parser si no ha sido instanciado nunca. Y reservamos sitio para guardar la informacion relativa al mapa
	CParserTile::CParserTile()
	{
		assert(!_instance);
		_instance = this;
		ultimaPos = 0;
		//mapa = new Mapa();//Reservo memoria para las caracteristicas globales del mapa

	}
	//------------------------------------------------------------------------------------------------

	//Dado que es un singleton seran necesario los metodos Init y Release
	bool CParserTile::Init()
	{
		assert(!_instance);

		//Si no hay una instancia de la clase, llamo a su constructor
		new CParserTile();

		return true;
	}
	//------------------------------------------------------------------------------------------------

	void CParserTile::Release()
	{
		//Si hay instancia de la clase la elimino
		if(_instance)
		{
			delete _instance;
		}
		//delete capas
	}
	//------------------------------------------------------------------------------------------------

	//Destruyo todos los punteros asociados al escenario
	CParserTile::~CParserTile()
	{
		//Borrado del map que contiene cada una de las capas. Para ello debo acceder al valor del map que almacena las capas
		releaseCapas();

		//Finalmente igualo a 0 la instancia
		_instance = 0;	
	}
	//-----------------------------------------------------------------------------------------------
	
	//Funcion que realizar el parse del fichero tmx
	bool CParserTile::parserFile(const std::string &filename)
	{

		
		BaseSubsystems::Log::Debug("Nombre del fichero a parsear "+filename);

		pugi::xml_document doc;
		const char *file = filename.c_str();
		pugi::xml_parse_result result = doc.load_file(file);
		
		assert(result && "Error al cargar el fichero tmx del escenario");

		//Leo las caracteristicas generales del mapa (dimensiones, propiedades)

		#pragma region CaracteristicasMapa 
		int width = doc.first_child().attribute("width").as_float();
		int height = doc.first_child().attribute("height").as_float();
		#pragma endregion

		#pragma region Lectura de los atributos de los tiles
		pugi::xml_object_range<pugi::xml_named_node_iterator> atributos = doc.first_child().first_child().children("tile");
		pugi::xml_object_range<pugi::xml_named_node_iterator>::const_iterator itAtributos = atributos.begin();

		//Si hay tiles con atributos
		if(itAtributos != atributos.end())
		{
			pugi::xml_object_range<pugi::xml_named_node_iterator> properties =  (*itAtributos).first_child().children("property");
			pugi::xml_object_range<pugi::xml_named_node_iterator>::const_iterator itProperties = properties.begin();
			//Declaracion de variables antes de un for
			int id = 0;
			std::string name = "";
			std::string valor ="";
			std::map<std::string, std::string> atributo;
			//Leemos los tiles que contengan atributos
			for(; itAtributos != atributos.end(); ++itAtributos)
			{
				//Leemos el id del tile que contenga atributo. Le sumo 1, por que en el xml me lo resta
				id = itAtributos->attribute("id").as_int()+1;
				//Obtenemos los atributos del tile en cuestion
				properties = (*itAtributos).first_child().children("property");
				//Los recorremos
				for(itProperties = properties.begin(); itProperties != properties.end(); ++itProperties)
				{
					//Nombre del atributo
					name = (*itProperties).attribute("name").as_string();
					//Su valor
					valor = (*itProperties).attribute("value").as_string();
					//Lo inserto en el map correspondiente al tile
					atributo.insert(std::make_pair(name,valor));	
				}
				_atributosMap.insert(std::make_pair(id, atributo));
				//Lo limpio para usarlo de nuevo en la siguiente iteracion
				atributo.clear();
			}
		}
		#pragma endregion
		
		//Leo todas las capas y las almaceno en un map. Cada capa contiene una matriz que contiene los ids de los tiles que la conforman, ademas de otras caracteristicas
		#pragma region Capas
		//CAPAS
		pugi::xml_object_range<pugi::xml_named_node_iterator> capas = doc.first_child().children("layer");
		pugi::xml_object_range<pugi::xml_named_node_iterator>::const_iterator itCapas = capas.begin();
		pugi::xml_object_range<pugi::xml_named_node_iterator>::const_iterator endCapas = capas.end();
		
		//Guardo sitio para la matriz entityList, de manera que me guarda [width][height] espacios para almacenar Map::CEntity*
		TMatriz matriz2(height, std::vector<Map::CEntity*>(width));
		TMatrizMatriz matriz3(2, matriz2);
		_entityList = matriz3;

		int numeroCapa = 0;
		for(; itCapas != endCapas; ++itCapas, ++numeroCapa)
		{
			capa[numeroCapa] = new Capa();

			//Nombre de la capa
			capa[numeroCapa]->name = itCapas->attribute("name").as_string();

			//RELLENO LA MATRIZ DE CADA UNA DE LAS CAPAS CON LOS IDS DE LOS TILES QUE LA CONFORMAN
			pugi::xml_node nodoData = itCapas->child("data");
			pugi::xml_object_range<pugi::xml_named_node_iterator> idTiles = nodoData.children("tile");
			pugi::xml_object_range<pugi::xml_named_node_iterator>::const_iterator itidTiles = idTiles.begin();
			pugi::xml_object_range<pugi::xml_named_node_iterator>::const_iterator endidTiles = idTiles.end();

			//Creamos la matriz para almcenar los ids y entityInfos con el tamaño del mapa
			typedef std::vector<std::vector<std::pair<int,CEntity*>>> Matriz;
			Matriz matri(height,std::vector<std::pair<int,CEntity*>>(width));

			//Dado que no he inicializado la matriz de la capa cuando la he definido, la inicializo ahora igualandola a la recien creada
			capa[numeroCapa]->matriz = matri;

			//RELLENO LA MATRIZ
			#pragma region Relleno de la matriz

			for(int i=0; i < height; ++i)
			{
				for(int j=0; j < width; ++j, ++itidTiles)
				{
					assert(itidTiles!=endidTiles);

					capa[numeroCapa]->matriz[i][j].first = itidTiles->attribute("gid").as_int();//Almaceno el id del tile

					if(capa[numeroCapa]->matriz[i][j].first != 0)
					{
						//CREAMOS EL ENTITYINFO ASOCIADO AL TILE
						TipoCapa tipocapa = TipoCapa(numeroCapa);
						createEntity(capa[numeroCapa]->matriz[i][j].first, //ID
							tipocapa, //numero de capa
							j, // pos X
							-i, //pos Y la posicion es negativa para ir construyendo los bloques en orden
							std::to_string(numeroCapa) + std::to_string(i)+ '_'+ std::to_string(j) + '_'// nombre 
						);	
					}
				}
			}

			#pragma endregion
		}

		#pragma endregion

		#pragma region Crear puertas

		//Dado que una puerta esta formada por 4 tiles, creo una matriz de dimensiones 1 por cada grupo de puertas y 4 porque son 4 tiles
		TMatriz matrix(_doorMap.size()/4, std::vector<Map::CEntity*>(4));
		_doorList = matrix;
		createDoorEntity(TipoTile::DOOR_HORIZONTAL_TRIGGER);
		createDoorEntity(TipoTile::DOOR_VERTICAL_DER_TRIGGER);
		createDoorEntity(TipoTile::DOOR_VERTICAL_IZQ_TRIGGER);

		#pragma endregion

		return true;
	}
	//------------------------------------------------------------------------------------------------

	//Funcion que crea la entidad, de manera que hace transparente en el metodo de parsear el tipo de capa, y asi lo gestionamos aqui 
	bool CParserTile::createEntity(int id, TipoCapa numcapa, float posX, float posY, const std::string &name)
	{
		TipoTile tipoTile = TipoTile(id);
		std::string archetype = TileArchetype::TipoTileToArchetype(tipoTile);
		
		CEntity *arch = Logic::CEntityFactory::getSingletonPtr()->getArchetype(archetype);
		
		assert(arch && "No hay arquetipo para crear los tiles!");

		//CREAMOS EL ENTITYINFO
		_entityInProgress = new Map::CEntity(archetype + name);
		_entityInProgress->setType(archetype); //Arquetipo

		if(numcapa == TipoCapa::Logic || numcapa == TipoCapa::PuntosInteres || numcapa == TipoCapa::PuntosAltares)
		{
			logicsEntities(id, numcapa, posX, posY, arch);
		}
		else//Si la capa es de colision o de fondo
		{
			createTileEntity(id, numcapa, posX, posY, arch);
		}

		//Meto en la matriz, en la pos correspondiente, su entityInfo
		capa[numcapa]->matriz[-posY][posX].second = _entityInProgress;

		return true;
	}//createEntity
	//--------------------------------------------------------------------------------

	bool CParserTile::createTileEntity(int id, TipoCapa numcapa, float posX, float posY, Map::CEntity * arquetipo)
	{
		//Calcula la proporcion del scale del prefab. Las dimensiones en ogre sno 100x100x100 mientras aqui es 1x1x1
		Vector3 proporcion = Vector3(Map::ANCHO_TILE, Map::ALTO_TILE, Map::PROFUNDO_TILE);
		//Esto es para darle profunidad de mas lejana a mas cercana a medida que la capa es mayor. Y para asignarle un material distinto
		//El material viene definido desde el arquetipo
		//OJO esto esta mas o menos suponienod que son 3 capas
		float posZ = 0;
		switch (numcapa)
		{
		case TipoCapa::Fondo://esta es la capa del fondo
			//para ajustarnos a la capa de colision vamos a necesitar la escala de dicha capa
			//esto esta un poco hardcodeado

			proporcion.z += Map::PROFUNDO_TILE*0.5;
			posZ = -1;
			break;
		case TipoCapa::Colision:
			posZ = 0;
			break;
		}

		//Posicion del tile
		std::string pos = std::to_string(posX*proporcion.x)+','+std::to_string(posY*proporcion.y)+','+std::to_string(posZ*proporcion.z);
		_entityInProgress->setAttribute("position", pos );							//POSITION		

		//Si el id es de tipo door, lo insertamos en el map que representa todos los tiles que forman las puertas
		if( id == TipoTile::TILE_DOOR)
		{
			//El map es un id del tile, junto con su posicion en tilePosicion, para posteriormente obtener asi su entityInfo de la matriz 
			_doorMap.insert(std::make_pair(id, Vector2(posX, posY)));
		}
		else
		{	
			//Aqui hacemos el merge con el arquetipo
			_entityInProgress->mergeArchetype(arquetipo);
			//Dado que el y pasado como parametro es negativo, aqui necesito convertilo en posicion, sino me da error de acceso
			_entityList[numcapa][-posY][posX] = _entityInProgress;
		}
		
		return true;
	}

	//--------------------------------------------------------------------------------

	bool CParserTile::logicsEntities(int id, TipoCapa numcapa, float posX, float posY, Map::CEntity * arquetipo)
	//bool CParserTile::createEntity(int id, int numcapa, std::pair<int,int> boundInf, std::pair<int,int> boundSup, int count, const std::string &name)
	{

		std::map<std::string, std::string>::const_iterator it = _atributosMap[id].begin();

		std::string name = "";
		std::string valor = "";
		for(; it != _atributosMap[id].end(); ++it)
		{
			
			 name = (*it).first;
			 valor = (*it).second;

			 //Metemos el peso en el entityInfo
			_entityInProgress->setAttribute(name,valor);
		}

		

		//dado que puedo tener proporciones no coincidentes con un tile en objetos lógicos
		//pero si quiero situarlos en una posicion tileada
		Vector3 proporcion = Vector3(Map::ANCHO_TILE, Map::ALTO_TILE, Map::PROFUNDO_TILE);

		float posZ = 0;

		if(arquetipo->hasAttribute("position"))//si tiene posicion la Z la dejamos inalterada
		{
			posZ = arquetipo->getVector3Attribute("position").z;
		}

		//Posicion del tile
		std::string pos = std::to_string(posX*proporcion.x)+','+std::to_string(posY*proporcion.y)+','+std::to_string(posZ*proporcion.z);
		_entityInProgress->setAttribute("position", pos );							//POSITION

		//Si la capa es de las entidades logicas
		if(numcapa == TipoCapa::Logic)
		{
			createLogicEntity(id, posX, posY);
		}
		//Si es de los puntos de interes
		else
		{
			if(numcapa == TipoCapa::PuntosInteres)
			{
				createInterestingPoint(id, posX, posY);
			}
			else
			{
				createAltarPoint(id, posX, posY);
			}
		}

		return true;

	}//createLogicEntity
	//------------------------------------------------------------------------------------------------

	void CParserTile::createDoorEntity(int id)
	{
		TDoorMap::const_iterator it = _doorMap.equal_range(id).first; //Iterador del primer elemento con esa clave
		TDoorMap::const_iterator end= _doorMap.equal_range(id).second;
		Vector2 posicionTrigger;
		Vector3 proporcion = Vector3(Map::ANCHO_TILE, Map::ALTO_TILE, Map::PROFUNDO_TILE);
		for(; it != end;++it, ++ultimaPos)
		{	
			//Obtenemos la posicion del trigger
			posicionTrigger = (*it).second;
			_doorList[ultimaPos][0] = capa[TipoCapa::Logic]->matriz[-posicionTrigger.y][posicionTrigger.x].second;

			//Tile 13 de ambos lados
			if(id == TipoTile::DOOR_HORIZONTAL_TRIGGER)
			{
				_doorList[ultimaPos][1] = capa[TipoCapa::Colision]->matriz[-posicionTrigger.y][posicionTrigger.x+1].second;
				_doorList[ultimaPos][2] = capa[TipoCapa::Colision]->matriz[-posicionTrigger.y][posicionTrigger.x-1].second;
			}
			//Tile 13 arriba y abajo
			else if(id == TipoTile::DOOR_VERTICAL_DER_TRIGGER)
			{
				_doorList[ultimaPos][1] = capa[TipoCapa::Colision]->matriz[-posicionTrigger.y+1][posicionTrigger.x].second;
				_doorList[ultimaPos][2] = capa[TipoCapa::Colision]->matriz[-posicionTrigger.y-1][posicionTrigger.x].second;
			}
			else if(id == TipoTile::DOOR_VERTICAL_IZQ_TRIGGER)
			{
				_doorList[ultimaPos][1] = capa[TipoCapa::Colision]->matriz[-posicionTrigger.y+1][posicionTrigger.x].second;
				_doorList[ultimaPos][2] = capa[TipoCapa::Colision]->matriz[-posicionTrigger.y-1][posicionTrigger.x].second;
			}

			//Tile 13 central
			_doorList[ultimaPos][3] = capa[TipoCapa::Colision]->matriz[-posicionTrigger.y][posicionTrigger.x].second;
		}
	}	
	//------------------------------------------------------------------------------------------------

	bool CParserTile::createLogicEntity(int id, float posX, float posY)
	{
		//Si el id es de tipo door trigger, lo insertamos en el map que representa todos los tiles que forman las puertas
		if( id == TipoTile::DOOR_HORIZONTAL_TRIGGER || id == TipoTile::DOOR_VERTICAL_DER_TRIGGER
			|| id == TipoTile::DOOR_VERTICAL_IZQ_TRIGGER)
		{
			//El map es un id del tile, junto con su posicion en tilePosicion, para posteriormente obtener asi su entityInfo de la matriz 
			_doorMap.insert(std::make_pair(id, Vector2(posX, posY)));
		}
		else
		{
			//Sino lo metemos en la lista de entidades logicas. 
			_logicEntityList.push_back(_entityInProgress);
		}

		return true;
	}

	bool CParserTile::createInterestingPoint(int id, float posX, float posY)
	{
		////Declaracion de variables que sera usadas en el for
		std::string name;
		std::string valor;
		
		std::map<std::string, std::string>::const_iterator it = _atributosMap[id].begin();

		for(; it != _atributosMap[id].end(); ++it)
		{
			if((*it).first.compare("Peso")) //caso mas probable, va primero en el if
				continue;

			 name = (*it).first;
			 valor = (*it).second;
			 break; //Si llegamos aqui es que hemos leido el atributo pesos, por lo que ya no necesitamos seguir leyendo
		}

		//Metemos el peso en el entityInfo
		_entityInProgress->setAttribute(name,valor);
		
		InterestingPointInfo ip = InterestingPointInfo(Vector3(posX*ANCHO_TILE,posY*ALTO_TILE,0), atoi(valor.c_str()));

		_interestingPoints.push_back(ip);
		
		return true;
	}

	//-------------------------------------------------------------------------------------------------

	bool CParserTile::createAltarPoint(int id, float posX, float posY)
	{	
		////Declaracion de variables que sera usadas en el for
		std::string nameradio;
		std::string valorradio;
		std::string namepeso;
		std::string valorpeso;
		
		std::map<std::string, std::string>::const_iterator it = _atributosMap[id].begin();

		for(; it != _atributosMap[id].end(); ++it)
		{
			if((*it).first.compare("peso") == 0)
			{
				namepeso = (*it).first;
				valorpeso = (*it).second;
			}
			else
			{
				if((*it).first.compare("radio") == 0)
				{
					nameradio = (*it).first;
					valorradio = (*it).second;
				}
				else
				{
					break;//Si llegamos aqui es que hemos leido los atributos peso y radio, por lo que ya no necesitamos seguir leyendo
				}
			}
		}

		//Metemos el peso en el entityInfo
		_entityInProgress->setAttribute(nameradio,valorradio);
		_entityInProgress->setAttribute(namepeso, valorpeso);
		
		AltarPointInfo ip = AltarPointInfo(Vector3(posX*ANCHO_TILE,posY*ALTO_TILE,0), atoi(valorradio.c_str()), atoi(valorpeso.c_str()));

		_altarPoints.push_back(ip);
		return true;
	}

	//------------------------------------------------------------------------------------------------

	//Funcion para borrar todas las entidades almacenadas
	void CParserTile::releaseEntityList()
	{
		for(int numCapa = 0; numCapa<capa.size();++numCapa)
		{
			for(int i=0; i< capa[numCapa]->matriz.size(); ++i)
			{
				for(int j =0; j< capa[numCapa]->matriz[i].size(); ++j)
				{
					if(capa[numCapa]->matriz[i][j].second !=NULL)
						delete capa[numCapa]->matriz[i][j].second;
				}
			}
			capa[numCapa]->matriz.clear();
		}
		_altarPoints.clear();
		_interestingPoints.clear();
		_logicEntityList.clear();
		_entityList.clear();
		_doorList.clear();
		_doorMap.clear();
		ultimaPos = 0;
	}
	//------------------------------------------------------------------------------------------------
	void CParserTile::releaseCapas()
	{
		#pragma region deleteCapas

		//Borrado de las capas del map
		std::map<int,Capa*>::iterator itCapas = capa.begin();
		std::map<int,Capa*>::iterator endCapas = capa.end();

		for(; itCapas != endCapas; ++itCapas)
		{
			delete itCapas->second;
		}

		capa.clear();
		#pragma endregion
	}
}