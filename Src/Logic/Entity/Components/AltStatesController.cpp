#include "AltStatesController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"
#include "LUA/ScriptManager.h"
#include "Logic/Entity/Components/LifeEnemy.h"
#include "Logic/Entity/Components/LifePlayer.h"
#include "Logic/Entity/Components/EntityController.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
namespace Logic 
{
	IMP_FACTORY(CAltStatesController);
	
	//---------------------------------------------------------

	CAltStatesController::~CAltStatesController()
	{}

	bool CAltStatesController::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("material"))
		{
			_materialDefault = entityInfo->getStringAttribute("material");
		}

		if(entityInfo->hasAttribute("materialCanibal"))
		{
			_materialCanibal = entityInfo->getStringAttribute("materialCanibal");
		}

		if(entityInfo->hasAttribute("materialPoison"))
		{
			_materialPoison = entityInfo->getStringAttribute("materialPoison");
		}

		if(entityInfo->hasAttribute("materialFrozen"))
		{
			_materialFrozen = entityInfo->getStringAttribute("materialFrozen");
		}

		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CAltStatesController::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("material"))
		{
			_materialDefault = entityInfo->getStringAttribute("material");
		}

		if(entityInfo->hasAttribute("materialCanibal"))
		{
			_materialCanibal = entityInfo->getStringAttribute("materialCanibal");
		}

		if(entityInfo->hasAttribute("materialPoison"))
		{
			_materialPoison = entityInfo->getStringAttribute("materialPoison");
		}

		if(entityInfo->hasAttribute("materialFrozen"))
		{
			_materialFrozen = entityInfo->getStringAttribute("materialFrozen");
		}

		return true;
	}

	bool CAltStatesController::activate()
	{
		_target = NULL;
		_maxTimeCanibal = 7000;
		_maxTimePoison = 7000;
		_maxTimeSlowdown = 5000;
		_maxTimeFrozen = 5000;
		_maxTickDamagePoison = 750;	
		_tickDamagePoison = 0;
		_percentDamagePoison = 0.05f;

		sendSET_MATERIAL(_materialDefault);

		return true;
	}

	void CAltStatesController::deactivate()
	{
		_vecAltStates.clear();
	}

	bool CAltStatesController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "SET_ALTSTATE";
	}

	void CAltStatesController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_ALTSTATE") == 0)
		{
			std::string state = dynamic_cast<SET_ALTSTATE*>(message.get())->getString();

			// Buscamos a ver si ya ha contraido el estado alterado, y de ser así no hacemos nada
			bool found = false;

			for (int i = 0; i < _vecAltStates.size(); ++i)
			{
				if (_vecAltStates.at(i).name == state)
					found = true;

			}

			// Si no lo tenia ya el enemigo, se lo aplicamos
			if (!found)
			{
				AltState altState = {state, false, true, 0};

				_vecAltStates.push_back(altState);
			}

		}
	}

	void CAltStatesController::onTick(unsigned int msecs)
	{
		for (int i = 0; i < _vecAltStates.size(); ++i)
		{
			if (_vecAltStates.at(i).name == "Poison")
				PoisonState(_vecAltStates.at(i), msecs);

			else if (_vecAltStates.at(i).name == "Canibal")
				CanibalState(_vecAltStates.at(i), msecs);

			else if (_vecAltStates.at(i).name == "Slowdown")
				SlowdownState(_vecAltStates.at(i), msecs);

			else if (_vecAltStates.at(i).name == "Frozen")
				FrozenState(_vecAltStates.at(i), msecs);
		}
	}

	void CAltStatesController::PoisonState(AltState &state, unsigned int msecs)
	{
		// Inicializamos los valores del estado
		if (state.initState)
		{
			sendSET_MATERIAL(_materialPoison);

			state.activeState = true;
			state.initState = false;
		}

		// Iniciamos la logica del estado
		if (state.activeState)
		{
			state.timeState += msecs;

			_tickDamagePoison += msecs;

			if (_tickDamagePoison >= _maxTickDamagePoison)
			{
				float damage;

				if (_entity->getTag() == "enemy")
					damage = _entity->getComponent<CLifeEnemy*>()->getLife() * _percentDamagePoison;
				else if (_entity->getTag() == "player")
					damage = _entity->getComponent<CLifePlayer*>()->getLife() * _percentDamagePoison*0.2;

				if (damage < 1)
					damage = 1;

				std::shared_ptr<DAMAGED>m(new DAMAGED());
				m->setFloat(damage);
				_entity->emitMessage(m);

				_tickDamagePoison = 0;

			}

			if (state.timeState >= _maxTimePoison)
			{
				sendSET_MATERIAL(_materialDefault);

				stopAltState(state.name);

				state.activeState = false;
				state.timeState = 0;
			}
		}
	}

	void CAltStatesController::CanibalState(AltState &state, unsigned int msecs)
	{
		// Inicializamos los valores del estado
		if (state.initState)
		{
			// Conseguimos el objetivo mas cercano
			int idTarget;
			LUA::CScriptManager::GetPtrSingleton()->executeFunction("SpawnerMgr", "GetNearestEnemyID", _entity->getPosition(), idTarget);

			// Si hemos conseguido un objetivo, podemos iniciar el estado
			if (idTarget != -1)
			{
				_target = _entity->getMap()->getEntityByID(idTarget);

				std::shared_ptr<CHANGE_TARGET>m(new CHANGE_TARGET());
				m->setLogicEntity(_target);
				_entity->emitMessage(m);

				sendSET_MATERIAL(_materialCanibal);

				//stopAltState(state.name);

				state.activeState = true;
				//state.initState = false;
			}

			// Si no hemos encontrado un enemigo no iniciamos el estado alterado
			else
			{
				state.initState = false;
			}
		}

		// Si hemos encontrado víctima, realizamos el estado
		if (state.activeState)
		{
			state.timeState += msecs;

			if (state.timeState >= _maxTimeCanibal || !_target)
			{
				std::shared_ptr<CHANGE_TARGET>m(new CHANGE_TARGET());
				m->setLogicEntity(Logic::CServer::getSingletonPtr()->getPlayer());
				_entity->emitMessage(m);

				sendSET_MATERIAL(_materialDefault);

				stopAltState(state.name);

				state.activeState = false;
				state.timeState = 0;
			}
		}
	}

	void CAltStatesController::SlowdownState(AltState &state, unsigned int msecs)
	{
		// Inicializamos los valores del estado
		if (state.initState)
		{
			std::shared_ptr<SET_VEL_MULT>m(new SET_VEL_MULT());
			m->setFloat(0.5f);
			_entity->emitMessage(m);

			state.activeState = true;
			state.initState = false;
		}

		// Si hemos encontrado víctima, realizamos el estado
		if (state.activeState)
		{
			state.timeState += msecs;
			//std::cout << "TimeSlow:" << state.timeState << " / " << _maxTimeSlowdown << std::endl;
			if (state.timeState >= _maxTimeSlowdown)
			{
				std::shared_ptr<SET_VEL_MULT>m(new SET_VEL_MULT());
				m->setFloat(1.f);
				_entity->emitMessage(m);

				stopAltState(state.name);

				state.activeState = false;
				state.timeState = 0;
			}
		}
	}

	void CAltStatesController::FrozenState(AltState &state, unsigned int msecs)
	{
		// Inicializamos los valores del estado
		if (state.initState)
		{
			sendSET_MATERIAL(_materialFrozen);

			CAnimatedGraphics* animGr = _entity->getComponent<CAnimatedGraphics*>();
			animGr->setAnimation("Congelado", "default", false, false, 0.0f, 1.0f, false);

			std::shared_ptr<SET_VEL_MULT>n(new SET_VEL_MULT());
			n->setFloat(0.f);
			_entity->emitMessage(n);

			std::shared_ptr<CAN_IA>m(new CAN_IA());
			m->setBool(false);
			_entity->emitMessage(m);

			state.activeState = true;
			state.initState = false;
		}

		// Si hemos encontrado víctima, realizamos el estado
		if (state.activeState)
		{
			state.timeState += msecs;

			if (state.timeState >= _maxTimeFrozen)
			{
				CAnimatedGraphics* animGr = _entity->getComponent<CAnimatedGraphics*>();
				animGr->setAnimation(animGr->getActualAnimation(), "default", true, false, 0.0f, 1.0f);

				std::shared_ptr<SET_VEL_MULT>n(new SET_VEL_MULT());
				n->setFloat(1.f);
				_entity->emitMessage(n);

				std::shared_ptr<CAN_IA>m(new CAN_IA());
				m->setBool(true);
				_entity->emitMessage(m);

				stopAltState(state.name);

				sendSET_MATERIAL(_materialDefault);

				state.activeState = false;
				state.timeState = 0;
			}
		}
	}

	void CAltStatesController::stopAltState(std::string name)
	{
		for (int i = 0; i < _vecAltStates.size(); ++i)
		{
			if (name == _vecAltStates.at(i).name)
			{
				_vecAltStates.erase(_vecAltStates.begin() + i);
				break;
			}
		}
	}

	void CAltStatesController::sendSET_MATERIAL(const std::string &material)
	{
		std::shared_ptr<SET_MATERIAL> m (new SET_MATERIAL());
		m->setString(material);
		_entity->emitMessage(m, this);
	}


	void CAltStatesController::enemyDie()
	{
		std::vector<AltState>::const_iterator it = _vecAltStates.begin();

		for(;it != _vecAltStates.end(); ++it)
		{
			if((*it).name == "Poison")
			{
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","MatarEnemigosEnvenenados");
			}
			else if((*it).name == "Canibal")
			{
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","MatarEnemigosConfusos");
			}
			else if((*it).name == "Slowdown")
			{
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","MatarEnemigosRalentizados");
			}
			else if((*it).name == "Frozen")
			{
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("FuncionesTrophyManager","aumentarValor","MatarEnemigosCongelado");
			}
		}

	}

} // namespace Logic

