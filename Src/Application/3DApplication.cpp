//---------------------------------------------------------------------------
// 3DApplication.cpp
//---------------------------------------------------------------------------

/**
@file 3DApplication.cpp

Contiene la implementación de la clase aplicacion para aplicaciones
basadas en Ogre. Esta clase maneja la ejecución de todo el juego.

@see Application::C3DApplication
@see Application::CBaseApplication

@author David Llansó
@date Julio, 2010
*/

#include "OgreClock.h"
#include "3DApplication.h"

#include "LUA\ScriptManager.h"
#include "LUA\Server.h"

#include "Graphics/Server.h"
#include "BaseSubsystems/Server.h"
#include "GUI/InputManager.h"
#include "GUI/Server.h"
#include "Logic/Server.h"
#include "Logic/Maps/ComponentFactory.h"
#include "Physics/Server.h"
#include "AI/Server.h"
#include "Sounds\Server.h"
#include "BaseSubsystems/ConfigurationManager.h"

#include <cassert>

namespace Application {

	C3DApplication::C3DApplication() : CBaseApplication()
	{

	} // C3DApplication

	//--------------------------------------------------------

	C3DApplication::~C3DApplication()
	{

	} // ~C3DApplication

	//--------------------------------------------------------

	bool C3DApplication::init() 
	{
		// Inicializamos la clase base.
		if (!CBaseApplication::init())
			return false;

		// Inicializamos los diferentes subsistemas, los de 
		// Ogre entre ellos.
		if (!BaseSubsystems::CServer::Init())
			return false;

		// Inicializamos el servidor gráfico.
		if (!Graphics::CServer::Init())
			return false;

		// Inicializamos el gestor de entrada de periféricos.
		if (!GUI::CInputManager::Init())
			return false;
		// Nos registramos como oyentes de los eventos del teclado.
		GUI::CInputManager::getSingletonPtr()->addKeyListener(this);
		// Y como oyentes de los eventos del ratón.
		GUI::CInputManager::getSingletonPtr()->addMouseListener(this);
		GUI::CInputManager::getSingletonPtr()->addJoystickListener(this);

		// Inicializamos el servidor de interfaz con el usuario.
		if (!GUI::CServer::Init())
			return false;

		// Inicialización del servidor de física.
		if (!Physics::CServer::Init())
			return false;

		// Inicialización del servidor de IA.
		if (!AI::CServer::Init())
			return false;

		// Inicialización del servidor de Sonidos.
		if (!Sounds::CServer::Init())
			return false;
		
		// Inicializamos el servidor de la lógica.
		if (!Logic::CServer::Init())
			return false;

		if(!BaseSubsystems::CConfigurationManager::Init())
			return false;

		// Creamos el reloj basado en Ogre.
		_clock = new COgreClock();

		initializeLUA();

		_timeStamp = 0;

		return true;

	} // init

	//--------------------------------------------------------

	void C3DApplication::release()
	{

		// Eliminamos el reloj de la aplicación.
		delete _clock;

		if(BaseSubsystems::CConfigurationManager::getSingletonPtr())
			BaseSubsystems::CConfigurationManager::Release();

		// Destruimos la factoría de componentes. La factoría
		// de componentes no es de construcción y destrucción explícita
		// debido a como se registran los componentes. Por ello Init y
		// Release no son simétricos.
		if(Logic::CComponentFactory::getSingletonPtr())
			delete Logic::CComponentFactory::getSingletonPtr();

		if(Logic::CServer::getSingletonPtr())
			Logic::CServer::Release();

		if(Sounds::CServer::getSingletonPtr())
			Sounds::CServer::Release();

		if(AI::CServer::getSingletonPtr())
			AI::CServer::Release();

		// Liberar los recursos del servidor de física
		if (Physics::CServer::getSingletonPtr())
			Physics::CServer::Release();
		
		if(GUI::CServer::getSingletonPtr())
			GUI::CServer::Release();

		if(GUI::CInputManager::getSingletonPtr())
		{
			// Dejamos de ser oyentes de los eventos del teclado.
			GUI::CInputManager::getSingletonPtr()->removeKeyListener(this);
			// Y de los eventos del ratón
			GUI::CInputManager::getSingletonPtr()->removeMouseListener(this);
			GUI::CInputManager::Release();
		}
		
		if(Graphics::CServer::getSingletonPtr())
			Graphics::CServer::Release();

		if(BaseSubsystems::CServer::getSingletonPtr())
			BaseSubsystems::CServer::Release();

		CBaseApplication::release();

	} // release

	//--------------------------------------------------------
	//clock_t time1 = clock();
	//clock_t time3 = clock();
	//clock_t time4 = clock();
	//clock_t time5 = clock();
	void C3DApplication::tick(unsigned int msecs) 
	{
		_timeStamp += msecs;

		
		//time1 += clock();

		if(_timeStamp >= 16)
		{
			++BaseSubsystems::Log::_numTicks;
			CBaseApplication::tick(_timeStamp);
			//time2 += clock();

			///esta linea permite el plug&play del joystick
			GUI::CInputManager::getSingletonPtr()->addJoystickListener(this);
			GUI::CInputManager::getSingletonPtr()->tick();

		
			GUI::CServer::getSingletonPtr()->tick(_timeStamp);
			//time3 += clock();

			Sounds::CServer::getSingletonPtr()->tick(_timeStamp);

			Graphics::CServer::getSingletonPtr()->tick(_timeStamp*0.001);
			
			//time4 += clock();
			_timeStamp = 0;

			
		}

		//Graphics::CServer::getSingletonPtr()->tick(msecs*0.001);

		//time5 += clock();
		
		/*float t1 = (float) time1;
		//float t2 = (float) (time2 - time1);
		float t3 = (float) (time3 - time1);
		float t4 = (float) (time4 - time3);
		float t5 = (float) (time5 - time4);*/
		//BaseSubsystems::Log::Debug("Inicial: "+std::to_string(t1));
		//BaseSubsystems::Log::Debug("Base Application(logica + physX): "+std::to_string(t2));
		//BaseSubsystems::Log::Debug("Input: "+std::to_string(t3));
		//BaseSubsystems::Log::Debug("Sounds: "+std::to_string(t4));
		//BaseSubsystems::Log::Debug("Graphics: "+std::to_string(t5));

	} // tick

	void C3DApplication::initializeLUA()
	{
		//Publicamos tantos las funciones como las clases en LUA
		LUA::CServer::getSingletonPtr()->publishAll();

		//Ejecutamos la función para cargar todos los scripts de lua
		LUA::CScriptManager::GetPtrSingleton()->executeProcedure("cargarFicheros");
	}

} // namespace Application
