/**
@file LifePlayer.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CLifePlayer
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "LifePlayer.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Application/BaseApplication.h"
#include "LUA/ScriptManager.h"
#include "Logic/Server.h"
#include "AnimatedGraphics.h"
#include "Arms.h"

#include "AnimatedGraphics.h"
#include "GUI/Server.h"
#include "GUI/PlayerController.h"
#include "Application/GameState.h"
#include "AvatarController.h"

#include "Logic/Maps/Map.h"

#include "Sounds/MusicManager.h"

namespace Logic 
{
	IMP_FACTORY(CLifePlayer);
	
	bool CLifePlayer::activate() 
	{
		_player = CServer::getSingletonPtr()->getPlayer();
		_camera = _entity->getMap()->getEntityByType("Camera");

		_playerAnimatedEntity = (_player->getComponent<CAnimatedGraphics*>())->getAnimatedEntity();
		_playerAnimatedEntity->setObserver(this, "Death");

		_gameOver = false;
		_dead = false;
		return true;

	} // activate

	void CLifePlayer::deactivate() 
	{
		_playerAnimatedEntity->removeObserver(this, "Die");
		_playerAnimatedEntity = 0;
		_player = 0;
		_camera = 0;
	} // deactivate

	//---------------------------------------------------------

	bool CLifePlayer::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_gameOver = false;

		if(entityInfo->hasAttribute("proporcionShake"))
		{
			_proporcionShake = entityInfo->getFloatAttribute("proporcionShake");
		}

		if(entityInfo->hasAttribute("deathSound"))
		{
			_deathSound = entityInfo->getStringAttribute("deathSound");
		}

		return true;

	} // Onspawn
	
	//---------------------------------------------------------

	void CLifePlayer::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType() == "DAMAGED" && !_invincible && _life > 0)
		{
			// Disminuir la vida de la entidad
			float damage = dynamic_cast<DAMAGED*>(message.get())->getFloat();
			_life -= damage;

			///Mandamos el shake
			std::shared_ptr<SHAKE> shakeMessage (new SHAKE());
			shakeMessage->setMaxShake(std::max(damage * _proporcionShake, 3.0f));
			shakeMessage->setMinShake(2.0f);
			_camera->emitMessage(shakeMessage, this);


			//Sacamos la particulilla de daño del player
			CEntityFactory::getSingletonPtr()->createEntityByType("particulaChispaDanhoPlayer", _entity->getPosition(), _entity->getMap());

			//Cambiamos la estadística en lua
			LUA::CScriptManager::GetPtrSingleton()->executeProcedure("GameMgr", "updateLifePj", _life);
				
			//BaseSubsystems::Log::Debug(_entity->getName()+" Vida: "+ std::to_string(_life));

			// Si han matado al jugador salir de la partida
			if (_life <= 0 && !_dead) 
			{
				//BaseSubsystems::Log::Debug(_entity->getName() + " ha muerto.");

				_dead=true;

				//quitamos las armas al player
				_entity->getComponent<CArms*>()->removeWeapons();

				GUI::CInputManager::getSingletonPtr()->removeKeyListener(Application::CBaseApplication::getSingletonPtr());


				CAnimatedGraphics* cmp = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");
				cmp->setAnimation("Death", "body", false, true);

				_entity->getComponent<CAvatarController*>()->setSleeping(true);
				_entity->getComponent<CArms*>()->setSleeping(true);
				_entity->getComponent<CAnimatedGraphics*>()->getAnimatedEntity()->setBoneManuallyControlled("Bone.007",false);
				_entity->getComponent<CAnimatedGraphics*>()->getAnimatedEntity()->setBoneManuallyControlled("Bone.010",false);

				_entity->getComponent<CArms*>()->attackOrder("Left", false);
				_entity->getComponent<CArms*>()->specialAttackOrder("Left", false);
				_entity->getComponent<CArms*>()->attackOrder("Right", false);
				_entity->getComponent<CArms*>()->specialAttackOrder("Right", false);

				//quitamos el control al jugador
				GUI::CServer::getSingletonPtr()->getPlayerController()->deactivate();

				//lanzamos audio de muerte
				Sounds::CMusicManager::getSingletonPtr()->stopAllSounds();
				Sounds::CMusicManager::getSingletonPtr()->playSound(_deathSound);

			}
		}

		if (message->getType().compare("SET_STAT") == 0)
		{
			std::string typeHability = dynamic_cast<SET_STAT*>(message.get())->getString();
			
			if(typeHability.compare("vidaActual") == 0)
			{
				_life = dynamic_cast<SET_STAT*>(message.get())->getFloat();
			}
		}

	} // process

	//-----------------------------------------------------------------------

	void CLifePlayer::animationFinished(const std::string &animation)
	{
		//BaseSubsystems::Log::Debug("Ha parado la animacion: "+animation);
		if(animation.compare("Death") == 0 && !_gameOver)
		{
			_gameOver = true;
			//Cambiamos al estado de menú, posteriormente será un estado de "fin de partida / game over /end"
			Application::CBaseApplication::getSingletonPtr()->push("gameOver");
			GUI::CInputManager::getSingletonPtr()->addKeyListener(Application::CBaseApplication::getSingletonPtr());
		}
	}

} // namespace Logic

