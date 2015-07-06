#include "IAStealLife.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/Life.h"
#include "LUA/ScriptManager.h"
#include "Logic/Maps/EntityFactory.h"
#include "AnimatedGraphics.h"
#include "particulaChupaAlmasController.h"
#include "Physics/Server.h"

namespace Logic 
{
	IMP_FACTORY(CIAStealLife);
	 
	bool CIAStealLife::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayStealLife"))
		{
			_maxDelaySteal = entityInfo->getIntAttribute("maxDelayStealLife");
		}

		if(entityInfo->hasAttribute("ammountStealLife"))
		{
			_ammountLifeSteal = entityInfo->getIntAttribute("ammountStealLife");
		}

		if(entityInfo->hasAttribute("minDistanceStealLife"))
		{
			_minDistanceSteal = entityInfo->getIntAttribute("minDistanceStealLife");
		}

		if(entityInfo->hasAttribute("animation_config"))
		{
			_animationConfig = entityInfo->getStringAttribute("animation_config");
		}
		_hasStolen = false;
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAStealLife::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayStealLife"))
		{
			_maxDelaySteal = entityInfo->getIntAttribute("maxDelayStealLife");
		}

		if(entityInfo->hasAttribute("ammountStealLife"))
		{
			_ammountLifeSteal = entityInfo->getIntAttribute("ammountStealLife");
		}

		if(entityInfo->hasAttribute("minDistanceStealLife"))
		{
			_minDistanceSteal = entityInfo->getIntAttribute("minDistanceStealLife");
		}

		if(entityInfo->hasAttribute("animation_config"))
		{
			_animationConfig = entityInfo->getStringAttribute("animation_config");
		}

		_hasStolen = false;
		return true;
	}

	//---------------------------------------------------------

	bool CIAStealLife::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CHANGE_TARGET" ||
			   message->getType() == "CAN_IA";
	} // accept

	//---------------------------------------------------------

	void CIAStealLife::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CHANGE_TARGET")
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

	CIAStealLife::~CIAStealLife()
	{
		
	} // ~Controller

	bool CIAStealLife::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_delaySteal = 0;
		_hasStolen = false;
		_canIA = true;
		return true;

	} // activate

	bool CIAStealLife::onAwake()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_hasStolen = false;
		_delaySteal = 0;
		_canIA = true;
		return true;

	} // onAwake

	void CIAStealLife::deactivate()
	{

	} // deactivate

	void CIAStealLife::onTick(unsigned int msecs)
	{
		if (_canIA)
		{
			Vector3 dist = _target->getCenterPosition() - _entity->getCenterPosition();
			float distLength = dist.length();
				
			Vector3 dir = _target->getCenterPosition() - _entity->getCenterPosition();
			dir.normalise();
			dir.z = 0;

			Physics::FilterMask mask;

			//Si a quien tiene que atacar es a un emeigo
			if(_target->getTag() == "enemy")
			{
				mask = Physics::FilterMask(Physics::FilterMask::eTilesMask | Physics::FilterMask::eEnemyTMask | Physics::FilterMask::eEnemyVMask);
			}
			else //si es al player
			{
				mask = Physics::FilterMask(Physics::FilterMask::eTilesMask | Physics::FilterMask::ePlayerMask);
			}

			CEntity *entidadColisiona = Physics::CServer::getSingletonPtr()->raycastSimple(_entity->getCenterPosition(),
				dir, _minDistanceSteal, mask);
			
			// Si no ha empezado a robar
			if (!_hasStolen && entidadColisiona)
			{
				// Si está cerca empezará a robarle
				if (entidadColisiona->getTag() == _target->getTag())
				{
					_hasStolen = true;

					stealLife();
				}
			}

			// Si ya ha robado
			if (_hasStolen)
			{
				_delaySteal += msecs;

				if (_delaySteal >= _maxDelaySteal)
				{
					_hasStolen = false;

					sendSET_ANIMATION("Idle","Drenar", true, 1);

					std::shared_ptr<CAN_MOVE>m(new CAN_MOVE);
					m->setBool(true);
					_entity->emitMessage(m);
				}
			}
		}
		else
		{
			_hasStolen = false;
			_delaySteal = 0;
		}

	} // tick

	void CIAStealLife::stealLife()
	{
		// Iniciamos el contador
		_delaySteal = 0;

		// Impedimos que se mueva
		std::shared_ptr<CAN_MOVE>m(new CAN_MOVE);
		m->setBool(false);
		_entity->emitMessage(m);

		// Ponemos la animación de moverse
		sendSET_ANIMATION("Drenar","Idle", true, 1);

		// Robamos vida si no es invencible
		std::string compLife;

		if (_target->getTag() == "enemy")
			compLife = "CLifeEnemy";
		else if (_target->getTag() == "player")
			compLife = "CLifePlayer";

		CLife* lifeTarget = (CLife*)_target->getComponent(compLife);

		if (!lifeTarget->getInvincible())
		{
			// Robamos vida al target
			std::shared_ptr<DAMAGED>m(new DAMAGED());
			m->setFloat(_ammountLifeSteal);
			_target->emitMessage(m);

			// Sumamos a nuestra vida lo robado
			std::shared_ptr<HEAL>n(new HEAL());
			n->setFloat(_ammountLifeSteal);
			_entity->emitMessage(n);
		}

		// Movemos la partícula de vida hacia el ladrón
		CEntity* particulaChupaVida = CEntityFactory::getSingletonPtr()->createEntityByType("ParticulaChupaAlmas",
			CServer::getSingletonPtr()->getPlayer()->getCenterPosition(), _entity->getMap());

		particulaChupaVida->getComponent<CParticulaChupaAlmasController*>()->moveTo(_entity);
		
	}

	void CIAStealLife::sendSET_ANIMATION(char *animacion, char* animacionActual, bool loop, float speed)
	{
		float duration = 0;

		if(!LUA::CScriptManager::GetPtrSingleton()->executeFunction("getTransitionTime", _animationConfig, animacionActual, animacion, duration))
		{
			duration = 0;
		}

		CAnimatedGraphics* cmp  = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");
		cmp->setAnimation(animacion, "default", loop, false, duration, speed);
	}

} // namespace Logic

