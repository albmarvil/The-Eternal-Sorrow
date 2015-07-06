/*
@filename Sound3D.h

Contiene la definición de un sonido con localizaciñon espacial en FMOD Studio.

@author Alberto Martínez Villarán
@date Marzo, 2015
@refactor Alberto Martínez Villarán
@date Mayo de 2015
*/

#ifndef __Sounds_Sound3D_H
#define __Sounds_Sound3D_H

#include <string>
#include "BaseSubsystems/Math.h"
#include <map>
#include "Sound.h"


namespace Sounds
{
	class CSound3D : public CSound
	{
	public:

		CSound3D(std::string &eventName, SoundType type);

		virtual ~CSound3D();

		void setPosition(const Vector3 &position);

		void setVelocity(const Vector3 &velocity);

		void setForwardDirection(const Vector3 &forward);

		void setUpDirection(const Vector3 &up);

		void tick (unsigned int msecs);

	protected:
		Vector3 _position;

		Vector3 _velocity;

		Vector3 _forward;

		Vector3 _up;

		//FMOD_3D_ATTRIBUTES _3DAttributes;

	};//CServer

};//namespace Listeners

#endif //__Listeners_Listener_H