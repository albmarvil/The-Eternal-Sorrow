/**
@file RibbonTrail.cpp

Contiene la implementación de la clase CRibbonTrail

@author Alberto Martínez Villarán
@date Mayo de 2015

*/


#include "RibbonTrail.h"
#include <assert.h>
#include "Scene.h"
#include "SceneNode.h"
#include "Entity.h"

#include "BaseSubsystems/Log.h"

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreTagPoint.h>
#include <OgreSceneManager.h>
#include <OgreRibbonTrail.h>



namespace Graphics
{
	CRibbonTrail::CRibbonTrail(const std::string &name) : 
		_name(name), _entToFollow(0), _nodeToFollow(0), _material(""), _loaded(false)
	{
	}

	CRibbonTrail::~CRibbonTrail()
	{
	}

	bool CRibbonTrail::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡El trail debe asociarse a una escena!!");

		if(_loaded && (_scene != scene))
			return false;

		if (!_loaded)
		{
			_scene = scene;
			return load();
		}

		return true;

	} // attachToScene
	
	//--------------------------------------------------------
		
	bool CRibbonTrail::deattachFromScene()
	{
		if(!_loaded)
			return false;
		else
		{
			assert(_scene && "¡¡El trail debe estar asociada a una escena!!");
			unload();
			_scene = 0;
		}

		return true;

	} // deattachFromScene
	
	//--------------------------------------------------------
		
	bool CRibbonTrail::load()
	{
		try
		{
			//Creo el trail
			_trail = _scene->getSceneMgr()->createRibbonTrail(_name + "_trail");
		}
		catch(std::exception e)
		{
			return false;
		}
		
		//Creo el nodo y lo asocio con el trail creado
		_trailNode = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(_name + "_trailNode");
		_trailNode->attachObject(_trail);

		_loaded = true;

		return true;

	} // load
	
	//--------------------------------------------------------
		
	void CRibbonTrail::unload()
	{
		if(_trailNode)
		{
			// desacoplamos la entidad de su nodo
			_trailNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_trailNode);
			_trailNode = 0;
		}
		if(_trail)
		{
			_scene->getSceneMgr()->destroyRibbonTrail(_trail);
			_trail = 0;
		}

		_loaded = false;

	} // load

	//--------------------------------------------------------
		
	

	void CRibbonTrail::setMaterial(const std::string &materialName)
	{
		_trail->setMaterialName(materialName);
	}
	
	
	//--------------------------------------------------------
		
	void CRibbonTrail::setVisible(bool visible)
	{
		assert(_trailNode && "el trail no ha sido cargada");

		_trailNode->setVisible(visible);

	} 
	
	//--------------------------------------------------------

	void CRibbonTrail::setColorChange(float r, float g, float b, float a)
	{
		/*float red = std::max(r, 1.0f);
		float green = std::max(g, 1.0f);
		float blue = std::max(b, 1.0f);
		float alpha = std::max(a, 1.0f);*/

		Ogre::ColourValue cv = Ogre::ColourValue(r, g, b, a);
		_trail->setColourChange(0, cv);

	} 
	
	//--------------------------------------------------------

	void CRibbonTrail::setInitialColor(float r, float g, float b, float a)
	{
		float red = std::max(r, 1.0f);
		float green = std::max(g, 1.0f);
		float blue = std::max(b, 1.0f);
		float alpha = std::max(a, 1.0f);

		Ogre::ColourValue cv = Ogre::ColourValue(red, green, blue, alpha);
		_trail->setInitialColour(0,cv);

	} 
	
	//--------------------------------------------------------


	void CRibbonTrail::setTrailLength(float length)
	{
		_trail->setTrailLength(length);
	}

	void CRibbonTrail::setInitialWidth(float width)
	{
		_trail->setInitialWidth(0, width);
	}

	void CRibbonTrail::setWidthChange(float change)
	{
		_trail->setWidthChange(0, change);
	}
	

	void CRibbonTrail::setEntityToFollow(CEntity *entity)
	{
		if(_entToFollow)
		{
			_trail->removeNode(_entToFollow->getEntityNode());
		}
		_entToFollow = entity;
		_trail->addNode(entity->getEntityNode());
	}

	void CRibbonTrail::setTagPointToFollow(CEntity *entity)
	{
		if(_entToFollow)
		{
			_trail->removeNode(_entToFollow->getTagPoint());
		}
		_entToFollow = entity;
		_trail->addNode(entity->getTagPoint());
	}

	void CRibbonTrail::setNodeToFollow(CSceneNode* node)
	{
		if(_nodeToFollow)
		{
			_trail->removeNode(_nodeToFollow->getOgreNode());
		}
		_nodeToFollow = node;
		_trail->addNode(node->getOgreNode());
	}

}