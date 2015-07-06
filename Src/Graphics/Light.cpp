//---------------------------------------------------------------------------
// Light.cpp
//---------------------------------------------------------------------------

#include "Light.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreLight.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CLight::CLight(const Vector3 &diffuse, const Vector3 &specular, const Vector3 &position, const std::string &nameLight)
		: scene(0), loaded(false), diff(diffuse), name(nameLight), pos(position), spec(specular)
	{

	} // CLight

	//-------------------------------------------------------

	CLight::~CLight() 
	{
	} // ~CLight
	
	//--------------------------------------------------------
		
	bool CLight::attachToScene(CScene *sc)
	{
		// Si no está cargada forzamos su carga.
		if (!loaded)
		{
			scene = sc;
			return load();
		}

		// Si ya estaba cargada en la escena se devuelve cierto.
		return true;

	} // attachToScene
	
	//--------------------------------------------------------
		
	bool CLight::deattachFromScene()
	{
		// Si la luz no está cargada no se puede quitar de
		// una escena. Ya que no pertenecerá a ninguna.
		if(!loaded)
			return false;
		// Si la luz está cargada forzamos su descarga.
		else
		{
			assert(scene && "¡¡La luz debe estar asociada a una escena!!");
			unload();
			scene = 0;
		}

		return true;

	} // deattachFromScene
	
	//--------------------------------------------------------
		
	void CLight::setPosition(const Vector3 &posicion)
	{
		assert(light && "La luz no ha sido cargada");

		//Seteamos la posicion de la luz
		light->setPosition(posicion);
	}

	void CLight::setVisible(bool state)
	{
		assert(light && "La luz no ha sido cargada");
		light->setVisible(state);
	}

	void CLight::setSpecularColour(const Vector3 &specular)
	{
		assert(light && "La luz no ha sido cargada");

		//Seteamos la propiedad specular de la luz
		light->setSpecularColour(specular.x, specular.y, specular.z);
	}

	void CLight::setDiffuseColour(const Vector3 &diffuse)
	{
		assert(light && "La luz no ha sido cargada");

		//Seteamos la propiedad difusa de la luz
		light->setDiffuseColour(diffuse.x, diffuse.y, diffuse.z);
	}

	//--------------------------------------------------------

	bool CLight::load()
	{
		return true;

	} // load
	
	//--------------------------------------------------------
		
	void CLight::unload()
	{
		if(light)
		{
			// quitamos la luz de la escena
			scene->getSceneMgr()->destroyLight(light);
			light = 0;
		}

		loaded = false;
	} // load

	//--------------------------------------------------------
		
	void CLight::tick(float secs)
	{
	} // tick
} // namespace Graphics
