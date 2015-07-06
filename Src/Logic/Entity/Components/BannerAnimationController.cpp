/**
@file BannerAnimationController.cpp

Contiene la declaración del componente que provoca la carga
de un nuevo mapa llamando al MapManager
cuando recibe un mensaje TOUCHED / UNTOUCHED.
 
@see Logic::BannerAnimationController
@see Logic::IComponent

@author Bárbara Domínguez de la Torre González
@date Junio, 2015
*/

#include "BannerAnimationController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Logic/Managers/MapManager.h"

#include "LUA/ScriptManager.h"

#include "Map/MapEntity.h"

#include "AnimatedGraphics.h"

namespace Logic 
{
	IMP_FACTORY(CBannerAnimationController);
	
	//---------------------------------------------------------

	bool CBannerAnimationController::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("defaultAnimation"))
		{
			current_animation = entityInfo->getStringAttribute("defaultAnimation");
		}
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CBannerAnimationController::activate()
	{
		_playerAnimatedEntity = (_entity->getComponent<CAnimatedGraphics*>())->getAnimatedEntity();
		_playerAnimatedEntity->setObserver(this, "Move");

		return true;

	} // activate
	
	//---------------------------------------------------------

	void CBannerAnimationController::deactivate()
	{

	} // deactivate
	
	//---------------------------------------------------------

	bool CBannerAnimationController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "TOUCHED" /*||
			message->getType() == "UNTOUCHED"*/;

	} // accept
	
	//---------------------------------------------------------

	void CBannerAnimationController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Cargamos el siguiente mapa
		if((message->getType().compare("TOUCHED") == 0))
		{
			if((dynamic_cast<TOUCHED*>(message.get())->getEntidad()->getTag().compare("player") == 0) ||
				(dynamic_cast<TOUCHED*>(message.get())->getEntidad()->getTag().compare("enemy") == 0))
			{
				//modificamos la animación del estandarte de la de idle a la de move
				if(current_animation.compare("Idle") == 0)
				{
					sendSET_ANIMATION("Move", false, true, 1.0f);
				}
			}
		}
		
	} // process

	//--------------------------------------------------------------------------------

	void CBannerAnimationController::sendSET_ANIMATION(char *animacion, bool loop, bool restart, float speed)
	{
		//Solo mandamos el mensaje si la animacion es diferente
		if(current_animation.compare(animacion) != 0)
		{
			//BaseSubsystems::Log::Debug("Cambio de animacion a "+std::string(animacion));

			float duration = 0;

			duration = 0;

			CAnimatedGraphics* cmp = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");

			cmp->setAnimation(animacion, "default", loop, restart, duration, speed);

			current_animation = animacion;
		}
	}//sendSET_ANIMACION

	//-------------------------------------------------------------

	void CBannerAnimationController::animationFinished(const std::string &animation)
	{
		//BaseSubsystems::Log::Debug("Ha parado la animacion: "+animation);
		if(animation.compare("Move") == 0)
		{
			sendSET_ANIMATION("Idle", true, true, 1.0f);
		}
	}
} // namespace Logic

