/**
@file Server.cpp

@see Physics::CSceee

@author Cesar Manuel Paz Guzman
@refactor Enero, 2015
*/

#ifndef __Physics_Scene_H
#define __Physics_Scene_H


#include <map>
#include <string>


namespace Physics {
	class CCollisionManager;
}
namespace physx {
	class PxControllerManager;
	class PxScene;
	class PxCudaContextManager;
	class PxDefaultCpuDispatcher;
	class PxPhysics;
	class PxMaterial;
	class PxRigidActor;
}

namespace Physics {

	class CScene
	{

	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor físico.
		*/
		static CScene *getSingletonPtr() { return _instance; }

		/**
		Inicializa la escena fisica. Esta operación sólo es necesario realizarla
		una vez durante la inicialización de la aplicación. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la escena fisica. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Crea la escena física. Se asume que sólo existirá una escena física, por lo que 
		sólo debe invocarse una vez.
		*/
		void createScene (physx::PxPhysics *_physics, physx::PxCudaContextManager *_cudaContextManager,Physics::CCollisionManager *_collisionManager);

		/**
		Destruye la escena física.
		*/
		void destroyScene ();

		physx::PxScene * getScene() const { return _scene;}
		physx::PxControllerManager *getControllerManager() const { return _controllerManager;}

		bool tick(unsigned int msecs);
		physx::PxMaterial *getDefaultMaterial() const{ return _defaultMaterial;}
		
		/*
		Método encargado de la construcciónd e los diferentes materiales del juego
		*/
		void setUpMaterials(physx::PxPhysics *_physics);

		/*
		Este método devuelve el puntero al meterial asociado al nombre otorgado

		@param materilaName Nombre del material a buscar
		@return material físico asociado, NULL si no se encuentra
		*/
		physx::PxMaterial *getPhysicMaterialByName(const std::string &materialName);
		
	private:

		/**
		Constructor de la clase.
		*/
		CScene(){}

		/**
		Destructor de la clase.
		*/
		virtual ~CScene(){}

		// Instancia única de la clase.
		static CScene *_instance;

		physx::PxDefaultCpuDispatcher *_cpuDispatcher;

		// Escena física
		physx::PxScene *_scene;

		// Gestor de controller managers
		physx::PxControllerManager* _controllerManager;

		// Material que se aplica por defecto a las nuevas entidades físicas de la escena
		physx::PxMaterial *_defaultMaterial;

		typedef std::map<std::string, physx::PxMaterial*> MaterialMap;
		typedef std::pair<std::string, physx::PxMaterial*> MaterialPair;
		MaterialMap _materials;
	};
}
#endif
