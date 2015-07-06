/**
@file Map.h

Contiene la declaraci�n de la clase CMap, Un mapa l�gico.

@see Logic::Map

@author David Llans�
@date Agosto, 2010
*/
#ifndef __Logic_Map_H
#define __Logic_Map_H

#include <map>
#include <vector>
#include "EntityID.h"
#include "BaseSubsystems/Math.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
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

// Declaraci�n de la clase
namespace Logic
{
	/**
	Clase que representa un mapa l�gico.
	<p>
	Esta clase se encarga de almacenar todas las entidades del mapa, tanto 
	aquellas que tienen representaci�n gr�fica y se ven como entidades
	puramente l�gicas. Gestiona la activaci�n y desactivaci�n de �stas y
	tiene tambi�n m�todos para buscar entidades, tanto por su nombre como 
	por su tipo y por su identificador.

	@ingroup logicGroup
	@ingroup mapGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CMap
	{
	public:
		/**
		M�todo factor�a que carga un mapa de fichero. Tras el parseo de
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
		Activa el mapa. Invocar� al m�todo activate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		 
		@return Devuelve true al invocador si todas las entidades se
		activaron sin problemas.
		*/
		bool activate();

		/**
		Desactiva el mapa. Invocar� al m�todo deactivate() de todas las 
		entidades para que se den por enterados y hagan lo que necesiten.
		*/
		void deactivate();

		/**
		Funci�n llamada en cada frame para que se realicen las funciones
		de actualizaci�n adecuadas.
		<p>
		Llamar� a los m�todos tick() de todas las entidades.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		void tick(unsigned int msecs);

		/**
		A�ade una nueva entidad al mapa. Si la entidad ya estaba incluida
		no se hace nada.

		@param entity Entidad a a�adir.
		*/
		void addEntity(CEntity *entity);

		/**
		Elimina una entidad del mapa. Si la entidad no estaba incluida
		no se hace nada. La funci�n desactiva previamente la entidad si
		�sta se encontraba activa.

		@note El mapa no se hace responsable de eliminar (con delete) la
		entidad.

		@param entity Entidad a eliminar.
		*/
		void removeEntity(CEntity *entity);

		/**
		Elimina y destruye todas las entidades del mapa dejando la lista 
		de entidades vac�a.
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
		en la lista. �til si se tienen varias entidades con el mismo
		nombre y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByName(const std::string &name, CEntity *start = 0);

		/**
		Recupera una entidad del mapa a partir de su tipo.

		@param type nombre del tipo de la entidad a recuperar.
		@param start Entidad desde la que se debe empezar a buscar
		en la lista. �til si se tienen varias entidades del mismo tipo
		y desde fuera se quiere ir accediendo a ellas una a una.
		Si no se especifica se empieza desde el principio.
		@return Entidad con el nombre pedido, NULL si no se encuentra.
		*/
		CEntity *getEntityByType(const std::string &type, CEntity *start = 0);

		/**
		Devuelve la escena gr�fica correspondiente a este mapa.

		@return Escena con las entidades gr�ficas.
		*/
		Graphics::CScene *getScene() {return _scene;}

		bool activarEntidad(CEntity *entity);

		/*
		Manda una notificaci�n de cambio de arquetipo al mapa, para que la siguiente vez que mande entidades al pool vac�e el pool.
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
		M�todo que devuelve el ID del tile de la capa de colisi�n en la tile posici�n indicada

		@param x Posici�n horizontal del tile en TilePosicion
		@param y Posicion vertical del tile en TilePosicion

		@return ID del tile en dicha posicion, 0 si no hay nada
		*/
		int getTileColisionTP(int x, int y);

		/*
		M�todo que devuelve el ID del tile de la capa de colisi�n en la posici�n del mundo indicada

		@param x Posici�n horizontal del tile en posicion mundo
		@param y Posicion vertical del tile en posicion mundo

		@return ID del tile en dicha posicion, 0 si no hay nada
		*/
		int getTileColision(Vector3 &pos);

		/*
		Este m�todo devuelve la anchura del mapa en tiles

		@return anchura en tiles del mapa
		*/
		int getMapTileWidth();

		/*
		Este m�todo devuelve la altura del mapa en tiles

		@return altura en tiles del mapa
		*/
		int getMapTileHeight();

	private:

		/*
		Crea la entidad gr�fica asociada a un tile a partir de la informaci�n construida en el parser
		y la a�ade a la lista de entidades graficas.

		@param entityInfo Informaci�n a partir de la cual se crea la entidad gr�fica
		@return true si se ha creado correctamente, false en caso contrario
		*/
		bool createTileGraphicEntity(Map::CEntity* entityInfo);


		/*
		Crea el actor f�sico asociada a un tile a partir de la informaci�n construida en el parser
		y la a�ade a la lista de actores f�sicos

		@param entityInfo Informaci�n a partir del cual se crea el actor f�sico
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
		Este metodo agrupa un grupo de tiles f�sicos en uno solo, reescalando apropiadamente

		@param grupo Lista de entityInfos de todos los tiles a agrupar
		@return entityInfo �nico a partir del grupo aportado.
		*/

		Map::CEntity agrupaTileFisico(std::vector<Map::CEntity*> grupo);

		/*
		Setea la configuraci�n global de la escena a partir de informacion parseada del mapa

		@param sceneConfiguration Es un entityInfo que contiene la informaci�on de la configuraci�n
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
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades.
		*/
		Graphics::CScene* _scene;


		/*
		Lista de las entidades gr�ficos que conforman los tiles est�ticos
		*/
		std::vector<Graphics::CEntity*> _graphicEntities;

		/*
		Lista de los actores f�sicos que conforman los tiles est�ticos
		*/
		std::vector<Physics::CActor*> _physicActors;

		bool _mapaCreado;

		//bool _archetypeChanged;

	}; // class CMap

} // namespace Logic

#endif // __Logic_Map_H
