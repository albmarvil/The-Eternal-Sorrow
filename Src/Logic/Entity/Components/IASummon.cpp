#include "IASummon.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include "LUA/ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CIASummon);
	 
	bool CIASummon::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("typeMinionSummon"))
		{
			_typeMinion = entityInfo->getStringAttribute("typeMinionSummon");
		}

		if(entityInfo->hasAttribute("distMinionSummon"))
		{
			_distMinion = entityInfo->getFloatAttribute("distMinionSummon");
		}

		if(entityInfo->hasAttribute("velRotMinionSummon"))
		{
			_velRotationMinion = entityInfo->getFloatAttribute("velRotMinionSummon");
		}

		if(entityInfo->hasAttribute("maxDelayInvokeSummon"))
		{
			_maxDelayCanInvoke = entityInfo->getIntAttribute("maxDelayInvokeSummon");
		}

		if(entityInfo->hasAttribute("maxDelaySendSummon"))
		{
			_maxDelaySendMinion = entityInfo->getIntAttribute("maxDelaySendSummon");
		}

		if(entityInfo->hasAttribute("minDistSlowdownApproach"))
		{
			_minDist = entityInfo->getIntAttribute("minDistSlowdownApproach");
		}

		if(entityInfo->hasAttribute("maxMinionsSummon"))
		{
			_maxMinions = entityInfo->getIntAttribute("maxMinionsSummon");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIASummon::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("typeMinionSummon"))
		{
			_typeMinion = entityInfo->getStringAttribute("typeMinionSummon");
		}

		if(entityInfo->hasAttribute("distMinionSummon"))
		{
			_distMinion = entityInfo->getFloatAttribute("distMinionSummon");
		}

		if(entityInfo->hasAttribute("velRotMinionSummon"))
		{
			_velRotationMinion = entityInfo->getFloatAttribute("velRotMinionSummon");
		}

		if(entityInfo->hasAttribute("maxDelayInvokeSummon"))
		{
			_maxDelayCanInvoke = entityInfo->getIntAttribute("maxDelayInvokeSummon");
		}

		if(entityInfo->hasAttribute("maxDelaySendSummon"))
		{
			_maxDelaySendMinion = entityInfo->getIntAttribute("maxDelaySendSummon");
		}

		if(entityInfo->hasAttribute("minDistSlowdownApproach"))
		{
			_minDist = entityInfo->getIntAttribute("minDistSlowdownApproach");
		}

		if(entityInfo->hasAttribute("maxMinionsSummon"))
		{
			_maxMinions = entityInfo->getIntAttribute("maxMinionsSummon");
		}

		return true;
	}

	//---------------------------------------------------------

	bool CIASummon::accept(const std::shared_ptr<Logic::IMessage> &message)
	{ 
		return message->getType() == "GET_LOGIC_ENTITY" || 
			   message->getType() == "CHANGE_TARGET" ||
			   message->getType() == "CAN_IA";
	} // accept

	//---------------------------------------------------------

	void CIASummon::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "GET_LOGIC_ENTITY")
		{
			InfoMinion info;
			info.entity = dynamic_cast<GET_LOGIC_ENTITY*>(message.get())->getEntidad();
			info.angle = 0;
			info.distance = _distMinion;
			info.time = 0;

			std::shared_ptr<CAN_IA>m(new CAN_IA());
			m->setBool(false);
			info.entity->emitMessage(m);

			// Mensajes específicos para los que son de tipo bala
			std::shared_ptr<CAN_TIME_TO_LIVE>n(new CAN_TIME_TO_LIVE());
			n->setBool(false);
			info.entity->emitMessage(n);
			
			_vecMinions.push_back(info);
		}

		else if (message->getType() == "CHANGE_TARGET")
		{
		 
			CEntity* ent = dynamic_cast<CHANGE_TARGET*>(message.get())->getLogicEntity();
			if(ent != NULL)
			{
				if(!ent->getIsDead())
				{
					_target = ent; 
				}
			}
		}

		else if (message->getType() == "CAN_IA")
		{
			_canIA = dynamic_cast<CAN_IA*>(message.get())->getBool();
		}

	} // process

	//---------------------------------------------------------

	CIASummon::~CIASummon()
	{
		
	} // ~Controller

	bool CIASummon::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();

		_leaderLevel = _entity->getEntityInfo()->getIntAttribute("level");
		_leaderTier = _entity->getEntityInfo()->getIntAttribute("tier");
		_canIA = true;
		_delaySendMinion = 0;
		_delayCanInvoke = 0;
		_canSendMinion = false;

		return true;

	} // activate

	bool CIASummon::onAwake()
	{
		_target = CServer::getSingletonPtr()->getPlayer();

		_delaySendMinion = 0;
		_delayCanInvoke = 0;
		_canIA = true;
		_leaderLevel = _entity->getEntityInfo()->getIntAttribute("level");
		_leaderTier = _entity->getEntityInfo()->getIntAttribute("tier");

		_canSendMinion = false;

		return true;
	}

	void CIASummon::deactivate()
	{
		for (int i = 0; i < _vecMinions.size(); ++i)
		{
			//BaseSubsystems::Log::Debug("Borro al minion");
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_vecMinions.at(i).entity);
		}

		_vecMinions.clear();

	} // deactivate

	
	void CIASummon::onTick(unsigned int msecs)
	{
		if (_canIA)
		{
			#pragma region Actualización del delay de los hijos

			for (int i = 0; i < _vecMinions.size(); ++i)
			{
				_vecMinions.at(i).time += msecs;
			}

			#pragma endregion

			#pragma region Lanzamiento de hijos

			// Esperamos un delay para poder enviar el hijo
			if (_delaySendMinion <= _maxDelaySendMinion)
			{
				_delaySendMinion += msecs;
			}

			// Cuando se supere, estaremos listos para enviar el hijo (previa consulta de condiciones)
			else
			{
				Vector3 direc = (_target->getCenterPosition() - _entity->getCenterPosition()) * Vector3(1,1,0); // No nos interesa el z
				float distance = direc.length();

				if (distance <= _minDist +5 && _vecMinions.size() > 0)
				{
					//Cogemos el minion que pueda ser lanzado
					std::vector<InfoMinion>::iterator it = _vecMinions.begin();

					for(; it != _vecMinions.end(); ++it)
					{
						if((*it).time >= _maxDelaySendMinion)
						{
							_delaySendMinion = 0;

							std::shared_ptr<CAN_IA>m(new CAN_IA());
							m->setBool(true);
							(*it).entity->emitMessage(m);

							// Mensajes específicos para los que son de tipo bala
							std::shared_ptr<CAN_TIME_TO_LIVE>n(new CAN_TIME_TO_LIVE());
							n->setBool(true);
							(*it).entity->emitMessage(n);

							std::shared_ptr<DESTINATION_POS>o(new DESTINATION_POS());
							o->setVector3(_target->getCenterPosition());
							(*it).entity->emitMessage(o);

							_vecMinions.erase(it);

							_canSendMinion = false;

							break;
						}
					}
				}
			}
		
			#pragma  endregion

			#pragma region Invocacion de hijos
		
			// Acumulamos el delay hasta poder invocar un nuevo hijo
			_delayCanInvoke += msecs;
		
			if (_delayCanInvoke >= _maxDelayCanInvoke && _vecMinions.size() < _maxMinions)
			{
				int levelMinion = _leaderLevel / 2;
				int tierMinion = _leaderTier;

				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("SpawnerMgr","SummonMinion", _entity->getName(), _typeMinion, levelMinion, tierMinion, Vector3::ZERO);

				_delayCanInvoke = 0;
			}

			#pragma endregion

		}

		else 
		{
			_delaySendMinion = 0;
			_delayCanInvoke = 0;
		}
		#pragma region Rotacion de hijos esperando
		
		// Rotamos cada hijo que tengamos esperando
		for (int i = 0; i < _vecMinions.size(); i++)
		{
			// Sacamos el vector director unificado a raiz del ángulo
			Vector3 dirUni = Math::getDirectionXY(_vecMinions.at(i).angle);

			// La posición resultante será la del líder + la dirección por la distancia
			Vector3 posFinal = _entity->getCenterPosition() + _vecMinions.at(i).distance * dirUni;

			// Enviamos el mensaje de movimiento
			std::shared_ptr<TELEPORT> m(new TELEPORT());
			m->setVector3(posFinal);
			_vecMinions.at(i).entity->emitMessage(m);

			// Aumentamos el ángulo
			_vecMinions.at(i).angle += _velRotationMinion;

			if (_vecMinions.at(i).angle >= 360)
				_vecMinions.at(i).angle = 0;
		}

		#pragma endregion
	
	} // tick

} // namespace Logic

