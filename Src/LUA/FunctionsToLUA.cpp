/**
@author Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#include "FunctionsToLUA.h"
#include "ScriptManager.h"
#include "Logic\Maps\EntityFactory.h"
#include "Logic\Maps\Map.h"
#include "Logic\Server.h"
#include "Logic\Maps\Map.h"
#include "Logic\Entity\Components\Life.h"
#include "Logic\Entity\Components\AnimatedGraphics.h"
#include "Logic\Entity\Components\WaitAnimations.h"
#include "Logic\Entity\Entity.h"
#include "Logic\Entity\Message.h"
#include "Logic\Entity\Components\ArrowEnemy.h"
#include "Logic\Entity\Components\PhysicEntity.h"
#include "Logic\Entity\Components\Billboard.h"
#include "Logic\Entity\Components\ParticleController.h"
#include "Logic\Entity\Components\PinchosController.h"
#include "Logic\Entity\Components\AltarFeedbackController.h"

#include "Map\MapEntity.h"
#include "Map\MapParser.h"
#include "Map\ParserTile.h"
#include "Physics\Server.h"

#include <CEGUI/System.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/Window.h>
#include "CEGUI\WindowRendererSets\Core\Scrollbar.h"
#include <cegui\widgets\ToggleButton.h>

#include "BaseSubsystems\ConfigurationManager.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"
extern "C" {
#include "lua.h"
#include <lauxlib.h>
#include <lualib.h>
}

#include <list>
#include <luabind/luabind.hpp>
#include "luabind/class_info.hpp"

using namespace luabind;

namespace FuncionestoLUA
{

	#pragma region Funciones para la lectura del blueprints
	void addComponent(const std::string &type, const std::string &component, bool value)
	{
		Logic::CEntityFactory::getSingletonPtr()->setMapBlueprints(type,component, value);
	}

	#pragma endregion

	#pragma region Funciones para la lectura del mapa y arquetipo.

	void AddMapEntity(const Map::CEntity &entity) {
		#ifdef _DEBUG
			assert(entity.getType().compare("") && "No se ha establecido tipo a la entidad");
			
			std::list<Map::CEntity*> entityList = Map::CMapParser::getSingletonPtr()->getEntityList();

			std::list<Map::CEntity*>::const_iterator it = entityList.begin();

			if(!entityList.empty())
			{
				for(; it != entityList.end(); ++it)
				{
					assert((*it)->getName().compare(entity.getName()) && "Ya existe una entidad con este nombre.");
				}
			}
		#endif // _DEBUG

		// La que recibimos ES DE LUA
		Map::CEntity *newEntity = new Map::CEntity(entity.getName());
		*newEntity = entity;

		Map::CMapParser::getSingletonPtr()->addEntityInfo(newEntity);

	}
	#pragma endregion
	/*
	Esta funcion crea la entidad a partir de un entityInfo, primero metiendo este en la cola de entityInfos correspondiente
	*/
	Logic::CEntity* NewEntity(Map::CEntity *entityInfo,  Logic::CMap *map)
	{
		AddMapEntity(entityInfo);

		Logic::CEntity* res = Logic::CEntityFactory::getSingletonPtr()->createEntity(entityInfo, map);
		return res;
	}

	void SendMinionCreated(Logic::CEntity* minionCreated, std::string leaderName)
	{
		Logic::CEntity* leader = Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(leaderName);

		std::shared_ptr<Logic::GET_LOGIC_ENTITY>m(new Logic::GET_LOGIC_ENTITY());
		m->setEntity(minionCreated);
		leader->emitMessage(m);
	}

	std::vector<Map::InterestingPointInfo> getInterestingPoints()
	{
		return Map::CParserTile::getSingletonPtr()->getInterestingPoints();
	}

	std::vector<Map::AltarPointInfo> getAltarPoints()
	{
		return Map::CParserTile::getSingletonPtr()->getAltarPoints();
	}

	void ActiveViewportEffect(const std::string &effect)
	{
		Logic::CServer::getSingletonPtr()->getMap()->getScene()->getCamera()->setCompositorEffect(effect, true);
	}

	void DisableViewportEffect(const std::string &effect)
	{
		Logic::CServer::getSingletonPtr()->getMap()->getScene()->getCamera()->setCompositorEffect(effect, false);
	}

	void startSound(const std::string &sound, const std::string &entityName)
	{
		std::shared_ptr<Logic::PLAY_SOUND> m (new Logic::PLAY_SOUND());
		m->setSound(std::string(sound));
		Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(entityName)->emitMessage(m);
	}

	void stopSound(const std::string &sound, const std::string &entityName)
	{
		std::shared_ptr<Logic::STOP_SOUND> m (new Logic::STOP_SOUND());
		m->setSound(std::string(sound));
		Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(entityName)->emitMessage(m);
	}

	//Este método esta cableado para cambiar el sonido ambiente, lo suyo sería desde lua mandar directamente el mensaje
	void setSoundParameter(std::string sound, std::string parameter, int value, std::string entity)
	{
		std::shared_ptr<Logic::SET_SOUND_PARAMETER> m (new Logic::SET_SOUND_PARAMETER());
		m->setSound(std::string(sound));
		m->setParameter(std::string(parameter));
		m->setValue(value);
		Logic::CServer::getSingletonPtr()->getMap()->getEntityByName(entity)->emitMessage(m);
	}

	//Manda un mensaje de OPEN_CHEST desde LUA a la entidad indicada
	void sendOpenChestMessage(Logic::CEntity &entity)
	{
		std::shared_ptr<Logic::OPEN_CHEST> m (new Logic::OPEN_CHEST());
		entity.emitMessage(m);

	}

	void sendCancelOpenChestMessage(Logic::CEntity &entity)
	{
		std::shared_ptr<Logic::CANCEL_OPEN> m (new Logic::CANCEL_OPEN());
		entity.emitMessage(m);
	}

	void openChestAnimation(Logic::CEntity &entity)
	{
		Logic::CAnimatedGraphics* cmp = (Logic::CAnimatedGraphics*) entity.getComponent("CAnimatedGraphics");
		cmp->setAnimation("Open", "default", false, true, 0.0f, 2.0f);
	}

	void setAnimation(Logic::CEntity *ent, const std::string &animation, const std::string &animSet = "default", bool loop = false, bool restart = false,
		float duration = 0.0f, float speed = 1.0f)
	{
		ent->getComponent<Logic::CAnimatedGraphics*>()->setAnimation(animation, animSet, loop, restart, duration, speed);
	}

	void SendHabilidadMessage(std::string name, int contador, Logic::CEntity *playerRef)
	{
		std::shared_ptr<Logic::SET_HABILIDAD> n(new Logic::SET_HABILIDAD());
		n->setString(name);
		n->setInt(contador);
		playerRef->emitMessage(n);
	}

	void SendAltStateMessage(std::string name, Logic::CEntity *enem)
	{
		std::shared_ptr<Logic::SET_ALTSTATE> n(new Logic::SET_ALTSTATE());
		n->setString(name);
		enem->emitMessage(n);
	}

	void SendDamagedMessage(Logic::CEntity *enem, float value)
	{
		std::shared_ptr<Logic::DAMAGED> n(new Logic::DAMAGED());
		n->setFloat(value);
		n->setString(enem->getType());
		enem->emitMessage(n);
	}

	void SendStatsMenssage(std::string name, float value, Logic::CEntity *playerRef)
	{
		std::shared_ptr<Logic::SET_STAT> n(new Logic::SET_STAT());
		n->setString(name);
		n->setFloat(value);
		playerRef->emitMessage(n);
	}

	Logic::CMap* createLogicMap(const std::string &name)
	{
		return new Logic::CMap(name);
	}

	void deleteLogicMap(Logic::CMap *map)
	{
		delete map;
	}
	
	void ChangeCollisionGroup(Logic::CEntity* ent)
	{
		std::shared_ptr<Logic::SET_PHYSIC_GROUP> m(new Logic::SET_PHYSIC_GROUP());
		m->setGroup(12);
		ent->emitMessage(m);
	}

	void sendPushMessage(Logic::CEntity *otherEntity, float speed, Vector3 direction)
	{
		direction.normalise();

		std::shared_ptr<Logic::PUSH> pushMessage(new Logic::PUSH());
		pushMessage->setDirection(direction);
		pushMessage->setSpeed(speed);
		otherEntity->emitMessage(pushMessage);
	}


	void tutorialComplete()
	{
		CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*>(CEGUI::System::getSingletonPtr()->
			getDefaultGUIContext().getRootWindow()->getChildElement("TabControlOpc/TabJuego/checkboxTutorial"));

		checkbox->setSelected(false);

	}

	void shakeCamera(Logic::CEntity *entity, float max, float min)
	{
		Logic::CEntity* cam = entity->getMap()->getEntityByType("Camera");

		std::shared_ptr<Logic::SHAKE> m(new Logic::SHAKE());
		m->setMaxShake(max);
		m->setMinShake(min);
		cam->emitMessage(m);
	}

	void scaleParticle(Logic::CEntity *entity, float scale, std::string namePart)
	{
		std::shared_ptr<Logic::SET_SCALE_PARTICLE> m(new Logic::SET_SCALE_PARTICLE());
		m->setOwner(entity);
		m->setNameParticle(namePart);
		m->setScale(scale);
		entity->emitMessage(m);
	}

	Vector3 getScreenPosition(Logic::CEntity *entity)
	{
		Graphics::CCamera *camera = entity->getMap()->getScene()->getCamera();
		
		Ogre::Vector2 p = camera->getScreenCoord(entity->getCenterPosition() + Vector3(0,40,0));

		return Vector3(p.x, p.y, 0);

	}

	void addEnemyToArrow(int id)
	{
		Logic::CServer::getSingletonPtr()->getPlayer()->getComponent<Logic::CArrowEnemy*>()->addEnemy(id,
			Logic::CServer::getSingletonPtr()->getMap()->getEntityByID(id));
	}

	void removeEnemyToArrow(int id)
	{
		Logic::CServer::getSingletonPtr()->getPlayer()->getComponent<Logic::CArrowEnemy*>()->removeEnemy(id);
	}

	void ChangeCollisionGroupTileAtravesable(Logic::CEntity* ent, bool touched)
	{
		ent->getComponent<Logic::CPhysicEntity*>()->changePhysicsGroup(
			(touched) ? Physics::eTileAtravesableAbajo : Physics::eTileAtravesableArriba); 
	}

	void updateBillboardChest(Logic::CEntity* ent, int time, int timeMax)
	{
		ent->getComponent<Logic::CBillBoard*>()->updateLife(time, timeMax);
	}

	void setVisibleBillboardCofre(Logic::CEntity* ent, bool visible)
	{
		ent->getComponent<Logic::CBillBoard*>()->setVisible(visible);
	}

	void startParticle(Logic::CEntity* ent)
	{
		ent->getComponent<Logic::CParticleController*>()->startParticle();
	}

	void stopParticle(Logic::CEntity* ent)
	{
		ent->getComponent<Logic::CParticleController*>()->stopParticle();
	}

	void downPinchos(Logic::CEntity *ent)
	{
		ent->getComponent<Logic::CPinchosController*>()->downPinchos();
	}

	void upPinchos(Logic::CEntity *ent)
	{
		ent->getComponent<Logic::CPinchosController*>()->upPinchos();
	}

	void IntantiateSoulAltar(Vector3 posDest, Vector3 posInit, Logic::CMap *map)
	{
		Logic::CEntity *ent = Logic::CEntityFactory::getSingletonPtr()->createEntityByType("AltarFeedback", posInit, map);

		Logic::CAltarFeedbackController *comp = (Logic::CAltarFeedbackController *) ent->getComponent("CAltarFeedbackController");

		comp->Config(posDest);
	}


}

namespace LUA
{
	
	bool CFunctionsToLUA::publishAllFunctions()
	{
		
		lua_State *_lua = LUA::CScriptManager::GetPtrSingleton()->getNativeInterpreter();

		module(_lua)[
			def("addComponent", &FuncionestoLUA::addComponent),
			def("AddEntity", &FuncionestoLUA::AddMapEntity),
			def("getInterestingPoints", &FuncionestoLUA::getInterestingPoints),
			def("getAltarPoints", &FuncionestoLUA::getAltarPoints),
			def("ActiveViewportEffect", &FuncionestoLUA::ActiveViewportEffect),
			def("DisableViewportEffect", &FuncionestoLUA::DisableViewportEffect),
			def("startSound", &FuncionestoLUA::startSound),
			def("stopSound", &FuncionestoLUA::stopSound),
			def("setSoundParameter", &FuncionestoLUA::setSoundParameter),
			//def("getScale", &FuncionestoLUA::getScaleArea),
			//def("getOffset", &FuncionestoLUA::getOffsetArea),
			def("NewEntity",&FuncionestoLUA::NewEntity),
			def("SendMinionCreated", &FuncionestoLUA::SendMinionCreated),
			def("sendOpenChestMessage", &FuncionestoLUA::sendOpenChestMessage),
			//def("getScrollPosition", &FuncionestoLUA::getScrollPosition),
			def("sendHabilidadMessage", &FuncionestoLUA::SendHabilidadMessage),
			def("SendAltStateMessage", &FuncionestoLUA::SendAltStateMessage),
			def("SendDamagedMessage", &FuncionestoLUA::SendDamagedMessage),
			def("createLogicMap", &FuncionestoLUA::createLogicMap),
			def("deleteLogicMap", &FuncionestoLUA::deleteLogicMap),
			def("SendStatsMenssage", &FuncionestoLUA::SendStatsMenssage),
			def("ChangeCollisionGroup", &FuncionestoLUA::ChangeCollisionGroup),
			def("openChestAnimation",&FuncionestoLUA::openChestAnimation),
			def("tutorialComplete", &FuncionestoLUA::tutorialComplete),
			def("getScreenPosition", &FuncionestoLUA::getScreenPosition),
			def("addEnemyToArrow", &FuncionestoLUA::addEnemyToArrow),
			def("removeEnemyToArrow", &FuncionestoLUA::removeEnemyToArrow),
			def("ChangeCollisionGroupTileAtravesable", &FuncionestoLUA::ChangeCollisionGroupTileAtravesable),
			def("sendCancelOpenChestMessage", &FuncionestoLUA::sendCancelOpenChestMessage),
			def("updateBillboardChest", &FuncionestoLUA::updateBillboardChest),
			def("setVisibleBillboardCofre", &FuncionestoLUA::setVisibleBillboardCofre),
			def("startParticle", &FuncionestoLUA::startParticle),
			def("stopParticle", &FuncionestoLUA::stopParticle),
			def("setAnimation", &FuncionestoLUA::setAnimation),
			def("shakeCamera", &FuncionestoLUA::shakeCamera),
			def("scaleParticle", &FuncionestoLUA::scaleParticle),
			def("upPinchos", &FuncionestoLUA::upPinchos),
			def("downPinchos", &FuncionestoLUA::downPinchos),
			def("InstantiateAltarSoul", &FuncionestoLUA::IntantiateSoulAltar),
			def("sendPushMessage", &FuncionestoLUA::sendPushMessage)
		];

		return true;
	}


}