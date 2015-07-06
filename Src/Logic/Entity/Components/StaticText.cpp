/**
@author Cesar Manuel Paz Guzman
@date Abril, 2015
*/

#include "StaticText.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Graphics/Scene.h"
#include "Graphics/MovableTextGraphics.h"
namespace Logic 
{
	IMP_FACTORY(CStaticText);
	
	//---------------------------------------------------------

	CStaticText::~CStaticText()
	{
		if(_text)
		{
			delete _text;
			_text = 0;
		}
	}

	bool CStaticText::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_text = new Graphics::CMovableText(_entity->getName(), "-", "TESFont");

		if(entityInfo->hasAttribute("scaleStaticText"))
		{
			_scaleStaticText = entityInfo->getVector3Attribute("scaleStaticText");		
		}

		if(entityInfo->hasAttribute("offsetStaticText"))
		{
			_offsetStaticText = entityInfo->getVector3Attribute("offsetStaticText");		
		}

		return true;
	} // spawn

	bool CStaticText::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("scaleStaticText"))
		{
			_scaleStaticText = entityInfo->getVector3Attribute("scaleStaticText");		
		}

		if(entityInfo->hasAttribute("offsetStaticText"))
		{
			_offsetStaticText = entityInfo->getVector3Attribute("offsetStaticText");		
		}

		return true;
	}
	//------------------------------------------------------------------------
	void CStaticText::createStaticText()
	{
		_entity->getMap()->getScene()->addMovableText(_text);
		_text->setPosition(_entity->getPosition() + _offsetStaticText);
		this->setScale(_scaleStaticText);
		setVisible(false);
	}

	bool CStaticText::activate()
	{
		createStaticText();
		return true;
	}

	void CStaticText::deactivate()
	{
		_entity->getMap()->getScene()->removeMovableText(_text);
	}

	void CStaticText::setText(const std::string &textoMostrar)
	{
		_text->setText(textoMostrar);
	}

	void CStaticText::setColor(const std::string &color)
	{
		_text->setColor(color);
	}

	void CStaticText::setScale(const Vector3 &scala)
	{
		_text->setScale(scala);
	}

	void CStaticText::setVisible(bool visible)
	{
		_text->setVisible(visible);
	}

	void CStaticText::onTick(unsigned int msecs)
	{
	}
} // namespace Logi