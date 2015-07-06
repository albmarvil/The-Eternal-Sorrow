
#include "LifeButler.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/LifeEnemy.h"

namespace Logic 
{
	IMP_FACTORY(CLifeButler);
	
	//---------------------------------------------------------

	bool CLifeButler::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // Onspawn
	
	//---------------------------------------------------------

	bool CLifeButler::respawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // respawn
	
	//---------------------------------------------------------

	bool CLifeButler::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "DAMAGED" ||
			   message->getType() == "HEAL";
	}

	void CLifeButler::process(const std::shared_ptr<Logic::IMessage> &message)
	{
 		if(message->getType() == "DAMAGED")
		{
			CLifeEnemy* compLife = (CLifeEnemy*)_entity->getComponent("CLifeEnemy");

			if (!compLife->getInvincible())
			{	
				compLife->damageEntity(dynamic_cast<DAMAGED*>(message.get())->getFloat(),dynamic_cast<DAMAGED*>(message.get())->getString());
			}
		}

		else if (message->getType() == "HEAL")
		{
			CLifeEnemy* compLife = (CLifeEnemy*)_entity->getComponent("CLifeEnemy");

			compLife->healEntity(dynamic_cast<HEAL*>(message.get())->getFloat());
		}

	} // process

} // namespace Logic

