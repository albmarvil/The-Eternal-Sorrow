/**
@file AltarFeedbackcontroller.cpp

Contiene la implementación de la clase CAltarFeedbackcontroller. Se encarga de mostrar al jugador que 
el enemigo que ha matado estaba dentro del radio del altar de sacrificio, y por tanto se acumula al
contador de enemigos muertos en el altar.

@author Bárbara Domínguez de la Torre
@date Junio, 2015
*/
#include "AltarFeedbackController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"

namespace Logic 
{
	IMP_FACTORY(CAltarFeedbackController);
	
	//---------------------------------------------------------

	bool CAltarFeedbackController::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("Velocidad"))
		{
			_vel = entityInfo->getFloatAttribute("Velocidad");
		}

		if(entityInfo->hasAttribute("RadioParada"))
		{
			_radio2 = entityInfo->getFloatAttribute("RadioParada");
			_radio2 *= _radio2;
		}
		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CAltarFeedbackController::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("Velocidad"))
		{
			_vel = entityInfo->getFloatAttribute("Velocidad");
		}

		if(entityInfo->hasAttribute("RadioParada"))
		{
			_radio2 = entityInfo->getFloatAttribute("RadioParada");
			_radio2 *= _radio2;
		}
		return true;
	}

	void CAltarFeedbackController::onTick(unsigned int msecs)
	{
		//calculamos el speed
		Vector3 speed = _direction * _vel;

		//calculamos la nueva posición
		Vector3 newPos = _entity->getPosition() + speed * msecs;

		//seteamos la nueva posición
		_entity->setPosition(newPos);

		//comprobamos si debemos parar el alma
		Vector3 distance = _pointDestino - _entity->getPosition();
		float sqrdistance = distance.squaredLength();

		if(sqrdistance <= _radio2)
		{
			//matamos la entidad
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}
	}

	void CAltarFeedbackController::Config(Vector3 posDest)
	{
		//guardamos la posición de destino que nos hará falta después
		_pointDestino = posDest;

		//calculamos la dirección de movimiento y la normalizamos
		_direction = _pointDestino - _entity->getPosition();
		_direction.normalise();
	}
} // namespace Logic

