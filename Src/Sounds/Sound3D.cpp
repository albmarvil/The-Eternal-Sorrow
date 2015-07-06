/*
@filename Sound3D.h

Contiene la definición de un Sonido 3D en FMOD Studio.

@author Alberto Martínez Villarán
@date Marzo, 2015
*/


#include "Sound3D.h"

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
	void ERRCHECK_sound3d(FMOD_RESULT result, const char *file, int line);
	#define ERRCHECK(_result) ERRCHECK_sound3d(_result, __FILE__, __LINE__)

	void ERRCHECK_sound3d(FMOD_RESULT result, const char *file, int line)
	{
#ifdef _DEBUG
		if (result != FMOD_OK)
		{
			BaseSubsystems::Log::Error(FMOD_ErrorString(result));
		}
#endif
	}


	CSound3D::CSound3D(std::string &eventName, SoundType type) : CSound(eventName, type)
	{
		//_server = CServer::getSingletonPtr();
		if(_server)
		{
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

			//_3DAttributes.position = _position;
			//_3DAttributes.velocity = _velocity;
			//_3DAttributes.forward = _forward;
			//_3DAttributes.up = _up;

			_position = Vector3::ZERO;
			_velocity = Vector3::ZERO;
			_forward = Vector3(0, 0, 1);
			_up = Vector3(0, 1, 0);

			_server->AddSound3D(this);
		}
	}

	CSound3D::~CSound3D()
	{
		_server->RemoveSound3D(this);
	}


	void CSound3D::setPosition(const Vector3 &position)
	{
		/*_position.x = position.x;
		_position.y = position.y;
		_position.z = position.z;
		_3DAttributes.position = _position;*/
		_position = position;
	}

	void CSound3D::setVelocity(const Vector3 &velocity)
	{
		/*_velocity.x = velocity.x;
		_velocity.y = velocity.y;
		_velocity.z = velocity.z;
		_3DAttributes.velocity = _velocity;*/
		_velocity = velocity;
	}


	void CSound3D::setForwardDirection(const Vector3 &forward)
	{
		/*_forward.x = forward.x;
		_forward.y = forward.y;
		_forward.z = forward.z;
		_3DAttributes.forward = _forward;*/

		_forward = forward;
	}

	void CSound3D::setUpDirection(const Vector3 &up)
	{
		/*_up.x = up.x;
		_up.y = up.y;
		_up.z = up.z;
		_3DAttributes.up = _up;*/
		_up= up;
	}

	void CSound3D::tick(unsigned int msecs)
	{

		bool is = false;
		ERRCHECK(_description->is3D(&is));

		if(is)
		{
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

			FMOD_RESULT result = _instance->set3DAttributes(&attributes);
			ERRCHECK(result);
		}
		
	}

};//namespace Sounds