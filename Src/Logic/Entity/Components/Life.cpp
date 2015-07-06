/**
@file Life.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "Life.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"

#include "Logic/Entity/Components/BarraLifeController.h"
namespace Logic 
{
	IMP_FACTORY(CLife);
	
	//---------------------------------------------------------

	bool CLife::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // Onspawn
	
	//---------------------------------------------------------

	bool CLife::respawn(const Map::CEntity *entityInfo) 
	{
		return true;
	} // respawn
	
	//---------------------------------------------------------

	void CLife::setLifeBar(CBarraLifeController *lifeBar)
	{ 
		_lifeBar = lifeBar;
		lifeBar->updateLifeBar(_life, _lifeMax);
		//BaseSubsystems::Log::Debug("Vida: "+std::to_string(_life)+" Vida Max: "+std::to_string(_lifeMax));
	}
	bool CLife::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "DAMAGED" || 
			   message->getType() == "INVINCIBLE" ||
			   message->getType() == "SET_STAT";
	} // accept
	
	//---------------------------------------------------------

	void CLife::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "INVINCIBLE")
		{
			_invincible = dynamic_cast<INVINCIBLE*>(message.get())->getBool();
		}
	} // process


	void CLife::onTick(unsigned int msecs)
	{
		if(_dead)
		{
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);	
		}
	}//tick


	bool CLife::activate()
	{
		_dead = false;
		_invincible = false;

		return true;
	}

	void CLife::deactivate()
	{
		_dead = false;
		_life = 0;
	}

	//--------------------------

	void CLife::ChangeLifeBar()
	{
	}

} // namespace Logic

