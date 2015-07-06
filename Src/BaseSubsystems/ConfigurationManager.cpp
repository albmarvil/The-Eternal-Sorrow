/**
@author César Manuel Paz Guzman
@date Mayo, 2015
*/

#include "ConfigurationManager.h"
#include <assert.h>

#include "OgreRectangle2D.h"
#include "Server.h"
#include "OgreRoot.h"
#include <OgreRenderWindow.h>

#include <cegui\widgets\ToggleButton.h>
// CEGUI
#include <CEGUI/CEGUI.h>
#include <CEGUI/System.h>
#include <CEGUI/DefaultResourceProvider.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

#include <cegui\widgets\ToggleButton.h>

#include <iostream>
#include <fstream>

#include "Sounds\Server.h"
#include "Sounds\GUISoundsManager.h"

namespace BaseSubsystems {

	CConfigurationManager* CConfigurationManager::_instance = 0;

	//--------------------------------------------------------

	CConfigurationManager::CConfigurationManager()
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_renderWindow = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow();

		_instance = this;

		_effectsInitialised = false;
	} 

	//--------------------------------------------------------

	CConfigurationManager::~CConfigurationManager()
	{
		_instance = 0;

	} // ~CServer
	
	//--------------------------------------------------------

	bool CConfigurationManager::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::MapManager no permitida!");

		new CConfigurationManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init



	//--------------------------------------------------------

	void CConfigurationManager::Release()
	{
		assert(_instance && "Logic::MapManager no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

		

	} // Release

	//--------------------------------------------------------

	bool CConfigurationManager::open()
	{
		//Obtenemos los valores iniciales de las opciones de configuracion de ogre
		initializeConfigurationOgre();
		
		//Y posteriomente los cambiamos segun los valores previamente guardados. 
		readFileConfiguration();

		return true;

	} // open


	//--------------------------------------------------------

	void CConfigurationManager::close() 
	{
		//Guardamos la configuracion adicional (volumen, resolucion, pantalla completa) en un fichero
		writeFileConfiguration();

	} // close

	void CConfigurationManager::saveConfig()
	{
		std::string valor ="";

		if(id != 0) //Si hemos cambiado de resolucion en el trascurso de la partida
		{
			valor = std::to_string(resoluciones[id].first) + " x "+std::to_string(resoluciones[id].second) + " @ 32-bit colour";
			_root->getRenderSystem()->setConfigOption("Video Mode",valor);
		}

		//Comprobamos si estamos en modo pantalla completa
		valor = _renderWindow->isFullScreen() ? "Yes" : "No";
		_root->getRenderSystem()->setConfigOption("Full Screen",valor);

		//Si hemos activado el vsync
		valor = vsync ? "Yes" : "No";
		_root->getRenderSystem()->setConfigOption("VSync", valor);

		//El valor marcado para el antialiasing
		_root->getRenderSystem()->setConfigOption("FSAA",std::to_string(_fsaa));

		//Y guardamos todo eso para que ogre lo vuelva a inicializar como esta
		_root->saveConfig();

		//Y esto lo hago por que en pantalla completa peta
		_renderWindow->setFullscreen(false,1366,768);

		//Y esto paa que dea la sensacion de que al pulsar exit, el juego termina. Si no lo pongo, el juego todavia queda ahi abierto
		//un rato cerrando cosas
		_renderWindow->setVisible(false);
		_renderWindow->setHidden(true);

	}

	void CConfigurationManager::changeResolution(int width, int height)
	{
		_renderWindow->resize(width, height);

		//Una forma de dividir mas rapida >> 1, estas dividiendo por 2^1 = 2. Solo para en caso de enteros, ya que esta operacion no vale para floats
		_renderWindow->reposition((resolucionMaxima.width() >> 1) - (width>> 1), (resolucionMaxima.height()>> 1) - (height>> 1));
	}

	void CConfigurationManager::updateResolution()
	{
		CEGUI::Combobox *combobox = (CEGUI::Combobox*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabVideo/ResListbox");
		int idSelected = combobox->getSelectedItem()->getID();

		if(id != idSelected)
		{
			!fullMode  //Si no estamos en modo pantalla completa, entonces cambiamos de resolucion
			? 
				changeResolution(resoluciones[idSelected].first, resoluciones[idSelected].second)
			: 
				_renderWindow->setFullscreen(true,resoluciones[idSelected].first,resoluciones[idSelected].second);

			id = idSelected;
		}

	}

	void CConfigurationManager::changeFullMode()
	{
		CEGUI::Combobox *combobox = (CEGUI::Combobox*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabVideo/ResListbox");
		int idSelected = combobox->getSelectedItem()->getID();

		CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*>(CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabVideo/checkbox"));
		fullMode = checkbox->isSelected();

		if(!fullMode)
		{
			_renderWindow->setFullscreen(false,resoluciones[idSelected].first,resoluciones[idSelected].second);
			_renderWindow->reposition(resolucionMaxima.width()/2 - resoluciones[idSelected].first/2, 
				resolucionMaxima.height()/2 - resoluciones[idSelected].second/2);
			fullMode = false;
		}
		else
		{
			_renderWindow->setFullscreen(true,resoluciones[idSelected].first,resoluciones[idSelected].second);
			fullMode = true;	
		}	
	}

	void  CConfigurationManager::vsyncActive()
	{
		vsync = vsync ? false : true;
	}

	void CConfigurationManager::addResolucion(int width, int height, int pos)
	{
		resoluciones[pos] = std::pair<int,int>(width, height);
	}

	void CConfigurationManager::initializeConfigurationOgre()
	{
		//OBTENEMOS LA RESOLUCION MAXIMA DEL MONITOR
		 HMONITOR hMonitor = MonitorFromWindow(HWND_DESKTOP, MONITOR_DEFAULTTONEAREST);
		 MONITORINFO lpmi;
		 lpmi.cbSize = sizeof(lpmi);
		 if(GetMonitorInfo(hMonitor, &lpmi))
		 {

			  resolucionMaxima.left = lpmi.rcMonitor.left;
			  resolucionMaxima.top = lpmi.rcMonitor.top;
			  resolucionMaxima.right = lpmi.rcMonitor.right;
			  resolucionMaxima.bottom = lpmi.rcMonitor.bottom;
		}
		   
		//MIRAMOS SI LA PANTALLA ESTA EN MODO COMPLETO O NO
		fullMode = _renderWindow->isFullScreen();

		//SI TIENE EL VSYNC ACTIVADO
		vsync = _renderWindow->isVSyncEnabled();
		id = 0;

		_fsaa = _renderWindow->getFSAA();
		//OBTENEMOS LA RESOLUCION ACTUAL
		std::string valor = _root->getRenderSystem()->getConfigOptions()["Video Mode"].currentValue;
		std::istringstream iss(valor);

		std::string width, aux, height;
		iss >> width;
		iss >> aux;
		iss >> height;
		resolucionActual = std::pair<std::string,std::string>(width,height);

		resoluciones = std::map<int,std::pair<int,int>>();
	}

	void CConfigurationManager::writeFileConfiguration()
	{
		//Seteamos First Time a No
		_mapConfigurationFile["First Time"] = "No";

		// Crea un fichero de salida
	    std::ofstream  fs("FileConfiguration.cfg"); 

	    // Enviamos una cadena al fichero de salida:
		std::map<std::string, std::string>::const_iterator it = _mapConfigurationFile.begin();

		for(; it != _mapConfigurationFile.end(); ++it)
		{
			fs << (*it).first<<"="<<(*it).second<<std::endl;
			std::cout<< (*it).first<<"="<<(*it).second<<std::endl;
		}

	    // Cerrar el fichero, 
	    fs.close();
	}

	void CConfigurationManager::readFileConfiguration()
	{
		std::ifstream fileConfiguration("FileConfiguration.cfg"); 

		char buffer[128];
		//Leemos cada linea hasta el final
		while(!fileConfiguration.eof()) 
		{
			//Obtenemos la linea
			fileConfiguration.getline(buffer,128);

			//Obtenemos las dos partes antes del = y despues
			char* clave = strtok(buffer, "=");
			char* valor = strtok(NULL, "=");

			if(clave != NULL && valor != NULL)
				_mapConfigurationFile[std::string(clave)] = std::string(valor);

		}
		fileConfiguration.close();

		Sounds::CServer::getSingletonPtr()->setEffectsMasterVolume(atoi(_mapConfigurationFile["Volume Effects"].c_str())/100.0f);
		Sounds::CServer::getSingletonPtr()->setMusicMasterVolume(atoi(_mapConfigurationFile["Volume Sound"].c_str())/100.0f);
	}

	std::string CConfigurationManager::getValueConfiguration(const std::string &clave) 
	{ 
		return _mapConfigurationFile[clave];
	}

	void CConfigurationManager::updateVolumeMusic()
	{
		CEGUI::Slider* slider = static_cast<CEGUI::Slider*>(CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabAudio/Slider"));

		CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabAudio/valorSoundMusic")->setProperty("Text",
			std::to_string((int)slider->getCurrentValue())+"/100");

		_mapConfigurationFile["Volume Sound"] = std::to_string((int)slider->getCurrentValue());

		Sounds::CServer::getSingletonPtr()->setMusicMasterVolume(slider->getCurrentValue() * 0.01f);
	}

	void CConfigurationManager::updateVolumeEffects()
	{
		CEGUI::Slider* slider = static_cast<CEGUI::Slider*>(CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabAudio/Slider2"));

		CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabAudio/valorSundEffects")->setProperty("Text",
			std::to_string((int)slider->getCurrentValue())+"/100");

		_mapConfigurationFile["Volume Effects"] = std::to_string((int)slider->getCurrentValue());

		Sounds::CServer::getSingletonPtr()->setEffectsMasterVolume(slider->getCurrentValue() * 0.01f);
		if(_effectsInitialised)
			Sounds::CGUISoundsManager::getSingletonPtr()->playSound("/Varios/IniciarPartida");
		else
			_effectsInitialised = true;
	}

	void CConfigurationManager::updateFsaa()
	{
		CEGUI::Combobox *combobox = (CEGUI::Combobox*)CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabVideo/comboboxFSAA");
		int idSelected = combobox->getSelectedItem()->getID();

		_fsaa = atoi(combobox->getSelectedItem()->getText().c_str());
	}
} // namespace Logic
