/**
@file PlayerController.cpp

Contiene la implementación de la clase CPlayerController. Se encarga de
recibir eventos del teclado y del ratón y de interpretarlos para
mover al jugador.

@see GUI::CPlayerController

@author David Llansó
@date Agosto, 2010
*/

#include "PlayerController.h"
#include "InputManager.h"


#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Arms.h"
#include "Logic/Entity/Message.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "Logic/Entity/Components/AvatarController.h"
#include "LUA/ScriptManager.h"

#include "GUI/Server.h"
#include "BaseSubsystems/Server.h"

#include <CEGUI/CEGUI.h>

#include "Graphics/Server.h"
#include "Physics/Server.h"

#pragma region Includes basura
#include "Logic/Managers/MapManager.h"
#include "Logic/Maps/EntityFactory.h"
#include "Graphics/PhysicsDebugVisualization.h"
#include "Graphics/Scene.h"
#include "Graphics\Camera.h"
#include "Sounds\Server.h"
#pragma endregion


#include <cassert>

//#define TURN_FACTOR 0.001f

namespace GUI {

	CPlayerController::CPlayerController() : _controlledAvatar(0), _armComponent(0)
	{
	} // CPlayerController

	//--------------------------------------------------------

	CPlayerController::~CPlayerController()
	{
		deactivate();

	} // ~CPlayerController

	//-------------------------------------------------------
	
	void CPlayerController::setControlledAvatar(Logic::CEntity *controlledAvatar)
	{
		_controlledAvatar = controlledAvatar;
		if(_controlledAvatar)
		{
			_armComponent = (Logic::CArms*)_controlledAvatar->getComponent("CArms");
			//aqui podemos coger el looking direction que tiene el jugador e inicializarlos
			_lookingDirection = ((Logic::CAvatarController*) _controlledAvatar->getComponent("CAvatarController"))->getLookingDirection();
		}
	}

	void CPlayerController::activate()
	{		
		CInputManager::getSingletonPtr()->addKeyListener(this);
		CInputManager::getSingletonPtr()->addMouseListener(this);
		CInputManager::getSingletonPtr()->addJoystickListener(this);

		initMapaTeclas();

		//CInputManager::getSingletonPtr()->setJoystickAsMouse(false);

		_minRadioApuntado = 0.15f;
		_maxRadioApuntadoX = 0.25f;
		_maxRadioApuntadoY = 0.4f;
		
		std::map<ORDERS,bool>::iterator it = _mapaTeclas.begin();

		for(; it != _mapaTeclas.end(); ++it)
		{
			(*it).second = false;
		}

	} // activate

	//--------------------------------------------------------

	void CPlayerController::tick(unsigned int msecs)
	{
		if(_controlledAvatar)
		{
			CInputManager::getSingletonPtr()->addJoystickListener(this);

			unsigned int w = 0;
			unsigned int h = 0;
			BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(w, h);
			_mouseState.setExtents(w, h);

			//traducimos de posicion relativa de pantalla a posicion mundo
			float normX_2 = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().getPosition().d_x/w;
			float normY_2 = CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().getPosition().d_y/h;

			if(!_mapaTeclas[ORDERS::MOUSE_MOV])
			{
				if(!CInputManager::getSingletonPtr()->getJoystickAsMouse())
				{
					float x = 0.5f;
					float y = 0.5f;
					if(_joystickState._RXabs == 0.0f && _joystickState._RYabs == 0.0f)
					{
						if(normX_2 < 0.5f)
						{
							x -= _minRadioApuntado;
						}
						else
						{
							x += _minRadioApuntado;
						}
					}
					else
					{

						Vector2 v = Vector2( _joystickState._RXabs * _maxRadioApuntadoX, _joystickState._RYabs * _maxRadioApuntadoY);

						float dist = v.length();

						if(dist < _minRadioApuntado)
						{
							v.normalise();
							v *= _minRadioApuntado;
						}

						x += v.x;
						y += -v.y;
					}

					//traducimos de pos relativa a pos absoluta en pixeles
					unsigned int x_abs = x * w;
					unsigned int y_abs = y * h;

					_mouseState.setPosition(x_abs, y_abs);

					//BaseSubsystems::Log::Debug("pos: " + std::to_string(x_abs) + " - " + std::to_string(y_abs));
					CServer::getSingletonPtr()->injectMousePos(x_abs, y_abs);
			
				}

				//si el joystick se ha movido seteamos la animación de idle
				std::shared_ptr<Logic::CONTROL> m(new Logic::CONTROL());
				m->setString("movepointer");
				_controlledAvatar->emitMessage(m);
			}


			Vector3 point = GetRealPos(normX_2, normY_2);
			point.z = 0;

			//Notificamos de la posición del mundo del raton
			std::shared_ptr<Logic::MOUSE_POS> m(new Logic::MOUSE_POS());
			m->setVector3(point);
			_controlledAvatar->emitMessage(m);


			//Ahora que tenemos la posición en el mundo del ratón calculamo si el jugador debe girarse
			if((point.x > _controlledAvatar->getPosition().x) && (_lookingDirection == -1))
			{
				//enviamos un mensaje de giro
				std::shared_ptr<Logic::CONTROL> m(new Logic::CONTROL());

				//enviamos la posición para que pueda calcular donde está el ratón
				m->setString("turn");
				m->setInt(1);
				_controlledAvatar->emitMessage(m);
				_lookingDirection = 1;
			}
			else
			{
				if((point.x <= _controlledAvatar->getPosition().x) && (_lookingDirection == 1))
				{
					//enviamos un mensaje de giro
					std::shared_ptr<Logic::CONTROL> m(new Logic::CONTROL());

					//enviamos la posición para que pueda calcular donde está el ratón
					m->setString("turn");
					m->setInt(-1);
					_controlledAvatar->emitMessage(m);
					_lookingDirection = -1;
				}
			}
		}
		
	}//tick

	//--------------------------------------------------------------------

	void CPlayerController::deactivate()
	{
		CInputManager::getSingletonPtr()->removeKeyListener(this);
		CInputManager::getSingletonPtr()->removeMouseListener(this);
		CInputManager::getSingletonPtr()->removeJoystickListener(this);

		CInputManager::getSingletonPtr()->setJoystickAsMouse(true);


		/*_armComponent->attackOrder("Right", false);
		_armComponent->specialAttackOrder("Right", false);

		_armComponent->attackOrder("Left", false);
		_armComponent->specialAttackOrder("Left", false);*/



	} // deactivate

	//--------------------------------------------------------
	
	bool CPlayerController::keyPressed(TKey key)
	{		
		if(_controlledAvatar)
		{

			_mapaTeclas[ORDERS::KEYBOARD_USED] = true;

			//keyPressedTest(key); // DEBUG

			switch(key.keyId)
			{
				case GUI::Key::A:
					_mapaTeclas[ORDERS::MOVELEFT] = true;
					controlMovimiento();
					break;
				case GUI::Key::D:
					_mapaTeclas[ORDERS::MOVERIGHT] = true;
					controlMovimiento();
					break;
				case GUI::Key::SPACE:
					_mapaTeclas[ORDERS::JUMP] = true;
					controlSalto();
					break;
				case GUI::Key::W:
					_mapaTeclas[ORDERS::JUMP] = true;
					controlSalto();
					break;
				case GUI::Key::LSHIFT:
					{
						if(_mapaTeclas[ORDERS::RIGHT_ATTACK])
						{
							_armComponent->specialAttackOrder("Right", true);
						}

						if(_mapaTeclas[ORDERS::LEFT_ATTACK])
						{
							_armComponent->specialAttackOrder("Left", true);
						}
						

						_mapaTeclas[ORDERS::SPECIAL] = true;
					}
					
					break;
				case GUI::Key::E:
					{
						std::shared_ptr<Logic::USE> m(new Logic::USE());
						m->setBool(true);
						_controlledAvatar->emitMessage(m);
						
						_mapaTeclas[ORDERS::USE] = true;
					}
					break;
				case GUI::Key::X:
					{
						std::shared_ptr<Logic::SWAP_WEAPONS> m(new Logic::SWAP_WEAPONS());
						_controlledAvatar->emitMessage(m);
					}
					break;
				default:
					return false;
					break;
			}
			return false;
		}
		return false;

	} // keyPressed

	bool CPlayerController::keyPressedTest(TKey key)
	{
		// Aquí van todas las teclas que no son necesarias para el juego y por tanto solo son para pruebas

		switch(key.keyId)
		{
			case GUI::Key::U:
				//llamar al map manager y hacer cosas
				Logic::MapManager::getSingletonPtr()->nextMap();
				break;
			case GUI::Key::Q:
				{
					Logic::CEntity* cam = _controlledAvatar->getMap()->getEntityByType("Camera");

					std::shared_ptr<Logic::SHAKE> m(new Logic::SHAKE());
					m->setMaxShake(30.0f);
					m->setMinShake(2.0f);
					cam->emitMessage(m);
				}
				break;
			case GUI::Key::NUMBER5:
				//llamar al map manager y hacer cosas
				Logic::MapManager::getSingletonPtr()->nextAmbientation(Logic::Ambientation::CASTILLO);
				break;
			case GUI::Key::NUMBER6:
				//llamar al map manager y hacer cosas
				Logic::MapManager::getSingletonPtr()->nextAmbientation(Logic::Ambientation::LAVA);
				break;
			case GUI::Key::NUMBER7:
				//llamar al map manager y hacer cosas
				Logic::MapManager::getSingletonPtr()->nextAmbientation(Logic::Ambientation::SNOW);
				break;
			case GUI::Key::NUMBER8:
				//llamar al map manager y hacer cosas
				Logic::MapManager::getSingletonPtr()->nextAmbientation(Logic::Ambientation::MAZMORRA);
				break;
			case GUI::Key::NUMBER9:
				Logic::CEntityFactory::getSingletonPtr()->createEntityByType("UsableObjectKatana", _controlledAvatar->getPosition() + Vector3(50,10,0), Logic::CServer::getSingletonPtr()->getMap());
				break;
			case GUI::Key::NUMBER0:
				Logic::CEntityFactory::getSingletonPtr()->createEntityByType("UsableObjectAK47", _controlledAvatar->getPosition() + Vector3(50,10,0), Logic::CServer::getSingletonPtr()->getMap());
				break;
			case GUI::Key::K:
				LUA::CScriptManager::GetPtrSingleton()->executeScript("Data_Player.ChangeStatValue('velocidad_ataque', 1)");
				LUA::CScriptManager::GetPtrSingleton()->executeScript("Data_Player.CheckStatValues('velocidad_ataque', 1)");
				LUA::CScriptManager::GetPtrSingleton()->executeScript("Data_Player.ChangeCValues('velocidad_ataque', 1)");
				break;
			case GUI::Key::L:
				LUA::CScriptManager::GetPtrSingleton()->executeScript("Data_Player.ChangeStatValue('velocidad_ataque', -1)");
				LUA::CScriptManager::GetPtrSingleton()->executeScript("Data_Player.CheckStatValues('velocidad_ataque', -1)");
				LUA::CScriptManager::GetPtrSingleton()->executeScript("Data_Player.ChangeCValues('velocidad_ataque', -1)");
				break;
			default:
				return false;
		}

		return false;

	} // keyPressed

	//--------------------------------------------------------

	bool CPlayerController::keyReleased(TKey key)
	{
		if(_controlledAvatar)
		{
			_mapaTeclas[ORDERS::KEYBOARD_USED] = false;

			//keyReleasedTest(key); // DEBUG

			switch(key.keyId)
			{
				case GUI::Key::A:
					_mapaTeclas[ORDERS::MOVELEFT] = false;
					controlMovimiento();
					break;
				case GUI::Key::D:
					_mapaTeclas[ORDERS::MOVERIGHT] = false;
					controlMovimiento();
					break;
				case GUI::Key::SPACE:
					_mapaTeclas[ORDERS::JUMP] = false;
					controlSalto();
					break;
				case GUI::Key::W:
					_mapaTeclas[ORDERS::JUMP] = false;
					controlSalto();
					break;
				case GUI::Key::LSHIFT:
					{
						if(_mapaTeclas[ORDERS::RIGHT_ATTACK])
						{
							_armComponent->specialAttackOrder("Right", false);
						}

						if(_mapaTeclas[ORDERS::LEFT_ATTACK])
						{
							_armComponent->specialAttackOrder("Left", false);
						}
						

						_mapaTeclas[ORDERS::SPECIAL] = false;
					}
					break;
				case GUI::Key::E:
					{
						_mapaTeclas[ORDERS::USE] = false;
						std::shared_ptr<Logic::USE> m(new Logic::USE());
						m->setBool(false);
						_controlledAvatar->emitMessage(m);
					}
					break;
				default:
					return false;
					break;
			}

			return false;
		}

		return false;

	} // keyReleased

	bool CPlayerController::keyReleasedTest(TKey key)
	{
		
		return false;

	} // keyReleased

	//--------------------------------------------------------
	
	bool CPlayerController::mouseMoved(const CMouseState &mouseState)
	{

		//BaseSubsystems::Log::Debug("ABS: (" + std::to_string(mouseState.posAbsX) + ", " + std::to_string(mouseState.posAbsY)+")");
		//BaseSubsystems::Log::Debug("MOV: (" + std::to_string(mouseState.movX) + ", " + std::to_string(mouseState.movY)+")");

		GUI::CServer::getSingletonPtr()->mouseMoved(mouseState);
		//CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getMouseCursor().setImage("cursorImages/mirilla");

		/*if(mouseState.scrool != 0)
		{
			std::shared_ptr<Logic::CAMERA_ZOOM> m(new Logic::CAMERA_ZOOM());
			m->setZoom(mouseState.scrool * -0.1);
			_controlledAvatar->getMap()->getEntityByName("Camera")->emitMessage(m);
		}*/
		
		_mouseState = mouseState;
		
		_mapaTeclas[ORDERS::MOUSE_MOV] = true;

		//si el ratón se ha movido enviamos un mensaje para setear la animación a idle
		std::shared_ptr<Logic::CONTROL> m(new Logic::CONTROL());
		m->setString("movepointer");
		_controlledAvatar->emitMessage(m);
		
		return true;
	} // mouseMoved

	//---------------------------------------------------------


	Vector3 CPlayerController::GetRealPos(float posX, float posY)
	{
		//Vector donde guardaremos la posición en el mundo del ratón
		Vector3 point;
		//Obtener el rayo a lanzar
		//A esta función le pasamos la posición relativa del ratón en x e y, que previamente hemos guardado en el vector moveMouse
		Ray mouseRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(posX, posY);

		//Definimos la distancia máxima a la que lanzaremos el rayo
		float max_distance = 600;

		//lanzamos el rayo y guardamos en la variable point la posición actual en el mundo del ratón
		Physics::CServer::getSingletonPtr()->raycastSimple(mouseRay, max_distance, Physics::CollisionGroup::ePlanoColision, point);


		return point;
	}
	
	//--------------------------------------------------------
		
	bool CPlayerController::mousePressed(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			_mapaTeclas[ORDERS::MOUSE_USED] = true;
			std::string mouseButton;

			// Detectamos el brazo
			if(mouseState.button == GUI::TButton::LEFT)
			{
				mouseButton = "Left";
				_mapaTeclas[ORDERS::LEFT_ATTACK] = true;
			}
			if(mouseState.button == GUI::TButton::RIGHT)
			{
				mouseButton = "Right";
				_mapaTeclas[ORDERS::RIGHT_ATTACK] = true;
			}

			// Solo si mantiene pulsado E se enviará el mensaje de coger el arma
			if (_mapaTeclas[ORDERS::USE])
			{
				std::shared_ptr<Logic::GRAB_WEAPON> m(new Logic::GRAB_WEAPON());
				m->setString(mouseButton);
				_controlledAvatar->emitMessage(m);

			}
			else if(_mapaTeclas[ORDERS::SPECIAL])
			{
				_armComponent->specialAttackOrder(mouseButton, true);
			}
			else
			{
				_armComponent->attackOrder(mouseButton, true);
			}

			return true;
		}

		return false;

	} // mousePressed

	//--------------------------------------------------------

	bool CPlayerController::mouseReleased(const CMouseState &mouseState)
	{
		if(_controlledAvatar)
		{
			_mapaTeclas[ORDERS::MOUSE_USED] = false;
			std::string mouseButton;

			
			// Detectamos el brazo
			if(mouseState.button == GUI::TButton::LEFT)
			{
				mouseButton = "Left";
				_mapaTeclas[ORDERS::LEFT_ATTACK] = false;
			}
			if(mouseState.button == GUI::TButton::RIGHT)
			{
				mouseButton = "Right";
				_mapaTeclas[ORDERS::RIGHT_ATTACK] = false;
			}

			_armComponent->attackOrder(mouseButton, false);
			_armComponent->specialAttackOrder(mouseButton, false);
			
			return true;
		}
		return false;

	} // mouseReleased

	//--------------------------------------------------------


	bool CPlayerController::povMoved(Joystick::POV pov)
	{
		if(Joystick::POV::NORTH == pov)///si contiene el flag NORTH
		{
			//mandamos mensaje de salto
			_mapaTeclas[ORDERS::JUMP] = true;
			_mapaTeclas[ORDERS::POV_USED] = true;
			controlSalto();
		}

		if(Joystick::POV::EAST & pov)///si contiene el flag EAST
		{
			_mapaTeclas[ORDERS::MOVERIGHT] = true;
			_mapaTeclas[ORDERS::MOVELEFT] = false;
			_mapaTeclas[ORDERS::POV_USED] = true;
			controlMovimiento();
		}
		else if(Joystick::POV::WEST & pov)
		{
			_mapaTeclas[ORDERS::MOVELEFT] = true;
			_mapaTeclas[ORDERS::MOVERIGHT] = false;
			_mapaTeclas[ORDERS::POV_USED] = true;
			controlMovimiento();
		}
		else if(Joystick::POV::CENTERED == pov)
		{
			_mapaTeclas[ORDERS::JUMP] = false;
			_mapaTeclas[ORDERS::MOVELEFT] = false;
			_mapaTeclas[ORDERS::MOVERIGHT] = false;
			_mapaTeclas[ORDERS::POV_USED] = false;
			controlMovimiento();
		}

		return false;
	}

	bool CPlayerController::axisMoved(Joystick::Axis axis, float abs, float movement)
	{
		switch (axis)
		{
		case Joystick::Axis::LX:
			{
				if(!_mapaTeclas[ORDERS::POV_USED] && !_mapaTeclas[ORDERS::KEYBOARD_USED]) //si no estamos usando la cruceta
				{
					if(abs > 0)//derecha
					{
						_mapaTeclas[ORDERS::MOVERIGHT] = true;
						_mapaTeclas[ORDERS::JOYSTICK_USED]  = true;
						controlMovimiento();
					}
					else if(abs < 0)
					{
						_mapaTeclas[ORDERS::MOVELEFT] = true;
						_mapaTeclas[ORDERS::JOYSTICK_USED]  = true;
						controlMovimiento();
					}
					else if((_mapaTeclas[ORDERS::MOVELEFT] || _mapaTeclas[ORDERS::MOVERIGHT]) && _mapaTeclas[ORDERS::JOYSTICK_USED])
					{
						_mapaTeclas[ORDERS::MOVELEFT] = false;
						_mapaTeclas[ORDERS::MOVERIGHT] = false;
						_mapaTeclas[ORDERS::JOYSTICK_USED]  = false;
						controlMovimiento();
					}
				}
			}
			break;
		case Joystick::Axis::LY:
			{
				if(!_mapaTeclas[ORDERS::POV_USED] && !_mapaTeclas[ORDERS::KEYBOARD_USED]) //si no estamos usando la cruceta
				{
					if(abs > 0.75) //saltamos
					{
						_mapaTeclas[ORDERS::JUMP] = true;
						controlSalto();
					}
					else if(_mapaTeclas[ORDERS::JUMP])
					{
						_mapaTeclas[ORDERS::JUMP] = false;
					}
				}
			}
			break;
		case Joystick::Axis::RZ:
			{
				if(abs > 0.0f)
				{
					/*if (_mapaTeclas[ORDERS::USE])
					{
						std::shared_ptr<Logic::GRAB_WEAPON> m(new Logic::GRAB_WEAPON());
						m->setString("Right");
						_controlledAvatar->emitMessage(m);
					}
					else
					{
						_armComponent->attackOrder("Right", true);
						_mapaTeclas[ORDERS::RIGHT_ATTACK] = true;
					}*/

					_armComponent->specialAttackOrder("Right", true);
					_mapaTeclas[ORDERS::RIGHT_ATTACK_SP] = true;
					
				}
				else if(abs == 0 && _mapaTeclas[ORDERS::RIGHT_ATTACK_SP] && !_mapaTeclas[ORDERS::MOUSE_USED])
				{
					_armComponent->specialAttackOrder("Right", false);
					_mapaTeclas[ORDERS::RIGHT_ATTACK_SP] = false;
				}
				
			}
			break;
		case Joystick::Axis::LZ:
			{
				if(abs > 0.0f)
				{
					/*if (_mapaTeclas[ORDERS::USE])
					{
						std::shared_ptr<Logic::GRAB_WEAPON> m(new Logic::GRAB_WEAPON());
						m->setString("Left");
						_controlledAvatar->emitMessage(m);
					}
					else
					{
						_armComponent->attackOrder("Left", true);
						_mapaTeclas[ORDERS::LEFT_ATTACK] = true;
					}*/
					_armComponent->specialAttackOrder("Left", true);
					_mapaTeclas[ORDERS::LEFT_ATTACK_SP] = true;
				}
				else if(abs == 0 && _mapaTeclas[ORDERS::LEFT_ATTACK_SP] && !_mapaTeclas[ORDERS::MOUSE_USED])
				{
					_armComponent->specialAttackOrder("Left", false);
					_mapaTeclas[ORDERS::LEFT_ATTACK_SP] = false;
				}
				
			}
			break;
		case Joystick::Axis::RY:
			if(_mapaTeclas[ORDERS::MOUSE_MOV] && abs != 0)
			{
				_mapaTeclas[ORDERS::MOUSE_MOV] = false;
			}
			_joystickState._RYabs = abs;
			_joystickState._RYMov = movement;
			break;
		case Joystick::Axis::RX:
			if(_mapaTeclas[ORDERS::MOUSE_MOV] && abs != 0)
			{
				_mapaTeclas[ORDERS::MOUSE_MOV] = false;
			}
			_joystickState._RXabs = abs;
			_joystickState._RXMov = movement;
			break;
		}
		return false;
	}

	bool CPlayerController::buttonPressed(Joystick::Button button)
	{
		switch (button)
		{
		case GUI::Joystick::UNASSIGNED:
			break;
		case GUI::Joystick::A:
			_mapaTeclas[ORDERS::JUMP] = true;
			controlSalto();
			break;
		case GUI::Joystick::B:
			break;
		case GUI::Joystick::X:
			{
				std::shared_ptr<Logic::USE> m(new Logic::USE());
				m->setBool(true);
				_controlledAvatar->emitMessage(m);
				_mapaTeclas[ORDERS::USE] = true;
			}
			break;
		case GUI::Joystick::Y:
			{
				std::shared_ptr<Logic::SWAP_WEAPONS> m(new Logic::SWAP_WEAPONS());
				_controlledAvatar->emitMessage(m);
			}
			break;
		case GUI::Joystick::LB:
			//_armComponent->specialAttackOrder("Left", true);
			if (_mapaTeclas[ORDERS::USE])
			{
				std::shared_ptr<Logic::GRAB_WEAPON> m(new Logic::GRAB_WEAPON());
				m->setString("Left");
				_controlledAvatar->emitMessage(m);
			}
			else
			{
				_armComponent->attackOrder("Left", true);
				_mapaTeclas[ORDERS::LEFT_ATTACK] = true;
			}
			break;
		case GUI::Joystick::RB:
			//_armComponent->specialAttackOrder("Right", true);
			if (_mapaTeclas[ORDERS::USE])
			{
				std::shared_ptr<Logic::GRAB_WEAPON> m(new Logic::GRAB_WEAPON());
				m->setString("Right");
				_controlledAvatar->emitMessage(m);
			}
			else
			{
				_armComponent->attackOrder("Right", true);
				_mapaTeclas[ORDERS::RIGHT_ATTACK] = true;
			}
			break;
		case GUI::Joystick::BACK:
			break;
		case GUI::Joystick::START:
			break;
		case GUI::Joystick::L3:
			_mapaTeclas[ORDERS::JUMP] = true;
			controlSalto();
			break;
		case GUI::Joystick::R3:
			{
				std::shared_ptr<Logic::USE> m(new Logic::USE());
				m->setBool(true);
				_controlledAvatar->emitMessage(m);
				_mapaTeclas[ORDERS::USE] = true;
			}
			break;
		}
		return false;
	}

	bool CPlayerController::buttonReleased(Joystick::Button button)
	{
		switch (button)
		{
		case GUI::Joystick::UNASSIGNED:
			break;
		case GUI::Joystick::A:
			_mapaTeclas[ORDERS::JUMP] = false;
			break;
		case GUI::Joystick::B:
			break;
		case GUI::Joystick::X:
			_mapaTeclas[ORDERS::USE] = false;
			break;
		case GUI::Joystick::Y:
			break;
		case GUI::Joystick::LB:
			//_armComponent->specialAttackOrder("Left", false);
			_armComponent->attackOrder("Left", false);
			_mapaTeclas[ORDERS::LEFT_ATTACK] = false;
			break;
		case GUI::Joystick::RB:
			//_armComponent->specialAttackOrder("Right", false);
			_armComponent->attackOrder("Right", false);
			_mapaTeclas[ORDERS::RIGHT_ATTACK] = false;
			break;
		case GUI::Joystick::BACK:
			break;
		case GUI::Joystick::START:
			break;
		case GUI::Joystick::L3:
			_mapaTeclas[ORDERS::JUMP] = false;
			break;
		case GUI::Joystick::R3:
			_mapaTeclas[ORDERS::USE] = false;
			break;
		}
		return false;
	}

	void CPlayerController::controlMovimiento()
	{
		std::string control = "";
		int dir = 0;

		// Si solo estamos pulsando A nos movemos a la izquierda
		if (_mapaTeclas[ORDERS::MOVELEFT] && !_mapaTeclas[ORDERS::MOVERIGHT])
		{
			control = "walk";
			dir = -1;
		}

		// Si solo estamos pulsando D nos movemos a la derecha
		if (!_mapaTeclas[ORDERS::MOVELEFT] && _mapaTeclas[ORDERS::MOVERIGHT])
		{
			control = "walk";
			dir = 1;
		}

		// Si se pulsan ambas direcciones a la vez o no se pulsa ninguna, se frena
		if (!_mapaTeclas[ORDERS::MOVELEFT] && !_mapaTeclas[ORDERS::MOVERIGHT] || _mapaTeclas[ORDERS::MOVELEFT] && _mapaTeclas[ORDERS::MOVERIGHT])
		{
			control = "stopWalk";

			//enviamos el mensaje para que pare de contar
			std::shared_ptr<Logic::START_COUNT> m(new Logic::START_COUNT());
			m->setString("walk");
			_controlledAvatar->emitMessage(m);
		}

		if (control != "") // Si ha habido al menos un cambio de control, enviamos mensaje
		{
			std::shared_ptr<Logic::CONTROL> m(new Logic::CONTROL());
			m->setString(control);
			m->setInt(dir);
			_controlledAvatar->emitMessage(m);
		}
	}

	//--------------------------------------------------------

	void CPlayerController::controlSalto()
	{
		std::shared_ptr<Logic::CONTROL> m (new Logic::CONTROL());
		m->setString("jump");
		m->setBool(_mapaTeclas[ORDERS::JUMP]);
		_controlledAvatar->emitMessage(m);

		if(!_mapaTeclas[ORDERS::JUMP])
		{
			//enviamos el mensaje para que pare de contar
			std::shared_ptr<Logic::START_COUNT> m(new Logic::START_COUNT());
			m->setString("jump");
			_controlledAvatar->emitMessage(m);
		}
	}
	//--------------------------------------------------------

	void CPlayerController::initMapaTeclas()
	{
		_mapaTeclas[ORDERS::MOVELEFT] = false;
		_mapaTeclas[ORDERS::MOVERIGHT] = false;
		_mapaTeclas[ORDERS::MOUSE_MOV] = true;
	}


	

} // namespace GUI
