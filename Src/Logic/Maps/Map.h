/**
@file Map.h

Contiene la declaración de la clase CMap, Un mapa lógico.

@see Logic::Map

@author David Llansó
@date Agosto, 2010
*/
#ifndef __Logic_Map_H
#define __Logic_Map_H

#include <map>
#include <vector>
#include "EntityID.h"
#include "BaseSubsystems/Math.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Map
{
	class CEntity;
}
namespace Logic 
{
	class CEntity;
}

namespace Graphics 
{
	class CScene;
	class CEntity;
}

namespace Physics
{
	class CActor;
}

// Declaración de la clase
namespace Logic
{
	/**
	Clase que representa un mapa lógico.
	<p>
	Esta clase se encarga de almacenar todas las entidades del mapa, tanto 
	aquellas que tienen representación gráfica y se ven como entidades
	puramente lógicas. Gestiona la activación y desactivación de éstas y
	tiene también métodos para buscar entidades, tanto por su nombre como 
	por su tipo y por su identificador.

	@ingroup logicGroup
	@ingroup mapGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CMap
	{
	public:
		/**
		Método factoría que carga un mapa de fichero. Tras el parseo de
		todas las entidades del mapa mediante CMapParser, genera todas las
		entidades con CEntityFactory.

		@param filename Nombre del archivo a cargar.
		@return Mapa generado.
		*/
		static CMap *createMapFromFile(const std::string &filename);

		/**
		Constructor.

		@param name Nombre que se le da a este mapa.
		*/
		CMap (const std::string &name);

		/**
		Destructor.
		*/
		~CMap();
		
		/**
		Activa el mapa. Invocará al método activate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true al invocador si todas las entidades se
		activaron sin problemas.
		*/
		bool activate();

		/**
		Desactiva el mapa. Invocará al método deactivate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Función llamada en cada frame para que se realicen las funciones
		de actualización adecuadas.
		<p>
		Llamará a los métodos tick() de todas las entidades.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		void tick(unsigned int msecs);

		/**
		Añade una nueva entidad al mapa. Si la entidad ya estaba incluida
		no se hace nada.

		@param entity Entidad a añadir.
		*/
		void addEntity(CEntity *entity);

		/**
		Elimina una entidad del mapa. Si la entidad no estaba incluida
		no se hace nada. La función desactiva previamente la entidad si
		ésta se encontraba activa.

		@note El mapa no se hace responsable de eliminar (con delete) la
		entidad.

		@param entity Entidad a eliminar.
		*/
		void removeEntity(CEntity *entity);

		/**
		Elimina y destruye todas las entidades del mapa dejando la lista 
		de entidades vacía.
		*/
		void destroyAllEntities();

		/**
		Recupera una entidad del mapa a partir de su ID.

		@param entityID ID de la entidad a recuperar.
		@return Entidad con el ID pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByID(TEntityID entityID);

		/**
		Recupera una entidad del mapa a partir de su nombre.

		@param name nombre de la entidad a recuperar.
		@param start Entidad desde la que se debe empezar a buscar
		en la lista. Útil si se tienen varias entidades con el mismo
		nombre y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByName(const std::string &name, CEntity *start = 0);

		/**
		Recupera una entidad del mapa a partir de su tipo.

		@param type nombre del tipo de la entidad a recuperar.
		@param start Entidad desde la que se debe empezar a buscar
		en la lista. Útil si se tienen varias entidades del mismo tipo
		y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByType(const std::string &type, CEntity *start = 0);

		/**
		Devuelve la escena gráfica correspondiente a este mapa.

		@return Escena con las entidades gráficas.
		*/
		Graphics::CScene *getScene() {return _scene;}

		bool activarEntidad(CEntity *entity);

		/*
		Manda una notificación de cambio de arquetipo al mapa, para que la siguiente vez que mande entidades al pool vacíe el pool.
		*/
		void archetypeChangeNotification ();

		/*
		Metodo que me devuelve un vector con los entityInfo escogidos para poner en los puntos tambien escogidos

		@param puntos Es el vector de todos los puntos en los que se puede colocar el objeto
		@param numMaxObjectos Es el numero maximo de ese tipo de objetos que podran aparecer en el mapa. 
		@param tipoPoint Es el tipo de objeto sobre el que se esta tratando

		@return Vector con los entityInfo escogidos segun la posicion escogida partiendo de los puntos de interes. 
		*/
		//std::vector<Map::CEntity*> eleccionPuntos(std::vector<Map::CEntity*> puntos, int numMaxObjetos, const std::string &tipoPoint);


		/*
		Método que devuelve el ID del tile de la capa de colisión en la tile posición indicada

		@param x Posición horizontal del tile en TilePosicion
		@param y Posicion vertical del tile en TilePosicion

		@return ID del tile en dicha posicion, 0 si no hay nada
		*/
		int getTileColisionTP(int x, int y);

		/*
		Método que devuelve el ID del tile de la capa de colisión en la posición del mundo indicada

		@param x Posición horizontal del tile en posicion mundo
		@param y Posicion vertical del tile en posicion mundo

		@return ID del tile en dicha posicion, 0 si no hay nada
		*/
		int getTileColision(Vector3 &pos);

		/*
		Este método devuelve la anchura del mapa en tiles

		@return anchura en tiles del mapa
		*/
		int getMapTileWidth();

		/*
		Este método devuelve la altura del mapa en tiles

		@return altura en tiles del mapa
		*/
		int getMapTileHeight();

	private:

		/*
		Crea la entidad gráfica asociada a un tile a partir de la información construida en el parser
		y la añade a la lista de entidades graficas.

		@param entityInfo Información a partir de la cual se crea la entidad gráfica
		@return true si se ha creado correctamente, false en caso contrario
		*/
		bool createTileGraphicEntity(Map::CEntity* entityInfo);


		/*
		Crea el actor físico asociada a un tile a partir de la información construida en el parser
		y la añade a la lista de actores físicos

		@param entityInfo Información a partir del cual se crea el actor físico
		@return true si se ha creado correctamente, false en caso contrario
		*/

		bool createTilePhysicEntity(Map::CEntity* entityInfo);

		/*
		Este predicado nos dice si dos entityInfo se corresponden a dos tiles fisicos contiguos
		(estan en la misma fila)

		@param anterior entityInfo del primer tile
		@param actual  entityInfo del segundo tile

		@return True si ambos tiles son contiguos, False en caso contrario
		*/
		bool tileFisicoContiguo(Map::CEntity* anterior, Map::CEntity* actual);

		/*
		Este metodo agrupa un grupo de tiles físicos en uno solo, reescalando apropiadamente

		@param grupo Lista de entityInfos de todos los tiles a agrupar
		@return entityInfo único a partir del grupo aportado.
		*/

		Map::CEntity agrupaTileFisico(std::vector<Map::CEntity*> grupo);

		/*
		Setea la configuración global de la escena a partir de informacion parseada del mapa

		@param sceneConfiguration Es un entityInfo que contiene la informaciñon de la configuración
		*/
		void setSceneConfiguration(Map::CEntity* sceneConfiguration);

		/**
		Tipo tabla de entidades de mapa.
		*/
		typedef std::map<TEntityID,CEntity*> TEntityMap;

		/**
		tabla con las entidades activas del mapa localizadas por su ID.
		*/
		TEntityMap _entityMap;

		/**
		Nombre del mapa.
		*/
		std::string _name;

		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades.
		*/
		Graphics::CScene* _scene;


		/*
		Lista de las entidades gráficos que conforman los tiles estáticos
		*/
		std::vector<Graphics::CEntity*> _graphicEntities;

		/*
		Lista de los actores físicos que conforman los tiles estáticos
		*/
		std::vector<Physics::CActor*> _physicActors;

		bool _mapaCreado;

		//bool _archetypeChanged;

	}; // class CMap

} // namespace Logic

#endif // __Logic_Map_H
