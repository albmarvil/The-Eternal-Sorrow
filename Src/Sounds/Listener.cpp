/*
@filename Listener.h

Contiene la definición de un Listener en FMOD Studio.

@author Alberto Martínez Villarán
@date Marzo, 2015
*/


#include "Listener.h"

#include "Server.h"
#include "BaseSubsystems/Log.h"
#include <cassert>

//includes de FMOD
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.hpp>

namespace Sounds
{
	///definición nazi de ERRCHECK sacado de los samples
	void ERRCHECK_listener(FMOD_RESULT result, const char *file, int line);
	#define ERRCHECK(_result) ERRCHECK_listener(_result, __FILE__, __LINE__)

	void ERRCHECK_listener(FMOD_RESULT result, const char *file, int line)
	{
#ifdef _DEBUG
		if (result != FMOD_OK)
		{
			BaseSubsystems::Log::Error(FMOD_ErrorString(result));
		}
#endif
	}

	int ListenerID::nextId = 0;

	CListener::CListener()
	{
		_server = CServer::getSingletonPtr();
		if(_server)
		{
			_ID = ListenerID::NextID();
			//_position.x = 0;
			//_position.y = 0;
			//_position.z = 0;

			//_velocity.x = 0;
			//_velocity.y = 0;
			//_velocity.z = 0;

			////Por defecto el forward es siguiendo el eje Z
			//_forward.x = 0;
			//_forward.y = 0;
			//_forward.z = 1;

			////Por defecto el forward es siguiendo el eje Y
			//_up.x = 0;
			//_up.y = 1;
			//_up.z = 0;

			_position = Vector3::ZERO;
			_velocity = Vector3::ZERO;
			_forward = Vector3(0, 0, 1);
			_up = Vector3(0, 1, 0);

			_server->AddListener(this);
		}
	}

	CListener::~CListener()
	{
		_server->RemoveListener(this);
		_ID = -1;
		_server = 0;
	}


	void CListener::setPosition(const Vector3 &position)
	{
		/*_position.x = position.x;
		_position.y = position.y;
		_position.z = position.z;*/
		//BaseSubsystems::Log::Debug("Listener pos: "+std::to_string(position.x)+" "+std::to_string(position.y)+" "+std::to_string(position.z));
		_position = position;
	}

	void CListener::setVelocity(const Vector3 &velocity)
	{
		/*_velocity.x = velocity.x;
		_velocity.y = velocity.y;
		_velocity.z = velocity.z;*/
		_velocity = velocity;
	}


	void CListener::setForwardDirection(const Vector3 &forward)
	{
		/*_forward.x = forward.x;
		_forward.y = forward.y;
		_forward.z = forward.z;*/
		_forward = forward;
	}

	void CListener::setUpDirection(const Vector3 &up)
	{
		/*_up.x = up.x;
		_up.y = up.y;
		_up.z = up.z;*/
		_up = up;
	}

	void CListener::tick(unsigned int msecs)
	{
		/*FMOD_VECTOR pos;
		pos.x = _position.x;
		pos.y = _position.y;
		pos.z = _position.z;

		FMOD_VECTOR vel;
		vel.x = _velocity.x;
		vel.y = _velocity.y;
		vel.z = _velocity.z;

		FMOD_VECTOR fwd;
		fwd.x = _forward.x;
		fwd.y = _forward.y;
		fwd.z = _forward.z;

		FMOD_VECTOR up;
		up.x = _up.x;
		up.y = _up.y;
		up.z = _up.z;*/

		FMOD_3D_ATTRIBUTES attributes;
		attributes.position.x = _position.x;
		attributes.position.y = _position.y;
		attributes.position.z = _position.z;

		attributes.velocity.x = _velocity.x;
		attributes.velocity.y = _velocity.y;
		attributes.velocity.z = _velocity.z;

		attributes.forward.x = _forward.x;
		attributes.forward.y = _forward.y;
		attributes.forward.z = _forward.z;

		attributes.up.x = _up.x;
		attributes.up.y = _up.y;
		attributes.up.z = _up.z;

		FMOD_RESULT result = _server->getStudioSystem()->setListenerAttributes(&attributes);//_server->getLowSystem()->set3DListenerAttributes(_ID, &pos, &vel, &fwd, &up);
		ERRCHECK(result);
	}

};//namespace Sounds