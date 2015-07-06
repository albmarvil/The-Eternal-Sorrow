//---------------------------------------------------------------------------
// GameState.cpp
//---------------------------------------------------------------------------

/**
@file GameState.cpp

Contiene la implementación del estado de juego.

@see Application::CApplicationState
@see Application::CGameState

@author David Llansó
@date Agosto, 2010
*/

#include "GameState.h"

#include "Logic/Server.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Managers/MapManager.h"
#include "Logic/Managers/PoolManager.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "Graphics/Server.h"
#include "Physics/Server.h"

#include "LUA\ScriptManager.h"

#include "Sounds/GUISoundsManager.h"

namespace Application {

	CGameState::CGameState(CBaseApplication *app) : CApplicationState(app), 
				_scene(0), _fps(0), _fpsTime(0), _fpsPerSecondTime(0)
	{	
	}

	CGameState::~CGameState()
	{	
	}

	bool CGameState::init() 
	{
		CApplicationState::init();

		#pragma region carga de ficheros únicos de juego
		// Cargamos el archivo con las definiciones de las entidades del juego
		Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.lua");

		#pragma endregion

		Physics::CServer::getSingletonPtr()->createScene();

		Logic::CPoolManager::Init();

		Logic::CEntityFactory::getSingletonPtr()->loadArchetypes("archetypesGeneral.lua");
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesRellenadoPoolInicial", "createEntities");
		Logic::CEntityFactory::getSingletonPtr()->unloadArchetypes();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGameInit");

		return true;

	} // init

	//--------------------------------------------------------

	void CGameState::release() 
	{

		CApplicationState::release();

		Logic::CPoolManager::Release();
		Physics::CServer::getSingletonPtr()->destroyScene();
		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();

		

	} // release

	bool CGameState::resume()
	{
		// Queremos que el GUI maneje al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->activate();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGameResume");

		return true;
	}

	void CGameState::pause()
	{
		// Desactivamos la clase que procesa eventos de entrada para controlar al jugador.
		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGamePause");
	}

	//--------------------------------------------------------

	void CGameState::activate() 
	{
		CApplicationState::activate();

		//Inicialización de los managers de juego
		#pragma region Incializacion de managers de juego
		//Inicializamos el game manager
		LUA::CScriptManager::GetPtrSingleton()->executeScript("GameMgr.InitMgr()");

		//al inicializar el map manager se cargará el primer nivel
		Logic::MapManager::Init();

		GUI::CServer::getSingletonPtr()->getPlayerController()->activate();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGameActivate");

		#pragma endregion

	} // activate

	//--------------------------------------------------------

	void CGameState::deactivate() 
	{
		CApplicationState::deactivate();

		GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGameDeactivate");

		#pragma region release de los managers de juego
		Logic::MapManager::Release();

		LUA::CScriptManager::GetPtrSingleton()->executeScript("GameMgr.Release()");

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGameRelease");
		#pragma endregion

	} // deactivate

	//--------------------------------------------------------

	void CGameState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

			// Simulación física
			Physics::CServer::getSingletonPtr()->tick(msecs);

			// Actualizamos la lógica de juego.
			Logic::CServer::getSingletonPtr()->tick(msecs);

			int aux_msecs = msecs;
			//tick del game Manager
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "tick", aux_msecs);

	} // tick

	//--------------------------------------------------------

	bool CGameState::keyPressed(GUI::TKey key)
	{
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->push("pause");
			break;
		default:
			return false;
		}
		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CGameState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased


	bool CGameState::povMoved(GUI::Joystick::POV pov)
	{
		return false;
	}

	bool CGameState::axisMoved(GUI::Joystick::Axis axis, float abs, float movement)
	{
		return false;
	}

	bool CGameState::buttonPressed(GUI::Joystick::Button button)
	{
		switch (button)
		{
		case GUI::Joystick::UNASSIGNED:
			break;
		case GUI::Joystick::A:
			break;
		case GUI::Joystick::B:
			break;
		case GUI::Joystick::X:
			break;
		case GUI::Joystick::Y:
			break;
		case GUI::Joystick::LB:
			break;
		case GUI::Joystick::RB:
			break;
		case GUI::Joystick::BACK:
			break;
		case GUI::Joystick::START:
			_app->push("pause");
			return true;
			break;
		case GUI::Joystick::L3:
			break;
		case GUI::Joystick::R3:
			break;
		}
		return false;
	}

	bool CGameState::buttonReleased(GUI::Joystick::Button button)
	{
		return false;
	}

} // namespace Application
