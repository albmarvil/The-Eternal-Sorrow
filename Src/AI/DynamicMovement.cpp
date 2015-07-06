/**
@file DynamicMovement.cpp

Contiene distintas clases que heredan de IMovement y que 
implementan distintos tipos de movimiento din�mico.

@author Gonzalo Fl�rez
@date Diciembre, 2010
*/
#include "DynamicMovement.h"
#include "Server.h"

namespace AI 
{
	/////////////////////////////////////////

	void CDynamicSeek::move(unsigned int msecs, DynamicMovement& currentProperties)
	{
		assert(_entity);

		//Obtenemos la distancia que hay desde el destino hasta la posicion. Y esa ser� la aceleraci�n que habria que tomar para llegar desde
		//la entidad hacia el destino

		currentProperties.linearAccel = _target - _entity->getPosition();

		//Si estamos a m�s distancia de la que podemos ir en este tick, entonces, la igualamos a la m�xima.
		if (currentProperties.linearAccel.squaredLength() > _maxLinearAccel2)
		{
			//normalizamos el vector, y obtenemos el vector de direccion, y lo multiplicamos por la aceleraci�n m�xima posible que puede tomar la entidad
			//Este valor, ser� la aceleraci�n de salida.
			currentProperties.linearAccel.normalise();
			currentProperties.linearAccel *= _maxLinearAccel;
		}
	}

	/////////////////////////////////////////
	/**
	Efect�a el movimiento.
		
	@param msecs Tiempo que dura el movimiento.
	@param currentProperties Par�metro de entrada/salida donde se reciben las velocidades actuales y 
	en �l devuelve los nuevos valores de aceleraci�n.
	*/
	void CDynamicArrive::move(unsigned int msecs, DynamicMovement &currentProperties)
	{
		assert(_entity);
		// Implementar el movimiento Arrive (din�mico).
		// En _entity tenemos un puntero a la entidad.
		// En _maxLinearSpeed tenemos la velocidad m�xima a la que se puede mover la entidad y 
		// en _maxLinearAccel la aceleraci�n m�xima.
		// Otras constantes �tiles son IMovement::SLOW_RADIUS (radio a partir del cual empezamos a frenar
		// para aproximarnos a un punto) y IMovement::ATTENUATION (atenuaci�n de la aceleraci�n).
		// currentProperties es un par�metro de entrada/salida en el que se recibe las velocidades/aceleraciones
		// actuales y se modifica con los nuevos valores de velocidad/aceleraci�n.

		Vector3 targetSpeed = _target - _entity->getPosition();
		float distance = targetSpeed.length();
		float targetMagnitude;

		if (distance >= IMovement::SLOW_RADIUS) 
		{
			targetMagnitude = _maxLinearSpeed;
		}
		else 
		{
			targetMagnitude = distance * _maxLinearSpeed / IMovement::SLOW_RADIUS;
		}

		targetSpeed /= distance;
		targetSpeed *= targetMagnitude;

		currentProperties.linearAccel = targetSpeed - currentProperties.linearSpeed;

		// Para que se vea mejor el efecto de la aceleraci�n, la atenuamos
		currentProperties.linearAccel/= IMovement::ATTENUATION;

		if (currentProperties.linearAccel.squaredLength() > _maxLinearAccel2) 
		{
			currentProperties.linearAccel.normalise();
			currentProperties.linearAccel *= _maxLinearAccel;
		}
	}

	/**
	Efect�a el movimiento.
	@param msecs Tiempo que dura el movimiento.
	@param currentProperties Par�metro de entrada/salida donde se reciben las velocidades actuales y 
	en �l devuelve los nuevos valores de aceleraci�n.
	*/
	void CDynamicFlee::move(unsigned int msecs, DynamicMovement& currentProperties)
	{
		assert(_entity);
		// Implementar el movimiento Flee (din�mico)
		// En _entity tenemos un puntero a la entidad.
		// En _maxLinearAccel tenemos la aceleraci�n m�xima que puede tener la entidad.
		// currentProperties es un par�metro de entrada/salida en el que se recibe las velocidades/aceleraciones
		// actuales y se modifica con los nuevos valores de velocidad/aceleraci�n.

		currentProperties.linearAccel = _entity->getPosition() - _target;
		currentProperties.linearAccel.normalise();
		currentProperties.linearAccel *= _maxLinearAccel;
	}
}