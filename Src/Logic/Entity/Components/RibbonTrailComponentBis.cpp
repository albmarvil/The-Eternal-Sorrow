/**
@file RibbonTrailComponentBis.cpp

@author Alberto Martínez
@date Mayo, 2015
*/

#include "RibbonTrailComponentBis.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Graphics/Scene.h"
#include "Map/MapEntity.h"

#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Entity/Components/WeaponController.h"
#include "Graphics/RibbonTrail.h"
#include "Graphics/SceneNode.h"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace Logic;

IMP_FACTORY(CRibbonTrailComponentBis);

CRibbonTrailComponentBis::~CRibbonTrailComponentBis() 
{

	delete _centerNode;
	delete _trail;
}

bool CRibbonTrailComponentBis::OnSpawn(const Map::CEntity *entityInfo)
{
	_trail = new Graphics::CRibbonTrail(_entity->getName() + "_bis");

	if(entityInfo->hasAttribute("trail2_visible"))
	{
		_startVisible = entityInfo->getBoolAttribute("trail2_visible");
	}

	if(entityInfo->hasAttribute("trail2_material"))
	{
		_trailMaterial = entityInfo->getStringAttribute("trail2_material");
	}

	if(entityInfo->hasAttribute("trail2_width"))
	{
		_width = entityInfo->getFloatAttribute("trail2_width");
	}

	if(entityInfo->hasAttribute("trail2_width_change"))
	{
		_widthChange = entityInfo->getFloatAttribute("trail2_width_change");
	}

	if(entityInfo->hasAttribute("trail2_length"))
	{
		_length = entityInfo->getFloatAttribute("trail2_length");
	}
	if(entityInfo->hasAttribute("trail2_offset"))
	{
		_offset = entityInfo->getVector3Attribute("trail2_offset");
	}

	if(entityInfo->hasAttribute("trail2_color_change"))
	{
		_colorChange = entityInfo->getVector3Attribute("trail2_color_change");
	}


	if(!_centerNode)
	{
		_centerNode = new Graphics::CSceneNode(_entity->getName() + "_bis");
	}
	

	return true;
} 

//---------------------------------------------------------

bool CRibbonTrailComponentBis::respawn(const Map::CEntity *entityInfo)
{


	if(entityInfo->hasAttribute("trail2_visible"))
	{
		_startVisible = entityInfo->getBoolAttribute("trail2_visible");
	}

	if(entityInfo->hasAttribute("trail2_material"))
	{
		_trailMaterial = entityInfo->getStringAttribute("trail2_material");
	}

	if(entityInfo->hasAttribute("trail2_width"))
	{
		_width = entityInfo->getFloatAttribute("trail2_width");
	}

	if(entityInfo->hasAttribute("trail2_width_change"))
	{
		_widthChange = entityInfo->getFloatAttribute("trail2_width_change");
	}

	if(entityInfo->hasAttribute("trail2_length"))
	{
		_length = entityInfo->getFloatAttribute("trail2_length");
	}

	if(entityInfo->hasAttribute("trail2_offset"))
	{
		_offset = entityInfo->getVector3Attribute("trail2_offset");
	}

	if(entityInfo->hasAttribute("trail2_color_change"))
	{
		_colorChange = entityInfo->getVector3Attribute("trail2_color_change");
	}


	if(!_centerNode)
	{
		_centerNode = new Graphics::CSceneNode(_entity->getName()+ "_bis");
	}
	return true;
}

//---------------------------------------------------------

bool CRibbonTrailComponentBis::activate()
{
	_entity->getMap()->getScene()->addRibbonTrail(_trail);

	assert(_centerNode);
	_centerNode->attachToScene(_entity->getMap()->getScene());
	_centerNode->setPosition(_entity->getCenterPosition() + _offset);
	_trail->setNodeToFollow(_centerNode);

	_trail->setMaterial(_trailMaterial);

	_trail->setColorChange(_colorChange.x, _colorChange.y, _colorChange.z);

	_trail->setTrailLength(_length);

	_trail->setInitialWidth(_width);

	_trail->setWidthChange(_widthChange);

	_trail->setVisible(_startVisible);

	return true;
	
}

//---------------------------------------------------------

void CRibbonTrailComponentBis::deactivate()
{

	assert(_centerNode);
	_centerNode->deattachToScene(_entity->getMap()->getScene());
	
	_entity->getMap()->getScene()->removeRibbonTrail(_trail);
}

//---------------------------------------------------------

bool CRibbonTrailComponentBis::accept(const std::shared_ptr<Logic::IMessage> &message)
{

	return message->getType() == "SET_TRAIL_VISIBLE";
}

//---------------------------------------------------------

void CRibbonTrailComponentBis::process(const std::shared_ptr<Logic::IMessage> &message)
{
	if (message->getType() == "SET_TRAIL_VISIBLE")
	{
		_startVisible = dynamic_cast<SET_TRAIL_VISIBLE*>(message.get())->getVisible();
		_trail->setVisible(_startVisible);
	}
}

//---------------------------------------------------------

void CRibbonTrailComponentBis::onTick(unsigned int msecs) 
{

	assert(_centerNode);
	_centerNode->setPosition(_entity->getCenterPosition() + _offset);
}

//---------------------------------------------------------

void CRibbonTrailComponentBis::setTrailWidthChange(float widthChange) 
{
	_trail->setWidthChange(widthChange);
}

//---------------------------------------------------------


void CRibbonTrailComponentBis::setTrailLength(float length) 
{
	_trail->setTrailLength(length);
}

//---------------------------------------------------------

void CRibbonTrailComponentBis::setTrailVisible(bool visible) 
{
	_trail->setVisible(visible);
}

//---------------------------------------------------------






