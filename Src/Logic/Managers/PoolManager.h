/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __PoolManager_H
#define __PoolManager_H

#include <string>
#include <map>
#include <list>
#include "Logic/Maps/EntityID.h"
#include "Logic/Maps/Map.h"
#include "BaseSubsystems/Math.h"

namespace Map
{
	class CEntity;
}

namespace Logic
{
	class CEntity;
}
namespace Logic
{
	class CPoolManager
	{
	private:
		typedef std::map<std::string, std::list<CEntity*>> TColaMultiMap;
	public:
		/**
		Devuelve la única instancia de la clase CPoolManager.
		
		@return Única instancia de la clase CPoolManager.
		*/
		static CPoolManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CPoolManager. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		/**
		Metodo usado por el spawner para crear una nueva entidad si no hay ninguna entidad dormida del tipo deseado
		o despertar alguna entidad dormida del tipo deseado. 
		
		@param type tipo de entidad que se desea spawnear
		@param map map a donde se quiere spawnear la entidad
		@param posicion posicion en donde se quiere spawnear la entidad
		**/
		CEntity * instanciate(Map::CEntity *entityInfo);

		
		
		/**
		Metodo para meter una entidad en la cola
		**/
		void addEntityDeactivateToPool(CEntity *_entity);

	private:
		friend class Logic::CMap;
		/**
		Metodo para destruir la cola de entidades al destruir el mapa. Es privado dado que solo lo usaremos en el release del spawner
		**/
		void destroyColaActual();

		/**
		Única instancia de la clase.
		*/
		static CPoolManager* _instance;
		
		//CMap *_map;
		

		//cola de entidades divididas por tipo
		TColaMultiMap _colaEntidades;

	protected:

		/**
		Constructor.
		*/
		CPoolManager ();

		/**
		Destructor.
		*/
		~CPoolManager();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();
	};
}

#endif