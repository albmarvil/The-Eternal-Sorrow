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
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor f�sico.
		*/
		static CScene *getSingletonPtr() { return _instance; }

		/**
		Inicializa la escena fisica. Esta operaci�n s�lo es necesario realizarla
		una vez durante la inicializaci�n de la aplicaci�n. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la escena fisica. Debe llamarse al finalizar la aplicaci�n.
		*/
		static void Release();

		/**
		Crea la escena f�sica. Se asume que s�lo existir� una escena f�sica, por lo que 
		s�lo debe invocarse una vez.
		*/
		void createScene (physx::PxPhysics *_physics, physx::PxCudaContextManager *_cudaContextManager,Physics::CCollisionManager *_collisionManager);

		/**
		Destruye la escena f�sica.
		*/
		void destroyScene ();

		physx::PxScene * getScene() const { return _scene;}
		physx::PxControllerManager *getControllerManager() const { return _controllerManager;}

		bool tick(unsigned int msecs);
		physx::PxMaterial *getDefaultMaterial() const{ return _defaultMaterial;}
		
		/*
		M�todo encargado de la construcci�nd e los diferentes materiales del juego
		*/
		void setUpMaterials(physx::PxPhysics *_physics);

		/*
		Este m�todo devuelve el puntero al meterial asociado al nombre otorgado

		@param materilaName Nombre del material a buscar
		@return material f�sico asociado, NULL si no se encuentra
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

		// Instancia �nica de la clase.
		static CScene *_instance;

		physx::PxDefaultCpuDispatcher *_cpuDispatcher;

		// Escena f�sica
		physx::PxScene *_scene;

		// Gestor de controller managers
		physx::PxControllerManager* _controllerManager;

		// Material que se aplica por defecto a las nuevas entidades f�sicas de la escena
		physx::PxMaterial *_defaultMaterial;

		typedef std::map<std::string, physx::PxMaterial*> MaterialMap;
		typedef std::pair<std::string, physx::PxMaterial*> MaterialPair;
		MaterialMap _materials;
	};
}
#endif
