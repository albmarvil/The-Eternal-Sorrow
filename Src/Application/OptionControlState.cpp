/*
author : Cesar Manuel Paz Guzman
date: Mayo, 2015
*/

#include "OptionControlState.h"

#include "LUA\ScriptManager.h"

#include "BaseSubsystems\Log.h"


namespace Application {

	COptionControlState::~COptionControlState() 
	{
	} // ~COptionState

	COptionControlState::COptionControlState(CBaseApplication *app) : CApplicationState(app)
	{		
	}

	//--------------------------------------------------------

	bool COptionControlState::init() 
	{

		CApplicationState::init();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIControlsInit");

		return true;

	} // init

	//--------------------------------------------------------

	void COptionControlState::release() 
	{
		
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	//NUNCA LLAMAREMOS AL RESUME NI PAUSE DE ESTE ESTADO, PORQUE NUNCA APILAREMOS NADA ENCIMA DEL ESTADO OPTION
	bool COptionControlState::resume()
	{
		return true;
	}

	void COptionControlState::pause()
	{
	}

	void COptionControlState::activate() 
	{
		CApplicationState::activate();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIControlsActivate");

	} // activate

	//--------------------------------------------------------

	void COptionControlState::deactivate() 
	{		
		CApplicationState::deactivate();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIControlsDeactivate");
	} // deactivate

	//--------------------------------------------------------

	void COptionControlState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool COptionControlState::keyPressed(GUI::TKey key)
	{
	
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool COptionControlState::keyReleased(GUI::TKey key)
	{
		switch(key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->pop();
			break;
		default:
			return false;
		}

		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool COptionControlState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool COptionControlState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool COptionControlState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------

	bool COptionControlState::povMoved(GUI::Joystick::POV pov)
	{
		return false;
	}

	bool COptionControlState::axisMoved(GUI::Joystick::Axis axis, float abs, float movement)
	{
		return false;
	}

	bool COptionControlState::buttonPressed(GUI::Joystick::Button button)
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
			_app->pop();
			return true;
			break;
		case GUI::Joystick::START:
			break;
		case GUI::Joystick::L3:
			break;
		case GUI::Joystick::R3:
			break;
		}
		return false;
	}

	bool COptionControlState::buttonReleased(GUI::Joystick::Button button)
	{
		return false;
	}

	
} // namespace Application
