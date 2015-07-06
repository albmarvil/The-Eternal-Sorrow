/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "LifeBrokenObjects.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Application/BaseApplication.h"
#include "LUA/ScriptManager.h"
#include "MovableText.h"
#include "Logic/Maps/Map.h"

namespace Logic 
{
	IMP_FACTORY(CLifeBrokenObjects);
	
	//---------------------------------------------------------

	bool CLifeBrokenObjects::OnSpawn(const Map::CEntity *entityInfo) 
	{
		bool ok =	entityInfo->hasAttribute("life") &&
					entityInfo->hasAttribute("objetosRompibles");

		assert(ok && "Error en el onspawn del componente lifeBrokenObjects");

		_life = entityInfo->getFloatAttribute("life");	

		_objetosRompiblesGenerar = entityInfo->getVectorSTLAttribute("objetosRompibles");

		return true;

	} // Onspawn
	
	//---------------------------------------------------------

	bool CLifeBrokenObjects::respawn(const Map::CEntity *entityInfo) 
	{
		bool ok =	entityInfo->hasAttribute("life") &&
					entityInfo->hasAttribute("objetosRompibles");

		assert(ok && "Error en el onspawn del componente lifeBrokenObjects");

		_life = entityInfo->getFloatAttribute("life");	

		_objetosRompiblesGenerar = entityInfo->getVectorSTLAttribute("objetosRompibles");

		return true;
	} // respawn
	
	void CLifeBrokenObjects::deactivate()
	{

	}

	//---------------------------------------------------------

	bool CLifeBrokenObjects::activate()
	{
		return true;
	}

	bool CLifeBrokenObjects::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "DAMAGED";
	}

	void CLifeBrokenObjects::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "DAMAGED")
		{
			float damage = dynamic_cast<DAMAGED*>(message.get())->getFloat();

			if(_life>=0)
			{
				_life -= damage;

				CEntity* entityMovableText = Logic::CEntityFactory::getSingletonPtr()->createEntityByType("MovableTextDamage", Vector3(_entity->getPosition().x, _entity->getPosition().y, 40), _entity->getMap());
				CMovableText2 *comp = entityMovableText->getComponent<CMovableText2*>();
				comp->setText(std::to_string((int)damage));
				comp->setColor("blanco");
			}

			if(_life<= 0)
			{
				//Instanciamos un humo 
				CEntityFactory::getSingletonPtr()->createEntityByType("HumoMuerteEnemigo",_entity->getCenterPosition(), _entity->getMap());
				
				//Instanciamos cada uno de los objetos rotos correspondiente del objeto rompible
				std::vector<std::string>::const_iterator itObjetos = _objetosRompiblesGenerar.begin();
				for (; itObjetos != _objetosRompiblesGenerar.end();++itObjetos)
				{
					CEntityFactory::getSingletonPtr()->createEntityByType(*itObjetos,_entity->getCenterPosition(), _entity->getMap());
				}

				//Creamos una pequeña vibracion de la camara
				Logic::CEntity* cam = _entity->getMap()->getEntityByType("Camera");

					std::shared_ptr<Logic::SHAKE> m(new Logic::SHAKE());
					m->setMaxShake(10.0f);
					m->setMinShake(2.0f);
					cam->emitMessage(m);

				//Eliminamos el objeto rompible
				CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);

				//Dropeamos objetos al romper el objeto
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("Drop","brokenObjectsNoTile",_entity->getCenterPosition());

				if (_entity->getType() == "pilaCalaveras") //Si es la pila de calaveras, generamos mas objetos
				{
					LUA::CScriptManager::GetPtrSingleton()->executeProcedure("Drop","brokenObjectsNoTile",_entity->getCenterPosition());
				}

				//Actualizamos en los logros un objeto rompible mas
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","DestruirObjetos");
			}
		}
	} // process

} // namespace Logic

