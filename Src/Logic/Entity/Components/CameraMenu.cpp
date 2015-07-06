

/**
@file CameraMenu.cpp

Contiene la implementación del componente que controla la cámara gráfica
de una escena.
 
@see Logic::CCameraMenu
@see Logic::IComponent

@author Alberto Martínez Villarán
@date Mayo de 2015
*/

#include "CameraMenu.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

namespace Logic 
{
	IMP_FACTORY(CCameraMenu);
	
	//---------------------------------------------------------

	bool CCameraMenu::OnSpawn(const Map::CEntity *entityInfo)
	{
	
		bool ok = true;
		ok &= entityInfo->hasAttribute("targetPosition");
		if(!ok)
		{
			return false;
		}

	
		_targetPosition = entityInfo->getVector3Attribute("targetPosition");

		
		if(entityInfo->hasAttribute("fov"))
		{
			_fov = entityInfo->getFloatAttribute("fov");
		}


		if(entityInfo->hasAttribute("near_clip_distance"))
		{
			_nearClipDistance = entityInfo->getFloatAttribute("near_clip_distance");
		}

		if(entityInfo->hasAttribute("far_clip_distance"))
		{
			_farClipDistance = entityInfo->getFloatAttribute("far_clip_distance");
		}

		return true;

	} // spawn


	//---------------------------------------------------------

	bool CCameraMenu::activate()
	{
		_graphicsCamera = _entity->getMap()->getScene()->getCamera();
		if(!_graphicsCamera)
			return false;
		_graphicsCamera->setCameraPosition(_entity->getPosition());
		_graphicsCamera->setTargetCameraPosition(_targetPosition);
		_graphicsCamera->setFOVyInDegrees(_fov);
		_graphicsCamera->setNearClipDistance(_nearClipDistance);
		_graphicsCamera->setFarClipDistance(_farClipDistance);

		
		return true;

	} // activate
	
	//---------------------------------------------------------

	//void CCameraMenu::deactivate()
	//{
	//	_target = 0;

	//	_targetLastPos =  Vector3();

	//} // deactivate
	
	//---------------------------------------------------------

	//void CCameraMenu::onTick(unsigned int msecs)
	//{
	//	if(_target )
	//	{
	//		// Actualizamos la posición de la cámara.
	//		//Para ello calculamos el movimiento que ha realizado el target

	//		Vector3 targetPosition = _target->getPosition();

	//		Vector3 targetMov = targetPosition - _targetLastPos;

	//		//El movimiento de nuestra camara sera una proporcion al movimiento del target
	//		Vector3 CameraMenuMov = targetMov * _relativeMovement;
	//		
	//		//la nueva posicion de la camara se calcula a partir del movimiento obtenido
	//		Vector3 CameraMenuPosition = CameraMenuMov + _entity->getPosition();


	//		//seteamos la nueva posicion. Tanto en la entidad de la camara como en la camara de Ogre
	//		_entity->setPosition(CameraMenuPosition);
	//		_graphicsCameraMenu->setCameraMenuPosition(CameraMenuPosition);


	//		//anotamos la posicion del target para el siguiente Tick
	//		_targetLastPos = targetPosition;

	//		// Actualizamos la posición hacia donde mira la cámara.
	//		//Para ello sumamos a la posicion del target la altura. (por defecto la posicion del target apunta a la base del modelo)

	//		targetPosition += Vector3(0, _targetHeight, 0);
	//		
	//		//seteamos en la camara de ogre la posicion del target
	//		_graphicsCameraMenu->setTargetCameraMenuPosition(targetPosition);
	//	}

	//} // tick

	////---------------------------------------------------------

	//bool CCameraMenu::accept(const std::shared_ptr<Logic::IMessage> &message)
	//{
	//	return message->getType() == "CameraMenu_ZOOM" ||
	//		message->getType() == "STOP_TARGETING_CAM";
	//} // accept

	////---------------------------------------------------------

	//void CCameraMenu::process(const std::shared_ptr<Logic::IMessage> &message)
	//{
	//	if(message->getType().compare("STOP_TARGETING_CAM") == 0)
	//	{
	//		_canTarget = dynamic_cast<STOP_TARGETING_CAM*>(message.get())->getBool();
	//		_shakeTarget = dynamic_cast<STOP_TARGETING_CAM*>(message.get())->getVector3();
	//	}
	//	else if(message->getType().compare("CameraMenu_ZOOM") == 0)
	//	{
	//		_targetDistance += dynamic_cast<CameraMenu_ZOOM*>(message.get())->getZoom();

	//		_entity->setPosition(Vector3(0, _targetHeight, _targetDistance) + _targetLastPos);

	//		_graphicsCameraMenu->setCameraMenuPosition(Vector3(0, _targetHeight, _targetDistance) + _targetLastPos);
	//	}
	//} // process

} // namespace Logic

