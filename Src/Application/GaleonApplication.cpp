//---------------------------------------------------------------------------
// GaleonApplication.cpp
//---------------------------------------------------------------------------

/**
@file GaleonApplication.cpp

Contiene la implementación de la clase aplicacion para aplicaciones
basadas en Ogre. Esta clase maneja la ejecución de todo el juego.

@see Application::CGaleonApplication
@see Application::C3DApplication

@author David Llansó
@date Septiembre, 2010
*/
#include "GaleonApplication.h"

#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "OptionState.h"
#include "OptionControlState.h"
#include "TrophyState.h"
#include "CreditosState.h"


#include "LUA/ScriptManager.h"


namespace Application {

	CGaleonApplication::CGaleonApplication() : C3DApplication()
	{

	} // CGaleonApplication

	//--------------------------------------------------------

	CGaleonApplication::~CGaleonApplication()
	{

	} // ~CGaleonApplication

	//--------------------------------------------------------

	bool CGaleonApplication::init() 
	{
		// Inicializamos la clase de la que heredamos.
		if (!C3DApplication::init())
			return false;

		LUA::CScriptManager *sm = LUA::CScriptManager::GetPtrSingleton();

		// Creamos los estados. La aplicación se hace responsable de
		// destruirlos.
		if(!addState("menu", new CMenuState(this)))
			return false;
		
		if(!addState("game", new CGameState(this)))
			return false;

		if(!addState("pause", new CPauseState(this)))
			return false;

		if(!addState("gameOver", new CGameOverState(this)))
			return false;

		if(!addState("option", new COptionState(this)))
			return false;

		if(!addState("optionControls", new COptionControlState(this)))
			return false;

		if(!addState("trophy", new CTrophyState(this)))
			return false;

		if(!addState("creditos", new CCreditosState(this)))
			return false;

		//Obtengo el estado inicial desde LUA	
		return setState(sm->getField("GaleonConfig", "initialState", "menu"));

	} // init

	//--------------------------------------------------------

	void CGaleonApplication::release()
	{
		// Desactivamos y eliminamos todos los estados.
		releaseAllStates();

		C3DApplication::release();

	} // release

} // namespace Application
