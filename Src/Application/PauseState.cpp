/*
author : Cesar Manuel Paz Guzman
date: Mayo, 2015
*/

#include "PauseState.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "Graphics/Scene.h"
#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "BaseSubsystems\Log.h"
#include "Logic/Entity/Components/CameraPause.h"

#include "Logic\Entity\Message.h"
#include "Logic\Entity\Entity.h"

#include "LUA\ScriptManager.h"

#include "CEGUI\Texture.h"
#include "cegui\System.h"
#include "CEGUI\GUIContext.h"
#include "CEGUI\RendererModules\Ogre\Renderer.h"
#include "CEGUI\BasicImage.h"
#include "CEGUI\ImageManager.h"
#include "CEGUI\WindowManager.h"
#include "CEGUI\Window.h"

namespace Application {

	CPauseState::CPauseState(CBaseApplication *app) : CApplicationState(app)
	{
		
	}

	bool CPauseState::init() 
	{
		
		CApplicationState::init();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIPauseInit"); 

		return true;

	} // init

	//--------------------------------------------------------

	void CPauseState::release() 
	{

		CApplicationState::release();

	} // release

	bool CPauseState::resume()
	{
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIPauseResume");
		return true;
	}

	void CPauseState::pause()
	{
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIPausePause");
	}

	void CPauseState::activate() 
	{
		CApplicationState::activate();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIPauseActivate");

		createSaiPause();

		_pulsandoD = false;
		_pulsandoA = false;
		_pulsandoS = false;
		_pulsandoW = false;

	} // activate

	//--------------------------------------------------------

	void CPauseState::createSaiPause()
	{
		CEGUI::OgreRenderer* renderer = static_cast<CEGUI::OgreRenderer*>(CEGUI::System::getSingleton().getRenderer());

		Ogre::TexturePtr texture = Logic::CServer::getSingletonPtr()->getMap()->getScene()->getTexturePause();

		CEGUI::Texture &guiTex = renderer->createTexture("pauseTex", texture);

		const CEGUI::Rectf rect(CEGUI::Vector2f(0.0f, 0.0f), guiTex.getOriginalDataSize());
		CEGUI::BasicImage* image = (CEGUI::BasicImage*)( &CEGUI::ImageManager::getSingleton().create("BasicImage", "RTTImage"));
		   image->setTexture(&guiTex);
		   image->setArea(rect);
		   image->setAutoScaled(CEGUI::ASM_Both);

	    _window = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Root/TabControl/PageItems/Modelo/RTTWindow");

		_window->setProperty("Image", "RTTImage");
	}

	void CPauseState::deactivate() 
	{		
		CApplicationState::deactivate();

		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIPauseDeactivate");

		CEGUI::OgreRenderer* renderer = static_cast<CEGUI::OgreRenderer*>(CEGUI::System::getSingleton().getRenderer());
		renderer->destroyTexture("pauseTex");
		_window->setProperty("Image", "");
		CEGUI::ImageManager::getSingleton().destroy("RTTImage");


	} // deactivate

	void CPauseState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);		

		if(_pulsandoD)
		{
			Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("CameraPause")->getComponent<Logic::CCameraPause*>()->
				pulsarD(msecs);
		}
		if(_pulsandoA)
		{
			Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("CameraPause")->getComponent<Logic::CCameraPause*>()->
				pulsarA(msecs);
		}

		if(_pulsandoW)
		{
			Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("CameraPause")->getComponent<Logic::CCameraPause*>()->
				pulsarW(msecs);
		}
		if(_pulsandoS)
		{
			Logic::CServer::getSingletonPtr()->getMap()->getEntityByType("CameraPause")->getComponent<Logic::CCameraPause*>()->
				pulsarS(msecs);
		}
	} // tick

	//--------------------------------------------------------

	bool CPauseState::keyPressed(GUI::TKey key)
	{
		CEGUI::Window *letra;
		switch (key.keyId)
		{
		case GUI::Key::W:
			letra = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Root/TabControl/PageItems/Modelo/w");

			letra->setProperty("Image", "NewHUD/LetraWPulsada");
			_pulsandoW = true;
			break;
		case GUI::Key::S:
			letra = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Root/TabControl/PageItems/Modelo/s");

			letra->setProperty("Image", "NewHUD/LetraSPulsada");
			_pulsandoS = true;
			break;
		case GUI::Key::A:
			letra = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Root/TabControl/PageItems/Modelo/a");

			letra->setProperty("Image", "NewHUD/LetraAPulsada");
			_pulsandoA = true;
			break;
		case GUI::Key::D:
			letra = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Root/TabControl/PageItems/Modelo/d");

			letra->setProperty("Image", "NewHUD/LetraDPulsada");
			_pulsandoD = true;
			break;
		default:
			break;
		}
		return true;

	}

	bool CPauseState::keyReleased(GUI::TKey key)
	{
		CEGUI::Window *letra;
		//@todo En función de la tecla pulsada pasamos a un estado o a otro
		switch (key.keyId)
		{
		case GUI::Key::ESCAPE:
			_app->pop();
			break;
		case GUI::Key::W:
			letra = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Root/TabControl/PageItems/Modelo/w");

			letra->setProperty("Image", "NewHUD/LetraW");
			_pulsandoW = false;
			break;
		case GUI::Key::S:
			letra = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Root/TabControl/PageItems/Modelo/s");

			letra->setProperty("Image", "NewHUD/LetraS");
			_pulsandoS = false;
			break;
		case GUI::Key::A:
			letra = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Root/TabControl/PageItems/Modelo/a");

			letra->setProperty("Image", "NewHUD/LetraA");
			_pulsandoA = false;
			break;
		case GUI::Key::D:
			letra = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChild("Root/TabControl/PageItems/Modelo/d");

			letra->setProperty("Image", "NewHUD/LetraD");
			_pulsandoD = false;
			break;
		default:
			break;
		}
		return true;

	} // keyReleased


	bool CPauseState::povMoved(GUI::Joystick::POV pov)
	{
		return false;
	}

	bool CPauseState::axisMoved(GUI::Joystick::Axis axis, float abs, float movement)
	{
		return false;
	}

	bool CPauseState::buttonPressed(GUI::Joystick::Button button)
	{
		switch (button)
		{
		case GUI::Joystick::UNASSIGNED:
			break;
		case GUI::Joystick::A:
			break;
		case GUI::Joystick::B:
			_app->pop();
			return true;
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
			_app->pop();
			return true;
			break;
		case GUI::Joystick::L3:
			break;
		case GUI::Joystick::R3:
			break;
		}
		return false;
	}

	bool CPauseState::buttonReleased(GUI::Joystick::Button button)
	{
		return false;
	}

} // namespace Application
