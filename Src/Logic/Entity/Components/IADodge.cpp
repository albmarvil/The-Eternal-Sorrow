#include "IADodge.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include "Logic/Entity/Components/LifeEnemy.h"
#include "AnimatedGraphics.h"

namespace Logic 
{
	IMP_FACTORY(CIADodge);
	 
	bool CIADodge::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("sucessPercentDodge"))
			_successPercent = entityInfo->getIntAttribute("sucessPercentDodge");

		if(entityInfo->hasAttribute("accelDodge"))
			_accelDodge = entityInfo->getFloatAttribute("accelDodge");

		if(entityInfo->hasAttribute("rangeDetectDodge"))
			_rangeDetect = entityInfo->getIntAttribute("rangeDetectDodge");
		
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIADodge::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("sucessPercentDodge"))
			_successPercent = entityInfo->getIntAttribute("sucessPercentDodge");

		if(entityInfo->hasAttribute("accelDodge"))
			_accelDodge = entityInfo->getFloatAttribute("accelDodge");

		if(entityInfo->hasAttribute("rangeDetectDodge"))
			_rangeDetect = entityInfo->getIntAttribute("rangeDetectDodge");

		return true;
	}

	//---------------------------------------------------------

	bool CIADodge::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "DAMAGED" ||
			   message->getType() == "FALLING" ||
			   message->getType() == "CHANGE_TARGET" ||
			   message->getType() == "CAN_IA";
	}

	void CIADodge::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "DAMAGED")
		{
			// Si no está esquivando....
			if (!_initIA && _canIA)
			{
				// Comprobamos primero si el enemigo está cerca del personaje
				Vector3 dist = _target->getCenterPosition() - _entity->getCenterPosition();
				float distLength = dist.length();

				if (distLength < _rangeDetect)
				{
					// Comprobamos si el aleatorio es un % viable
					int random = rand()% 101;

					if (random <= _successPercent)
					{
						_initIA = true;

						std::shared_ptr<CAN_MOVE> m(new CAN_MOVE);
						m->setBool(false);
						_entity->emitMessage(m);
					}
				}
			}

			// Si sigue sin activarse la IA (es decir, no se cumplen las condiciones), restamos vida al enemigo.
			if (!_initIA || !_canIA)
			{
				CLifeEnemy* compLife = (CLifeEnemy*)_entity->getComponent("CLifeEnemy");

				if (!compLife->getInvincible())
				{	
					compLife->damageEntity(dynamic_cast<DAMAGED*>(message.get())->getFloat(),
						dynamic_cast<DAMAGED*>(message.get())->getString());
				}
			}
		}

		else if(message->getType().compare("FALLING") == 0)
		{
			_isFalling = dynamic_cast<FALLING*>(message.get())->getBool();
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

	CIADodge::~CIADodge()
	{
		
	} // ~Controller

	bool CIADodge::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_currentDelayDodge = 0;
		_stepIA = 0;
		_velMov = 0;
		_velFinal = 0;
		_direction = 0;
		_canIA = true;
		_firstStepIA = true;
		_initIA = false;
		_isFalling = true;
		_canEnd = false;
		return true;

	} // activate

	void CIADodge::deactivate()
	{
		
	} // deactivate

	bool CIADodge::onAwake()
	{
		_target = CServer::getSingletonPtr()->getPlayer();
		_stepIA = 0;
		_velMov = 0;
		_velFinal = 0;
		_firstStepIA = true;
		_direction = 0;
		_canIA = true;
		_initIA = false;
		_isFalling = true;
		_canEnd = false;

		return true;
	}

	bool CIADodge::onSleep()
	{
		return true;
	}

	void CIADodge::onTick(unsigned int msecs)
	{
		// Si se ha activado la IA, la ejecutamos
		if (_initIA && _canIA)
		{
			if (_firstStepIA)
			{
				int randX = rand() % 9 + 8;
				float randXFinal = randX/100.f;

				if (_target->getCenterPosition().x > _entity->getCenterPosition().x)
					randXFinal *= -1;

				_direction = Vector3(randXFinal, 1,0);
				_velMov = 0.5f * _direction;

				_entity->getComponent<CAnimatedGraphics*>()->setAnimation("JumpA");

				_firstStepIA = false;

				_canEnd = false;
			}

			_velMov.y -= _accelDodge * _direction.y * msecs;

			_velFinal = _velMov * msecs;

			//La direccion ya viene normalizada		
			std::shared_ptr<AVATAR_WALK> m(new AVATAR_WALK());
			m->setVector3(_velFinal);
			_entity->emitMessage(m);

			if (_canEnd && !_isFalling) //_velFinal.y <= 0)
			{
				_firstStepIA = true;
				_initIA = false;

				_entity->getComponent<CAnimatedGraphics*>()->setAnimation("Walk", "default", true);

				std::shared_ptr<CAN_MOVE> n(new CAN_MOVE);
				n->setBool(true);
				_entity->emitMessage(n);
			}

			_canEnd = true;
		}

	} // tick

} // namespace Logic

