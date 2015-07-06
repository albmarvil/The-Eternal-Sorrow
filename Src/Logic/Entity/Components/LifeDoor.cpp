/**
@file LifeDoor.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLifeDoor
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "LifeDoor.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Application/BaseApplication.h"
#include "LUA/ScriptManager.h"
#include "MovableText.h"
#include "AvatarController.h"
#include "Logic/Maps/Map.h"

namespace Logic 
{
	IMP_FACTORY(CLifeDoor);
	
	//---------------------------------------------------------

	bool CLifeDoor::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("life"))
		{
			_life = entityInfo->getFloatAttribute("life");
		}

		if(entityInfo->hasAttribute("particulaPuertaCerrada"))
		{
			_nombreParticulaPuertaCerrada = entityInfo->getStringAttribute("particulaPuertaCerrada");
		}

		_explosion = false;
		return true;

	} // Onspawn
	
	//---------------------------------------------------------

	bool CLifeDoor::respawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("life"))
		{
			_life = entityInfo->getFloatAttribute("life");
		}

		if(entityInfo->hasAttribute("particulaPuertaCerrada"))
		{
			_nombreParticulaPuertaCerrada = entityInfo->getStringAttribute("particulaPuertaCerrada");
		}

		_explosion = false;
		return true;
	} // respawn
	
	void CLifeDoor::deactivate()
	{

	}

	//---------------------------------------------------------

	bool CLifeDoor::activate()
	{
		bool is_door;
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("UpdateLifeDoor", _life, 0, is_door);

		if(is_door)
		{
			_particulaPuertaCerrada = CEntityFactory::getSingletonPtr()->createEntityByType(_nombreParticulaPuertaCerrada,_entity->getPosition(), _entity->getMap());
			///BaseSubsystems::Log::Debug("Particula puerta cerrada");
		}
		return true;
	}

	void CLifeDoor::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "DAMAGED")
		{
			//Cambiamos la vida de la puerta en lua, si es necesario
			bool is_door;
			float damage = dynamic_cast<DAMAGED*>(message.get())->getFloat();

			LUA::CScriptManager::GetPtrSingleton()->executeFunction("UpdateLifeDoor", _life, damage, is_door);

			//Si esta en el reto de puerta y la vida es positiva
			if(is_door && _life>=0)
			{
				//si el booleano es verdadero, entonces la vida cambia, sino, es que no nos importa
				//la vida de la puerta

				// Disminuir la vida de la entidad
				_life -= damage;

				CEntity* entityMovableText = Logic::CEntityFactory::getSingletonPtr()->createEntityByType("MovableTextDamage", Vector3(_entity->getPosition().x, _entity->getPosition().y, 40), _entity->getMap());
				CMovableText2 *comp = entityMovableText->getComponent<CMovableText2*>();
				comp->setText(std::to_string((int)damage));
				comp->setColor("blanco");
			}
			//Si todavia no hubo explosion y hay que romper la puerta, entonces explosion de puerta, shake, eliminado de puerta cerrada, y puesta
			//la particula de puerta abierta
			if(!_explosion && is_door && _life<= 0)
			{
				_explosion = true;
				CEntityFactory::getSingletonPtr()->createEntityByType("ExplosionPuerta",_entity->getCenterPosition(), _entity->getMap());

				int randStones = rand() % 4 + 3;

				for (int i = 0; i < randStones; ++i)
				{
					int randTypeStone = rand () % 2;

					if (randTypeStone == 0)
						CEntityFactory::getSingletonPtr()->createEntityByType("Stone1",_entity->getCenterPosition(), _entity->getMap());
					else
						CEntityFactory::getSingletonPtr()->createEntityByType("Stone2",_entity->getCenterPosition(), _entity->getMap());
				}

				Logic::CEntity* cam = _entity->getMap()->getEntityByType("Camera");

					std::shared_ptr<Logic::SHAKE> m(new Logic::SHAKE());
					m->setMaxShake(30.0f);
					m->setMinShake(2.0f);
					cam->emitMessage(m);

					CServer::getSingletonPtr()->getPlayer()->getComponent<CAvatarController*>()->movePlayerAtDoor();

					CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_particulaPuertaCerrada);

					_particulaPuertaAbierta = CEntityFactory::getSingletonPtr()->createEntityByType("ParticulaPuertaAbierta",_entity->getPosition(), _entity->getMap());
			}
		}
	} // process

} // namespace Logic

