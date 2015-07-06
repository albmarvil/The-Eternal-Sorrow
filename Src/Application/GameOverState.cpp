//---------------------------------------------------------------------------
// GameOverState.cpp
//---------------------------------------------------------------------------

/**
@file GameOverState.cpp

Contiene la implementación del estado de menú.

@see Application::CApplicationState
@see Application::CGameOverState

@author David Llansó
@date Agosto, 2010
*/

#include "GameOverState.h"



#include <CEGUI/CEGUI.h>

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "Graphics/Scene.h"
#include "LUA\ScriptManager.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Sounds/MusicManager.h"

#include "BaseSubsystems\Log.h"

namespace Application {


	//--------------------------------------------------------
	CGameOverState::CGameOverState(CBaseApplication *app) : CApplicationState(app)
	{
	
	}

	bool CGameOverState::init() 
	{

		CApplicationState::init();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGameOverInit");
		return true;

	} // init

	//--------------------------------------------------------

	void CGameOverState::release() 
	{
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGameOverRelease");
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	//NUNCA SE LLAMARA AL RESUME/PAUSE DE ESTE ESTADO PORQUE NUNCA APILAREMOS NADA ENCIMA
	bool CGameOverState::resume()
	{

		return true;
	}

	void CGameOverState::pause()
	{

	}

	void CGameOverState::activate() 
	{
		CApplicationState::activate();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGameOverActivate");
		
		//Desactivamos los eventos de teclado y raton al player, de manera que no me queden almacenados dichos eventos
		//GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();

		//Para los huesos
		Logic::CServer::getSingletonPtr()->getMap()->getScene()->pauseBone(false);
		//BaseSubsystems::Log::Debug("ESTADO DE GAME OVER");

		Sounds::CMusicManager::getSingletonPtr()->stopAllSounds();

	} // activate

	//--------------------------------------------------------

	void CGameOverState::deactivate() 
	{		

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIGameOverDeactivate");

	} // deactivate

	//--------------------------------------------------------

	void CGameOverState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CGameOverState::keyPressed(GUI::TKey key)
	{

		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CGameOverState::keyReleased(GUI::TKey key)
	{
		if (key.keyId == GUI::Key::RETURN)
		{
			_app->setState("menu");
		}

		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CGameOverState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CGameOverState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CGameOverState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
} // namespace Application
