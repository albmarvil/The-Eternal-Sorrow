/**
@file RibbonTrailComponent.cpp

@author Alberto Martínez
@date Mayo, 2015
*/

#include "RibbonTrailComponent.h"
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

IMP_FACTORY(CRibbonTrailComponent);

CRibbonTrailComponent::~CRibbonTrailComponent() 
{
	if(_centerNode)
	{
		delete _centerNode;
	}
	delete _trail;
}

bool CRibbonTrailComponent::OnSpawn(const Map::CEntity *entityInfo)
{
	_trail = new Graphics::CRibbonTrail(_entity->getName());

	if(entityInfo->hasAttribute("needNode"))
	{
		_needNode = entityInfo->getBoolAttribute("needNode");
	}

	if(entityInfo->hasAttribute("trail_visible"))
	{
		_startVisible = entityInfo->getBoolAttribute("trail_visible");
	}

	if(entityInfo->hasAttribute("trail_material"))
	{
		_trailMaterial = entityInfo->getStringAttribute("trail_material");
	}

	if(entityInfo->hasAttribute("trail_width"))
	{
		_width = entityInfo->getFloatAttribute("trail_width");
	}

	if(entityInfo->hasAttribute("trail_width_change"))
	{
		_widthChange = entityInfo->getFloatAttribute("trail_width_change");
	}

	if(entityInfo->hasAttribute("trail_length"))
	{
		_length = entityInfo->getFloatAttribute("trail_length");
	}

	if(entityInfo->hasAttribute("trail_color_change"))
	{
		_colorChange = entityInfo->getVector3Attribute("trail_color_change");
	}

	if(entityInfo->hasAttribute("trail_center"))
	{
		_followCenter = entityInfo->getBoolAttribute("trail_center");

		if(_followCenter && !_centerNode)
		{
			_centerNode = new Graphics::CSceneNode(_entity->getName());
		}
	}

	return true;
} 

//---------------------------------------------------------

bool CRibbonTrailComponent::respawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("needNode"))
	{
		_needNode = entityInfo->getBoolAttribute("needNode");
	}

	if(entityInfo->hasAttribute("trail_visible"))
	{
		_startVisible = entityInfo->getBoolAttribute("trail_visible");
	}

	if(entityInfo->hasAttribute("trail_material"))
	{
		_trailMaterial = entityInfo->getStringAttribute("trail_material");
	}

	if(entityInfo->hasAttribute("trail_width"))
	{
		_width = entityInfo->getFloatAttribute("trail_width");
	}

	if(entityInfo->hasAttribute("trail_width_change"))
	{
		_widthChange = entityInfo->getFloatAttribute("trail_width_change");
	}

	if(entityInfo->hasAttribute("trail_length"))
	{
		_length = entityInfo->getFloatAttribute("trail_length");
	}
	if(entityInfo->hasAttribute("trail_color_change"))
	{
		_colorChange = entityInfo->getVector3Attribute("trail_color_change");
	}
	
	if(entityInfo->hasAttribute("trail_center"))
	{
		_followCenter = entityInfo->getBoolAttribute("trail_center");

		if(_followCenter && !_centerNode)
		{
			_centerNode = new Graphics::CSceneNode(_entity->getName());
		}
	}
	return true;
}

//---------------------------------------------------------

bool CRibbonTrailComponent::activate()
{
	_entity->getMap()->getScene()->addRibbonTrail(_trail);

	if(_followCenter) //seguimos el centro de la entidad
	{
		assert(_centerNode);
		_centerNode->attachToScene(_entity->getMap()->getScene());
		_centerNode->setPosition(_entity->getCenterPosition());
		_trail->setNodeToFollow(_centerNode);
	}
	else if(_needNode)//se engancha al nodo de escena de la entidad
	{
		CGraphics* cmp = (CGraphics*)_entity->getComponent("CGraphics");

		if(cmp != NULL)
		{
			_trail->setEntityToFollow(cmp->getGraphicsEntity());
		}
		else
		{
			CAnimatedGraphics *cmp2 = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");

			if(cmp2 != NULL)
			{
				_trail->setEntityToFollow(cmp2->getAnimatedEntity());
			}
			else
			{
				return false;
			}
		}
	}

	_trail->setMaterial(_trailMaterial);

	_trail->setTrailLength(_length);

	_trail->setInitialWidth(_width);

	_trail->setWidthChange(_widthChange);

	_trail->setColorChange(_colorChange.x, _colorChange.y, _colorChange.z);

	_trail->setVisible(_startVisible);

	return true;
	
}

//---------------------------------------------------------

void CRibbonTrailComponent::deactivate()
{
	if(_followCenter)
	{
		assert(_centerNode);
		_centerNode->deattachToScene(_entity->getMap()->getScene());
	}
	_entity->getMap()->getScene()->removeRibbonTrail(_trail);
}

//---------------------------------------------------------

bool CRibbonTrailComponent::accept(const std::shared_ptr<Logic::IMessage> &message)
{

	return (message->getType() == "SET_ARMTOWEAPON" && !_needNode && !_followCenter) ||
			message->getType() == "SET_TRAIL_VISIBLE";
}

//---------------------------------------------------------

void CRibbonTrailComponent::process(const std::shared_ptr<Logic::IMessage> &message)
{
	if(message->getType() == "SET_ARMTOWEAPON")
	{
		CWeaponController *cmp = (CWeaponController*) _entity->getComponent("CWeaponController");
		if(cmp != NULL)
		{
			_trail->setNodeToFollow(cmp->getPivotNode());
		}

		/*CGraphics* cmp = (CGraphics*)_entity->getComponent("CGraphics");

		if(cmp != NULL)
		{
			_trail->setTagPointToFollow(cmp->getGraphicsEntity());
		}
		else
		{
			CAnimatedGraphics *cmp2 = (CAnimatedGraphics*) _entity->getComponent("CAnimatedGraphics");

			if(cmp2 != NULL)
			{
				_trail->setTagPointToFollow(cmp2->getAnimatedEntity());
			}
		}*/
	}
	else if (message->getType() == "SET_TRAIL_VISIBLE")
	{
		_startVisible = dynamic_cast<SET_TRAIL_VISIBLE*>(message.get())->getVisible();
		_trail->setVisible(_startVisible);
	}
}

//---------------------------------------------------------

void CRibbonTrailComponent::onTick(unsigned int msecs) 
{
	if(_followCenter)
	{
		assert(_centerNode);
		_centerNode->setPosition(_entity->getCenterPosition());
	}
}

//---------------------------------------------------------

void CRibbonTrailComponent::setTrailWidthChange(float widthChange) 
{
	_trail->setWidthChange(widthChange);
}

//---------------------------------------------------------


void CRibbonTrailComponent::setTrailLength(float length) 
{
	_trail->setTrailLength(length);
}

//---------------------------------------------------------

void CRibbonTrailComponent::setTrailVisible(bool visible) 
{
	_trail->setVisible(visible);
}

//---------------------------------------------------------






