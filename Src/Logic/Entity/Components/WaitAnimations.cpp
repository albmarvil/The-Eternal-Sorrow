
#include "WaitAnimations.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Maps/Map.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "Logic/Entity/Components/Weapon.h"
#include "BaseSubsystems/RandomUtils.h"


#include "Map/MapEntity.h"
#include "LUA\ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CWaitAnimations);
	

	//---------------------------------------------------------

	bool CWaitAnimations::OnSpawn(const Map::CEntity *entityInfo) 
	{
		//Leemos el tiempo máximo de espera desde el arquetipo
		if(entityInfo->hasAttribute("maxWaitTime"))
		{
			_maxTimeStop = entityInfo->getFloatAttribute("maxWaitTime");
		}
		_timeStop = 0;
		return true;
	}

	//----------------------------------------------------------
	bool CWaitAnimations::activate()
	{
		CAnimatedGraphics* componente = (CAnimatedGraphics*)_entity->getComponent("CAnimatedGraphics");
		if(componente != NULL)
		{
			componente->getAnimatedEntity()->setObserver(this, "IdleWait1");
			componente->getAnimatedEntity()->setObserver(this, "IdleWait2");
			componente->getAnimatedEntity()->setObserver(this, "IdleWait3");
		}
		return true;
	}

	//------------------------------------------------------------

	void CWaitAnimations::deactivate() 
	{
		//eliminamos los observers
		CAnimatedGraphics* componente = (CAnimatedGraphics*)_entity->getComponent("CAnimatedGraphics");
		if(componente != NULL)
		{
			componente->getAnimatedEntity()->removeObserver(this, "IdleWait1");
			componente->getAnimatedEntity()->removeObserver(this, "IdleWait2");
			componente->getAnimatedEntity()->removeObserver(this, "IdleWait3");
		}

	} // deactivate

	//---------------------------------------------------------

	void CWaitAnimations::onTick(unsigned int msecs)
	{
		//Si se puede contar el tiempo se aumenta y se cambia la animación
		if(_isCountAttackL && _isCountAttackR && _isCountTurn && _isCountWalk)
		{
			//si el tiempo que el jugador lleva quieto es mayor o igual que el máximo
			//se cambia la animación de idle
			_timeStop += msecs;
			if (_timeStop >= _maxTimeStop)
			{
				//ponemos el contador de nuevo a 0
				_timeStop= 0;
			
				//elegimos una de las tres animaciones posibles
				int rnd = BaseSubsystems::RandomUtils::RandomRange(1, 3);
			
				//seteamos la animación actual, ahora será la animación correspondiente al número que ha salido
				std::stringstream sstm;
				sstm << "IdleWait" << rnd;
				_animActual = sstm.str();

				//ponemos la animación
				Logic::CAnimatedGraphics* cmp = (Logic::CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");
				cmp->setAnimation(_animActual, "default", false, true, 0.0f, 1.0f);

			}
		}
		
	} //onTick

	//---------------------------------------------------------

	bool CWaitAnimations::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CONTROL" ||
			message->getType() == "START_COUNT";
	} // accept

	//---------------------------------------------------------

	void CWaitAnimations::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("CONTROL") == 0)
		{
			std::string control = dynamic_cast<CONTROL*>(message.get())->getString();
			if((!control.compare("turn")) || (!control.compare("attackR")) || (!control.compare("attackL")) || (!control.compare("walk")))
			{
				//el personaje a girado o atacado, ponemos el contador a 0 y forzamos la animación de Idle
				_timeStop = 0;

				SetCount(control, false);
				
				if(_isCountWalk)
				{
					if(_animActual.compare("Idle") != 0)
					{
						sendSET_ANIMATION("Idle", true, true, 1.0);
						_animActual = "Idle";
					}
				}
			}
		}
		if(message->getType().compare("START_COUNT") == 0)
		{
			std::string count = dynamic_cast<START_COUNT*>(message.get())->getString();
			//si es un mensaje de stop salto ponemos el bool del salto a false
			SetCount(count, true);
		}
	} // process
	//-------------------------------------------------------------------------------

	void CWaitAnimations::sendSET_ANIMATION(char *animacion, bool loop, bool restart, float speed)
	{
		//Solo mandamos el mensaje si la animacion es diferente
		if(_animActual.compare(animacion) != 0)
		{
			//BaseSubsystems::Log::Debug("Cambio de animacion a "+std::string(animacion));

			float duration = 0;

			if(!LUA::CScriptManager::GetPtrSingleton()->executeFunction("getTransitionTime", "SaiAnimations", _animActual, animacion, duration))
			{
				duration = 0;
			}

			CAnimatedGraphics* cmp = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");

			cmp->setAnimation(animacion, "default", loop, restart, duration, speed);

			_animActual = animacion;
		}
	}

	//--------------------------------------------------------------------------------------------

	void CWaitAnimations::animationFinished(const std::string &animation)
	{
		if(_isCountWalk)
		{
			//BaseSubsystems::Log::Debug("Finaliza la espera "+std::string(animation));
			sendSET_ANIMATION("Idle", true, true, 1.0);
		}
	}

	//-------------------------------------------------------------------------------------------

	void CWaitAnimations::SetCount(std::string data, bool active)
	{
		if(!data.compare("turn"))
		{
			//ponemos el contador a false para que no inicie la cuenta de tiempo
			_isCountTurn = active;
		}
		else
		{
			if(!data.compare("attackR"))
			{
				_isCountAttackR = active;
			}
			else
			{
				if(!data.compare("attackL"))
				{
					_isCountAttackL = active;
				}
				else
				{
					_isCountWalk = active;
				}
			}
		}
	}
} // namespace Logic