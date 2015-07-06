/**
@file MapManager.h

Contiene la declaraci�n de la clase MapManager, Singleton que se encarga de
la gesti�n de la carga de mapas y ambientaciones.

@see Logic::MapManager

@author Alberto Mart�nez
@date Agosto, 2015
*/
#ifndef __Logic_MapManager_H
#define __Logic_MapManager_H

#include <string>


namespace Logic
{
	class CServer;

	/*
	Este enumerado contiene los tipos de ambientaciones. Cada una se corresponder� con un arquetipo diferente
	*/

	enum Ambientation
	{
		CASTILLO = 0,
		LAVA = 3,
		SNOW = 2,
		MAZMORRA = 1
	};

	/**
	El manager de mapas permite cargar desde cualquier parte de la l�gica un mapa y una nueva ambientaci�n.
	Tiene m�todos espec�ficos para la carga del primer nivel y el arquetipo inicial

	@ingroup logicGroup

	@author Alberto Mart�nez
	@date Agosto, 2015
	*/
	class MapManager
	{
	public:

		/**
		Devuelve la �nica instancia de la clase MapManager.
		
		@return �nica instancia de la clase MapManager.
		*/
		static MapManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		static bool InitMenu();

		/**
		Libera la instancia de MapManager. Debe llamarse al finalizar la 
		partida.
		*/
		static void Release();

		/*
		Tick de juego.
		durante el tick el MapManager har� todas las tareas que asignadas durante el tick anterior

		@param msecs Incremento de tiempo respecto al tick anterior
		*/
		void tick(unsigned int msecs);

		/*
		Anota para el siguiente tick el calculo y cargado del siguiente mapa
		*/
		void nextMap();

		/*
		Anota para el siguiente tick el cargado de una nueva ambientacion(arquetipo)
		@param ambientacion Ambientacion que queremos cargar
		*/
		void nextAmbientation (Ambientation ambientacion);
		
	protected:

		/*
		Este m�todo seleccionar� una nueva plantilla usando un sistema probabil�stico ponderado.
		Una vez elegida la plantilla se cargar� y har� uso de la ambientaci�n cargada
		*/
		void changeMap();

		/*
		Este m�todo sirve para cargar una nueva ambientaci�n. Cargando el arquetipo correspondiente.
		Solo cargar� algo nuevo si la ambientaci�n nueva es diferente

		@param ambientacion Ambientaci�n destino
		*/
		void changeAmbientation(Ambientation ambientacion);

		/*
		Carga el mapa inicial y el arquetipo inicial del juego
		*/
		void loadInitialLevel();

		/*
		Carga y construye un mapa
		@param mapName Nombre del mapa a cargar
		*/
		void loadLevelResources(std::string mapName);

		/*
		Descarga los recursos del mapa cargado
		*/
		void unloadLevelResources();


		/**
		Constructor.
		*/
		MapManager ();

		/**
		Destructor.
		*/
		~MapManager();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		est�ticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		bool openMenu();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();

		/**
		Nombre de mapa a cargar.
		*/
		std::string _currentMapName;

		/**
		Ambientacion actual (arquetipo)
		*/
		Ambientation _currentAmbientation;

		/**
		Ambientacion actual (arquetipo)
		*/
		Ambientation _nextAmbientation;

		/*
		Flag que indica la carga de un mapa en el siguiente tick
		*/
		bool _loadMap;

		/*
		Flag que indica la carga de una ambientacon en el siguiente tick
		*/
		bool _loadAmbientation;

	private:
		/**
		�nica instancia de la clase.
		*/
		static MapManager* _instance;

	}; // class MapManager

} // namespace Logic

#endif // __Logic_MapManager_H
