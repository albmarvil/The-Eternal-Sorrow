#include "PuntualLight.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include <assert.h>
#include "Graphics/LPuntual.h"
#include "Graphics/Light.h"
#include "Graphics/Scene.h"

namespace Logic
{
	IMP_FACTORY(CPuntualLight);

	CPuntualLight::~CPuntualLight() 
	{
		if(light)
		{
			delete light;
			light = 0;
		}
	} 

	bool CPuntualLight::OnSpawn(const Map::CEntity *entityInfo)
	{
		_scene = _entity->getMap()->getScene();

		//leo los parametros obligatorios para esta luz
		assert(entityInfo->hasAttribute("diffuse"));
		diff = entityInfo->getVector3Attribute("diffuse");
		assert(entityInfo->hasAttribute("specular"));
		spec = entityInfo->getVector3Attribute("specular");
		
		//La creo
		light = new Graphics::CLPuntual(_entity->getName(), _entity->getPosition(), spec, diff);
		assert(light && "Luz puntual mal creada");

		//La añado a la escena
		if(!_scene->addLight((Graphics::CLPuntual *)light))
			return false;

		//configuracion de la atenuacion
		_hasAttenuation = false;
		if(entityInfo->hasAttribute("attenuation_range"))//si tiene uno de los parámetros suponemos el resto
		{
			_hasAttenuation = true;
			_range = entityInfo->getFloatAttribute("attenuation_range");
			assert(entityInfo->hasAttribute("attenuation_constant"));
			_constant = entityInfo->getFloatAttribute("attenuation_constant");
			assert(entityInfo->hasAttribute("attenuation_linear"));
			_linear = entityInfo->getFloatAttribute("attenuation_linear");
			assert(entityInfo->hasAttribute("attenuation_quadratic"));
			_quadratic = entityInfo->getFloatAttribute("attenuation_quadratic");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CPuntualLight::respawn(const Map::CEntity* entityInfo)
	{
		//Actualizo la nueva escena a la que va a pertenecer la luz
		_scene = _entity->getMap()->getScene();

		assert(light && "Para activar una luz, esta debe de existir");

		_hasAttenuation = false;
		//leo los parametros obligatorios para esta luz
		assert(entityInfo->hasAttribute("diffuse"));
		diff = entityInfo->getVector3Attribute("diffuse");
		

		assert(entityInfo->hasAttribute("specular"));
		spec = entityInfo->getVector3Attribute("specular");
		

		if(entityInfo->hasAttribute("attenuation_range"))//si tiene uno de los parámetros suponemos el resto
		{
			_hasAttenuation = true;
			_range = entityInfo->getFloatAttribute("attenuation_range");
			assert(entityInfo->hasAttribute("attenuation_constant"));
			_constant = entityInfo->getFloatAttribute("attenuation_constant");
			assert(entityInfo->hasAttribute("attenuation_linear"));
			_linear = entityInfo->getFloatAttribute("attenuation_linear");
			assert(entityInfo->hasAttribute("attenuation_quadratic"));
			_quadratic = entityInfo->getFloatAttribute("attenuation_quadratic");
		}

		return true;
	}

	bool CPuntualLight::activate()
	{
		//Añado la luz puntual a la escena
		if(!_scene->addLight((Graphics::CLPuntual *)light))
			return false;

		light->setPosition(_entity->getPosition());

		if(_hasAttenuation)
		{
			((Graphics::CLPuntual *)light)->setAttenuation(_range, _constant, _linear, _quadratic);
		}	

		light->setDiffuseColour(diff);
		light->setSpecularColour(spec);

		//Nos registramos como observador de la posicion logica de la entidad
		_entity->addEntityTransformListener(this);
		return true;
	}

	//-----------------------------------------------------

	void CPuntualLight::deactivate()
	{
		assert(light && "Para desactivar una luz, esta debe de existir");

		//Elimino la luz de la escena
		_scene->removeLight(light);

		//Nos desregistramos de observador de la posicion logica de la entidad
		_entity->removeEntityTransformListener(this);

	}

	//-----------------------------------------------------


	bool CPuntualLight::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//return message->getType() == "SET_TRANSFORM" ||
		return	message->getType() == "SET_ATTENUATION";
	}

	void CPuntualLight::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		/*if(message->getType() == "SET_TRANSFORM")
		{
			Matrix4 trans = dynamic_cast<SET_TRANSFORM*>(message.get())->getTransform();
			light->setPosition(trans.getTrans());
		}
		else*/ if (message->getType() == "SET_ATTENUATION")
		{
			Vector4 att = dynamic_cast<SET_ATTENUATION*>(message.get())->getAttenuation();
			((Graphics::CLPuntual*)light)->setAttenuation(att.x,att.y, att.z, att.w);
		}
	}

	void CPuntualLight::OnEntitySetTransform(const Matrix4 &transform)
	{
		light->setPosition(transform.getTrans());
	}
}