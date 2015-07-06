/**
@file PlayerController.h

Contiene la declaración de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llansó
@date Agosto, 2010
*/
#ifndef __GUI_PlayerController_H
#define __GUI_PlayerController_H

#include "InputManager.h"
#include "Ogre.h"
#include "BaseSubsystems\Math.h"


// Predeclaración de clases para ahorrar tiempo de compilación
namespace Logic 
{
	class CEntity;
	class CArms;
}

namespace Graphics 
{
	class CServer;
}

namespace Physics
{
	class CServer;
}

// Declaración de la clase
namespace GUI
{
	/**
	Esta clase sirve para gestionar el teclado y el ratón, y mover un avatar 
	acorde con las pulsaciones y el movimiento. En el constructor se registra en 
	el gestor de teclado y en el de ratón (InputManager) para ser avisado ante 
	los eventos. No obstante, puede activarse y desactivarse a voluntad con 
	activate() y deactivate(). El componente CAvatarController del jugador
	es responsable de ponerse como foco a traves de GUI::CServer y así recibir 
	las instrucciones dadas por esta clase.

	@ingroup GUIGroup

	@author David Llansó
	@date Agosto, 2010
	*/
	class CPlayerController : public CKeyboardListener, public CMouseListener, public CJoystickListener
	{
	public:

		/**
		Constructor.
		*/
		CPlayerController ();

		/**
		Destructor.
		*/
		~CPlayerController();

		/**
		Establece el componente del jugador al que enviaremos acciones 
		de movimiento en función de las teclas pulsadas y el movimiento 
		del ratón.

		@param avatarController Componente al que enviaremos acciones de 
		movimiento en función de las teclas pulsadas y el movimiento del 
		ratón.
		*/
		void setControlledAvatar(Logic::CEntity *controlledAvatar); 
										 

		/**
		Activa la la clase, se registra en el CInputManager y así empieza a 
		escuchar eventos.
		*/
		void activate();

		/**
		Desctiva la la clase, se deregistra en el CInputManager y deja de 
		escuchar eventos.
		*/
		void deactivate();

		/*
		Actualiza la posicion del mundo del raton
		*/
		void tick(unsigned int msecs);

		/***************************************************************
		Métodos de CKeyboardListener
		***************************************************************/
		
		/**
		Método que será invocado siempre que se pulse una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyPressed(TKey key);
		
		bool keyPressedTest(TKey key); // para teclas de debug

		/**
		Método que será invocado siempre que se termine la pulsación
		de una tecla.

		@param key Código de la tecla pulsada.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool keyReleased(TKey key);

		bool keyReleasedTest(TKey key); // para teclas de debug

		/***************************************************************
		Métodos de CKeyboardListener
		***************************************************************/
		
		/**
		Método que será invocado siempre que se mueva el ratón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mouseMoved(const CMouseState &mouseState);
		
		/**
		Método que será invocado siempre que se pulse un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mousePressed(const CMouseState &mouseState);

		/**
		Método que será invocado siempre que se termine la pulsación
		de un botón.

		@param mouseState Estado del ratón cuando se lanza el evento.
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool mouseReleased(const CMouseState &mouseState);


		/***************************************************************
		Métodos de CJoystickListener
		***************************************************************/
		/**
		Método que será invocado siempre que se use la cruceta del joystick

		@param pov Estado de la cruceta del joystick CENTERED significa que no se esta usando
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool povMoved(Joystick::POV pov);

		/**
		Método que será invocado siempre que se use los sticks o gatillos del mando (los ejes)

		@param axis identificador del eje movido
		@param abs localización absoluta del stick en ese isntante
		@param movement movimiento relativo en el eje [-1, 1] que hay en ese instante
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool axisMoved(Joystick::Axis axis, float abs, float movement);

		/**
		Método que será invocado siempre que se pulse un boton del joystick

		@param button Identificador del boton pulsado
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool buttonPressed(Joystick::Button button);


		/**
		Método que será invocado siempre que se deje de pulsar un boton del joystick

		@param button Identificador del boton liberado
		@return true si el evento ha sido procesado. En este caso 
		el gestor no llamará a otros listeners.
		*/
		bool buttonReleased(Joystick::Button button);

		void initMapaTeclas();

		void controlMovimiento();

		void controlSalto();

		Vector3 GetRealPos(float posX, float posY);

		int getPlayerLookingDirection() { return _lookingDirection; }

	protected:
		

		/**
		Entidad jugador al que enviaremos acciones de movimiento en
		función de las teclas pulsadas y el movimiento del ratón.
		*/
		Logic::CEntity *_controlledAvatar;

		/*
		Componente Arms del player. Este componente es usado para mandar las ordenes de ataque
		*/
		Logic::CArms *_armComponent;

		std::map<ORDERS,bool> _mapaTeclas;

		/**
		variable que almacena la dirección en la que mira el jugadr
		*/
		int _lookingDirection;
		

		CMouseState _mouseState;

		CJoystickState _joystickState;


		float _minRadioApuntado;

		float _maxRadioApuntadoX;

		float _maxRadioApuntadoY;

	}; // class CPlayerController

} // namespace GUI

#endif // __GUI_PlayerController_H
