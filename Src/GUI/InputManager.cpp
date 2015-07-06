//---------------------------------------------------------------------------
// InputManager.cpp
//---------------------------------------------------------------------------

/**
@file InputManager.cpp

Contiene la implementación del gestor de periféricos de entrada
así como las clases oyentes que deben extender las clases que
quieran ser avisadas de los eventos de dichos periféricos:
ratón y teclado.

@see GUI::CInputManager
@see GUI::CKeyboardListener
@see GUI::CMouseListener

@author David Llansó
@date Julio, 2010
*/

#include "InputManager.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems\Log.h"

#include <OISInputManager.h>
#include <Xinput.h>

#include <sstream>
#include <cassert>


namespace GUI{

	CInputManager *CInputManager::_instance = 0;

	//--------------------------------------------------------

	CInputManager::CInputManager() :
		_mouse(0),
		_keyboard(0),
		_joystick(0),
		_inputSystem(0)
	{
		assert(!_instance && "¡Segunda inicialización de GUI::CInputManager no permitida!");
		_instance = this;

	} // CInputManager

	//--------------------------------------------------------

	CInputManager::~CInputManager() 
	{
		assert(_instance);

		_instance = 0;

	} // ~CInputManager

	//--------------------------------------------------------

	bool CInputManager::Init() 
	{
		assert(!_instance && "¡Segunda inicialización de GUI::CInputManager no permitida!");

		new CInputManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CInputManager::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CInputManager::open() 
	{
		if(!BaseSubsystems::CServer::getSingletonPtr())
			return false;

		// Cogemos el sistema de entrada.
		_inputSystem = BaseSubsystems::CServer::getSingletonPtr()->getInputSystem();

		// Cogemos el buffer del teclado y nos hacemos oyentes.
		_keyboard = BaseSubsystems::CServer::getSingletonPtr()->getBufferedKeyboard();
		if(_keyboard)
			_keyboard->setEventCallback(this);

		// Cogemos el buffer del ratón y nos hacemos oyentes.
		_mouse = BaseSubsystems::CServer::getSingletonPtr()->getBufferedMouse();
		if(_mouse)
			_mouse->setEventCallback(this);


		DWORD dwResult;

		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(0, &state);

		if(dwResult == ERROR_SUCCESS)
		{
			BaseSubsystems::Log::Debug("Joystick detectado! - XInput");
			_usingXInput = true;
			
		}
		else
		{
			_usingXInput = false;
			//cogemos el buffer del joystick y nos hacemos oyentes
			_joystick = BaseSubsystems::CServer::getSingletonPtr()->getBufferedJoystick();
			if(_joystick)
			{
				BaseSubsystems::Log::Debug("Joystick detectado! - OIS");
				_joystick->setEventCallback(this);
			}
		}
		
		///Iniciamos el estado del raton
		unsigned int w = 0;
		unsigned int h = 0;
		BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(w,h);

		_mouseState.setExtents(w, h);
		_mouseState.setPosition(w/2, h/2);

		_rightStickAsMouse = true;

		return true;

	} // open

	//--------------------------------------------------------

	void CInputManager::close()
	{
		// No somos responsables de la destrucción de los objetos.
		_mouse = 0;
		_keyboard = 0;
		_joystick = 0;
		_inputSystem = 0;

	} // close

	//--------------------------------------------------------

	void CInputManager::tick() 
	{
		// Se necesita capturar todos los dispositivos.
		if(_mouse) {
			_mouse->capture();
		}

		if(_keyboard) {
			_keyboard->capture();
		}


		DWORD dwResult;

		XINPUT_STATE state;

		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(0, &state);

		if(dwResult == ERROR_SUCCESS)
		{
			_usingXInput = true;
			captureXinputPOV(state.Gamepad.wButtons);
			captureXinputButtons(state.Gamepad.wButtons);
			captureXinputTriggers(state.Gamepad.bLeftTrigger, Joystick::Axis::LZ);
			captureXinputTriggers(state.Gamepad.bRightTrigger, Joystick::Axis::RZ);
			captureXinputAxis(state.Gamepad.sThumbLX, Joystick::Axis::LX);
			captureXinputAxis(state.Gamepad.sThumbLY, Joystick::Axis::LY);
			captureXinputAxis(state.Gamepad.sThumbRX, Joystick::Axis::RX);
			captureXinputAxis(state.Gamepad.sThumbRY, Joystick::Axis::RY);
		}
		else
		{
			_usingXInput = false;
			if(_joystick)
			{
				_joystick->capture();

				unsigned int h = 0;
				unsigned int w = 0;
				BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(w, h);
				_mouseState.setExtents(w, h);

				if(_joystickState._RYabs != 0 && _rightStickAsMouse && !_joystickListeners.empty())
				{
					//simulamos movimiento del raton
					
					float diff = _joystickState._RYabs;
					diff *= _MAX_STICK_SPEED;
					float res = _mouseState.posAbsY + diff;

					res = res<0? 0 : res;

					res = res > h? h : res;
						
					_mouseState.setPosition(_mouseState.posAbsX, res);
					_mouseState.movX = 0;
					_mouseState.movY = diff;
			
					std::list<CMouseListener*>::const_iterator it;
					it = _mouseListeners.begin();
					for (; it != _mouseListeners.end(); it++) 
					{
						if ((*it)->mouseMoved(_mouseState))
							break;
					}
				}

				if(_joystickState._RXabs != 0 && _rightStickAsMouse && !_joystickListeners.empty())
				{
					//simulamos movimiento del raton
					float diff = _joystickState._RXabs;
							
					diff *= _MAX_STICK_SPEED;
					float res = _mouseState.posAbsX + diff;

					res = res<0? 0 : res;

					res = res > w? w : res;
						
					_mouseState.setPosition(res, _mouseState.posAbsY);
					_mouseState.movX = diff;
					_mouseState.movY = 0;
			
					std::list<CMouseListener*>::const_iterator it;
					it = _mouseListeners.begin();
					for (; it != _mouseListeners.end(); it++) 
					{
						if ((*it)->mouseMoved(_mouseState))
							break;
					}
				}

			}
		}

	} // capture

	//--------------------------------------------------------

	void CInputManager::addKeyListener(CKeyboardListener *keyListener) 
	{
		if(_keyboard)
			_keyListeners.push_front(keyListener);
		
	} // addKeyListener

	//--------------------------------------------------------

	void CInputManager::addMouseListener(CMouseListener *mouseListener) 
	{
		if(_mouse)
			_mouseListeners.push_front(mouseListener);
		
	} // addMouseListener

	//--------------------------------------------------------

	void CInputManager::removeKeyListener(CKeyboardListener *keyListener) 
	{
		_keyListeners.remove(keyListener);

	} // removeKeyListener

	//--------------------------------------------------------

	void CInputManager::removeMouseListener(CMouseListener *mouseListener) 
	{
		_mouseListeners.remove(mouseListener);

	} // removeMouseListener

	//--------------------------------------------------------

	void CInputManager::removeAllListeners() 
	{
		_keyListeners.clear();
		_mouseListeners.clear();
		_joystickListeners.clear();

	} // removeAllListeners

	//--------------------------------------------------------

	void CInputManager::removeAllKeyListeners() 
	{
		_keyListeners.clear();

	} // removeAllKeyListeners

	//--------------------------------------------------------

	void CInputManager::removeAllMouseListeners() 
	{
		_mouseListeners.clear();

	} // removeAllMouseListeners

	//--------------------------------------------------------

	bool CInputManager::keyPressed(const OIS::KeyEvent &e) 
	{
		if (!_keyListeners.empty()) 
		{
			std::list<CKeyboardListener*>::const_iterator it;
			it = _keyListeners.begin();
			for (; it != _keyListeners.end(); it++) 
			{
				if ((*it)->keyPressed(ois2galeon(e)))
				  break;
			}
		}

		return true;

	} // keyPressed

	//--------------------------------------------------------

	bool CInputManager::keyReleased(const OIS::KeyEvent &e) 
	{
		if (!_keyListeners.empty()) 
		{
			std::list<CKeyboardListener*>::const_iterator it;
			it = _keyListeners.begin();
			for (; it != _keyListeners.end(); it++) 
			{
				if ((*it)->keyReleased(ois2galeon(e)))
				  break;
			}
		}

		return true;

	} // keyReleased

	//--------------------------------------------------------

	TKey CInputManager::ois2galeon(const OIS::KeyEvent &e)
	{
		unsigned int text = e.text;
		//Las teclas del numpad no vienen con el texto. Lo metemos a mano.
        switch (e.key)
        {
        case OIS::KC_DECIMAL:
                text = 46;
                break;
        case OIS::KC_DIVIDE:
                text = 47;
                break;
        case OIS::KC_NUMPAD0:
                text = 48;
                break;
        case OIS::KC_NUMPAD1:
                text = 49;
                break;
        case OIS::KC_NUMPAD2:
                text = 50;
                break;
        case OIS::KC_NUMPAD3:
                text = 51;
                break;
        case OIS::KC_NUMPAD4:
                text = 52;
                break;
        case OIS::KC_NUMPAD5:
                text = 53;
                break;
        case OIS::KC_NUMPAD6:
                text = 54;
                break;
        case OIS::KC_NUMPAD7:
                text = 55;
                break;
        case OIS::KC_NUMPAD8:
                text = 56;
                break;
        case OIS::KC_NUMPAD9:
                text = 57;
                break;
        }

		return TKey(text,(const Key::TKeyID)e.key);

	}


	//--------------------------------------------------------

	bool CInputManager::mouseMoved(const OIS::MouseEvent &e) 
	{
		if (!_mouseListeners.empty()) 
		{
			// Actualizamos el estado antes de enviarlo
			_mouseState.setExtents(e.state.width, e.state.height);
			/*BaseSubsystems::Log::Debug("ABS: " + std::to_string(e.state.X.abs) + " - " + std::to_string(e.state.Y.abs));
			
			float diffx = e.state.X.abs;
			diffx -= _mouseState.posAbsX;
			float diffy = e.state.Y.abs;
			diffy -= _mouseState.posAbsY;

			BaseSubsystems::Log::Debug("DIFF: " + std::to_string(diffx) + " - " + std::to_string(diffy));*/
			_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			
			_mouseState.movX = e.state.X.rel;
			_mouseState.movY = e.state.Y.rel;
			//BaseSubsystems::Log::Debug("MOV: " + std::to_string(_mouseState.movX) + " - " + std::to_string(_mouseState.movY));
			
			_mouseState.scrool = e.state.Z.rel;
			_mouseState.button = Button::UNASSIGNED;

			std::list<CMouseListener*>::const_iterator it;
			it = _mouseListeners.begin();
			for (; it != _mouseListeners.end(); it++) 
			{
				if ((*it)->mouseMoved(_mouseState))
				  return true;
			}
		}

		return false;

	} // mouseMoved

	//--------------------------------------------------------

	bool CInputManager::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID button) 
	{
		if (!_mouseListeners.empty()) 
		{
			// Actualizamos el estado antes de enviarlo
			_mouseState.setExtents(e.state.width, e.state.height);
			_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			_mouseState.movX = e.state.X.rel;
			_mouseState.movY = e.state.Y.rel;
			_mouseState.scrool = e.state.Z.rel;
			_mouseState.button = (TButton)button;

			std::list<CMouseListener*>::const_iterator it;
			it = _mouseListeners.begin();
			for (; it != _mouseListeners.end(); it++) 
			{
				if ((*it)->mousePressed(_mouseState))
				  break;
			}
		}

		return true;

	} // mousePressed

	//--------------------------------------------------------

	bool CInputManager::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID button) 
	{
		if (!_mouseListeners.empty()) 
		{
			// Actualizamos el estado antes de enviarlo
			_mouseState.setExtents(e.state.width, e.state.height);
			_mouseState.setPosition(e.state.X.abs,e.state.Y.abs);
			_mouseState.movX = e.state.X.rel;
			_mouseState.movY = e.state.Y.rel;
			_mouseState.scrool = e.state.Z.rel;
			_mouseState.button = (TButton)button;

			std::list<CMouseListener*>::const_iterator it;
			it = _mouseListeners.begin();
			for (; it != _mouseListeners.end(); it++) 
			{
				if ((*it)->mouseReleased(_mouseState))
				  break;
			}
		}

		return true;

	} // mouseReleased


	bool CInputManager::povMoved(const OIS::JoyStickEvent &e, int pov)
	{
		if(!_joystickListeners.empty())
		{
			Joystick::POV dir = Joystick::POV(e.state.mPOV[0].direction);

			//DEBUG
			switch (dir)
			{
			case Joystick::POV::CENTERED:
				BaseSubsystems::Log::Debug("CENTERED");
				break;
			case Joystick::POV::NORTH:
				BaseSubsystems::Log::Debug("NORTH");
				break;
			case Joystick::POV::SOUTH:
				BaseSubsystems::Log::Debug("SOUTH");
				break;
			case Joystick::POV::EAST:
				BaseSubsystems::Log::Debug("EAST");
				break;
			case Joystick::POV::WEST:
				BaseSubsystems::Log::Debug("WEST");
				break;
			case Joystick::POV::NORTHEAST:
				BaseSubsystems::Log::Debug("NORTHEAST");
				break;
			case Joystick::POV::NORTHWEST:
				BaseSubsystems::Log::Debug("NORTHWEST");
				break;
			case Joystick::POV::SOUTHEAST:
				BaseSubsystems::Log::Debug("SOUTHEAST");
				break;
			case Joystick::POV::SOUTHWEST:
				BaseSubsystems::Log::Debug("SOUTHWEST");
				break;
			}

			std::list<CJoystickListener*>::const_iterator it = _joystickListeners.begin();

			for(; it != _joystickListeners.end(); ++it)
			{
				if((*it)->povMoved(dir))
					break;
			}
		}

		return true;
	}//povMoved

	//-------------------------------------------------

	bool CInputManager::povMoved(Joystick::POV pov)
	{
		if(!_joystickListeners.empty())
		{
			//DEBUG
			/*switch (pov)
			{
			case Joystick::POV::CENTERED:
				BaseSubsystems::Log::Debug("CENTERED");
				break;
			case Joystick::POV::NORTH:
				BaseSubsystems::Log::Debug("NORTH");
				break;
			case Joystick::POV::SOUTH:
				BaseSubsystems::Log::Debug("SOUTH");
				break;
			case Joystick::POV::EAST:
				BaseSubsystems::Log::Debug("EAST");
				break;
			case Joystick::POV::WEST:
				BaseSubsystems::Log::Debug("WEST");
				break;
			case Joystick::POV::NORTHEAST:
				BaseSubsystems::Log::Debug("NORTHEAST");
				break;
			case Joystick::POV::NORTHWEST:
				BaseSubsystems::Log::Debug("NORTHWEST");
				break;
			case Joystick::POV::SOUTHEAST:
				BaseSubsystems::Log::Debug("SOUTHEAST");
				break;
			case Joystick::POV::SOUTHWEST:
				BaseSubsystems::Log::Debug("SOUTHWEST");
				break;
			}*/

			std::list<CJoystickListener*>::const_iterator it = _joystickListeners.begin();

			for(; it != _joystickListeners.end(); ++it)
			{
				if((*it)->povMoved(pov))
					break;
			}
		}

		return true;
	}//povMoved

	//-------------------------------------------------

	bool CInputManager::axisMoved(const OIS::JoyStickEvent &e, int axis)
	{

		if(!_joystickListeners.empty())
		{
			Joystick::Axis eaxis = Joystick::Axis(axis);
			float absoluto = e.state.mAxes[axis].abs;
			absoluto /= OIS::JoyStick::MAX_AXIS;
			if(axis == 4 && e.state.mAxes[axis].abs < 0)
			{
				eaxis = Joystick::Axis::RZ;
				absoluto = abs(absoluto);
			}

			
			//si es el stick izquierdo y no supera el umbral definido para el stick izquierdo, se capa a 0
			if((eaxis == Joystick::Axis::LX || eaxis == Joystick::Axis::LY) && !(e.state.mAxes[axis].abs < _LEFT_THRESHOLD * OIS::JoyStick::MIN_AXIS || e.state.mAxes[axis].abs > _LEFT_THRESHOLD * OIS::JoyStick::MAX_AXIS))
			{
				absoluto = 0.0f;
			}
			//lo mismo con el umbral del stick derecho
			else if((eaxis == Joystick::Axis::RX || eaxis == Joystick::Axis::RY) && !(e.state.mAxes[axis].abs < _RIGHT_THRESHOLD * OIS::JoyStick::MIN_AXIS || e.state.mAxes[axis].abs > _RIGHT_THRESHOLD * OIS::JoyStick::MAX_AXIS))
			{
				absoluto = 0.0f;
			}

			
			float movement = 0.0f;
			switch (eaxis)
			{
			case GUI::Joystick::LY:
				movement = _joystickState._LYabs - absoluto;
				_joystickState._LYMov = movement;
				_joystickState._LYabs = absoluto;
				break;
			case GUI::Joystick::LX:
				movement = _joystickState._LXabs - absoluto;
				_joystickState._LXMov = movement;
				_joystickState._LXabs = absoluto;
				break;
			case GUI::Joystick::RY:
				movement = _joystickState._RYabs - absoluto;
				_joystickState._RYMov = movement;
				_joystickState._RYabs = absoluto;
				break;
			case GUI::Joystick::RX:
				movement = _joystickState._RXabs - absoluto;
				_joystickState._RXMov = movement;
				_joystickState._RXabs = absoluto;
				break;
			}

			std::list<CJoystickListener*>::const_iterator it = _joystickListeners.begin();

			for(; it != _joystickListeners.end(); ++it)
			{
				if((*it)->axisMoved(eaxis, absoluto, 0.0f))
					break;
			}
		}
		
		return true;
	}//axisMoved

	//-------------------------------------------------------------


	bool CInputManager::axisMoved(Joystick::Axis axis, float abs, float mov)
	{

		if(!_joystickListeners.empty())
		{
			/*if(abs!=0)
			{
				BaseSubsystems::Log::Debug("AXIS: " + std::to_string(axis) + " - " + std::to_string(abs)+" - " + std::to_string(mov));
			}*/
			

			if(_rightStickAsMouse)
			{
				switch (axis)
				{
				case GUI::Joystick::RY:
					{
						if(abs != 0 )
						{
							//simulamos movimiento del raton
							unsigned int h = 0;
							unsigned int w = 0;
							BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(w, h);
							_mouseState.setExtents(w, h);
							float diff = (-abs);
							
							diff *= _MAX_STICK_SPEED;
							//BaseSubsystems::Log::Debug("RY: " + std::to_string(diff));
							float res = _mouseState.posAbsY + diff;

							res = res<0? 0 : res;

							res = res > h? h : res;
						
							_mouseState.setPosition(_mouseState.posAbsX, res);
							_mouseState.movX = 0;
							_mouseState.movY = diff;
			
							std::list<CMouseListener*>::const_iterator it;
							it = _mouseListeners.begin();
							for (; it != _mouseListeners.end(); it++) 
							{
								if ((*it)->mouseMoved(_mouseState))
								  break;
							}
						}
					}
					break;
				case GUI::Joystick::RX:
					{
						if(abs != 0)
						{
							//simulamos movimiento del raton
							unsigned int h = 0;
							unsigned int w = 0;
							BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(w, h);
							_mouseState.setExtents(w, h);
							float diff = abs;
							
							diff *=  _MAX_STICK_SPEED;
							//BaseSubsystems::Log::Debug("RX: " + std::to_string(diff));
							float res = _mouseState.posAbsX + diff;

							res = res<0? 0 : res;

							res = res > w? w : res;

							_mouseState.setPosition(res, _mouseState.posAbsY );
							_mouseState.movX = diff;
							_mouseState.movY = 0;
			
							std::list<CMouseListener*>::const_iterator it;
							it = _mouseListeners.begin();
							for (; it != _mouseListeners.end(); it++) 
							{
								if ((*it)->mouseMoved(_mouseState))
								  break;
							}
						}
					}
					break;
				}
			}

			std::list<CJoystickListener*>::const_iterator it = _joystickListeners.begin();

			for(; it != _joystickListeners.end(); ++it)
			{
				if((*it)->axisMoved(axis, abs, mov))
					break;
			}
		}
		
		return true;
	}//axisMoved

	//-------------------------------------------------------------

	bool CInputManager::sliderMoved(const OIS::JoyStickEvent &e, int sliderID)
	{
		//BaseSubsystems::Log::Debug("SLIDER: " + std::to_string(sliderID));
		return true;
	}//sliderMoved

	//------------------------------------------------------------

	bool CInputManager::buttonPressed(const OIS::JoyStickEvent &e, int button)
	{
	
		if(!_joystickListeners.empty())
		{
			
			///DEBUG
			//BaseSubsystems::Log::Debug("BTT PRESSED: " + std::to_string(button));

			Joystick::Button bt = Joystick::Button(button);

			std::list<CJoystickListener*>::const_iterator it = _joystickListeners.begin();

			for(; it != _joystickListeners.end(); ++it)
			{
				if((*it)->buttonPressed(bt))
					break;
			}
		}

		return true;
	}//buttonPresed

	//--------------------------------------------------------------

	bool CInputManager::buttonPressed(Joystick::Button button)
	{
	
		if(!_joystickListeners.empty())
		{
			
			///DEBUG
			//BaseSubsystems::Log::Debug("BTT PRESSED: " + std::to_string(button));

			std::list<CJoystickListener*>::const_iterator it = _joystickListeners.begin();

			for(; it != _joystickListeners.end(); ++it)
			{
				if((*it)->buttonPressed(button))
					break;
			}
		}

		return true;
	}//buttonPresed

	//--------------------------------------------------------------

	bool CInputManager::buttonReleased(const OIS::JoyStickEvent &e, int button)
	{
		if(!_joystickListeners.empty())
		{
			
			///DEBUG
			//BaseSubsystems::Log::Debug("BTT RELEASED: " + std::to_string(button));

			Joystick::Button bt = Joystick::Button(button);

			std::list<CJoystickListener*>::const_iterator it = _joystickListeners.begin();

			for(; it != _joystickListeners.end(); ++it)
			{
				if((*it)->buttonReleased(bt))
					break;
			}
		}

		return true;
	}//buttonReleased

	//---------------------------------------------------------------

	bool CInputManager::buttonReleased(Joystick::Button button)
	{
		if(!_joystickListeners.empty())
		{
			
			///DEBUG
			//BaseSubsystems::Log::Debug("BTT RELEASED: " + std::to_string(button));


			std::list<CJoystickListener*>::const_iterator it = _joystickListeners.begin();

			for(; it != _joystickListeners.end(); ++it)
			{
				if((*it)->buttonReleased(button))
					break;
			}
		}

		return true;
	}//buttonReleased

	//---------------------------------------------------------------

	void CInputManager::addJoystickListener(CJoystickListener *joystickListener)
	{
		std::list<CJoystickListener*>::iterator it = _joystickListeners.begin();
		std::list<CJoystickListener*>::iterator end = _joystickListeners.end();
		bool exists = false;
		for(; it != end; ++it)
		{
			exists = exists || (*it) == joystickListener;
			if(exists)
				break;
		}

		if((_joystick || _usingXInput) && !exists)
		{
			_joystickListeners.push_front(joystickListener);
		}
	}///addJoystickListener

	//--------------------------------------------------------------

	void CInputManager::removeJoystickListener(CJoystickListener *joystickListener)
	{
		_joystickListeners.remove(joystickListener);
	}//removeJoystickListener

	//-------------------------------------------------------------

	void CInputManager::removeAllJoystickListeners()
	{
		_joystickListeners.clear();
	}//removeAllJoystickListeners

	//----------------------------------------------------------
	
	
	void CInputManager::captureXinputPOV(unsigned short input)
	{
		Joystick::XInputPOV p = Joystick::XInputPOV(input & Joystick::XInputPOV::XI_POV_ALL);
		
		if(_joystickState._pov != p)
		{
			if(Joystick::XInputPOV::XI_CENTERED == p)
			{
				//BaseSubsystems::Log::Debug("XI_CENTERED: " + std::to_string(input));
				_joystickState._pov = p;
				Joystick::POV pov = xinputPOV2galeon(Joystick::XInputPOV::XI_CENTERED);
				povMoved(pov);
			}
			else if(Joystick::XInputPOV::XI_NORTH == p)
			{
				//BaseSubsystems::Log::Debug("XI_NORTH: " + std::to_string(input));
				_joystickState._pov = p;
				Joystick::POV pov = xinputPOV2galeon(Joystick::XInputPOV::XI_NORTH);
				povMoved(pov);
			}
			else if(Joystick::XInputPOV::XI_NORTHEAST == p)
			{
				//BaseSubsystems::Log::Debug("XI_NORTHEAST: " + std::to_string(input));
				_joystickState._pov = p;
				Joystick::POV pov = xinputPOV2galeon(Joystick::XInputPOV::XI_NORTHEAST);
				povMoved(pov);
			}
			else if(Joystick::XInputPOV::XI_EAST == p)
			{
				//BaseSubsystems::Log::Debug("XI_EAST: " + std::to_string(input));
				_joystickState._pov = p;
				Joystick::POV pov = xinputPOV2galeon(Joystick::XInputPOV::XI_EAST);
				povMoved(pov);
			}
			else if(Joystick::XInputPOV::XI_SOUTHEAST == p)
			{
				//BaseSubsystems::Log::Debug("XI_SOUTHEAST: " + std::to_string(input));
				_joystickState._pov = p;
				Joystick::POV pov = xinputPOV2galeon(Joystick::XInputPOV::XI_SOUTHEAST);
				povMoved(pov);
			}
			else if(Joystick::XInputPOV::XI_SOUTH == p)
			{
				//BaseSubsystems::Log::Debug("XI_SOUTH: " + std::to_string(input));
				_joystickState._pov = p;
				Joystick::POV pov = xinputPOV2galeon(Joystick::XInputPOV::XI_SOUTH);
				povMoved(pov);
			}
			else if(Joystick::XInputPOV::XI_SOUTHWEST == p)
			{
				//BaseSubsystems::Log::Debug("XI_SOUTHWEST: " + std::to_string(input));
				_joystickState._pov = Joystick::XInputPOV(input);
				Joystick::POV pov = xinputPOV2galeon(Joystick::XInputPOV::XI_SOUTHWEST);
				povMoved(pov);
			}
			else if(Joystick::XInputPOV::XI_WEST == p)
			{
				//BaseSubsystems::Log::Debug("XI_WEST: " + std::to_string(input));
				_joystickState._pov = p;
				Joystick::POV pov = xinputPOV2galeon(Joystick::XInputPOV::XI_WEST);
				povMoved(pov);
			}
			else if(Joystick::XInputPOV::XI_NORTHWEST == p)
			{
				//BaseSubsystems::Log::Debug("XI_NORTHWEST: " + std::to_string(input));
				_joystickState._pov = p;
				Joystick::POV pov = xinputPOV2galeon(Joystick::XInputPOV::XI_NORTHWEST);
				povMoved(pov);
			}
		}
	}//captureXinputPOV

	//---------------------------------------------

	void CInputManager::captureXinputTriggers(unsigned char input, Joystick::Axis axis)
	{
		float abs = input;

		if(!(abs > _LEFT_THRESHOLD * _MAX_TRIGGER))
		{
			abs = 0.0f;
		}
		else
		{
			abs /= _MAX_TRIGGER;
		}

		
		float movement = 0.0f;

		switch (axis)
		{
		case GUI::Joystick::LZ:
			movement = abs - _joystickState._LZabs;
			_joystickState._LZMov = movement;
			_joystickState._LZabs = abs;
			break;
		case GUI::Joystick::RZ:
			movement = abs - _joystickState._LZabs;
			_joystickState._LZMov = movement;
			_joystickState._LZabs = abs;
			break;
		}
		axisMoved(axis, abs, movement);
	}//captureXinputTriggers

	//---------------------------------------------

	void CInputManager::captureXinputButtons(unsigned short input)
	{
		Joystick::XInputButton nuevo = Joystick::XInputButton(input);
		Joystick::XInputButton viejo = _joystickState._button;

		if((Joystick::XInputButton::XI_A & nuevo) && !(Joystick::XInputButton::XI_A & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_A);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_A & nuevo) && (Joystick::XInputButton::XI_A & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_A);
			buttonReleased(bt);
		}

		///////////////

		if((Joystick::XInputButton::XI_B & nuevo) && !(Joystick::XInputButton::XI_B & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_B);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_B & nuevo) && (Joystick::XInputButton::XI_B & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_B);
			buttonReleased(bt);
		}

		///////////////

		if((Joystick::XInputButton::XI_X & nuevo) && !(Joystick::XInputButton::XI_X & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_X);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_X & nuevo) && (Joystick::XInputButton::XI_X & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_X);
			buttonReleased(bt);
		}

		//////////////

		if((Joystick::XInputButton::XI_Y & nuevo) && !(Joystick::XInputButton::XI_Y & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_Y);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_Y & nuevo) && (Joystick::XInputButton::XI_Y & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_Y);
			buttonReleased(bt);
		}

		//////////////

		if((Joystick::XInputButton::XI_LB & nuevo) && !(Joystick::XInputButton::XI_LB & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_LB);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_LB & nuevo) && (Joystick::XInputButton::XI_LB & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_LB);
			buttonReleased(bt);
		}


		//////////////

		if((Joystick::XInputButton::XI_RB & nuevo) && !(Joystick::XInputButton::XI_RB & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_RB);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_RB & nuevo) && (Joystick::XInputButton::XI_RB & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_RB);
			buttonReleased(bt);
		}

		///////////////

		if((Joystick::XInputButton::XI_L3 & nuevo) && !(Joystick::XInputButton::XI_L3 & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_L3);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_L3 & nuevo) && (Joystick::XInputButton::XI_L3 & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_L3);
			buttonReleased(bt);
		}


		///////////////

		if((Joystick::XInputButton::XI_R3 & nuevo) && !(Joystick::XInputButton::XI_R3 & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_R3);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_R3 & nuevo) && (Joystick::XInputButton::XI_R3 & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_R3);
			buttonReleased(bt);
		}


		///////////////

		if((Joystick::XInputButton::XI_START & nuevo) && !(Joystick::XInputButton::XI_START & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_START);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_START & nuevo) && (Joystick::XInputButton::XI_START & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_START);
			buttonReleased(bt);
		}

		///////////////

		if((Joystick::XInputButton::XI_BACK & nuevo) && !(Joystick::XInputButton::XI_BACK & viejo)) //si el boton A no esta en la mascara vieja pero si en la nueva (PRESSED)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_BACK);
			buttonPressed(bt);
		}
		else if(!(Joystick::XInputButton::XI_BACK & nuevo) && (Joystick::XInputButton::XI_BACK & viejo)) //Si NO en nuevo y SI en viejo (Released)
		{
			Joystick::Button bt = xinputButton2galeon(Joystick::XInputButton::XI_BACK);
			buttonReleased(bt);
		}

		_joystickState._button = nuevo;

	}//captureXinputButtons

	//-----------------------------------------------

	void CInputManager::captureXinputAxis(short input, Joystick::Axis axis)
	{
		float abs = input;
		if((axis == Joystick::Axis::LX || axis == Joystick::Axis::LY) && !(abs < _LEFT_THRESHOLD * _MIN_STICK || abs > _LEFT_THRESHOLD * _MAX_STICK))
		{
			abs = 0.0f;
		}
		else if((axis == Joystick::Axis::RX || axis == Joystick::Axis::RY) && !(abs < _RIGHT_THRESHOLD * _MIN_STICK || abs > _RIGHT_THRESHOLD * _MAX_STICK))
		{
			abs = 0.0f;
		}
		else
		{
			abs /= _MAX_STICK;
		}

		float movement = 0.0f;
		switch (axis)
		{
		case GUI::Joystick::LY:
			movement = abs - _joystickState._LYabs;
			_joystickState._LYMov = movement;
			_joystickState._LYabs = abs;
			break;
		case GUI::Joystick::LX:
			movement = abs - _joystickState._LXabs;
			_joystickState._LXMov = movement;
			_joystickState._LXabs = abs;
			break;
		case GUI::Joystick::RY:
			{
				movement = abs - _joystickState._RYabs;
				_joystickState._RYMov = movement;
				_joystickState._RYabs = abs;
			}
			break;
		case GUI::Joystick::RX:
			{
				movement = abs - _joystickState._RXabs;
				_joystickState._RXMov = movement;
				_joystickState._RXabs = abs;
			}
			break;
		}
		/*if(abs!=0)
			BaseSubsystems::Log::Debug("AXIS: " + std::to_string(axis) + " - " + std::to_string(abs) + " - " + std::to_string(movement));*/
		axisMoved(axis, abs, movement);

	}///captureXinputAxis

	//-------------------------------------------------

	Joystick::Button CInputManager::xinputButton2galeon(Joystick::XInputButton button)
	{
		switch (button)
		{
		case GUI::Joystick::XI_UNASSIGNED:
			return Joystick::Button::UNASSIGNED;
			break;
		case GUI::Joystick::XI_A:
			return Joystick::Button::A;
			break;
		case GUI::Joystick::XI_B:
			return Joystick::Button::B;
			break;
		case GUI::Joystick::XI_X:
			return Joystick::Button::X;
			break;
		case GUI::Joystick::XI_Y:
			return Joystick::Button::Y;
			break;
		case GUI::Joystick::XI_LB:
			return Joystick::Button::LB;
			break;
		case GUI::Joystick::XI_RB:
			return Joystick::Button::RB;
			break;
		case GUI::Joystick::XI_BACK:
			return Joystick::Button::BACK;
			break;
		case GUI::Joystick::XI_START:
			return Joystick::Button::START;
			break;
		case GUI::Joystick::XI_L3:
			return Joystick::Button::L3;
			break;
		case GUI::Joystick::XI_R3:
			return Joystick::Button::R3;
			break;
		default:
			return Joystick::Button::UNASSIGNED;
			break;
		}
	}

	Joystick::POV CInputManager::xinputPOV2galeon(Joystick::XInputPOV pov)
	{
		switch (pov)
		{
		case GUI::Joystick::XI_CENTERED:
			return Joystick::POV::CENTERED;
			break;
		case GUI::Joystick::XI_NORTH:
			return Joystick::POV::NORTH;
			break;
		case GUI::Joystick::XI_SOUTH:
			return Joystick::POV::SOUTH;
			break;
		case GUI::Joystick::XI_WEST:
			return Joystick::POV::WEST;
			break;
		case GUI::Joystick::XI_EAST:
			return Joystick::POV::EAST;
			break;
		case GUI::Joystick::XI_NORTHEAST:
			return Joystick::POV::NORTHEAST;
			break;
		case GUI::Joystick::XI_SOUTHEAST:
			return Joystick::POV::SOUTHEAST;
			break;
		case GUI::Joystick::XI_NORTHWEST:
			return Joystick::POV::NORTHWEST;
			break;
		case GUI::Joystick::XI_SOUTHWEST:
			return Joystick::POV::SOUTHWEST;
			break;
		default:
			return Joystick::POV::CENTERED;
			break;
		}
	}
	
} // namespace GUI