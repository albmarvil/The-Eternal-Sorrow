/*
author : Cesar Manuel Paz Guzman
date: Mayo, 2015
*/

#include "OptionState.h"

#include "GUI/Server.h"

#include <CEGUI/CEGUI.h>

#include "LUA\ScriptManager.h"

#include "BaseSubsystems\Log.h"
#include "cegui\System.h"
#include "CEGUI\widgets\Combobox.h"
#include "CEGUI\GUIContext.h"
#include "cegui\widgets\ListboxTextItem.h"
#include "BaseSubsystems\ConfigurationManager.h"
#include "cegui\widgets\ToggleButton.h"
#include "cegui\widgets\Slider.h"


namespace Application {

	COptionState::~COptionState() 
	{
	} // ~COptionState

	COptionState::COptionState(CBaseApplication *app) : CApplicationState(app)
	{		
	}

	//--------------------------------------------------------

	bool COptionState::init() 
	{

		CApplicationState::init();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIOptionInit");
		
		firstTime = false;
		return true;

	} // init

	//--------------------------------------------------------

	void COptionState::release() 
	{
		
		CApplicationState::release();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIOptionRelease");

	} // release

	//--------------------------------------------------------

	bool COptionState::resume()
	{
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIOptionActivate");
		return true;
	}

	void COptionState::pause()
	{
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIOptionDeactivate");
	}

	void COptionState::activate() 
	{
		CApplicationState::activate();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIOptionActivate");


		if(!firstTime)
		{
			createComboboxs();
			firstTime = true;
		}
	} // activate

	//--------------------------------------------------------

	void COptionState::deactivate() 
	{		

		CApplicationState::deactivate();
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GUIOptionDeactivate");

	} // deactivate

	//--------------------------------------------------------

	void COptionState::tick(unsigned int msecs) 
	{
		CApplicationState::tick(msecs);

	} // tick

	//--------------------------------------------------------

	bool COptionState::keyPressed(GUI::TKey key)
	{
		
		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool COptionState::keyReleased(GUI::TKey key)
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
	
	bool COptionState::mouseMoved(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseMoved

	//--------------------------------------------------------
		
	bool COptionState::mousePressed(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mousePressed

	//--------------------------------------------------------


	bool COptionState::mouseReleased(const GUI::CMouseState &mouseState)
	{
		return false;

	} // mouseReleased
			
	//--------------------------------------------------------

	void COptionState::createComboboxs()
	{

		//CREAMOS LAS RESOLUCIONES
		createResolution(854, 480, 1);

		createResolution(1280, 720, 2);
		
		if(BaseSubsystems::CConfigurationManager::getSingletonPtr()->getResolucionMaxima().width() == 1366)
		{
			createResolution(1366, 768, 3);
		}

		//Si la maxima es 1600 se le incluye
		if(BaseSubsystems::CConfigurationManager::getSingletonPtr()->getResolucionMaxima().width() == 1600)
		{
			createResolution(1366, 768, 3);

			createResolution(1600, 900, 4);
		}

		//Si la maximoa es 1920 entonces se incluye la de 1600 y la de 1920
		if(BaseSubsystems::CConfigurationManager::getSingletonPtr()->getResolucionMaxima().width() == 1920)
		{
			createResolution(1366, 768, 3);

			createResolution(1600, 900, 4);

			createResolution(1920, 1080, 5);
		}

		//CREAMOS LO DE PANTALLA COMPLETA 
		CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*>(CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabVideo/checkbox"));
		checkbox->setSelected(BaseSubsystems::CConfigurationManager::getSingletonPtr()->isFullMode());

		//CREAMOS LO DEL VSYNC
		 checkbox = static_cast<CEGUI::ToggleButton*>(CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabVideo/checkbox2"));
		 checkbox->setSelected(BaseSubsystems::CConfigurationManager::getSingletonPtr()->isVsync());

		//CREAMOS LOS DOS REGULADORES DEL VOLUMEN 
		CEGUI::Slider* slider = static_cast<CEGUI::Slider*>(CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabAudio/Slider"));
		slider->setMaxValue(100.0f);
		slider->setClickStep(10.0f);
		slider->setCurrentValue(stoi(BaseSubsystems::CConfigurationManager::getSingletonPtr()->_mapConfigurationFile["Volume Sound"]));

		CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabAudio/valorSoundMusic")->setProperty("Text",
			std::to_string((int)slider->getCurrentValue())+"/100");

		slider = static_cast<CEGUI::Slider*>(CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabAudio/Slider2"));
		slider->setMaxValue(100.0f);
		slider->setClickStep(10.0f);
		slider->setCurrentValue(stoi(BaseSubsystems::CConfigurationManager::getSingletonPtr()->_mapConfigurationFile["Volume Effects"]));
		
		CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabAudio/valorSundEffects")->setProperty("Text",
			std::to_string((int)slider->getCurrentValue())+"/100");


		//CREAMOS LAS OPCIONES DE ANTIALIASING
		createFSAA("0", 1);
		createFSAA("2", 2);
		createFSAA("4", 3);
		createFSAA("8", 4);
	}

	void COptionState::createResolution(int width, int height, int position)
	{
		std::string resolution = std::to_string(width)+" x "+std::to_string(height);

		CEGUI::Combobox *combobox = (CEGUI::Combobox*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabVideo/ResListbox");
		combobox->setReadOnly(true);

		CEGUI::ListboxTextItem * itemCombobox = new CEGUI::ListboxTextItem(resolution, position);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		combobox->addItem(itemCombobox);
		BaseSubsystems::CConfigurationManager::getSingletonPtr()->resoluciones[position] = std::pair<int,int>(width,height);
		if(BaseSubsystems::CConfigurationManager::getSingletonPtr()->getResolucionActual() == std::pair<std::string,std::string>(std::to_string(width),std::to_string(height)))
		{
			BaseSubsystems::CConfigurationManager::getSingletonPtr()->setIdResolucion(position);
			itemCombobox->setSelected(true); // Select this item
			combobox->setText(itemCombobox->getText()); // Copy the item's text into the Editbox
		}
	}


	void COptionState::createFSAA(const std::string &value, int position)
	{
		CEGUI::Combobox * combobox = (CEGUI::Combobox*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabVideo/comboboxFSAA");
		combobox->setReadOnly(true);

		CEGUI::ListboxTextItem *itemCombobox = new CEGUI::ListboxTextItem(value, position);
		itemCombobox->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");
		combobox->addItem(itemCombobox);
		if(BaseSubsystems::CConfigurationManager::getSingletonPtr()->getFSAA() == atoi(value.c_str()))
		{
			itemCombobox->setSelected(true); // Select this item
			combobox->setText(itemCombobox->getText()); // Copy the item's text into the Editbox
		}
	}

	bool COptionState::povMoved(GUI::Joystick::POV pov)
	{
		return false;
	}

	bool COptionState::axisMoved(GUI::Joystick::Axis axis, float abs, float movement)
	{
		return false;
	}

	bool COptionState::buttonPressed(GUI::Joystick::Button button)
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

	bool COptionState::buttonReleased(GUI::Joystick::Button button)
	{
		return false;
	}

	
} // namespace Application
