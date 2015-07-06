//author César Manuel Paz Guzmán

#include "TrophyState.h"

#include "LUA\ScriptManager.h"

#include "BaseSubsystems\Log.h"

namespace Application {


	//--------------------------------------------------------
	CTrophyState::CTrophyState(CBaseApplication *app) : CApplicationState(app)
	{
	
	}

	bool CTrophyState::init() 
	{

		CApplicationState::init();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUItrophyInit");
		return true;

	} // init

	//--------------------------------------------------------

	void CTrophyState::release() 
	{
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUItrophyRelease");
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	bool CTrophyState::resume()
	{

		return true;
	}

	void CTrophyState::pause()
	{

	}

	void CTrophyState::activate() 
	{
		CApplicationState::activate();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUItrophyActivate");

	} // activate

	//--------------------------------------------------------

	void CTrophyState::deactivate() 
	{		

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUItrophyDeactivate");

	} // deactivate

	//--------------------------------------------------------

	void CTrophyState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool CTrophyState::keyPressed(GUI::TKey key)
	{

		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CTrophyState::keyReleased(GUI::TKey key)
	{
		if (key.keyId == GUI::Key::ESCAPE)
		{
			_app->pop();
		}

		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CTrophyState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CTrophyState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CTrophyState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased


} // namespace Application
