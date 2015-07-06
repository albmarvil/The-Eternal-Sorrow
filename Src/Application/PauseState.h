/*
author : Cesar Manuel Paz Guzman
date: Mayo, 2015
*/

#ifndef __Application_PauseState_H
#define __Application_PauseState_H

#include "ApplicationState.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Application 
{
	class CBaseApplication;
}

namespace CEGUI
{
	class Window;
}

namespace Application 
{

	class CPauseState : public CApplicationState 
	{
	public:
		/** 
		Constructor de la clase 
		*/
		CPauseState(CBaseApplication *app);

		virtual ~CPauseState(){}

		/**
		Función llamada cuando se crea el estado (se "engancha" en la
		aplicación, para que inicialice sus elementos.

		@return true si todo fue bien.
		*/
		virtual bool init();

		/**
		Función llamada cuando se elimina ("desengancha") el
		estado de la aplicación.
		*/
		virtual void release();

		virtual bool resume();

		virtual void pause();

		/**
		Función llamada por la aplicación para que se ejecute
		la funcionalidad del estado.

		@param msecs Número de milisegundos transcurridos desde
		la última llamada (o desde la áctivación del estado, en caso
		de ser la primera vez...).
		*/
		virtual void tick(unsigned int msecs);


		/**
		Función llamada por la aplicación cuando se activa
		el estado.
		*/
		virtual void activate();

		/**
		Función llamada por la aplicación cuando se desactiva
		el estado.
		*/
		virtual void deactivate();
		
		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla. Será la aplicación quién llame a este método 
		cuando el estado esté activo. Esta clase NO se registra en
		el InputManager sino que es la aplicación quien lo hace y 
		delega en los estados.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		virtual bool keyReleased(GUI::TKey key);

		virtual bool keyPressed(GUI::TKey key);
		/***************************************************************
		Métodos de CJoystickListener
		***************************************************************/
		/**
		Método que será invocado siempre que se use la cruceta del joystick

		@param pov Estado de la cruceta del joystick CENTERED significa que no se esta usando
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool povMoved(GUI::Joystick::POV pov);

		/**
		Método que será invocado siempre que se use los sticks o gatillos del mando (los ejes)

		@param axis identificador del eje movido
		@param abs localización absoluta del stick en ese isntante
		@param movement movimiento relativo en el eje [-1, 1] que hay en ese instante
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool axisMoved(GUI::Joystick::Axis axis, float abs, float movement);

		/**
		Método que será invocado siempre que se pulse un boton del joystick

		@param button Identificador del boton pulsado
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool buttonPressed(GUI::Joystick::Button button);


		/**
		Método que será invocado siempre que se deje de pulsar un boton del joystick

		@param button Identificador del boton liberado
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool buttonReleased(GUI::Joystick::Button button);

		CEGUI::Window* _window;

		
	protected:
		void createSaiPause();

		bool _pulsandoD;
		bool _pulsandoA;
		bool _pulsandoS;
		bool _pulsandoW;
	}; // CPauseState

} // namespace Application

#endif //  __Application_PauseState_H
