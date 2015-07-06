/**
@file AnimatedGraphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de una entidad estática.
 
@see Logic::CAnimatedGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010

@refactor Alberto Martínez Villarán
@date Marzo, 2015
*/

#include "AnimatedGraphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/WaitAnimations.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"

#include "LUA\ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CAnimatedGraphics);
	
	//---------------------------------------------------------

	Graphics::CEntity* CAnimatedGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("animation_config"))
		{
			_animationConfig = entityInfo->getStringAttribute("animation_config");
		}

		_animatedGraphicsEntity = new Graphics::CAnimatedEntity(_entity->getName(),_model, _animationConfig);
		_animatedGraphicsEntity->setNeedNode(_needNode);
		/*if(!_scene->addEntity(_animatedGraphicsEntity))
			return 0;*/

		_animatedGraphicsEntity->setTransform(_entity->getTransform());
		
		if(entityInfo->hasAttribute("defaultAnimation"))
		{
			_defaultAnimation = entityInfo->getStringAttribute("defaultAnimation");
		}

		_speed = 1.0f;
		if(entityInfo->hasAttribute("defaultSpeedAnimation"))
		{
			_speed = entityInfo->getFloatAttribute("defaultSpeedAnimation");
		}


		return _animatedGraphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	bool CAnimatedGraphics::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return CGraphics::accept(message) ||
			   /*message->getType() == "SET_ANIMATION" ||*/
			   message->getType() == "STOP_ANIMATION" ||
			   message->getType() == "SET_ANIM_SPEED_MULT";

	} // accept
	
	//---------------------------------------------------------

	bool CAnimatedGraphics::activate()
	{
		if(!CGraphics::activate())
		{
			return false;
		}

		_multSpeed = 1;
		//Las animaciones estarán por defecto en la categoría o conjunto de animación "default"
		_animatedGraphicsEntity->setAnimation(_defaultAnimation, "default", true, false, 0.0f, _speed);

		_actualAnimation = _defaultAnimation;
		//Nos registramos como observadores de la animación por defecto
		//_animatedGraphicsEntity->setObserver(this, _defaultAnimation);

		///seteamos el modo de blend
		_animatedGraphicsEntity->setAnimationBlendMode(Graphics::BlendMode::CUMULATIVE);

		return true;
	}
	void CAnimatedGraphics::deactivate()
	{

		//Eliminamos esta entidad de la escena grafica
		if(_animatedGraphicsEntity)
		{
			//Primero la hacemos invisible
			_animatedGraphicsEntity->setVisible(false);
			_animatedGraphicsEntity->stopAllAnimations();
			_scene->removeEntity(_animatedGraphicsEntity);
		}
		_scene = 0;
	}
	//---------------------------------------------------------

	void CAnimatedGraphics::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		CGraphics::process(message);

		/*if(message->getType().compare("SET_ANIMATION") == 0)
		{
			// Paramos todas las animaciones antes de poner una nueva.
			// Un control más sofisticado debería permitir interpolación
			// de animaciones. Galeon no lo plantea.
			//_animatedGraphicsEntity->stopAllAnimations();
			SET_ANIMATION *msg = dynamic_cast<SET_ANIMATION*>(message.get());
			std::string animName = msg->getAnimation(); 
			std::string animSet = msg->getAnimationSet();
			bool loop = msg->getLoop();
			bool restart = msg->getRestartFlag();
			float duration = msg->getDuration();
			float speed = msg->getSpeed();

			//Como ha recibido un mensaje para cambiar la animación reseteamos el tiempo en el qspecialAttackOrderue permanece
			//sin hacer nada
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "resetTemp");

			_animatedGraphicsEntity->setAnimation(animName, animSet, loop, restart, duration, speed);
		}
		else*/ if (message->getType().compare("STOP_ANIMATION") == 0)
		{
			STOP_ANIMATION *msg = dynamic_cast<STOP_ANIMATION*>(message.get());
			std::string animName = msg->getAnimation();
			std::string animSet = msg->getAnimationSet();
			_animatedGraphicsEntity->stopAnimation(animName, animSet);
		}

		else if (message->getType().compare("SET_ANIM_SPEED_MULT") == 0)
		{
			_multSpeed = dynamic_cast<SET_ANIM_SPEED_MULT*>(message.get())->getFloat();
		}

	} // process
	
	//---------------------------------------------------------
	
	void CAnimatedGraphics::animationFinished(const std::string &animation)
	{
		// Si acaba una animación y tenemos una por defecto la ponemos
		//_animatedGraphicsEntity->stopAllAnimations();
		//_animatedGraphicsEntity->setAnimation(_defaultAnimation,true, false);
		//BaseSubsystems::Log::Debug("CAnimatedGraphics->animationFinished - "+animation);
	}

	void CAnimatedGraphics::setAnimation(const std::string &animation, const std::string &animSet, bool loop, bool restart, float duration, float speed, bool saveAnim)
	{
		if (saveAnim)
			_actualAnimation = animation;

		_animatedGraphicsEntity->setAnimation(animation, animSet, loop, restart, duration, speed * _multSpeed);
	}

	
	

} // namespace Logic

