//---------------------------------------------------------------------------
// Server.cpp
//---------------------------------------------------------------------------

/**
@file Server.cpp

Contiene la implementación de la clase principal de gráficos, la que permite crear
la ventana, etc.

@see Graphics::CServer

@author David Llansó
@date Julio, 2010
*/

#include "Server.h"
#include "Scene.h"
#include "Camera.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OgreMeshManager.h>

#include <../OIS/OISMouse.h>

#pragma region Includes para debug físico
#include "PhysicsDebugVisualization.h"
#pragma endregion

#include "Graphics/ParticleManager.h"
#include "PoolOgreEntity.h"

namespace Graphics 
{
	CServer *CServer::_instance = 0;

	CServer::CServer() : _root(0), _renderWindow(0), _activeScene(0), _dummyScene(0)
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");

		_instance = this;

	} // CServer

	//--------------------------------------------------------

	CServer::~CServer() 
	{
		assert(_instance);

		_instance = 0;

	} // ~CServer

	//--------------------------------------------------------

	bool CServer::Init() 
	{
		assert(!_instance && "Segunda inicialización de Graphics::CServer no permitida!");

		new CServer();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CServer::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CServer::open()
	{
		if(!BaseSubsystems::CServer::getSingletonPtr())
			return false;

		_root = BaseSubsystems::CServer::getSingletonPtr()->getOgreRoot();

		_renderWindow = BaseSubsystems::CServer::getSingletonPtr()->getRenderWindow();

#pragma region Inicialización del singleton de dibujado físico
		Graphics::CPhysicsDebugVisualization::Init();
#pragma endregion

		if (!Graphics::CParticleManager::Init())
			return false;

		/*if(!CPoolOgreEntity::Init())
		{
			return false;
		}*/

		_dummyScene = createScene("dummy_scene");
		
		// Por defecto la escena activa es la dummy
		setScene(_dummyScene);

		return true;

	} // open

	//--------------------------------------------------------

	void CServer::close() 
	{
		if(_activeScene)
		{
			_activeScene->deactivate();
			_activeScene = 0;
		}
		while(!_scenes.empty())
		{
			removeScene(_scenes.begin());
		}

#pragma region Cierre del singleton de dibujado físico
		Graphics::CPhysicsDebugVisualization::Release();
#pragma endregion

		//CPoolOgreEntity::getSingletonPtr()->Release();

		Graphics::CParticleManager::getSingletonPtr()->Release();
	} // close

	//--------------------------------------------------------
		
	typedef std::pair<std::string,CScene*> TStringScenePar;

	CScene* CServer::createScene(const std::string& name)
	{
		//Nos aseguramos de que no exista ya una escena con este nombre.
		assert(_scenes.find(name)==_scenes.end() && 
			"Ya se ha creado una escena con este nombre.");

		CScene *scene = new CScene(name);
		TStringScenePar ssp(name,scene);
		_scenes.insert(ssp);
		return scene;

	} // createScene

	//--------------------------------------------------------

	void CServer::removeScene(CScene* scene)
	{
		// Si borramos la escena activa tenemos que quitarla.
		if(_activeScene == scene)
			_activeScene = 0;
		_scenes.erase(scene->getName());
		delete scene;

	} // removeScene

	//--------------------------------------------------------

	void CServer::removeScene(const std::string& name)
	{
		CScene* scene = (*_scenes.find(name)).second;
		removeScene(scene);

	} // removeScene

	//--------------------------------------------------------

	void CServer::removeScene(TScenes::const_iterator iterator)
	{
		CScene* scene = (*iterator).second;
		// Si borramos la escena activa tenemos que quitarla.
		if(_activeScene == scene)
			_activeScene = 0;
		_scenes.erase(iterator);
		delete scene;

	} // removeScene

	//--------------------------------------------------------

	void CServer::setScene(CScene* scene)
	{
		// En caso de que hubiese una escena activa la desactivamos.
		if(_activeScene)
			_activeScene->deactivate();

		if(scene)
		{
			// Sanity check. Nos aseguramos de que la escena pertenezca 
			// al servidor. Aunque nadie más puede crear escenas...
			assert((*_scenes.find(scene->getName())).second == scene && 
				"Esta escena no pertenece al servidor");

			_activeScene = scene;
		}
		// Si se añade NULL ponemos la escena dummy.
		else
			_activeScene = _dummyScene;

		_activeScene->activate();

	} // createScene

	//--------------------------------------------------------

	void CServer::setScene(const std::string& name)
	{
		// En caso de que hubiese una escena activa la desactivamos.
		if(_activeScene)
			_activeScene->deactivate();

		// Nos aseguramos de que exista una escena con este nombre.
		assert(_scenes.find(name) != _scenes.end() && 
			"Ya se ha creado una escena con este nombre.");
		_activeScene = (*_scenes.find(name)).second;

		_activeScene->activate();

	} // createScene

	//--------------------------------------------------------

	void CServer::tick(float secs) 
	{
		if(_activeScene != _dummyScene)
			_activeScene->tick(secs);
		if(_root)
		{
			// Actualizamos todas las ventanas de reenderizado.
			Ogre::WindowEventUtilities::messagePump();

#pragma region Dibujado de la info física
			Graphics::CPhysicsDebugVisualization::getSingletonPtr()->tick(secs);
#pragma endregion

			Graphics::CParticleManager::getSingletonPtr()->tick(secs);

			// Reenderizamos un frame
			_root->renderOneFrame(secs);
		}
	} // tick

	void CServer::createPlaneMesh(const std::string &meshName, const Vector3 normal, const float dist,const float width,const float height,const int nTriangles)
	{
		Ogre::Plane plane = Ogre::Plane(normal, dist);

		Ogre::MeshManager::getSingleton().createPlane(meshName,
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			plane,
			width,//ancho
			height,//alto
			nTriangles,//triangulos en x
			nTriangles,//triangulos en y
			true,//uso de normal
			1,
			1,//veces textura en u
			1,//veces textura en v
			normal.perpendicular()//vector up para pintado de textura
			);
	}//CreatePlaneMesh

	//-------------------------------------------------------------------

	Ray CServer::getCameraToViewportRay(float screenx, float screeny)
	{
		//Se obtiene la cámara actual de la escena
		Graphics::CCamera* camera = _activeScene->getCamera();

		//Se obtiene la camara de ogre y se llama a su función getCameraToViewportRay para obtener el rayo que debemos lanzar y por
		//lo devolvemos
		return camera->getCamera()->getCameraToViewportRay(screenx,screeny);
	}

} // namespace Graphics
