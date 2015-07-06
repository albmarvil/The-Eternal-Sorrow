//---------------------------------------------------------------------------
// GameState.h
//---------------------------------------------------------------------------

/**
@file GameState.h

Contiene la declaraci�n del estado de juego.

@see Application::CApplicationState
@see Application::CGameState

@author David Llans�
@date Agosto, 2010
*/

#ifndef __Application_GameState_H
#define __Application_GameState_H

#include "ApplicationState.h"
#include <sstream>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Application 
{
	class CBaseApplication;
}

namespace Graphics 
{
	class CScene;
	class CCamera;
	class CEntity;
	class CStaticEntity;
	class CAnimatedEntity;
}

namespace CEGUI
{
	class Window;
}

namespace Application 
{



	/**
	Clase CGameState; representa el estado de juego de la aplicaci�n.
	Se encarga de la preparaci�n y la carga del mapa l�gico del juego.
	Tiene cableados los nombres de ficheros de mapa y de blueprints por
	simplicidad. En la activaci�n y desactivaci�n se encarga de activar
	y desactivar el m�dulo que se encarga de manejar al jugador leyendo 
	eventos de entrada de perif�ricos y de activar y desactivar el mapa 
	de Logic::CServer. En el tick() tambi�n se actualiza el m�dulo de 
	l�gica.
	<p>
	Como ejemplo de Ventana 2D para el GUI se tiene una peque�a ventana 
	que muestra el tiempo transcurrido. 
	Esto provoca que este estado sea CEGUI dependiente, lo cual no es 
	deseable. La aplicaci�n	deber�a ser independiente de las tecnolog�as 
	usadas.

	@ingroup applicationGroup

	@author David Llans�
	@date Agosto, 2010
	*/
	class CGameState : public CApplicationState 
	{
	public:
		/** 
		Constructor de la clase 
		*/
		CGameState(CBaseApplication *app);

		/** 
		Destructor 
		*/
		virtual ~CGameState();

		/**
		Funci�n llamada cuando se crea el estado (se "engancha" en la
		aplicaci�n, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

		virtual bool resume();

		virtual void pause();

		/**
		Funci�n llamada cuando se elimina ("desengancha") el
		estado de la aplicaci�n.
		*/
		virtual void release();

		/**
		Funci�n llamada por la aplicaci�n cuando se activa
		el estado.
		*/
		virtual void activate();

		/**
		Funci�n llamada por la aplicaci�n cuando se desactiva
		el estado.
		*/
		virtual void deactivate();

		/**
		Funci�n llamada por la aplicaci�n para que se ejecute
		la funcionalidad del estado.

		@param msecs N�mero de milisegundos transcurridos desde
		la �ltima llamada (o desde la �ctivaci�n del estado, en caso
		de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs);

		// M�todos de CKeyboardListener
		
		/**
		M�todo que ser� invocado siempre que se pulse una tecla. 
		Ser� la aplicaci�n qui�n llame a este m�todo cuando el 
		estado est� activo. Esta clase NO se registra en el 
		InputManager sino que es la aplicaci�n quien lo hace y 
		delega en los estados.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool keyPressed(GUI::TKey key);
		
		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de una tecla. Ser� la aplicaci�n qui�n llame a este m�todo 
		cuando el estado est� activo. Esta clase NO se registra en
		el InputManager sino que es la aplicaci�n quien lo hace y 
		delega en los estados.

		@param key C�digo de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool keyReleased(GUI::TKey key);

		// M�todos de CMouseListener
		
		/**
		M�todo que ser� invocado siempre que se mueva el rat�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mouseMoved(const GUI::CMouseState &mouseState);
		
		/**
		M�todo que ser� invocado siempre que se pulse un bot�n. La
		aplicaci�n avisa de este evento al estado actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		virtual bool mousePressed(const GUI::CMouseState &mouseState);

		/**
		M�todo que ser� invocado siempre que se termine la pulsaci�n
		de un bot�n. La aplicaci�n avisa de este evento al estado 
		actual.

		@param mouseState Estado del rat�n cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners. 
		*/
		virtual bool mouseReleased(const GUI::CMouseState &mouseState);


		/***************************************************************
		M�todos de CJoystickListener
		***************************************************************/
		/**
		M�todo que ser� invocado siempre que se use la cruceta del joystick

		@param pov Estado de la cruceta del joystick CENTERED significa que no se esta usando
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool povMoved(GUI::Joystick::POV pov);

		/**
		M�todo que ser� invocado siempre que se use los sticks o gatillos del mando (los ejes)

		@param axis identificador del eje movido
		@param abs localizaci�n absoluta del stick en ese isntante
		@param movement movimiento relativo en el eje [-1, 1] que hay en ese instante
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool axisMoved(GUI::Joystick::Axis axis, float abs, float movement);

		/**
		M�todo que ser� invocado siempre que se pulse un boton del joystick

		@param button Identificador del boton pulsado
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool buttonPressed(GUI::Joystick::Button button);


		/**
		M�todo que ser� invocado siempre que se deje de pulsar un boton del joystick

		@param button Identificador del boton liberado
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamar� a otros listeners.
		*/
		bool buttonReleased(GUI::Joystick::Button button);

	protected:

		/**
		Escena del estado.
		*/
		Graphics::CScene* _scene;

		/**
		Ventana que muestra el tiempo de juego transcurrido.
		*/
		CEGUI::Window* _timeWindow;

		/*
		Texto por pantalla indicando los fps
		*/
		std::stringstream text;
		int _fps;
		float _fpsTime;
		float _fpsPerSecondTime;

		unsigned int _timeStamp;

	}; // CGameState

} // namespace Application

#endif //  __Application_GameState_H
