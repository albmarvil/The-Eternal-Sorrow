//---------------------------------------------------------------------------
// LSpot.cpp
//---------------------------------------------------------------------------

/**
@file LSpot.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CLSpot

@author David Llansó
@date Julio, 2010
*/

#include "LPuntual.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreLight.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CLPuntual::CLPuntual(const std::string &namelight, const Vector3 &position, const Vector3 &especular, const Vector3 &diffuse)
		: CLight(diffuse, especular, position, namelight)
	{
	} 

	//-------------------------------------------------------

	CLPuntual::~CLPuntual() 
	{
		assert(!scene && "¡¡Para destruir una luz esta no puede pertenecer a una escena!!");
	} 

	//-------------------------------------------------------
		
	bool CLPuntual::load()
	{
		light = scene->getSceneMgr()->createLight(name);

		light->setType(Ogre::Light::LT_POINT);

		//Ponemos la posición inicial de la luz
		light->setPosition(pos);

		//Le damos un color difuso y uno especular
		light->setDiffuseColour(diff.x, diff.y, diff.z);
		light->setSpecularColour(spec.x, spec.y, spec.z);
		loaded = true;
		return true;

	} // load

	//--------------------------------------------------------
		
	void CLPuntual::tick(float secs)
	{
	} 

	//--------------------------------------------------------


	void CLPuntual::setAttenuation(float range, float constat, float linear, float quadratic)
	{
		light->setAttenuation(range, constat, linear, quadratic);
	}

} // namespace Graphics
