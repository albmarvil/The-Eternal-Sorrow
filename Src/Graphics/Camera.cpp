

//---------------------------------------------------------------------------
// Camera.cpp
//---------------------------------------------------------------------------

/**
@file Camera.cpp

Contiene la implementación de la clase que maneja la cámara.

@see Graphics::CCamera

@author David Llansó
@date Julio, 2010
*/

#include "Camera.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <Ogre.h>
#include <OgreCamera.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CCamera::CCamera(const std::string &name, CScene *scene)
		: _scene(0)
	{
		_name = name;
		_scene = scene;

		// Creamos la estructura de nodos de la cámara. Los nodos cuelgan
		// de la raiz, son globales.
		_cameraNode = _scene->getSceneMgr()->getRootSceneNode()->
					createChildSceneNode(name + "_camera_node");
		_targetNode = scene->getSceneMgr()->getRootSceneNode()->
					createChildSceneNode(name + "_target_node");

		// Hacemos que el nodo de la cámara siempre esté mirando al nodo
		// objetivo.
		_cameraNode->setAutoTracking(true, _targetNode);
		// Fijamos el viraje de la cámara para se mantenga paralelo al
		// suelo.
		_cameraNode->setFixedYawAxis(true);

		_camera = scene->getSceneMgr()->createCamera(name + "_camera");
		//HACK: Valores cableados de las distancias para reenderizar. 
		// Deberían poder configurarse.
		_camera->setNearClipDistance(5);
		_camera->setFarClipDistance(500);
		_camera->setAutoAspectRatio(true);
		// Finalmente adjuntamos la cámara a su nodo.
		_cameraNode->attachObject (_camera);

		_compositorManager = Ogre::CompositorManager::getSingletonPtr();
		_actualCameraEffect = "";
	} // CCamera

	//--------------------------------------------------------

	CCamera::~CCamera() 
	{
		// desacoplamos la cámara de su nodo
		_cameraNode->detachAllObjects();
		_scene->getSceneMgr()->destroyCamera(_camera);
		_scene->getSceneMgr()->destroySceneNode(_cameraNode);
		_scene->getSceneMgr()->destroySceneNode(_targetNode);

	} // ~CCamera

	//--------------------------------------------------------
	
	const Vector3 &CCamera::getCameraPosition() 
	{
		return _cameraNode->getPosition();
	}

	//--------------------------------------------------------

	const Vector3 &CCamera::getTargetCameraPosition() 
	{
		return _targetNode->getPosition();
	}

	//--------------------------------------------------------

	const Quaternion &CCamera::getCameraOrientation() 
	{
		return _camera->getOrientation();
	}

	//--------------------------------------------------------

	void CCamera::setCameraPosition(const Vector3 &newPosition)
	{
		_cameraNode->setPosition(newPosition);
	}

	//--------------------------------------------------------

	void CCamera::setTargetCameraPosition(const Vector3 &newPosition)
	{
		_targetNode->setPosition(newPosition);
	}

	//--------------------------------------------------------


	void CCamera::setFOVyInRadians (float rads)
	{
		//Lo pasamos al tipo Radian
		Radian rad = Radian(rads);
		_camera->setFOVy(rad);
	}

	//--------------------------------------------------------


	void CCamera::setFOVyInDegrees (float degrees)
	{
		//lo convertimos a radianes y luego al tipo Radian
		Radian rad = Radian(Math::fromDegreesToRadians(degrees));
		_camera->setFOVy(rad);
	}

	Ogre::Radian CCamera::getFovyY() const
	{
		return _camera->getFOVy();
	}

	float CCamera::getAspectRatio() const
	{
		return _camera->getAspectRatio();
	}
	//--------------------------------------------------------

	void CCamera::setNearClipDistance (float distance)
	{
		_camera->setNearClipDistance(distance);
	}

	//--------------------------------------------------------

	void CCamera::setFarClipDistance (float distance)
	{
		_camera->setFarClipDistance(distance);
	}

	//--------------------------------------------------------

	void CCamera::setCompositorEffect(const std::string &effect, bool activate)
	{
		if(activate)
		{
			if(_actualCameraEffect != "")
			{
				_compositorManager->setCompositorEnabled(_scene->getViewport(), _actualCameraEffect, false);
				_compositorManager->removeCompositor(_scene->getViewport(), _actualCameraEffect);
			}

			_actualCameraEffect = effect;
			_compositorManager->addCompositor(_scene->getViewport(), _actualCameraEffect);
			_compositorManager->setCompositorEnabled(_scene->getViewport(), _actualCameraEffect, true);
		}
		else if (_actualCameraEffect == effect)
		{

			_compositorManager->setCompositorEnabled(_scene->getViewport(), _actualCameraEffect, false);
			_compositorManager->removeCompositor(_scene->getViewport(), _actualCameraEffect);
			
		}
		
	}

	Matrix4 CCamera::getViewMatrix() const
	{
		return _camera->getViewMatrix();
	}

	Matrix4 CCamera::getProjectionMatrix() const
	{
		return _camera->getProjectionMatrix();
	}

	Vector2 CCamera::getScreenCoord(const Vector3& positionWorld)
	{
		Vector3 point = _camera->getProjectionMatrix() * _camera->getViewMatrix() * positionWorld;
		Vector2 screenPoint = Vector2((point.x/2.0f) + 0.5f, 0.5f - (point.y/2.0f));

		return screenPoint;
	}
} // namespace Graphics
