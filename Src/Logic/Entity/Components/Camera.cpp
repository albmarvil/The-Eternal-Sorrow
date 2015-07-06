

/**
@file Camera.cpp

Contiene la implementación del componente que controla la cámara gráfica
de una escena.
 
@see Logic::CCamera
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Camera.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"

#include "BaseSubsystems/Server.h"

namespace Logic 
{
	IMP_FACTORY(CCamera);
	
	//---------------------------------------------------------

	bool CCamera::OnSpawn(const Map::CEntity *entityInfo)
	{
		_graphicsCamera = _entity->getMap()->getScene()->getCamera();
		if(!_graphicsCamera)
			return false;

		bool ok = true;
		ok &= entityInfo->hasAttribute("targetDistance");
		ok &= entityInfo->hasAttribute("targetHeight");
		ok &= entityInfo->hasAttribute("relative_movement");
		if(!ok)
		{
			return false;
		}

	
		_targetDistance = entityInfo->getFloatAttribute("targetDistance");

	
		_targetHeight = entityInfo->getFloatAttribute("targetHeight");


		_relativeMovement = entityInfo->getFloatAttribute("relative_movement");

		
		if(entityInfo->hasAttribute("fov"))
		{
			_fov = entityInfo->getFloatAttribute("fov");
			_graphicsCamera->setFOVyInDegrees(_fov);
		}


		if(entityInfo->hasAttribute("near_clip_distance"))
		{
			_nearClipDistance = entityInfo->getFloatAttribute("near_clip_distance");
			_graphicsCamera->setNearClipDistance(_nearClipDistance);
		}

		if(entityInfo->hasAttribute("far_clip_distance"))
		{
			_farClipDistance = entityInfo->getFloatAttribute("far_clip_distance");
			_graphicsCamera->setFarClipDistance(_farClipDistance);
		}


		return true;

	} // spawn

	

	//---------------------------------------------------------

	bool CCamera::activate()
	{
		_target = CServer::getSingletonPtr()->getPlayer();

		_targetLastPos = _target->getPosition();
		_targetLastX = _target->getPosition().x;
		_targetLastY = _target->getPosition().y;

		//seteamos la posicion de la entidad de la camara inicial con respecto al target, usando la distancia y la altura
		_entity->setPosition(Vector3(0, _targetHeight, _targetDistance) + _targetLastPos);

		_graphicsCamera->setCameraPosition(Vector3(0, _targetHeight, _targetDistance) + _targetLastPos);
		_graphicsCamera->setTargetCameraPosition(_targetLastPos);
		
		return true;

	} // activate
	
	//---------------------------------------------------------

	void CCamera::deactivate()
	{
		_target = 0;

		_targetLastPos =  Vector3();

	} // deactivate
	
	//---------------------------------------------------------

	bool CCamera::canMoveHorizontal(Vector3 position)
	{
		//Obtenemos la mitad del angulo correspondiente al fov vertical
		float V = _graphicsCamera->getFovyY().valueRadians()/2;

		//Obtenemos los angulos correspondiente al fov horizontal en radianes
		_fovHorizontal = 2*atan(tan(V)*_graphicsCamera->getAspectRatio());

		//Obtengo la hipotenusa
		float hipotenusa = _targetDistance / cos(_fovHorizontal/2);


		//-------------------------MOVIMIENTO A LA IZQUIERDA-------------------------------

		//Obtenemos la direccion correspondiente a girar la camara los grados maximos/2 del fov horizontal
		Vector3 dirNew = Quaternion(Ogre::Radian(_fovHorizontal/2), Vector3::UNIT_Y) * Vector3::UNIT_Z;
		dirNew.normalise();

		//Obtengo el punto del extremo izquierdo de la camara
		Vector3 posPoint = position - dirNew * hipotenusa;

		//Obtengo su posicion en tilePosicion
		posPoint /= Vector3(20,-20,1);

		//Si es menor a 0 entonces no se puede seguir moviendo
		if(posPoint.x<=0)
		{
			return false;
		}

		//-------------------------MOVIMIENTO A LA DERECHA-------------------------------

		posPoint = position + dirNew * hipotenusa;

		posPoint /= Vector3(20,-20,1);

		if(posPoint.x >= _entity->getMap()->getMapTileWidth())
		{
			return false;
		}

		return true;
	}

	bool CCamera::canMoveVertical(Vector3 position)
	{

		//A dierencia del horizontal, en este se calcula con el fov vertical. 

		float V = _graphicsCamera->getFovyY().valueRadians()/2;

		float hipotenusa = _targetDistance / cos(V);


		//-------------------------MOVIMIENTO A ABAJO-------------------------------

		Vector3 dirNew = Quaternion(Ogre::Radian(V), Vector3::UNIT_X) * Vector3::UNIT_Z;
		dirNew.normalise();

		Vector3 posPoint = position - dirNew * hipotenusa;

		posPoint /= Vector3(20,-20,1);

		if(posPoint.y<=0)
		{
			return false;
		}


		//-------------------------MOVIMIENTO A ARRIBA-------------------------------

		posPoint = position + dirNew * hipotenusa;

		posPoint /= Vector3(20,-20,1);

		if(posPoint.y >= _entity->getMap()->getMapTileHeight())
		{
			return false;
		}
	}

	void CCamera::onTick(unsigned int msecs)
	{
		if(_target )
		{
			// Actualizamos la posición de la cámara.
			//Para ello calculamos el movimiento que ha realizado el target

			Vector3 targetPosition = _target->getPosition();

			float targetMovX = targetPosition.x - _targetLastX;
			float targetMovY = targetPosition.y - _targetLastY;

			//El movimiento de nuestra camara sera una proporcion al movimiento del target
			float cameraMovX = targetMovX * _relativeMovement;
			float cameraMovY = targetMovY * _relativeMovement;

			//la nueva posicion de la camara se calcula a partir del movimiento obtenido
			Vector3 cameraPosition = Vector3(cameraMovX, cameraMovY,0) + _entity->getPosition();

			//Si no nos podemos mover en ninguno de los 4 extremos, entonces, la posicion de la camara se queda tal y como esta
			if(!canMoveHorizontal(cameraPosition) && !canMoveVertical(cameraPosition))
			{
				cameraPosition = _entity->getPosition();
				_graphicsCamera->setTargetCameraPosition(Vector3(cameraPosition.x+_shakeTarget.x, cameraPosition.y+_shakeTarget.y,targetPosition.z));
			}
			//Si no nos podemos mover horizontalmente
			else if(!canMoveHorizontal(cameraPosition))
			{
				//Solo nos movemos verticalmente segun el movimiento relativo al player
				cameraPosition = Vector3(0,cameraMovY,0)+_entity->getPosition();
				_targetLastY = targetPosition.y;

				//Y como target establecemos la posicion en X de la camara y la posicion y del player
				targetPosition += Vector3(0, _targetHeight, 0);
				_graphicsCamera->setTargetCameraPosition(Vector3(cameraPosition.x+_shakeTarget.x, targetPosition.y+_shakeTarget.y,targetPosition.z));
			}
			//Si no nos podemos mover de forma vertical
			else if(!canMoveVertical(cameraPosition))
			{
				//Nos movemos horizontalmente segun el movimiento reltivo del player
				cameraPosition = Vector3(cameraMovX,0,0)+_entity->getPosition();
				_targetLastX = targetPosition.x;

				//Establecemos como target la posicion en X del personaje y en Y la de la camara sin moverla
				targetPosition += Vector3(0, _targetHeight, 0);
				_graphicsCamera->setTargetCameraPosition(Vector3(targetPosition.x+_shakeTarget.x, cameraPosition.y+_shakeTarget.y,targetPosition.z));
			}
			//Si nos podemos mover en cualqier direccion, entonces el movimiento es segun el movimiento relativo del player tanto en x como en y
			else
			{
				_targetLastX = targetPosition.x;
				_targetLastY = targetPosition.y;

				// Actualizamos la posición hacia donde mira la cámara.
				// Para ello sumamos a la posicion del target la altura. (por defecto la posicion del target apunta a la base del modelo)
				// Además le añadimos si hay algún temblor
				targetPosition += Vector3(0, _targetHeight, 0) + _shakeTarget;
			
				_shakeTarget = 0;
				//seteamos en la camara de ogre la posicion del target
				_graphicsCamera->setTargetCameraPosition(targetPosition);
			}

			_shakeTarget = 0;
			//seteamos la nueva posicion. Tanto en la entidad de la camara como en la camara de Ogre
			_entity->setPosition(cameraPosition);
			_graphicsCamera->setCameraPosition(cameraPosition);
	
		}

	} // tick

	//---------------------------------------------------------

	bool CCamera::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CAMERA_ZOOM" ||
			message->getType() == "STOP_TARGETING_CAM";
	} // accept

	//---------------------------------------------------------

	void CCamera::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("STOP_TARGETING_CAM") == 0)
		{
			_canTarget = dynamic_cast<STOP_TARGETING_CAM*>(message.get())->getBool();
			_shakeTarget = dynamic_cast<STOP_TARGETING_CAM*>(message.get())->getVector3();
		}
		else if(message->getType().compare("CAMERA_ZOOM") == 0)
		{
			_targetDistance += dynamic_cast<CAMERA_ZOOM*>(message.get())->getZoom();

			_entity->setPosition(Vector3(0, _targetHeight, _targetDistance) + _targetLastPos);

			_graphicsCamera->setCameraPosition(Vector3(0, _targetHeight, _targetDistance) + _targetLastPos);
		}
	} // process

} // namespace Logic
