/**
@file DynamicMovement.cpp

Contiene distintas clases que heredan de IMovement y que 
implementan distintos tipos de movimiento dinámico.

@author Gonzalo Flórez
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

		//Obtenemos la distancia que hay desde el destino hasta la posicion. Y esa será la aceleración que habria que tomar para llegar desde
		//la entidad hacia el destino

		currentProperties.linearAccel = _target - _entity->getPosition();

		//Si estamos a más distancia de la que podemos ir en este tick, entonces, la igualamos a la máxima.
		if (currentProperties.linearAccel.squaredLength() > _maxLinearAccel2)
		{
			//normalizamos el vector, y obtenemos el vector de direccion, y lo multiplicamos por la aceleración máxima posible que puede tomar la entidad
			//Este valor, será la aceleración de salida.
			currentProperties.linearAccel.normalise();
			currentProperties.linearAccel *= _maxLinearAccel;
		}
	}

	/////////////////////////////////////////
	/**
	Efectúa el movimiento.
		
	@param msecs Tiempo que dura el movimiento.
	@param currentProperties Parámetro de entrada/salida donde se reciben las velocidades actuales y 
	en él devuelve los nuevos valores de aceleración.
	*/
	void CDynamicArrive::move(unsigned int msecs, DynamicMovement &currentProperties)
	{
		assert(_entity);
		// Implementar el movimiento Arrive (dinámico).
		// En _entity tenemos un puntero a la entidad.
		// En _maxLinearSpeed tenemos la velocidad máxima a la que se puede mover la entidad y 
		// en _maxLinearAccel la aceleración máxima.
		// Otras constantes útiles son IMovement::SLOW_RADIUS (radio a partir del cual empezamos a frenar
		// para aproximarnos a un punto) y IMovement::ATTENUATION (atenuación de la aceleración).
		// currentProperties es un parámetro de entrada/salida en el que se recibe las velocidades/aceleraciones
		// actuales y se modifica con los nuevos valores de velocidad/aceleración.

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

		// Para que se vea mejor el efecto de la aceleración, la atenuamos
		currentProperties.linearAccel/= IMovement::ATTENUATION;

		if (currentProperties.linearAccel.squaredLength() > _maxLinearAccel2) 
		{
			currentProperties.linearAccel.normalise();
			currentProperties.linearAccel *= _maxLinearAccel;
		}
	}

	/**
	Efectúa el movimiento.
	@param msecs Tiempo que dura el movimiento.
	@param currentProperties Parámetro de entrada/salida donde se reciben las velocidades actuales y 
	en él devuelve los nuevos valores de aceleración.
	*/
	void CDynamicFlee::move(unsigned int msecs, DynamicMovement& currentProperties)
	{
		assert(_entity);
		// Implementar el movimiento Flee (dinámico)
		// En _entity tenemos un puntero a la entidad.
		// En _maxLinearAccel tenemos la aceleración máxima que puede tener la entidad.
		// currentProperties es un parámetro de entrada/salida en el que se recibe las velocidades/aceleraciones
		// actuales y se modifica con los nuevos valores de velocidad/aceleración.

		currentProperties.linearAccel = _entity->getPosition() - _target;
		currentProperties.linearAccel.normalise();
		currentProperties.linearAccel *= _maxLinearAccel;
	}
}