//---------------------------------------------------------------------------
// LSpotlight.cpp
//---------------------------------------------------------------------------

/**
@file LSpotlight.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CLSpotlight

@author David Llansó
@date Julio, 2010
*/

#include "LSpotlight.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreLight.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CLSpotlight::CLSpotlight(const std::string &namelight, const Vector3 &position, const Vector3 &especular,
			const Vector3 &direction, const Vector2 &rang, const Vector3 &diffuse)
			: CLight(diffuse, especular, position, namelight), range(rang), dir(direction)
	{
	} 

	//-------------------------------------------------------

	CLSpotlight::~CLSpotlight() 
	{
		assert(!scene && "¡¡Para destruir una luz esta no puede pertenecer a una escena!!");
	} 
	
	//-------------------------------------------------------
		
	bool CLSpotlight::load()
	{
		light = scene->getSceneMgr()->createLight(name);

		light->setType(Ogre::Light::LT_SPOTLIGHT);
		
		//Ponemos la posición inicial de la luz
		light->setPosition(pos);

		//Ponemos la dirección en la que mira la luz
		light->setDirection(dir);

		//Le damos un color difuso y uno especular
		light->setDiffuseColour(diff.x, diff.y, diff.z);
		light->setSpecularColour(spec.x, spec.y, spec.z);

		loaded = true;

		return true;

	} // load

	//--------------------------------------------------------
		
	void CLSpotlight::tick(float secs)
	{
	} // tick

	//-------------------------------------------------------

	void CLSpotlight::setRange(const Vector2 &rang)
	{
		assert(light && "La luz no ha sido cargada");

		//Seteamos el rango en el que empieza a degradarse la luz focal
		light->setSpotlightRange(Ogre::Degree(rang.x), Ogre::Degree(rang.y));
	}

	void CLSpotlight::setOrientation(const Vector3 &orientation)
	{
		assert(light && "La luz no ha sido cargada");

		//Seteamos la direccion de la luz direccional
		light->setDirection(orientation);
	}
} // namespace Graphics
