/**
@file MapManager.cpp

Contiene la implementación de la clase MapManager, Singleton que se encarga de
la gestión de la carga de mapas y ambientaciones.

@see Logic::CServer

@author David Llansó
@date Agosto, 2010
*/

#include "MapManager.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"

#include "Physics/Server.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

#include "LUA\ScriptManager.h"

#include "Graphics/Server.h"
#include "Graphics/Transition.h"
#include "Application/Clock.h"
#include "Application/OgreClock.h"
#include "Application\BaseApplication.h"

#include <cassert>
#include "BaseSubsystems/ConfigurationManager.h"
#include "BaseSubsystems/RandomUtils.h"

namespace Logic {

	MapManager* MapManager::_instance = 0;

	//--------------------------------------------------------

	MapManager::MapManager()
	{
		_instance = this;
		_loadAmbientation = false;
		_loadMap = false;

	} // MapManager

	//--------------------------------------------------------

	MapManager::~MapManager()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool MapManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::MapManager no permitida!");

		new MapManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------


	bool MapManager::InitMenu()
	{
		assert(!_instance && "Segunda inicialización de Logic::MapManager no permitida!");

		new MapManager();

		if (!_instance->openMenu())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void MapManager::Release()
	{
		assert(_instance && "Logic::MapManager no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool MapManager::open()
	{
		//segunda fase de inicializacion
		//El init/open del Map manager se hace al iniciar una partida
		//Aqui deberíamos cargar la primera pantalla

		

		loadInitialLevel();

		return true;

	} // open

	//--------------------------------------------------------


	bool MapManager::openMenu()
	{
		_currentMapName = "Menu";
		
		_currentAmbientation = Ambientation::CASTILLO;
		//cargamos la ambientación
		Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypesGeneral.lua");
		//int rnd =  BaseSubsystems::RandomUtils::RandomRange(0, 2);
		////BaseSubsystems::Log::Debug("RND: " + std::to_string(rnd));
		//switch (rnd)
		//{
		//case 0:
		//	changeAmbientation(Ambientation::CASTILLO);
		//	break;
		//case 1:
		//	changeAmbientation(Ambientation::MAZMORRA);
		//	break;
		//case 2:
		//	changeAmbientation(Ambientation::LAVA);
		//	break;
		//}

		changeAmbientation(Ambientation::CASTILLO);

		#pragma region  Transicion de salida de mapa

		Graphics::CFadeInOutTransition actualTransition = Graphics::CFadeInOutTransition(800.0f, Graphics::TipoEfecto::SALIDA);
		actualTransition.activateTransitionEffect(Graphics::TipoEfecto::SALIDA);

		unsigned int deltaTime = 0;
		Application::COgreClock clock = Application::COgreClock();
		clock.updateTime();
		while (!actualTransition.isEffectFinished())
		{
			clock.updateTime();
			deltaTime = clock.getLastFrameDuration();

			actualTransition.tick(deltaTime);

			Graphics::CServer::getSingletonPtr()->tick(deltaTime*0.001);
		}

		actualTransition.deactivateTransitionEffect(Graphics::TipoEfecto::SALIDA);

		#pragma endregion

			
		// Cargamos el nivel a partir del nombre del mapa. 
		//Si ya hay algun mapa cargado el se encarga de descargarlo
		Logic::CServer::getSingletonPtr()->loadLevel(_currentMapName);

		// Activamos el mapa que ha sido cargado para la partida.
		Logic::CServer::getSingletonPtr()->activateMap();

		//hacemos un tick al server logico
		clock.updateTime();
		deltaTime = clock.getLastFrameDuration();
		Logic::CServer::getSingletonPtr()->getMap()->tick(deltaTime);

		#pragma region  Transicion de entrada de mapa
		actualTransition.activateTransitionEffect(Graphics::TipoEfecto::ENTRADA);

		clock.updateTime();
		clock.updateTime();
		while (!actualTransition.isEffectFinished())
		{
			clock.updateTime();
			deltaTime = clock.getLastFrameDuration();

			actualTransition.tick(deltaTime);

			Graphics::CServer::getSingletonPtr()->tick(deltaTime*0.001);
		}

		actualTransition.deactivateTransitionEffect(Graphics::TipoEfecto::ENTRADA);


		#pragma endregion


		//Seteamos el reloj
		Application::CBaseApplication::getSingletonPtr()->updateTime();

		return true;

	} // open

	//--------------------------------------------------------

	void MapManager::close() 
	{
		//liberamos recursos del mapa actual
		unloadLevelResources();
		//liberamos los arquetipos
		Logic::CEntityFactory::getSingletonPtr()->unloadArchetypes();

	} // close

	//--------------------------------------------------------

	void MapManager::loadInitialLevel()
	{	
		_currentAmbientation = Ambientation::CASTILLO;

		//cargamos la ambientación
		Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypesGeneral.lua");

		#pragma region  Transicion de salida de mapa

		Graphics::CFadeInOutTransition actualTransition = Graphics::CFadeInOutTransition(800.0f, Graphics::TipoEfecto::SALIDA);
		actualTransition.activateTransitionEffect(Graphics::TipoEfecto::SALIDA);

		unsigned int deltaTime = 0;
		Application::COgreClock clock = Application::COgreClock();
		clock.updateTime();
		while (!actualTransition.isEffectFinished())
		{
			clock.updateTime();
			deltaTime = clock.getLastFrameDuration();

			actualTransition.tick(deltaTime);

			Graphics::CServer::getSingletonPtr()->tick(deltaTime*0.001);
		}

		actualTransition.deactivateTransitionEffect(Graphics::TipoEfecto::SALIDA);

		#pragma endregion

			
		//cargamos el mapa incial
		loadLevelResources("TutoNuevo");

		//hacemos un tick al server logico
		clock.updateTime();
		deltaTime = clock.getLastFrameDuration();
		Logic::CServer::getSingletonPtr()->getMap()->tick(deltaTime);

		#pragma region  Transicion de entrada de mapa
		actualTransition.activateTransitionEffect(Graphics::TipoEfecto::ENTRADA);

		clock.updateTime();
		clock.updateTime();
		while (!actualTransition.isEffectFinished())
		{
			clock.updateTime();
			deltaTime = clock.getLastFrameDuration();

			actualTransition.tick(deltaTime);

			Graphics::CServer::getSingletonPtr()->tick(deltaTime*0.001);
		}

		actualTransition.deactivateTransitionEffect(Graphics::TipoEfecto::ENTRADA);


		#pragma endregion

		//Seteamos el reloj
		Application::CBaseApplication::getSingletonPtr()->updateTime();
	}

	//--------------------------------------------------------


	void MapManager::loadLevelResources(std::string mapName)
	{
		_currentMapName = mapName;

		//reseteamos los valores del spawner manager
		//cargamos un reto para la habitación, siempre y cuando no sea la sala inicial
		/*if(_currentMapName.compare("Inicial") != 0)
		{
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("SpawnerMgr", "ResetValues");
		}*/

		// Cargamos el nivel a partir del nombre del mapa. 
		//Si ya hay algun mapa cargado el se encarga de descargarlo
		Logic::CServer::getSingletonPtr()->loadLevel(_currentMapName);

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "LoadReto");


		// Activamos el mapa que ha sido cargado para la partida.
		Logic::CServer::getSingletonPtr()->activateMap();

		//actualizamos los valores del GameMgr
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "InitValues");

		

		//Inicializamos las estadísticas del personaje
		/*if(_currentMapName.compare("Inicial") == 0)
		{
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "InitStatsPj");
		}*/
		
	}

	//--------------------------------------------------------


	void MapManager::unloadLevelResources()
	{
		Logic::CServer::getSingletonPtr()->unLoadLevel();
	}

	//--------------------------------------------------------


	void MapManager::changeAmbientation(Ambientation ambientacion)
	{
		if(ambientacion != _currentAmbientation)
		{
			_currentAmbientation = ambientacion;
			std::string archetype;
			switch (_currentAmbientation)
			{
			case Ambientation::CASTILLO:
				archetype = "archetypesGeneral.lua";
				break;
			case Ambientation::LAVA:
				archetype = "archetypesLava.lua";
				break;
			case Ambientation::SNOW:
				archetype = "archetypesSnow.lua";
				break;
			case Ambientation::MAZMORRA:
				archetype = "archetypesDungeon.lua";
				break;
			default:
				archetype = "archetypesDefaultAmbientation.lua";
				break;
			}

			//Logic::CServer::getSingletonPtr()->getMap()->archetypeChangeNotification();
			Logic::CEntityFactory::getSingletonPtr()->loadArchetypes(archetype);
		}
		
	}
	//--------------------------------------------------------

	void MapManager::changeMap()
	{
		//sacamos el nombre del mapa i-ésimo
		std::string mapName = "";
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("FuncionesMapList","escogerMapa",mapName,0); 
		//lo cargamos
		loadLevelResources(mapName);

	}
	//--------------------------------------------------------

	void MapManager::nextMap()
	{
		LUA::CScriptManager::GetPtrSingleton()->executeScript("HUD.setVisible(false)");
			
		_loadMap = true;
	}

	//--------------------------------------------------------

	void MapManager::nextAmbientation(Ambientation ambientacion)
	{
		_loadAmbientation = true;
		_nextAmbientation = ambientacion;
	}

	//--------------------------------------------------------

	void MapManager::tick( unsigned int msecs)
	{
		if(_loadAmbientation)
		{
			changeAmbientation(_nextAmbientation);
			_loadAmbientation = false;
		}

		if(_loadMap)
		{

				
			#pragma region  Transicion de salida de mapa

			Graphics::CFadeInOutTransition actualTransition = Graphics::CFadeInOutTransition(800.0f, Graphics::TipoEfecto::SALIDA);
			actualTransition.activateTransitionEffect(Graphics::TipoEfecto::SALIDA);

			unsigned int deltaTime = 0;
			Application::COgreClock clock = Application::COgreClock();
			clock.updateTime();
			while (!actualTransition.isEffectFinished())
			{
				clock.updateTime();
				deltaTime = clock.getLastFrameDuration();

				actualTransition.tick(deltaTime);

				Graphics::CServer::getSingletonPtr()->tick(deltaTime*0.001);
			}

			actualTransition.deactivateTransitionEffect(Graphics::TipoEfecto::SALIDA);

			#pragma endregion

			
			changeMap();

			//hacemos un tick al server logico
			clock.updateTime();
			deltaTime = clock.getLastFrameDuration();
			Logic::CServer::getSingletonPtr()->getMap()->tick(deltaTime);

			#pragma region  Transicion de entrada de mapa
			actualTransition.activateTransitionEffect(Graphics::TipoEfecto::ENTRADA);

			clock.updateTime();
			clock.updateTime();
			while (!actualTransition.isEffectFinished())
			{
				clock.updateTime();
				deltaTime = clock.getLastFrameDuration();

				actualTransition.tick(deltaTime);

				Graphics::CServer::getSingletonPtr()->tick(deltaTime*0.001);
			}

			actualTransition.deactivateTransitionEffect(Graphics::TipoEfecto::ENTRADA);


			#pragma endregion

			LUA::CScriptManager::GetPtrSingleton()->executeScript("HUD.setVisible(true)");
			//LUA::CScriptManager::GetPtrSingleton()->executeProcedure("CartelCentralHUD","activate");

			_loadMap = false;

			//Seteamos el reloj
			Application::CBaseApplication::getSingletonPtr()->updateTime();
		}
	}

} // namespace Logic
