#include "BarrilTrigger.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"

namespace Logic 
{
	IMP_FACTORY(CBarrilTrigger);
	
	//---------------------------------------------------------

	bool CBarrilTrigger::OnSpawn(const Map::CEntity *entityInfo)
	{
		/*if(entityInfo->hasAttribute("damage"))
			_damage = entityInfo->getFloatAttribute("damage");

		touch = 0;*/
		return true;

	} // spawn

	//---------------------------------------------------------

	bool CBarrilTrigger::activate() 
	{
		if(_entity->getEntityInfo()->hasAttribute("damage"))
		{
			_damage = _entity->getEntityInfo()->getFloatAttribute("damage");
		}

		touch = 0;
		return true;
	} // respawn

	//---------------------------------------------------------

	bool CBarrilTrigger::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("TOUCHED") == 0)
		{
			return true;
		}

	} // accept
	
	//---------------------------------------------------------

	void CBarrilTrigger::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if((message->getType().compare("TOUCHED") == 0) && (touch == false))
		{
			Logic::CEntity *otherEntity = dynamic_cast<TOUCHED*>(message.get())->getEntidad();
		
			//primero comprobamos con que ha chocado
			if(otherEntity == NULL)
			{
				//ha colisionado con un tile, se destruye
				touch = true;
			}
			else
			{
				bool aux = otherEntity->getType().compare("NormalBullet") == 0 || 
					       otherEntity->getType().compare("SpecialBullet") == 0;

				if((otherEntity->getType().compare("Player") != 0) && (!aux))
				{
					//ha chocado contra un objetivo, enviamos un mensaje de daño
					std::shared_ptr<DAMAGED> m(new DAMAGED());
					m->setFloat(_damage);
					otherEntity->emitMessage(m);

					//se destruye la entidad
					touch = true;
				}
			}
		}
	} // process

	//---------------------------------------------------------

	void CBarrilTrigger::onTick(unsigned int msecs)
	{
		if(touch)
		{
			Logic::CEntityFactory::getSingletonPtr()->createEntityByType("Explosion", _entity->getPosition(),_entity->getMap());
			Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}
	}


} // namespace Logic
