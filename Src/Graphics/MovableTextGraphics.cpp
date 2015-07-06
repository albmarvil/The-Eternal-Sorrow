#include "MovableText.h"
#include "MovableTextGraphics.h"
#include <assert.h>
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/Log.h"

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics
{
	CMovableText::CMovableText(const std::string &name, const std::string &text, const std::string &fontName):
		_name(name), _text(text), _scene(0),_loaded(false), _fontName(fontName)
	{
		
	}

	CMovableText::~CMovableText(){
	}

	bool CMovableText::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡La particula debe asociarse a una escena!!");

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
		
	bool CMovableText::deattachFromScene()
	{
		if(!_loaded)
			return false;
		else
		{
			assert(_scene && "¡¡La particula debe estar asociada a una escena!!");
			unload();
			_scene = 0;
		}

		return true;

	} // deattachFromScene
	
	//--------------------------------------------------------
		
	bool CMovableText::load()
	{
		try
		{
			//Creo la particula con su nombre y su script
			_movableText = new Ogre::MovableText(_name, _text,_fontName);
			_movableText->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_ABOVE);
		}
		catch(std::exception e)
		{
			return false;
		}
		
		//Creo el nodo y lo asocio con la particula creada
		_sceneNode = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(_name + "_movableText");
		_sceneNode->attachObject(_movableText);

		_loaded = true;

		return true;
		
		
	} // load
	
	//--------------------------------------------------------
		
	void CMovableText::unload()
	{
		if(_sceneNode)
		{
			// desacoplamos la entidad de su nodo
			_sceneNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_sceneNode);
			_sceneNode = 0;
		}
		if(_movableText)
		{
			delete _movableText;
			_movableText = 0;
		}

		_loaded = false;

	} // load

	//--------------------------------------------------------
		
	void CMovableText::tick(float secs)
	{
	} // tick

	void CMovableText::setPosition(const Vector3 &position)
	{
		assert(_sceneNode && "La particula no ha sido cargada");

		_sceneNode->setPosition(position);

		//BaseSubsystems::Log::Debug("POSITION222: "+std::to_string(position.x)+","+std::to_string(position.y));
		
	} 
	
	void CMovableText::setText(const std::string &textoMostrar)
	{
		assert(_movableText && "MovableText no existe");
		_movableText->setCaption(textoMostrar);
	}

	void CMovableText::setColor(const std::string &color)
	{
		if (color == "red")
		{
			_movableText->setColor(Ogre::ColourValue::Red);
		}
		else if (color == "green")
		{
			_movableText->setColor(Ogre::ColourValue::Green);
		}
		else if (color == "blanco")
		{
			_movableText->setColor(Ogre::ColourValue::White);
		}
	}

	bool CMovableText::setAlpha(float constanteDegradacion)
	{
		if (_movableText->getColor().a> 0.01)
		{
			//BaseSubsystems::Log::Debug("Color Alpha: "+ std::to_string(_movableText->getColor().a));
			Ogre::ColourValue color = _movableText->getColor();
			color.a = color.a - constanteDegradacion;
			_movableText->setColor(color);
			return false;
		}
		else
		{
			return true;
		}
	}

	void CMovableText::setScale(const Vector3 &scale)
	{
		assert(_sceneNode && "La particula no ha sido cargada");
		_sceneNode->setScale(scale);
	}

	void CMovableText::setVisible(bool visible)
	{
		assert(_sceneNode && "La particula no ha sido cargada");
		_sceneNode->setVisible(visible);
	}
}