/**
@author Cesar Manuel Paz Guzman
@date Enero, 2015
*/

#ifndef PARSERTILE_H
#define PARSERTILE_H 

#include <string>
#include <vector> 
#include <map>
#include <list>


#include "TipoTile.h"
#include "BaseSubsystems/Math.h"

namespace Map
{
	class CEntity;
}
/*Este .h contiene 2 clases. La mas importante es la CParserTile que es donde se realiza el parser. 
Las otras corresponden con cada una de las "secciones" del xml. Lo hago asi para que sea facilmente escalable, por si
hay algun atributo mas en el mapa y hay que ponerlo aqui, o si tenemos que añadir alguna otra cosa.

Las 2 clases que corresponden las secciones contiene sus constructores y destructores. Un conjunto de getters y declaracion
de la clase CParserTile como amiga, para mantener la encapsulacion y consistencia de datos. 
*/

namespace Map
{


	/**
	Esta clase define una capa del fichero proveniente del editor de Tiles.
	La capa tiene entres sus atributos el nombre y la matriz con los IDs
	parseados del editor.
	*/
	class Capa{
	
	public:
		/*
		Definición del tipo Matriz que guarda la capa
		*/
		typedef std::vector<std::vector<std::pair<int,Map::CEntity*>>> Matriz;

		/*
		Constructor de la capa
		*/
		Capa(){}

		/*
		Destructor de la capa
		*/
		~Capa(){}

		/*
		Declaración de la clase amiga del parser
		*/
		friend class CParserTile;

		/*
		Obtiene el nombre de la capa

		@return Nombre de la capa
		*/
		std::string getName() const { return name;}


		/*
		Obtiene la matriz de IDs de los tiles de dicha capa

		@return Matriz de IDs de los tiles de la capa
		*/
		Matriz getMatrizIDs() const { return matriz;}

	protected:

		/*
		Nombre de la capa
		*/
		std::string name;

		/*
		Matriz de IDs de la capa
		*/
		Matriz matriz;
		
		
	};

	class InterestingPointInfo
	{
	private:
		Vector3 posIP;
		int weightIP;

	public:
		InterestingPointInfo(Vector3 pos, int weight) : posIP(pos), weightIP(weight)
		{
		}

		Vector3 GetPos() const {return posIP;}
		int GetWeight() const{return weightIP;}
	};

	class AltarPointInfo
	{
	private:
		Vector3 posIP;
		float radioIP;
		int weightIP;

	public:
		AltarPointInfo(Vector3 pos, float radio, int weight) : posIP(pos), radioIP(radio), weightIP(weight)
		{
		}

		Vector3 GetPos() const {return posIP;}
		float GetRadio() const {return radioIP;}
		int GetWeight() const {return weightIP;}
	};

	/*
	Clase que parsea el XML con la informacion de los tiles.
	Esta clase es un singleton, y si se solicita varios parseos sucesivos se va sobreescribiendo la información contenida 
	por la misma
	*/
	class CParserTile
	{
	public:
		/*
		Lista que contiene los entityInfo
		*/
		typedef std::list<Map::CEntity*> TEntityList;

		/*
		Matriz que contiene entityInfos
		*/
		typedef std::vector<std::vector<Map::CEntity*>> TMatriz;

		typedef std::vector<TMatriz> TMatrizMatriz;

		typedef std::map<int, std::map<std::string, std::string>> TAtributosMap;

		//typedef std::map<int, std::vector<CEntity*>> TInterestingPoints;
		//typedef std::vector<InterestingPointInfo> TInterestingPointVec;

		/**
		Devuelve el listado de las entidades leidas del mapa.

		@return Entidades parseadas.
		*/
		TMatrizMatriz getEntityList() const {return _entityList;}

		/*
		Devuelve el listado de las entidades leidas en la capa de logica del mapa

		@return Entidades parseadas de la capa de logica
		*/
		const TEntityList getLogicEntityList() const {return _logicEntityList; }

		/**
		Elimina todas las entidades que contenga la lista de entidades 
		parseadas.
		*/
		void releaseEntityList();

		/**
		Elimina todas las entidades que contenga la lista de entidades 
		parseadas.

		@return Instancia única de la clase
		*/
		static CParserTile* getSingletonPtr() {return _instance;}

		static bool Init();

		static void Release();


		/*
		Funcion que parsea el fichero que se le ha mandado.

		@param filename Nombre del fichero desde el cual se va a parsear la informacion

		@return True si el fichero se ha parseado correctamente
		*/
		bool parserFile(const std::string &filename);

		/*
		Devuelve la capa indicada por la entrada

		@param numeroCapa Numero de la capa a la que se accede

		@return Puntero a la capa solicitada
		*/
		Capa* getCapa(int numeroCapa){ return capa[numeroCapa];}


		/*
		Elimina las capas almacenadas en el parser
		*/
		void releaseCapas();

		//Crea la entidad que va a representar al Tile. Debemos tener leida previamente el fichero map.txt, para obtener
		//la informacion correspondente al type = "Tile"

		/*
		Crea la entidad que va a representar al Tile. Esta entidad llevará la información 
		para que el tile se pinte en la posicion correcta. Cada capa corresponderá con un 
		arquetipo diferente

		@param id ID del tipo de tile a crear
		@param capa Identificador de la capa a crear
		@param posX Posicion X del tile
		@param posY Posicion Y del tile
		@param name nombre del tile
		*/
		bool createEntity(int id, TipoCapa capa, float posX, float posY, const std::string &name);

		/*
		Crea la entidad que va a representar al Tile. Esta entidad llevará la información 
		para que el tile se pinte en la posicion correcta. Cada capa corresponderá con un 
		arquetipo diferente

		@param id ID del tipo de tile a crear
		@param capa Identificador de la capa a crear
		@param posX Posicion X del tile
		@param posY Posicion Y del tile
		@param arquetipo arquetipo correspondiente al tile
		*/
		bool createTileEntity(int id, TipoCapa capa, float posX, float posY, Map::CEntity * arquetipo);

		/*
		Crea la entidad logica que esta marcada en dicho tile. Transformará su TilePosicion a 
		posicion real del mundo (la pos en Z no la toca), pero el resto de atributos no los tocará.

		@param id ID del tipo de tile a crear
		@param capa Identificador de la capa a crear
		@param posX Posicion X del tile
		@param posY Posicion Y del tile
		@param name arquetipo arquetipo correspondiente al tile
		*/
		bool logicsEntities(int id, TipoCapa capa, float posX, float posY, Map::CEntity * arquetipo);

		/*
		Mira que tiles 13 son adyacentes y Crea una puerta con los tiles que la conforman agrupados.
		@param id del trigger que forma la puerta. Se buscara los tiles 13 adyacentes a este trigger
		*/
		void createDoorEntity(int id);
		
		/*
		Crea las entidades logicas 
		@param id ID del tipo de tile a crear
		@param posX Posicion X del tile
		@param posY Posicion Y del tile
		*/
		bool createLogicEntity(int id, float posX, float posY);

		/*
		Crea los puntos de interes
		@param id ID del tipo punto de interes a crear
		*/
		bool createInterestingPoint(int id, float posX, float posY);

		/*
		Crea los putnos de interés para colocar los altares de sacrificio
		@param id ID del tipo punto de interes a crear
		*/
		bool createAltarPoint(int id, float posX, float posY);

		/*
		Devuelve la matriz que contiene las puertas con sus tiles.
		**/
		TMatriz getDoors(){ return _doorList;}

		/*
		Devuelve una matriz de puntos de interes. 
		*/
		std::vector<InterestingPointInfo> getInterestingPoints(){ return _interestingPoints;}

		/*
		Devuelve una matriz de puntos de altar
		*/
		std::vector<AltarPointInfo> getAltarPoints(){return _altarPoints;}

	protected:

		//El atributo capa sera el usado para obtener el id del tile, y su posicion. 
		std::map<int,Capa*> capa; //Map con las capas y su contenido 


	private://Estos atributos los marco como privado ya que no sera un atributo que defina una caracteristica externa del parser, ya que esta incluido en uno protegido

		typedef std::multimap<int,Vector2> TDoorMap;

		static CParserTile* _instance;

		//El constructor recibe como parametro el nombre del fichero a parsear. Como norma declaro también un constructor por defecto
		CParserTile();

		virtual ~CParserTile();

		/*
		Atributos correspondiente a cada tile
		**/
		TAtributosMap _atributosMap;

		//Entidad en progreso de ser añadida a la lista de entidades tile
		CEntity *_entityInProgress;

		TMatrizMatriz _entityList;

		/*
		Lista de entidades de la capa de logica
		*/
		TEntityList _logicEntityList;

		/**
		Matriz que representa los puntos de interes. Por ahora sera de la siguiente forma:
		*/
		std::vector<InterestingPointInfo> _interestingPoints;

		std::vector<AltarPointInfo> _altarPoints;
		

		//Matriz donde se guardaran los tiles correspondientes a las puertas
		TMatriz _doorList;

		//Atributos que solo usare como comodines para encontrar cuales son los tiles door adyacentes.
		TDoorMap _doorMap;
		int ultimaPos;
	};

}
#endif