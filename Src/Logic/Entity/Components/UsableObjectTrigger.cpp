/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "UsableObjectTrigger.h"

#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CUsableObjectTrigger);
	
	//---------------------------------------------------------

	bool CUsableObjectTrigger::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(_entity->getEntityInfo()->hasAttribute("weaponEntityType"))
		{
			_weaponType = _entity->getEntityInfo()->getStringAttribute("weaponEntityType");
		}

		return true;

	} // spawn

	bool CUsableObjectTrigger::respawn(const Map::CEntity *entityInfo)
	{
		if(_entity->getEntityInfo()->hasAttribute("weaponEntityType"))
		{
			_weaponType = _entity->getEntityInfo()->getStringAttribute("weaponEntityType");
		}

		return true;

	} // spawn

	//---------------------------------------------------------

	bool CUsableObjectTrigger::activate() 
	{
		return true;
	} // respawn

	//---------------------------------------------------------

	bool CUsableObjectTrigger::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "TOUCHED" || 
			   message->getType() == "UNTOUCHED";
	} // accept
	
	//---------------------------------------------------------

	void CUsableObjectTrigger::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("TOUCHED") == 0)
		{
			CEntity *otherEntity = dynamic_cast<TOUCHED*>(message.get())->getEntidad();
		
			// Si tiene entida lógica, seguimos (los tiles colisionan pero no tienen, con esto los saltamos)
			if (otherEntity)
			{
				// Solo nos interesa el player
				if(otherEntity->getType().compare("Player") == 0)
				{
					if (_entity->getTag() == "weapon")
					{
						std::shared_ptr<TOUCHING_WEAPON> m(new TOUCHING_WEAPON());
						m->setEntity(_entity);
						m->setStringWeaponType(_weaponType);
						m->setStringUsableObjectType(_entity->getType());
						otherEntity->emitMessage(m);
					}
				}
			}
		}
		else if (message->getType().compare("UNTOUCHED") == 0)
		{
			CEntity *otherEntity = dynamic_cast<UNTOUCHED*>(message.get())->getEntidad();

			// Si tiene entida lógica, seguimos (los tiles colisionan pero no tienen, con esto los saltamos)
			if (otherEntity)
			{
				//primero comprobamos con que ha chocado
				if(otherEntity->getType().compare("Player") == 0)
				{
					if (_entity->getTag() == "weapon")
					{
						std::shared_ptr<UNTOUCHED> m(new UNTOUCHED());
						m->setEntity(_entity);
						otherEntity->emitMessage(m);
					}
				}
			}
		}
	} // process

	//---------------------------------------------------------

	void CUsableObjectTrigger::onTick(unsigned int msecs)
	{
	}


} // namespace Logic

