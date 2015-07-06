/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "SoulFeedbackController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"

#include "LUA/ScriptManager.h"
#include "Graphics/Server.h"
#include "Graphics\Camera.h"
#include "Graphics\Scene.h"
#include "Physics/Server.h"

#include "Logic/Maps/EntityFactory.h"
#include "BaseSubsystems/Server.h"
#include "ParticleController.h"

#include "AvatarController.h"

namespace Logic 
{
	IMP_FACTORY(CSoulFeedbackController);
	
	//---------------------------------------------------------

	CSoulFeedbackController::~CSoulFeedbackController()
	{

	}

	bool CSoulFeedbackController::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_destino = Vector2(0.28,0.13);

		//Obtenemos la posicion relativa actual 
		unsigned int w = 0;
		unsigned int h = 0;
		
		BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(w, h);

		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CSoulFeedbackController::respawn(const Map::CEntity *entityInfo)
	{
		_destino = Vector2(0.28,0.13);

		//Obtenemos la posicion relativa actual 
		unsigned int w = 0;
		unsigned int h = 0;
		
		BaseSubsystems::CServer::getSingletonPtr()->getWindowExtents(w, h);

		return true;
	}

	bool CSoulFeedbackController::activate()
	{
		Graphics::CCamera *camera = _entity->getMap()->getScene()->getCamera();
		
		Ogre::Vector3 p = CServer::getSingletonPtr()->getPlayer()->getCenterPosition();
		p = camera->getViewMatrix() * p;
		p = camera->getProjectionMatrix() * p;

		_actual.x =  0.5 + p.x * 0.5;
		_actual.y =  0.5 - p.y * 0.5;

		return true;
	}

	void CSoulFeedbackController::deactivate()
	{

	}

	bool CSoulFeedbackController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return true;
	} // accept
	
	//---------------------------------------------------------

	void CSoulFeedbackController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
	} // process

	void CSoulFeedbackController::onTick(unsigned int msecs)
	{
		
		//BaseSubsystems::Log::Debug("Posicion actual: "+std::to_string(_actual.x)+" y: "+std::to_string(_actual.y));

		//Obtenemos la posicion en mundo correspondiente al destino (barra de vida)
		getRealPos(_pointDestino, _destino.x, _destino.y);

		getRealPos(_pointActual, _actual.x, _actual.y);

		//Obtenemos la posicion relativa a la cual deberiamos ir en este tick
		Vector3 dir = (_pointDestino - _pointActual) * Vector3(1,1,0);
		dir.normalise();
		_actual = _actual + Vector2(dir.x, -dir.y)*msecs * calculateSpeed();

		//Obtenemos la posicion en el mundo real corresponiente a esta posicion a la que ir
		getRealPos(_pointActual, _actual.x, _actual.y);

		//Seteamos la posicion de la entidad a diha posicion
		_entity->setPosition(_pointActual+Vector3(0,0,30));

		//Si hemos llegado al destino entonces destruimos esta entidad y sumamos la vida

		if(_pointDestino.squaredDistance(_pointActual) <= 6)
		{
			_entity->getComponent<CParticleController*>()->setQuota(0);
			CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
			LUA::CScriptManager::GetPtrSingleton()->executeScript("LifeHUD.setLife(0)");
		}
	}

	void CSoulFeedbackController::getRealPos(Vector3 &point, float posX, float posY)
	{
		//A esta función le pasamos la posición relativa en x e y
		Ray mouseRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(posX, posY);

		//Definimos la distancia máxima a la que lanzaremos el rayo
		float max_distance = 600;

		//lanzamos el rayo y guardamos en la variable point la posición actual en el mundo del ratón
		Physics::CServer::getSingletonPtr()->raycastSimple(mouseRay, max_distance, Physics::CollisionGroup::ePlanoColision, point);
	}

	float CSoulFeedbackController::calculateSpeed()
	{
		Vector3 speedPlayer = CServer::getSingletonPtr()->getPlayer()->getComponent<CAvatarController*>()->getActualSpeed();
		int lookingDirectionPlayer = CServer::getSingletonPtr()->getPlayer()->getComponent<CAvatarController*>()->getLookingDirection();


		//Si se esta moviendo hacia la izqueirda
		return (lookingDirectionPlayer * speedPlayer.x < 0) ? 0.0005 : 0.001;
				
	}
} // namespace Logic

