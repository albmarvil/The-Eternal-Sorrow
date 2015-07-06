/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/


#include "CameraPause.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

namespace Logic 
{
	IMP_FACTORY(CCameraPause);
	
	//---------------------------------------------------------

	bool CCameraPause::OnSpawn(const Map::CEntity *entityInfo)
	{
		bool ok =	entityInfo->hasAttribute("offsetZ") && 
					entityInfo->hasAttribute("angle");

		//Distancia de la camara con respecto al personaje
		float offsetZ = entityInfo->getFloatAttribute("offsetZ");

		_angle = entityInfo->getFloatAttribute("angle");

		_targetPosition = _entity->getPosition() + Vector3(0,0,offsetZ);

		return true;

	} // spawn

	bool CCameraPause::respawn(const Map::CEntity *entityInfo)
	{
		bool ok =	entityInfo->hasAttribute("offsetZ") && 
					entityInfo->hasAttribute("angle");

		//Distancia de la camara con respecto al personaje
		float offsetZ = entityInfo->getFloatAttribute("offsetZ");

		_angle = entityInfo->getFloatAttribute("angle");

		_targetPosition = _entity->getPosition() + Vector3(0,0,offsetZ);

		return true;
	}
	//---------------------------------------------------------

	bool CCameraPause::activate()
	{
		_graphicsCamera = _entity->getMap()->getScene()->createCameraPause();

		if(!_graphicsCamera)
			return false;

		_graphicsCamera->setCameraPosition(_entity->getPosition());
		_graphicsCamera->setTargetCameraPosition(_targetPosition);

		_entity->getMap()->getScene()->createViewportPause();

		_positionInitial = _entity->getPosition();

		return true;

	} // activate
	
	//---------------------------------------------------------

	void CCameraPause::deactivate()
	{
		_entity->getMap()->getScene()->deleteViewportPause();
		_entity->getMap()->getScene()->destroyCameraPause();
		
	} // deactivate

	void CCameraPause::onTick(unsigned int msecs)
	{
	}

	void CCameraPause::pulsarD(unsigned int msecs)
	{
		//float radio  =  abs(_targetPosition.z - _positionInitial.z);
		float radio  = abs(Vector3(_targetPosition.x, 0, _targetPosition.z).distance(Vector3(_positionInitial.x, 0, _positionInitial.z)));

		_angle += msecs * 0.001;

		Vector3 newPos = Vector3(_targetPosition.x + radio*cos(_angle), _positionInitial.y ,
			_targetPosition.z - radio*sin(_angle));

		_graphicsCamera->setCameraPosition(newPos);
	}

	void CCameraPause::pulsarA(unsigned int msecs)
	{
		//float radio  =  abs(_targetPosition.z - _positionInitial.z);
		float radio  = abs(Vector3(_targetPosition.x, 0, _targetPosition.z).distance(Vector3(_positionInitial.x, 0, _positionInitial.z)));

		_angle -= msecs * 0.001;

		Vector3 newPos = Vector3(_targetPosition.x + radio*cos(_angle), _positionInitial.y ,
			_targetPosition.z - radio*sin(_angle));

		_graphicsCamera->setCameraPosition(newPos);
	}

	void CCameraPause::pulsarW(unsigned int msecs)
	{
		Vector3 pos = _graphicsCamera->getCameraPosition();
		pos.y = _targetPosition.y;

		Vector3 dir = _targetPosition - pos;
		dir.normalise();

		
		Vector3 newPos = Vector3(_graphicsCamera->getCameraPosition().x + dir.x, 
			_graphicsCamera->getCameraPosition().y, _graphicsCamera->getCameraPosition().z + dir.z);

		_graphicsCamera->setCameraPosition(newPos);

		_positionInitial.z = _graphicsCamera->getCameraPosition().z;
		_positionInitial.x = _graphicsCamera->getCameraPosition().x;
	}

	void CCameraPause::pulsarS(unsigned int msecs)
	{

		Vector3 pos = _graphicsCamera->getCameraPosition();
		pos.y = _targetPosition.y;

		Vector3 dir = pos - _targetPosition;
		dir.normalise();

		Vector3 newPos = Vector3(_graphicsCamera->getCameraPosition().x + dir.x, _graphicsCamera->getCameraPosition().y,
			_graphicsCamera->getCameraPosition().z + dir.z);
		_graphicsCamera->setCameraPosition(newPos);

		_positionInitial.z = _graphicsCamera->getCameraPosition().z;
		_positionInitial.x = _graphicsCamera->getCameraPosition().x;
	}

} // namespace Logic

