/**
@file CPushbackComponent.cpp

Contiene la implementacion del componente de retroceso. Es un componente genérico que acepta el mensaje PUSH y en su tick tiene la logica
relacionada con el movimiento originado pro dicho empuje

@see Logic::CPushbackComponent
@see Logic::IComponent

@author Alberto Martínez
@date Junio, 2015
*/



#include "PushbackComponent.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "PhysicController.h"


namespace Logic 
{
	IMP_FACTORY(CPushbackComponent);
	
	//---------------------------------------------------------

	bool CPushbackComponent::OnSpawn(const Map::CEntity *entityInfo) 
	{

		if(entityInfo->hasAttribute("accel"))
		{
			float accel = entityInfo->getFloatAttribute("accel");

			_accel = Vector3(accel, accel, 0);
		}
		
		return true;

	} // spawn


	bool CPushbackComponent::respawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("accel"))
		{
			float accel = entityInfo->getFloatAttribute("accel");

			_accel = Vector3(accel, accel, 0);
		}

		return true;

	} // respawn
	//---------------------------------------------------------


	bool CPushbackComponent::activate() 
	{
		
		_initSpeed = 0.0f;
		_pushDirection = Vector3::ZERO;
		_actualSpeed = Vector3::ZERO;
		_pushed = false;
		_falling = false;

		_controller = (CPhysicController*) _entity->getComponent("CPhysicController");

		assert(_controller);

		return true;

	} // activate
	//---------------------------------------------------------

	bool CPushbackComponent::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "PUSH" ||
			message->getType() == "FALLING";

	} // accept
	
	//---------------------------------------------------------

	void CPushbackComponent::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "PUSH")
		{
			PUSH* m = dynamic_cast<PUSH*>(message.get());
			push(m->getDirection(), m->getSpeed());
		}
		else if (message->getType() == "FALLING")
		{
			_falling = dynamic_cast<FALLING*>(message.get())->getBool();
		}

	} // process

	//---------------------------------------------------------------------------------

	void CPushbackComponent::onTick(unsigned int msecs)
	{
		
		if(_pushed)
		{
			if(!_falling)
				_actualSpeed.y -= _accel.y * msecs;

			if(_actualSpeed.y <= 0)
				_actualSpeed.y = 0.0f;

			//si hemos sido empujados a la derecha, restamos la aceleracion
			if(_pushDirection.x >= 0)
			{
				_actualSpeed.x -= _accel.x * msecs;
			}
			else//si hemos sido empujados a la izq, sumamos. Todo para encontrar el equilibrio en el 0,0,0
			{
				_actualSpeed.x += _accel.x * msecs;
			}

			///si empujado a la derecha y vel en x ya es negativa, nos paramos. Lo mismo para el lado izquierdo
			if((_pushDirection.x >= 0 && _actualSpeed.x <= 0) || (_pushDirection.x < 0 && _actualSpeed.x >= 0))
			{
				//BaseSubsystems::Log::Debug("Fin del empuje");
				_pushed = false;
				_actualSpeed = Vector3::ZERO;
				_initSpeed = 0.0f;
				_pushDirection = Vector3::ZERO;
			}

			Vector3 desp = _actualSpeed * msecs;

			_controller->avatarWalk(desp);


			///DEBUG
			//BaseSubsystems::Log::Debug("Vel: " + std::to_string(_actualSpeed.x) + ", " + std::to_string(_actualSpeed.y));
		}
		
	}


	void CPushbackComponent::push(const Vector3 &direction, float speed)
	{
		if(!_pushed)
		{
			_pushed = true;
			_initSpeed = speed;
			_pushDirection = direction;
			_pushDirection.normalise();

			_actualSpeed = _pushDirection * _initSpeed;

			//BaseSubsystems::Log::Debug("Dir: " + std::to_string(_pushDirection.x) + ", " + std::to_string(_pushDirection.y));
			//BaseSubsystems::Log::Debug("Vel: " + std::to_string(_actualSpeed.x) + ", " + std::to_string(_actualSpeed.y));
		}
	}

} // namespace Logic