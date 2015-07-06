//---------------------------------------------------------------------------
// MenuState.cpp
//---------------------------------------------------------------------------

/**
@file MenuState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CMenuState

@author David Llansó
@date Agosto, 2010
*/

#include "MenuState.h"

#include "GUI/Server.h"

#include "LUA\ScriptManager.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic\Managers\PoolManager.h"
#include "Logic\Managers\MapManager.h"
#include "Logic\Server.h"

#include "Physics\Server.h"

#include "BaseSubsystems\Log.h"
#include "BaseSubsystems\ConfigurationManager.h"

#include "Sounds/MusicManager.h"
#include "Sounds/GUISoundsManager.h"

namespace Application {

	CMenuState::~CMenuState() 
	{
	} // ~CMenuState

	CMenuState::CMenuState(CBaseApplication *app) : CApplicationState(app)
	{		
	}

	//--------------------------------------------------------

	bool CMenuState::init() 
	{

		CApplicationState::init();

		#pragma region carga de ficheros únicos de juego
		// Cargamos el archivo con las definiciones de las entidades del juego
		//Logic::CEntityFactory::getSingletonPtr()->loadBluePrints("blueprints.lua");

		// Cargamos la lista de mapas que existen en el juego.
		//Map::CParserMapList::getSingletonPtr()->parseFile("mapList");
		#pragma endregion

		//Physics::CServer::getSingletonPtr()->createScene();

		//Logic::CPoolManager::Init();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIMenuInit");


		return true;

	} // init

	//--------------------------------------------------------

	void CMenuState::release() 
	{
		
		CApplicationState::release();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIMenuRelease");

	/*	Logic::CPoolManager::Release();
		Physics::CServer::getSingletonPtr()->destroyScene();
		Logic::CEntityFactory::getSingletonPtr()->unloadBluePrints();*/

	} // release

	//--------------------------------------------------------

	bool CMenuState::resume()
	{
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIMenuResume");

		return true;
	}

	void CMenuState::pause()
	{
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIMenuPause");
	}

	void CMenuState::activate() 
	{
		CApplicationState::activate();

		
		//Inicialización de los managers de juego

		//al inicializar el map manager se cargará el primer nivel
		Logic::MapManager::InitMenu();

		//Logic::MapManager::getSingletonPtr()->tick(100);

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIMenuActivate");

		//Activamos la musica del menu
		Sounds::CMusicManager::getSingletonPtr()->stopAllSounds();
		Sounds::CMusicManager::getSingletonPtr()->playSound("/Musica/MenuTheme");


	} // activate

	//--------------------------------------------------------

	void CMenuState::deactivate() 
	{		
		CApplicationState::deactivate();

		Logic::MapManager::Release();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIMenuDeactivate");

		//Desactivamos la musica del menu
		Sounds::CMusicManager::getSingletonPtr()->stopSound("/Musica/MenuTheme");

	} // deactivate

	//--------------------------------------------------------

	void CMenuState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		// Simulación física
		Physics::CServer::getSingletonPtr()->tick(msecs);

		// Actualizamos la lógica de juego.
		Logic::CServer::getSingletonPtr()->tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CMenuState::keyPressed(GUI::TKey key)
	{

		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CMenuState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","Release");
			BaseSubsystems::CConfigurationManager::getSingletonPtr()->saveConfig();
			_app->exitRequest();
			break;
		case GUI::Key::RETURN:
			_app->setState("game");
			Sounds::CMusicManager::getSingletonPtr()->stopAllSounds();
			Sounds::CGUISoundsManager::getSingletonPtr()->playSound("/Varios/IniciarPartida");
			break;
		default:
			return false;
		}

		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CMenuState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CMenuState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CMenuState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------
		
	bool CMenuState::startReleased(const CEGUI::EventArgs& e)
	{
		_app->setState("game");
		Sounds::CMusicManager::getSingletonPtr()->stopAllSounds();
		Sounds::CGUISoundsManager::getSingletonPtr()->playSound("/Varios/IniciarPartida");
		return true;

	} // startReleased
			
	//--------------------------------------------------------

	bool CMenuState::exitReleased(const CEGUI::EventArgs& e)
	{
		_app->exitRequest();
		return true;

	} // exitReleased


	bool CMenuState::povMoved(GUI::Joystick::POV pov)
	{
		return false;
	}

	bool CMenuState::axisMoved(GUI::Joystick::Axis axis, float abs, float movement)
	{
		return false;
	}

	bool CMenuState::buttonPressed(GUI::Joystick::Button button)
	{
		switch (button)
		{
		case GUI::Joystick::UNASSIGNED:
			break;
		case GUI::Joystick::A:
			_app->setState("game");
			Sounds::CMusicManager::getSingletonPtr()->stopAllSounds();
			Sounds::CGUISoundsManager::getSingletonPtr()->playSound("/Varios/IniciarPartida");
			return true;
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
			BaseSubsystems::CConfigurationManager::getSingletonPtr()->saveConfig();
			_app->exitRequest();
			return true;
			break;
		case GUI::Joystick::START:
			_app->setState("game");
			Sounds::CMusicManager::getSingletonPtr()->stopAllSounds();
			Sounds::CGUISoundsManager::getSingletonPtr()->playSound("/Varios/IniciarPartida");
			return true;
			break;
		case GUI::Joystick::L3:
			break;
		case GUI::Joystick::R3:
			break;
		}
		return false;
	}

	bool CMenuState::buttonReleased(GUI::Joystick::Button button)
	{
		return false;
	}

} // namespace Application
