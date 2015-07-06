/*
author Cesar Manuel Paz Guzman
date Febrero 2015
*/

#include "FloatingObject.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Graphics/AnimationObject.h"
#include "Graphics.h"

namespace Logic 
{
	IMP_FACTORY(CFloatingObject);
	
	//---------------------------------------------------------

	CFloatingObject::~CFloatingObject()
	{
		if(_animationObject)
		{
			delete _animationObject;
			_animationObject = NULL;
		}
	}

	bool CFloatingObject::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_animationObject = new Graphics::CAnimationObject(_entity->getName());

		_tipoAnimacion = 0; //Animacion por defecto flotante.
		if(entityInfo->hasAttribute("tipoAnimation"))
			_tipoAnimacion = entityInfo->getIntAttribute("tipoAnimation");

		return true;
	} // spawn

	bool CFloatingObject::respawn(const Map::CEntity *entityInfo)
	{
		return true;
	}
	//------------------------------------------------------------------------

	bool CFloatingObject::activate()
	{
		_animationObject->createAnimation(_entity->getComponent<CGraphics*>()->getGraphicsEntity(),_tipoAnimacion);
		return true;
	}


	void CFloatingObject::deactivate()
	{
		_animationObject->destroyAnimationState();
	}


	void CFloatingObject::onTick(unsigned int msecs)
	{
		_animationObject->tick(msecs);	
	}

} // namespace Logic