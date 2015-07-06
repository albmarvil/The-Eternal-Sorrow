/*
@filename Listener.h

Contiene la definición de un Listener en FMOD Studio.

@author Alberto Martínez Villarán
@date Marzo, 2015
*/

#ifndef __Sounds_Listener_H
#define __Sounds_Listener_H

#include <string>
#include "BaseSubsystems/Math.h"

namespace Sounds
{
	class CServer;
}

namespace Sounds
{

	class ListenerID
	{
	public:
		static int NextID(){ return ++nextId; }
	protected:
		static int nextId;
	};

	class CListener
	{
	public:

		CListener();

		virtual ~CListener();

		void setPosition(const Vector3 &position);

		void setVelocity(const Vector3 &velocity);

		void setForwardDirection(const Vector3 &forward);

		void setUpDirection(const Vector3 &up);

		void tick (unsigned int msecs);

	protected:

		CServer* _server;

		int _ID;

		Vector3 _position;

		Vector3 _velocity;

		Vector3 _forward;

		Vector3 _up;

	};//CServer

};//namespace Listeners

#endif //__Listeners_Listener_H