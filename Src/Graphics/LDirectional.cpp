//---------------------------------------------------------------------------
// LDirectional.cpp
//---------------------------------------------------------------------------

/**
@file LDirectional.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CLDirectional

@author David Llansó
@date Julio, 2010
*/

#include "LDirectional.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreLight.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CLDirectional::CLDirectional(const std::string &namelight, const Vector3 &position, const Vector3 &especular,
		const Vector3 &direction, const Vector3 &diffuse) : CLight(diffuse, especular, position, namelight), dir(direction)
	{
	} 

	//-------------------------------------------------------

	CLDirectional::~CLDirectional() 
	{
		assert(!scene && "¡¡Para destruir una luz esta no puede pertenecer a una escena!!");
	} 
	
	//-------------------------------------------------------
		
	bool CLDirectional::load()
	{
		light = scene->getSceneMgr()->createLight(name);

		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setDirection(dir);

		//Ponemos la posición inicial de la luz
		light->setPosition(pos);

		//Le damos un color difuso y uno especular
		light->setDiffuseColour(diff.x, diff.y, diff.z);
		light->setSpecularColour(spec.x, spec.y, spec.z);

		loaded = true;

		return true;

	} // load

	//--------------------------------------------------------
		
	void CLDirectional::tick(float secs)
	{
	} // tick

	//--------------------------------------------------------
	
	void CLDirectional::setOrientation(const Vector3 &orientation)
	{
		assert(light && "La luz no ha sido cargada");

		//Seteamos la direccion de la luz direccional
		light->setDirection(orientation);
	}
} // namespace Graphics
