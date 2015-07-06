/*
author Cesar Manuel Paz Guzman
date Enero 2015
*/


#include "Directional.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include <assert.h>
#include "Graphics/LDirectional.h"
#include "Graphics/Light.h"
#include "Graphics/Scene.h"

namespace Logic
{
	IMP_FACTORY(CDirectional);

	CDirectional::~CDirectional() 
	{
		if(light)
		{
			delete light;
			light = 0;
		}
	} 

	bool CDirectional::OnSpawn(const Map::CEntity *entityInfo)
	{
		_scene = _entity->getMap()->getScene();

		assert(entityInfo->hasAttribute("diffuse"));
		diff = entityInfo->getVector3Attribute("diffuse");
		assert(entityInfo->hasAttribute("specular"));
		spec = entityInfo->getVector3Attribute("specular");
		assert(entityInfo->hasAttribute("direction"));
		dir = entityInfo->getVector3Attribute("direction");
		
		dir.normalise();
		//Nos creamos la nueva luz direccional
		light = new Graphics::CLDirectional(_entity->getName(), _entity->getPosition(), spec, dir, diff);
		assert(light && "Luz direccional mal creada");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CDirectional::respawn(const Map::CEntity* entityInfo)
	{
		//Actualizo la nueva escena a la que va a pertenecer la luz
		_scene = _entity->getMap()->getScene();

		assert(light && "Para activar una luz, esta debe de existir");

		assert(entityInfo->hasAttribute("diffuse"));
		diff = entityInfo->getVector3Attribute("diffuse");
		

		assert(entityInfo->hasAttribute("specular"));
		spec = entityInfo->getVector3Attribute("specular");
		

		assert(entityInfo->hasAttribute("direction"));
		dir = entityInfo->getVector3Attribute("direction");
		
		dir.normalise();

		return true;
	}

	bool CDirectional::activate()
	{
		//Añado la luz direccional a la escena
		if(!_scene->addLight((Graphics::CLDirectional *)light))
			return false;

		light->setPosition(_entity->getPosition());
		light->setDiffuseColour(diff);
		light->setSpecularColour(spec);
		((Graphics::CLDirectional *)light)->setOrientation(dir);
		return true;
	}

	//-----------------------------------------------------

	void CDirectional::deactivate()
	{
		assert(light && "Para desactivar una luz, esta debe de existir");

		//Elimino la luz de la escena
		_scene->removeLight(light);
		_scene = 0;
	}
}