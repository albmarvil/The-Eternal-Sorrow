#include "TileFragile.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Map/MapEntity.h"
#include "LUA/ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CTileFragile);
	
	//---------------------------------------------------------

	bool CTileFragile::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if (entityInfo->hasAttribute("numTouchsToBreak"))
		{
			_numTouchsLeft = entityInfo->getIntAttribute("numTouchsToBreak");
			_canTouch = true;

		}

		if (entityInfo->hasAttribute("numHitsToBreak"))
		{
			_numHitsLeft = entityInfo->getIntAttribute("numHitsToBreak");
			_canHit = true;
		}

		if (entityInfo->hasAttribute("matBreak1"))
		{
			_matBreak1 = entityInfo->getStringAttribute("matBreak1");
		}

		if (entityInfo->hasAttribute("matBreak2"))
		{
			_matBreak2 = entityInfo->getStringAttribute("matBreak2");
		}

		if (entityInfo->hasAttribute("matBreak3"))
		{
			_matBreak3 = entityInfo->getStringAttribute("matBreak3");
		}

		return true;

	} // spawn


	bool CTileFragile::respawn(const Map::CEntity *entityInfo) 
	{
		if (entityInfo->hasAttribute("numTouchsToBreak"))
		{
			_numTouchsLeft = entityInfo->getIntAttribute("numTouchsToBreak");
		}

		if (entityInfo->hasAttribute("numHitsToBreak"))
		{
			_numHitsLeft = entityInfo->getIntAttribute("numHitsToBreak");
		}

		if (entityInfo->hasAttribute("matBreak1"))
		{
			_matBreak1 = entityInfo->getStringAttribute("matBreak1");
		}

		if (entityInfo->hasAttribute("matBreak2"))
		{
			_matBreak2 = entityInfo->getStringAttribute("matBreak2");
		}

		if (entityInfo->hasAttribute("matBreak3"))
		{
			_matBreak3 = entityInfo->getStringAttribute("matBreak3");
		}

		return true;

	} // respawn
	//---------------------------------------------------------


	bool CTileFragile::activate() 
	{
		_isTouched = false;
		_canDamageAgain = true;

		assignMaterial(); //<---------- cambiar material

		return true;

	} // activate
	//---------------------------------------------------------

	bool CTileFragile::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "TOUCHED" ||
			   message->getType() == "UNTOUCHED" ||
			   message->getType() == "DAMAGED"; 

	} // accept
	
	//---------------------------------------------------------

	void CTileFragile::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "TOUCHED" && _canTouch)
		{
			CEntity* ent = dynamic_cast<TOUCHED*>(message.get())->getEntidad();

			// Si ha tocado y no estaba siendo tocado, dañará al tile
			if (ent->getType() == "Player")
			{
				if (_canDamageAgain)
				{
					--_numTouchsLeft;

					assignMaterial(); //<---------- cambiar material
				}

				_canDamageAgain = false;
			}
		}

		else if (message->getType() == "UNTOUCHED" && _canTouch)
		{
			_canDamageAgain = true;

		}
		else if (message->getType() == "DAMAGED" && _canHit)
		{
			--_numHitsLeft;
		}
	} // process

	//---------------------------------------------------------------------------------

	void CTileFragile::onTick(unsigned int msecs)
	{
		if ((_canTouch && _numTouchsLeft <= 0) || (_canHit && _numHitsLeft <= 0))
		{
			Logic::CEntityFactory::getSingletonPtr()->createEntityByType("Chispa",_entity->getPosition(), _entity->getMap());
			Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","DestruirTiles");

			int randStones = rand() % 4 + 3;

			for (int i = 0; i < randStones; ++i)
			{
				int randTypeStone = rand () % 2;

				if (randTypeStone == 0)
					CEntityFactory::getSingletonPtr()->createEntityByType("Stone1",_entity->getCenterPosition(), _entity->getMap());
				else
					CEntityFactory::getSingletonPtr()->createEntityByType("Stone2",_entity->getCenterPosition(), _entity->getMap());
			}

			//cuando borra la entidad mandamos un mensaje "BROKEN" para que aparezca un objeto recogible
			std::shared_ptr<Logic::BROKEN> m(new Logic::BROKEN());
			_entity->emitMessage(m);
		}
		
	}

	void CTileFragile::assignMaterial()
	{
		if (_numTouchsLeft == 3)
		{
			std::shared_ptr<SET_MATERIAL>m(new SET_MATERIAL());
			m->setString(_matBreak1);
			_entity->emitMessage(m);

		}

		else if (_numTouchsLeft == 2)
		{
			std::shared_ptr<SET_MATERIAL>m(new SET_MATERIAL());
			m->setString(_matBreak2);
			_entity->emitMessage(m);
		}

		else if (_numTouchsLeft == 1)
		{
			std::shared_ptr<SET_MATERIAL>m(new SET_MATERIAL());
			m->setString(_matBreak3);
			_entity->emitMessage(m);
		}
	}

} // namespace Logic