/**
@file Components/Component.cpp

Contiene la implementación de la clase base de los componentes.
 
@see Logic::IComponent
@see Logic::CCommunicationPort

@author David Llansó
@date Julio, 2010
*/

#include "Component.h"
#include "Entity.h"

namespace Logic 
{
	bool IComponent::spawn(CEntity *entity, const Map::CEntity *entityInfo) 
	{
		_entity = entity;

		return OnSpawn(entityInfo);

	} // spawn
	
	//---------------------------------------------------------

	void IComponent::tick(unsigned int msecs)
	{
		processMessages();

		//Si la entidad no esta durmiendo y el componente al que se hace tick tampoco hago su tick
		if(!_entity->isSleeping() && !getSleeping())
		{
			onTick(msecs);
		}

	} // tick

	//---------------------------------------------------------
	void IComponent::setSleeping(bool _dormido)
	{
		_isSleeping = _dormido;

		if (!_isSleeping)
			onAwake();
		else
			onSleep();
	}

} // namespace Logic
