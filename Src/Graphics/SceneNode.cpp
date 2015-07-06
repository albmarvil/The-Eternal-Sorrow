//---------------------------------------------------------------------------
// SceneNode.cpp
//---------------------------------------------------------------------------

/**
@file SceneNode.cpp

Contiene la implementacion de la clase que representa un nodo de escena de Ogre. E

@see Graphics::CSceneNode

@author Alberto Martínez
@date Mayo, 2015
*/

#include "SceneNode.h"
#include "Scene.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics
{

	CSceneNode::CSceneNode(const std::string &name)
	{
		_scene = 0;
		_name = name;
	}

	CSceneNode::~CSceneNode()
	{
		if(_scene)
		{
			deattachToScene(_scene);
		}
	}

	void CSceneNode::setPosition(const Vector3 &position)
	{
		_node->setPosition(position);
	}

	void CSceneNode::deattachToScene(CScene* scene)
	{
		_scene->getSceneMgr()->destroySceneNode(_node);
		_scene = 0;
	}


	void CSceneNode::attachToScene(CScene* scene)
	{
		_scene = scene;
		_node = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(_name + "_WildNode");
	}

}