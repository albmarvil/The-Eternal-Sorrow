//---------------------------------------------------------------------------
// Scene.cpp
//---------------------------------------------------------------------------

/**
@file Scene.cpp

Contiene la implementación de la clase contenedora de los elementos
de una escena.

@see Graphics::CScene

@author David Llansó
@date Julio, 2010
*/

#include "Scene.h"
#include "Camera.h"
#include "Server.h"
#include "StaticEntity.h"
#include "Particle.h"
#include "LDirectional.h"
#include "LSpotLight.h"
#include "LPuntual.h"
#include "Light.h"
#include "BaseSubsystems/Server.h"
#include "BillBoards.h"
#include "MovableTextGraphics.h"
#include "RibbonTrail.h"
#include <assert.h>

#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreStaticGeometry.h>
#include <OgreColourValue.h>
#include <OgreParticleSystem.h>
#include <OgreEntity.h>
#include <OgreHardwarePixelBuffer.h>

#include "PoolOgreEntity.h"

namespace Graphics 
{
	CScene::CScene(const std::string& name) : _viewport(0), 
		_staticGeometry(0), _directionalLight(0), _statePause(false)
	{
		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();
		_mouse = BaseSubsystems::CServer::getSingletonPtr()->GetMouse();
		_sceneMgr = _root->createSceneManager(Ogre::ST_INTERIOR, name);
		_sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
		_camera = new CCamera(name,this);
		_name = name;

	} // CScene

	//--------------------------------------------------------

	CScene::~CScene() 
	{
		

		deactivate();
		_sceneMgr->destroyStaticGeometry(_staticGeometry);
		delete _camera;
		_root->destroySceneManager(_sceneMgr);

	} // ~CScene

	//--------------------------------------------------------

	CCamera* CScene::createCameraPause()
	{
		_cameraPause = new CCamera("Pause",this);

		return _cameraPause;
	}

	void  CScene::destroyCameraPause()
	{
		if(_cameraPause)
		{
			delete _cameraPause;
			_cameraPause = 0;
		}
	}
	
	void CScene::createViewportPause()
	{
		_texturePause = Ogre::TextureManager::getSingletonPtr()->createManual(
			"RTT",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,
			512,
			406,
			0,
			Ogre::PF_R8G8B8,
			Ogre::TU_RENDERTARGET);
		_texture = _texturePause->getBuffer()->getRenderTarget();
		_viewportPause = _texture->addViewport(_cameraPause->getCamera());
		_viewportPause->setOverlaysEnabled(false);
		_viewportPause->setClearEveryFrame(true);
		_viewportPause->setBackgroundColour(Ogre::ColourValue::Black);
	}

	void CScene::deleteViewportPause()
	{
		if(_viewportPause)
			_texture->removeAllViewports();
	}

	void CScene::pauseBone(bool pause)
	{
		TEntityList::const_iterator it = _dynamicEntities.begin();
		TEntityList::const_iterator end = _dynamicEntities.end();
		for(; it != end && !pause; it++)
		{
			(*it)->pauseBone(pause);
		}
		_statePause = !pause;
	}

	bool CScene::addEntity(CEntity* entity)
	{
		
		if(!entity->attachToScene(this))
			return false;
		
		_dynamicEntities.push_back(entity);
		return true;

	} // addEntity

	//--------------------------------------------------------

	bool CScene::addStaticEntity(CStaticEntity* entity)
	{
		if(!entity->attachToScene(this))
			return false;

		_staticEntities.push_back(entity);
		return true;

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::removeEntity(CEntity* entity)
	{
		entity->deattachFromScene();
		_dynamicEntities.remove(entity);

	} // addEntity

	bool CScene::addMovableText(CMovableText *_text)
	{
		if(!_text->attachToScene(this))
			return false;

		return true;
	}
	void CScene::removeMovableText(CMovableText *_text)
	{
		_text->deattachFromScene();
	}
	//--------------------------------------------------------

	void CScene::removeStaticEntity(CStaticEntity* entity)
	{
		entity->deattachFromScene();
		_staticEntities.remove(entity);

	} // addStaticEntity

	//--------------------------------------------------------

	void CScene::activate()
	{
		buildStaticGeometry();
		// HACK en pruebas
		_viewport = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()
						->addViewport(_camera->getCamera());
		_viewport->setBackgroundColour(Ogre::ColourValue::Black);

		//Aqui lleno el pool de entidades graficas 
		//CPoolOgreEntity::getSingletonPtr()->createGraphicsPool(_sceneMgr);

	} // activate

	//--------------------------------------------------------

	void CScene::deactivate()
	{
		if(_directionalLight)
		{
			_sceneMgr->destroyLight(_directionalLight);
			_directionalLight = 0;
		}
		if(_viewport)
		{
			BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow()->
					removeViewport(_viewport->getZOrder());
			_viewport = 0;
		}
		_sceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));

		//Destruimos todas las entidades graficas del pool
		//CPoolOgreEntity::getSingletonPtr()->destroyColaActual(_sceneMgr);
		

	} // deactivate
	
	//--------------------------------------------------------

	
	void CScene::tick(float secs)
	{	
		TEntityList::const_iterator it = _dynamicEntities.begin();
		TEntityList::const_iterator end = _dynamicEntities.end();
		for(; it != end && !_statePause; it++)
		{
			(*it)->tick(secs);
		}

	} // tick

	//--------------------------------------------------------

	void CScene::buildStaticGeometry()
	{
		if(!_staticGeometry && !_staticEntities.empty())
		{
			_staticGeometry = 
					_sceneMgr->createStaticGeometry("static");

			TStaticEntityList::const_iterator it = _staticEntities.begin();
			TStaticEntityList::const_iterator end = _staticEntities.end();
			for(; it != end; it++)
				(*it)->addToStaticGeometry();

			_staticGeometry->build();
		}

	} // buildStaticGeometry

	//--------------------------------------------------------

	bool CScene::addParticle(CParticle *particula, const Vector3 &pos)
	{
		if(!particula->attachToScene(this, pos))
			return false;

		return true;
	}

	//--------------------------------------------------------

	void CScene::removeParticle(CParticle *particula)
	{
		particula->deattachFromScene();
	}
	
	bool CScene::addLight(CLPuntual *puntual)
	{
		if(!puntual->attachToScene(this))
			return false;

		return true;
	}

	bool CScene::addLight(CLDirectional *direccional)
	{
		if(!direccional->attachToScene(this))
			return false;

		return true;
	}

	bool CScene::addLight(CLSpotlight *spotlight)
	{
		if(!spotlight->attachToScene(this))
			return false;

		return true;  
	}

	void CScene::removeLight(CLight *light)
	{
		light->deattachFromScene();
	}

	bool CScene::addBillBoards(CBillBoards *_BillBoards)
	{
		if(!_BillBoards->attachToScene(this))
			return false;

		return true;  
	}
	void CScene::removeBillBoards(CBillBoards *_BillBoards)
	{
		_BillBoards->deattachFromScene();
	}

	bool CScene::addRibbonTrail(CRibbonTrail *trail)
	{
		if(!trail->attachToScene(this))
			return false;

		return true;  
	}
	void CScene::removeRibbonTrail(CRibbonTrail *trail)
	{
		trail->deattachFromScene();
	}

	//---------------------------------------------------------------------------

	void CScene::setAmbientLight(Vector3 &ambientLight)
	{
		_sceneMgr->setAmbientLight(Ogre::ColourValue(ambientLight.x, ambientLight.y, ambientLight.z));
	}//setAmbientLight

	//--------------------------------------------------------------------------

} // namespace Graphics
