#include "AltStatCanibal.h"

#include "Logic/Entity/Entity.h"
#include "LUA/ScriptManager.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"

#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CAltStatCanibal);
	
	//---------------------------------------------------------

	CAltStatCanibal::~CAltStatCanibal()
	{
	}

	bool CAltStatCanibal::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("material"))
		{
			_material = entityInfo->getStringAttribute("material");
		}
		else
		{
			return false;
		}

		if(entityInfo->hasAttribute("materialCanibal"))
		{
			_canibalMaterial = entityInfo->getStringAttribute("materialCanibal");
		}
		else
		{
			return false;
		}

		return true;

	} // Onspawn

	bool CAltStatCanibal::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("material"))
		{
			_material = entityInfo->getStringAttribute("material");
		}
		else
		{
			return false;
		}

		if(entityInfo->hasAttribute("materialCanibal"))
		{
			_canibalMaterial = entityInfo->getStringAttribute("materialCanibal");
		}
		else
		{
			return false;
		}
		return true;
	}
	
	//--------------------------------------------------------

	bool CAltStatCanibal::activate()
	{
		_target = NULL;
		_maxTimeCanibal = 15000;
		_timeCanibal = 0;
		_activeCanibal = false;

		sendSET_MATERIAL(_material);

		return true;
	}

	void CAltStatCanibal::deactivate()
	{
	}

	//---------------------------------------------------------

	bool CAltStatCanibal::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CANIBAL";
	} // accept

	//---------------------------------------------------------

	void CAltStatCanibal::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("CANIBAL") == 0)
		{
			_timeCanibal = 0;
			_activeCanibal = true;

			int idTarget;
			LUA::CScriptManager::GetPtrSingleton()->executeFunction("SpawnerMgr", "GetNearestEnemyID", _entity->getPosition(), idTarget);

			if (idTarget != -1)
			{
				_target = _entity->getMap()->getEntityByID(idTarget);

				std::shared_ptr<CHANGE_TARGET>m(new CHANGE_TARGET());
				m->setLogicEntity(_target);
				_entity->emitMessage(m);

				sendSET_MATERIAL(_canibalMaterial);
			}
		}

	} // process

	void CAltStatCanibal::onTick(unsigned int msecs)
	{
		if (_activeCanibal)
		{
			_timeCanibal += msecs;

			if (_timeCanibal >= _maxTimeCanibal)
			{
				stopCanibal();
			}

			if (!_target)
				stopCanibal();

			else if (_target->getIsDead())
				stopCanibal();
		}
	}

	void CAltStatCanibal::stopCanibal()
	{
		std::shared_ptr<CHANGE_TARGET>m(new CHANGE_TARGET());
		m->setLogicEntity(Logic::CServer::getSingletonPtr()->getPlayer());
		_entity->emitMessage(m);

		sendSET_MATERIAL(_material);

		_activeCanibal = false;
		_timeCanibal = 0;
	}


	void CAltStatCanibal::sendSET_MATERIAL(const std::string &material)
	{
		std::shared_ptr<SET_MATERIAL> m (new SET_MATERIAL());
		m->setString(material);
		_entity->emitMessage(m, this);
	}

} // namespace Logic

