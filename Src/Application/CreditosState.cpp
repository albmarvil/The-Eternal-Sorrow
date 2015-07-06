//author César Manuel Paz Guzmán

#include "CreditosState.h"

#include "LUA\ScriptManager.h"

#include "BaseSubsystems\Log.h"
#include <CEGUI/CEGUI.h>
#include "CEGUI\GUIContext.h"
#include "cegui\System.h"

namespace Application {


	//--------------------------------------------------------
	CCreditosState::CCreditosState(CBaseApplication *app) : CApplicationState(app)
	{
	
	}

	bool CCreditosState::init() 
	{

		CApplicationState::init();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUICreditosInit");
		return true;

	} // init

	//--------------------------------------------------------

	void CCreditosState::release() 
	{
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUICreditosRelease");
		CApplicationState::release();

	} // release

	//--------------------------------------------------------

	bool CCreditosState::resume()
	{

		return true;
	}

	void CCreditosState::pause()
	{

	}

	void CCreditosState::activate() 
	{
		CApplicationState::activate();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUICreditosActivate");

	} // activate

	//--------------------------------------------------------

	void CCreditosState::deactivate() 
	{		

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUICreditosDeactivate");

	} // deactivate

	//--------------------------------------------------------

	void CCreditosState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

		CEGUI::Window* creditos = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("texto");

		float y = creditos->getArea().getPosition().d_y.d_scale;
		float alt = y + creditos->getArea().getHeight().d_scale;
		float x = creditos->getArea().getPosition().d_x.d_scale;
		float ancho = x + creditos->getArea().getWidth().d_scale;

		creditos->setProperty("Area","{{"+std::to_string(x)+
			",0},{"+std::to_string(y-0.0008)+",0},{"+std::to_string(ancho)+",0},{"
			+std::to_string(alt-0.0008)+",0}}");
		
		creditos = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("sai");

		y = creditos->getArea().getPosition().d_y.d_scale;
		alt = y + creditos->getArea().getHeight().d_scale;
		x = creditos->getArea().getPosition().d_x.d_scale;
		ancho = x + creditos->getArea().getWidth().d_scale;

		creditos->setProperty("Area","{{"+std::to_string(x)+
			",0},{"+std::to_string(y-0.0008)+",0},{"+std::to_string(ancho)+",0},{"
			+std::to_string(alt-0.0008)+",0}}");

	} // tick

	//--------------------------------------------------------

	bool CCreditosState::keyPressed(GUI::TKey key)
	{

		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CCreditosState::keyReleased(GUI::TKey key)
	{
		if (key.keyId == GUI::Key::ESCAPE)
		{
			_app->pop();
		}

		return true;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CCreditosState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool CCreditosState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool CCreditosState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased


} // namespace Application
