/**
@file SmartObjectJump.cpp

Contiene la declaración de la clase del componente encargado de definir el comportamiento de un
SmartObject de salto.

@see Logic::CSmartObjectJump
@see Logic::IComponent

@author Alberto Martínez
@date Febrero, 2015
*/

#include "SmartObjectJump.h"

#include "EntityController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"


namespace Logic 
{
	IMP_FACTORY(CSmartObjectJump);
	
	//---------------------------------------------------------

	CSmartObjectJump::~CSmartObjectJump() 
	{
	} // ~CSmartObjectJump
	
	//---------------------------------------------------------

	bool CSmartObjectJump::OnSpawn(const Map::CEntity *entityInfo)
	{
		
		if(entityInfo->hasAttribute("jump_direction"))
		{
			_jumpDirection = entityInfo->getIntAttribute("jump_direction");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CSmartObjectJump::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("jump_direction"))
		{
			_jumpDirection = entityInfo->getIntAttribute("jump_direction");
		}
		return true;
	}

	bool CSmartObjectJump::activate()
	{
		

		return true;
	}

	void CSmartObjectJump::deactivate()
	{
		
	}

	//---------------------------------------------------------

	

	bool CSmartObjectJump::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "TOUCHED" ;

	} // accept
	
	//---------------------------------------------------------

	void CSmartObjectJump::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("TOUCHED") == 0)
		{
			//Si algo o alguien nos ha tocado tenemos que comprobar que ha sido un enemigo
			

			CEntity* entity = dynamic_cast<TOUCHED*>(message.get())->getEntidad();
			if(entity != NULL)
			{
				if(entity->getTag() == "enemy")//aqui bastaria con poner el tag de enemigo
				{
					//Comprobamos que la dirección en la que se mueve es la dirección de mi salto
					int enemyDirection = (entity->getComponent<CEntityController*>())->getDirActual();
					if(enemyDirection == _jumpDirection)//en este caso le mandamos el mensaje de salto
					{
						std::shared_ptr<ENEMY_JUMP> m(new ENEMY_JUMP());
						entity->emitMessage(m);
					}
				}
			}

		}
		

	} // process

} // namespace Logic

