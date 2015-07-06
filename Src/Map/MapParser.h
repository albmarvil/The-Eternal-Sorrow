/**
@file MapParser.h

Contiene la declaraci�n de la clase que encapsula el parseo de mapas.

@see Map::CMapParser

@author David Llans� Garc�a
@date Agosto, 2010
*/


#ifndef __MapParser_H
#define __MapParser_H

#include <string>
#include <list>

// Predeclaraci�n de clases.
namespace Map
{
	class CEntity;
	class CParser;
	class CScanner;
}

namespace LUA
{
	class CFunctionsToLUA;
}


/**

@author David Llans�
@date Agosto, 2010
*/

namespace Map {
/*

	@ingroup mapParserGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CMapParser
	{
	public:

		/**
		Devuelve la �nica instancia de la clase CMapParser.
		
		@return �nica instancia de la clase CMapParser.
		*/
		static CMapParser* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la factor�a. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();


		/** 
		Metodo que parsea un fichero de luA
		
		@param filename nombre del fichero con los datos del mapa.
		@param name Nombre del stream para mensajes de error.
		@return true si el parseo fue satisfactorio.
		*/
		bool parseFile(const std::string& filename);

		/**
		Tipo lista de entidades de mapa.
		*/
		typedef std::list<Map::CEntity*> TEntityList;

		/**
		Devuelve el listado de las entidades leidas del mapa.

		@return Entidades parseadas.
		*/
		const TEntityList getEntityList() {return _entityList;}

		/**
		Elimina todas las entidades que contenga la lista de entidades 
		parseadas.
		*/
		void releaseEntityList();

		void addEntityInfo(CEntity* entityInfo);

	private:
		/**
		�nica instancia de la clase.
		*/
		static CMapParser* _instance;

		/**
		Constructor.
		*/
		CMapParser();

		/**
		Destructor.
		*/
		~CMapParser();

		/**
		Entidad temporal que se encuentra en medio de su parseo. Es usado
		por la clase CParser para ir generando la lista de entidades.
		*/
		Map::CEntity *_entityInProgress;

		/**
		Lista de entidades parseada.
		*/
		std::list<Map::CEntity*> _entityList;
	};

} // namespace Map

#endif // __MapParser_H
