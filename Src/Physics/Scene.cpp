/**
@file Server.cpp

Contiene la implementación de la escena fisica

@see Physics::CScene

@author Cesar Manuel Paz Guzman
@date Febrero 2015
*/
#include "Scene.h"
#include <PxPhysicsAPI.h>
#include "CollisionManager.h"
#include <assert.h>

using namespace physx;
using namespace Physics;

// Única instancia del servidor
CScene *CScene::_instance = NULL;

bool CScene::Init() 
{
	if (!_instance) {
		_instance = new CScene();
	}

	return true;
} 

void CScene::Release()
{
	if(_instance) {
		delete _instance;
		_instance = NULL;
	}
} 

void CScene::createScene (physx::PxPhysics *_physics, physx::PxCudaContextManager *_cudaContextManager,Physics::CCollisionManager *_collisionManager)
{
	
	// Crear el descriptor de la escena
	PxSceneDesc sceneDesc(_physics->getTolerancesScale());

	// Establecer la gravedad en el eje Y
	sceneDesc.gravity = PxVec3(0.0f, -300.0f, 0.0f);
	
	// Establecer el gestor de colisiones
	sceneDesc.simulationEventCallback = _collisionManager;


	// Establecer un gestor de tareas por CPU
	if (!sceneDesc.cpuDispatcher) 
	{
		int mNbThreads = 1; // hilos de ejecución
		_cpuDispatcher  = PxDefaultCpuDispatcherCreate(mNbThreads);
		assert (_cpuDispatcher && "Error en PxDefaultCpuDispatcherCreate");

		sceneDesc.cpuDispatcher = _cpuDispatcher;
	}

	// Establecer el shader que controla las colisiones entre entidades.
	// Usamos un shader que emula la gestión de grupos de PhysX 2.8
	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = _collisionManager->filterShader;

	// Intentar establecer un gestor de tareas por GPU 
	// Sólo Windows
#ifdef PX_WINDOWS
	if (!sceneDesc.gpuDispatcher && _cudaContextManager)
	{
		sceneDesc.gpuDispatcher = _cudaContextManager->getGpuDispatcher();
	}
#endif

	//sceneDesc.flags = PxSceneFlag::eENABLE_KINEMATIC_PAIRS | 
	//	PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS | 
	//	PxSceneFlag::eENABLE_CCD |
	//	PxSceneFlag::eENABLE_STABILIZATION;


	// Crear la escena física
	_scene = _physics->createScene(sceneDesc);
	assert(_scene && "Error en PxPhysics::createScene");

	// Crear PxControllerManager. Es necesario para crear character controllers
	_controllerManager = PxCreateControllerManager(*_scene);

	//Por ultimo voy a crear el material por defecto de todos los actores que compondran la escena fisica
	float staticFriction = 0.5f;
	float dynamicFriction = 0.5f;
	float restitution = 0.1f;
	_defaultMaterial = _physics->createMaterial(staticFriction, dynamicFriction, restitution);

	setUpMaterials(_physics);

	// Activamos la notificación de eventos entre entidades kinemáticas.
	_scene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_PAIRS, true);
	_scene->setFlag(PxSceneFlag::eENABLE_KINEMATIC_STATIC_PAIRS, true);
	_scene->setFlag(PxSceneFlag::eENABLE_CCD, true);
}

void CScene::destroyScene ()
{
	if (_defaultMaterial) {
		_defaultMaterial->release();
		_defaultMaterial = NULL;
	}

	MaterialMap::iterator it = _materials.begin();
	MaterialMap::iterator end = _materials.end();

	for(; it != end; ++it)
	{
		it->second->release();
	}
	_materials.clear();

	if (_controllerManager) {
		_controllerManager->release();
		_controllerManager = NULL;
	}

	if (_scene) {
		_scene->release();
		_scene = NULL;
	}

	if (_cpuDispatcher) {
		_cpuDispatcher->release();
		_cpuDispatcher = NULL;
	}
}

bool CScene::tick(unsigned int msecs) 
{
	assert(_scene);

	// Empezar la simulación física. Actualmente usamos intervalos de tiempo variables,
	// debemos tener cuidado porque PhysX puede no comportarse bien si el tiempo 
	// transcurrido es demasiado grande.
	_scene->simulate(msecs * 0.001f);

	// Esperamos a que la simulación física termine. En principio podríamos utilizar
	// este intervalo de tiempo para hacer algo más útil. Existe una versión de este
	// método no bloqueante.
	return _scene->fetchResults(true);
} 



void CScene::setUpMaterials(physx::PxPhysics *_physics)
{

	///EJEMPLO
	//Cada material tiene 3 parámetros
	//float staticFriction = 0.5f;    ///friccion con objetos estáticos
	//float dynamicFriction = 0.5f;   ///fricción contra cuerpos dinamicos
	//float restitution = 0.1f;       ///valor de restitución o "rebote"

	///Material para las monedas con rebote
	std::string nameCoin = "coin";
	float staticFrictionCoin = 0.5f;    
	float dynamicFrictionCoin = 0.5f;   
	float restitutionCoin = 0.9f;      
	physx::PxMaterial* coinMaterial = _physics->createMaterial(staticFrictionCoin, dynamicFrictionCoin, restitutionCoin);
	MaterialPair pairCoin(nameCoin, coinMaterial);
	_materials.insert(pairCoin);

	std::string nameStone = "stone";
	float staticFrictionStone = 0.9f;    
	float dynamicFrictionStone = 0.5f;   
	float restitutionStone = 0.5f;      
	physx::PxMaterial* stoneMaterial = _physics->createMaterial(staticFrictionStone, dynamicFrictionStone, restitutionStone);
	MaterialPair pairStone(nameStone, stoneMaterial);
	_materials.insert(pairStone);
}


physx::PxMaterial* CScene::getPhysicMaterialByName(const std::string &materialName)
{
	MaterialMap::iterator find = _materials.find(materialName);

	if( find != _materials.end())
	{
		return find->second;
	}
	return NULL;
}
