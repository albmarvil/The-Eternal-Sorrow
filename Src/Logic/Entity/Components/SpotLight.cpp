/**
@file Lights.cpp

Contiene la implementación del componente que controla la luz de la escena
 
@see Logic::CLights
@see Logic::IComponent

@author Bárbara Domínguez
@date Enero, 2015
*/
#include "SpotLight.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include <assert.h>
#include "Graphics/LSpotLight.h"
#include "Graphics/Light.h"
#include "Graphics/Scene.h"

namespace Logic 
{
	IMP_FACTORY(CSpotLight);
	
	//---------------------------------------------------------

	CSpotLight::~CSpotLight() 
	{
		if(light)
		{
			delete light;
			light = 0;
		}
	} 

	bool CSpotLight::OnSpawn(const Map::CEntity *entityInfo)
	{

		_scene = _entity->getMap()->getScene();

		assert(entityInfo->hasAttribute("diffuse"));
		diff = entityInfo->getVector3Attribute("diffuse");
		assert(entityInfo->hasAttribute("specular"));
		spec = entityInfo->getVector3Attribute("specular");
		assert(entityInfo->hasAttribute("direction"));
		dir = entityInfo->getVector3Attribute("direction");

		if(entityInfo->hasAttribute("range"))
		{
			Vector3 aux = entityInfo->getVector3Attribute("range");
			range.x = aux.x;
			range.y = aux.y;
		}

		light = new Graphics::CLSpotlight(_entity->getName(), _entity->getPosition(), spec, dir, range, diff);
		assert(light && "Luz focal mal creada");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CSpotLight::respawn(const Map::CEntity* entityInfo)
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
		

		return true;
	}

	bool CSpotLight::activate()
	{

		//Añado la luz direccional a la escena
		if(!_scene->addLight((Graphics::CLSpotlight*)light))
			return false;

		light->setPosition(_entity->getPosition());
		light->setDiffuseColour(diff);
		light->setSpecularColour(spec);
		((Graphics::CLSpotlight *)light)->setOrientation(dir);

		//Nos registramos como oyentes de la pos logica de la entidad
		_entity->addEntityTransformListener(this);
		return true;
	}

	//-----------------------------------------------------

	void CSpotLight::deactivate()
	{
		assert(light && "Para desactivar una luz, esta debe de existir");

		//Elimino la luz de la escena
		_scene->removeLight(light);

		//Nos desregistramos como oyente de la pos logica de la entidad
		_entity->removeEntityTransformListener(this);

	}

	//-----------------------------------------------------

	bool CSpotLight::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return false;//message->getType() == "SET_TRANSFORM";
	}

	void CSpotLight::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		/*if(message->getType() == "SET_TRANSFORM")
		{
			Matrix4 trans = dynamic_cast<SET_TRANSFORM*>(message.get())->getTransform();
			light->setPosition(trans.getTrans());
		}*/
	}

	void CSpotLight::OnEntitySetTransform(const Matrix4 &transform)
	{
		light->setPosition(transform.getTrans());
	}

} // namespace Logic
