/**
@file Server.cpp

Contiene la implementación del servidor de física. 

@see Physics::CServer

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
@author Alberto Martínez Villarán
@refactor Enero, 2015
@refactor Cesar Manuel Paz Guzman
@setup Febrero, 2015
*/

#include "Server.h"
#include "Conversions.h"
#include "ErrorManager.h"
#include "CollisionManager.h"
#include "Scene.h"
#include "Actor.h"
#include "StaticActor.h"
#include "StaticPlane.h"
#include "DynamicActor.h"
#include "CapsuleController.h"

#include "Logic/Entity/Components/Physics.h"

#include <assert.h>
#include <algorithm>

#include <PxPhysicsAPI.h>
#include <extensions\PxExtensionsAPI.h>
#include <extensions\PxVisualDebuggerExt.h> 

using namespace Physics;
using namespace Logic;
using namespace physx;


// Única instancia del servidor
CServer *CServer::_instance = NULL;

//--------------------------------------------------------

CServer::CServer() : _cudaContextManager(NULL), _scene(NULL)
{
	// Crear gestor de errores
	_errorManager = new CErrorManager();

	// Crear gestor de memoria
	_allocator = new PxDefaultAllocator();

	// Crear gestor de colisiones
	_collisionManager = new CCollisionManager();

	// Crear PxFoundation. Es necesario para instanciar el resto de objetos de PhysX
	_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, *_allocator, *_errorManager);
	assert(_foundation && "Error en PxCreateFoundation");

	// Crear PxProfileZoneManager. Es necesario para habitiar algunas opciones de 
	// profiling de rendimiento en el PhysX Visual Debugger 
	_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(_foundation);
	assert(_profileZoneManager && "Error en PxProfileZoneManager::createProfileZoneManage");

	// Crear CudaContextManager. Permite aprovechar la GPU para hacer parte de la simulación física.
	// Se utiliza posteriormente al crear la escena física.
	// Sólo Windows
	#ifdef PX_WINDOWS

	PxCudaContextManagerDesc cudaContextManagerDesc;
	_cudaContextManager = PxCreateCudaContextManager(*_foundation, cudaContextManagerDesc, 
														   _profileZoneManager);
	if( _cudaContextManager )
	{
		if( !_cudaContextManager->contextIsValid() )
		{
			_cudaContextManager->release();
			_cudaContextManager = NULL;
		}
	}

	#endif

	// Crear PxPhysics. Es el punto de entrada al SDK de PhysX
	PxTolerancesScale toleranceScale;
	bool recordMemoryAllocations = true;

	_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *_foundation, toleranceScale, recordMemoryAllocations, _profileZoneManager);
	assert(_physics && "Error en PxCreatePhysics");

	// Inicializar el módulo PxCooking. Es necesario para cocinar mallas y para deserializar actores a partir de ficheros RepX
	PxCookingParams params(toleranceScale);
	_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *_foundation, params);

	// Inicializar extensiones
	if (!PxInitExtensions(*_physics))
		assert(false && "PxInitExtensions failed!");

	// Intentar conectar con PhysX Visual Debugger (PVD)
	_pvdConnection = NULL;

	// Sólo en modo DEBUG
	#ifdef _DEBUG
	debugger::comm::PvdConnectionManager *pvdConnectionManager = _physics->getPvdConnectionManager();
	assert(pvdConnectionManager && "Error en PxPhysics::getPvdConnectionManager");

	// Configurar máquina, puerto y tiempo de espera (en millisegundos)
	const char *ip = "127.0.0.1";
	int port = 5425;						
	unsigned int timeout = 100;				

	// Configurar qué información queremos mandar al PVD (debug, profile, memory)
	PxVisualDebuggerConnectionFlags connectionFlags = PxVisualDebuggerExt::getAllConnectionFlags();

	// Intentar establecer la conexión
	_pvdConnection = PxVisualDebuggerExt::createConnection(pvdConnectionManager, ip, port, timeout, connectionFlags);
	//esta linea habilita el debug de las scene queries
	_physics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
	
	#endif

	//Inicializo cada grupo con los grupos con los que puede colisionar
	InicializarGruposColision();
	collisionGroupConfig();
} //CServer()

//--------------------------------------------------------

CServer::~CServer() 
{
	// Destruir objetos en orden inverso a como fueron creados
	if (_pvdConnection) {
		_pvdConnection->release();
		_pvdConnection = NULL;
	}

	if (_cooking) {
		_cooking->release();
		_cooking = NULL;
	}

	if (_cudaContextManager) {
		_cudaContextManager->release();
		_cudaContextManager = NULL;
	}

	if (_physics) {
		_physics->release();
		_physics = NULL;
	}

	if (_profileZoneManager) { 
		_profileZoneManager->release();
		_profileZoneManager = NULL;
	}

	PxCloseExtensions();

	if (_foundation) {
		_foundation->release();
		_foundation = NULL;
	}
	
	if (_collisionManager) {
		delete _collisionManager;
		_collisionManager = NULL;
	}

	if (_allocator) {
		delete _allocator;
		_allocator = NULL;
	}

	if (_errorManager) {
		delete _errorManager;
		_errorManager = NULL;
	}
} //~CServer

//-------------------------------------------------------

bool CServer::Init() 
{
	if (!_instance) {
		_instance = new CServer();
	}

	return true;
} //Init

//--------------------------------------------------------

void CServer::Release()
{
	if(_instance) {
		delete _instance;
		_instance = NULL;
	}
} //Release

//--------------------------------------------------------

bool CServer::tick(unsigned int msecs) 
{
	assert(Physics::CScene::getSingletonPtr());

	return Physics::CScene::getSingletonPtr()->tick(msecs);
} //tick

//--------------------------------------------------------

void CServer::createScene()
{
	//Creamos la escena
	Physics::CScene::Init();
	_scene = Physics::CScene::getSingletonPtr();
	_scene->createScene(_physics,_cudaContextManager,_collisionManager);

}//createScene

//--------------------------------------------------------

void CServer::destroyScene()
{
	//Destruimos la escena
	if(Physics::CScene::getSingletonPtr())
	{
		Physics::CScene::getSingletonPtr()->destroyScene();
		Physics::CScene::Release();
		_scene = 0;
	}
}//destroyScene


//--------------------------------------------------------

void CServer::InicializarGruposColision()
{
	//En este metodo seteamos el mapa inicial de colisiones, esto se hace habilitando para todos los grupos las colisiones de todos contra todos
	//En el caso especial del plano de colision las deshabilitaremos totalmente.

	//creamos una mascara que tenga todos los bits a 1. Si ePlanoColision me marca el ultimo grupo y son N grupos, basta con poner N bits a 1.
	//Ej: CollisionGroup::ePlanoColision = 7 --> mask = 1111111

	int nGroups = CollisionGroup::END;

	unsigned int mask = 0;

	for(int j = CollisionGroup::ePlanoColision + 1; j < nGroups; ++j)
	{
		mask |= (1<<j);
	}
	FilterMask fMask = FilterMask(mask);

	for (int i = CollisionGroup::ePlanoColision + 1; i < nGroups; ++i)
	{
		CollisionGroup g = CollisionGroup(i);
		_groupCollisionMap.insert(std::pair<CollisionGroup, FilterMask>(g, fMask));

	}
	_groupCollisionMap.insert(std::pair<CollisionGroup, FilterMask>(CollisionGroup::ePlanoColision, FilterMask(0)));

}//InicializarGruposColision

//--------------------------------------------------------

void CServer::collisionGroupConfig()
{
	////////////////////////////

	// Balas enemigas que no colisionan con tiles ni enemigos
	setGroupCollisions(CollisionGroup::eBulletEnemNoClip, false); // Primero se pone que no colisione con nadie
	setGroupCollisions(CollisionGroup::eBulletEnemNoClip, CollisionGroup::ePlayer, true); // Y ya con quien sí colisionará

	// LaserBall no colisiona con nada menos con el laser
	setGroupCollisions(CollisionGroup::eLaserBall, false); // Primero se pone que no colisione con nadie

	// Balas enemigas que colsionan con el escenario
	setGroupCollisions(CollisionGroup::eBulletEnemClip, false);
	setGroupCollisions(CollisionGroup::eBulletEnemClip, CollisionGroup::ePlayer, true);
	setGroupCollisions(CollisionGroup::eBulletEnemClip, CollisionGroup::eTiles, true);

	// Balas del jugador que no colisionan con tiles ni con el jugador
	setGroupCollisions(CollisionGroup::eBulletPlayerNoClip, false);
	setGroupCollisions(CollisionGroup::eBulletPlayerNoClip, CollisionGroup::eEnemyT, true);
	setGroupCollisions(CollisionGroup::eBulletPlayerNoClip, CollisionGroup::eEnemyV, true);

	// Balas del jugador que no colisionan con el jugador
	setGroupCollisions(CollisionGroup::eBulletPlayerClip, false);
	setGroupCollisions(CollisionGroup::eBulletPlayerClip, CollisionGroup::eEnemyV, true);
	setGroupCollisions(CollisionGroup::eBulletPlayerClip, CollisionGroup::eEnemyT, true);
	setGroupCollisions(CollisionGroup::eBulletPlayerClip, CollisionGroup::eTiles, true);
	setGroupCollisions(CollisionGroup::eBulletPlayerClip, CollisionGroup::eDoor, true);

	//configuración del grupo de la moneda
	setGroupCollisions(CollisionGroup::eCoin, false);
	setGroupCollisions(CollisionGroup::eCoin, CollisionGroup::eTiles, true);
	setGroupCollisions(CollisionGroup::eCoin, CollisionGroup::eDoor, true);
	setGroupCollisions(CollisionGroup::eCoin, CollisionGroup::eDoorNoCol, true);
	//setGroupCollisions(CollisionGroup::eCoin, CollisionGroup::ePlayer, true);
	//setGroupCollisions(CollisionGroup::eCoin, CollisionGroup::eCoin, true);

	setGroupCollisions(CollisionGroup::eDoorNoCol, CollisionGroup::ePlayer, false);
	
	//Seteamos los grupos de colisión de las balas para que choquen contra la puerta
	setGroupCollisions(CollisionGroup::eDoorNoCol, CollisionGroup::eBulletEnemClip, true);
	setGroupCollisions(CollisionGroup::eDoorNoCol, CollisionGroup::eBulletEnemNoClip, true);
	setGroupCollisions(CollisionGroup::eDoorNoCol, CollisionGroup::eBulletPlayerClip, true);
	setGroupCollisions(CollisionGroup::eDoorNoCol, CollisionGroup::eBulletPlayerNoClip, true);

	//Seteamos los grupos de colision de la plataforma atravesable
	setGroupCollisions(CollisionGroup::eTileAtravesableArriba,true);
	setGroupCollisions(CollisionGroup::eTileAtravesableAbajo, true);
	setGroupCollisions(CollisionGroup::eTileAtravesableArriba, CollisionGroup::eEnemyV, false);
	setGroupCollisions(CollisionGroup::eTileAtravesableAbajo, CollisionGroup::eEnemyV, false);
	setGroupCollisions(CollisionGroup::eTileAtravesableAbajo, CollisionGroup::ePlayer, false);

	///Colision de los props, colisiona con todo, salvo con player y enemigos
	setGroupCollisions(CollisionGroup::eProps, true);
	setGroupCollisions(CollisionGroup::eProps, CollisionGroup::ePlayer, false);
	setGroupCollisions(CollisionGroup::eProps, CollisionGroup::eEnemyT, false);
	setGroupCollisions(CollisionGroup::eProps, CollisionGroup::eEnemyV, false);
}//collisionGroupConfig

//--------------------------------------------------------

void CServer::setupFiltering(physx::PxRigidActor* actor, CollisionGroup collisionGroup)
{

		//Guardamos en filterGròup el grupo al que pertenece el actor
		PxU32 filterGroup = (1<<collisionGroup);

		//Obtenemos la mascara de colision para este grupo

		PxU32 filterMask = _groupCollisionMap[collisionGroup];


		PxFilterData filterData;
		filterData.word0 = filterGroup; //word0 = grupo del actor
		filterData.word1 = filterMask;  //word1 = grupos con el que puede chocar el actor

		//Obtenemos la malla fisica
		PxShape* shapes;
		actor->getShapes(&shapes, 1, 0);

		//Y asignamos los filtros de colision a la malla fisica del actor, de manera que sepa con quien tiene que simular la colision
		shapes->setSimulationFilterData(filterData);
		shapes->setQueryFilterData(filterData);
}//setupFiltering

//--------------------------------------------------------


void CServer::setGroupCollisions(CollisionGroup group1, CollisionGroup group2, bool enable)
{

	unsigned int mask1 = _groupCollisionMap[group1];
	unsigned int  maskToModify = (1<<group2);

	FilterMask newMask1 = FilterMask(0);
	if(!enable)
	{
		maskToModify = ~maskToModify;
		newMask1 = FilterMask(mask1 & maskToModify);
	}
	else
	{
		newMask1 = FilterMask(mask1 | maskToModify);
	}

	_groupCollisionMap[group1] = newMask1;

	//repetimos para el segundo grupo
	unsigned int mask2 = _groupCollisionMap[group2];
	maskToModify = (1<<group1);

	FilterMask newMask2 = FilterMask(0);
	if(!enable)
	{
		maskToModify = ~maskToModify;
		newMask2 = FilterMask(mask2 & maskToModify);
	}
	else
	{
		newMask2 = FilterMask(mask2 | maskToModify);
	}

	_groupCollisionMap[group2] = newMask2;
}//setGroupCollisions

//--------------------------------------------------------

void CServer::setGroupCollisions(CollisionGroup group, bool enable)
{

	int nGroups = CollisionGroup::END;

	for (int i = CollisionGroup::ePlanoColision + 1; i < nGroups; ++i)
	{
		setGroupCollisions(group, CollisionGroup(i), enable);
	}
	
}//setGroupCollisions

//--------------------------------------------------------

void CServer::UpdateGroup(unsigned int &filterMask, CollisionGroup group, bool add)
{
	if(add)
	{
		filterMask |= (1<<group);
	}
	else
	{
		unsigned int mask = (1<<group);
		filterMask &= ~mask;
	}
}//UpdateGroup

//--------------------------------------------------------

std::vector<Logic::CEntity*> CServer::raycastMultiple(const Vector3 &origin, const Vector3 &dir, float distance, int numMaxTouches)
{
	std::vector<Logic::CEntity*> result;

	// Establecer parámetros del rayo
	PxVec3 orig = Vector3ToPxVec3(origin);   // origen     
	PxVec3 unitDir = Vector3ToPxVec3(dir);  // dirección normalizada   
	unitDir.normalize();
	PxReal maxDistance = distance;         // distancia máxima

	// Lanzar el rayo

	PxRaycastHit hits[64];
	PxRaycastBuffer buffer (hits, numMaxTouches);

	//flags del rayo
	PxHitFlags flags = PxHitFlag::eMESH_ANY;

	bool status = _scene->getScene()->raycast(orig, unitDir, maxDistance, buffer, flags);
	
	if(!status)//si no colisiona con nada devolvemos la lista vacía
	{
		return result;
	}

	for (PxU32 i = 0; i < buffer.nbTouches; i++)
	{
		PxRigidActor *actor = buffer.getTouch(i).shape->getActor();

		IPhysics *component = (IPhysics *) actor->userData;

		if (component) 
		{
			result.push_back( component->getEntity());
		}
		
	}

	return result;
}//raycastMultiple

////--------------------------------------------------------
std::vector<Logic::CEntity*> CServer::raycastMultiple(const Ray &ray, float distance, int numMaxTouches)
{
	Vector3 orig = ray.getOrigin();
	Vector3 dir = ray.getDirection();
	
	return raycastMultiple(orig, dir, distance, numMaxTouches);
}//raycastMultiple

////--------------------------------------------------------


std::vector<Logic::CEntity*> CServer::raycastMultiple(const Vector3 &origin, const Vector3 &dir, float distance, const int numMaxTouches, FilterMask mask, std::vector<Vector3> &pointsCollision)
{
	std::vector<Logic::CEntity*> result;

	// Establecer parámetros del rayo
	PxVec3 orig = Vector3ToPxVec3(origin);   // origen     
	PxVec3 unitDir = Vector3ToPxVec3(dir);  // dirección normalizada   
	unitDir.normalize();
	PxReal maxDistance = distance;         // distancia máxima

	// Lanzar el rayo

	PxRaycastHit hits[64];
	PxRaycastBuffer buffer (hits, numMaxTouches);

	//flags del rayo
	PxHitFlags flags = PxHitFlag::eMESH_ANY;

	bool status = _scene->getScene()->raycast(orig, unitDir, maxDistance, buffer, flags);
	
	if(!status)//si no colisiona con nada devolvemos la lista vacía
	{
		return result;
	}

	for (PxU32 i = 0; i < buffer.nbTouches; i++)
	{
		PxRigidActor *actor = hits[i].shape->getActor();

		PxU32 gr1 = hits[i].shape->getSimulationFilterData().word0;

		FilterMask mask1 = FilterMask(gr1);

		unsigned int res = mask & mask1;
		//comprobamos que es del grupo que queremos
		if (res > 1) 
		{
			IPhysics *component = (IPhysics *) actor->userData;

			if (component) 
			{
				pointsCollision.push_back(PxVec3ToVector3(hits[i].position));
				result.push_back(component->getEntity());
			}
		}
		
	}

	return result;

}//raycastMultiple

////--------------------------------------------------------
std::vector<Logic::CEntity*> CServer::raycastMultiple(const Ray &ray, float distance, int numMaxTouches, FilterMask mask,std::vector<Vector3> &pointsCollision)
{
	Vector3 orig = ray.getOrigin();
	Vector3 dir = ray.getDirection();
	
	return raycastMultiple(orig, dir, distance, numMaxTouches, mask,pointsCollision);
}//raycastMultiple

////--------------------------------------------------------


Logic::CEntity* CServer::raycastSimple(const Vector3 &origin, const Vector3 &dir, float distance, Vector3 &collisionPoint)
{
	// Establecer parámetros del rayo
	PxVec3 orig = Vector3ToPxVec3(origin);   // origen     
	PxVec3 unitDir = Vector3ToPxVec3(dir);  // dirección normalizada   
	unitDir.normalize();
	PxReal maxDistance = distance;         // distancia máxima

	// Lanzar el rayo

	PxRaycastBuffer buffer;

	//flags del rayo
	//PxHitFlags flags = PxHitFlag::eMESH_ANY;

	bool status = _scene->getScene()->raycast(orig, unitDir, maxDistance, buffer);
	
	if(!status)//si no colisiona con nada devolvemos null
	{
		return NULL;
	}

	collisionPoint = PxVec3ToVector3(buffer.block.position);

	PxRigidActor *actor = buffer.block.shape->getActor();

	IPhysics *component = (IPhysics *) actor->userData;

	if (component) 
	{
		return component->getEntity();
	}
		
	return NULL;

}//raycastSimple

////--------------------------------------------------------
Logic::CEntity* CServer::raycastSimple(const Ray &ray, float distance, Vector3 &collisionPoint)
{
	Vector3 orig = ray.getOrigin();
	Vector3 dir = ray.getDirection();
	
	return raycastSimple(orig, dir, distance, collisionPoint);
}//raycastSimple

////--------------------------------------------------------


Logic::CEntity* CServer::raycastSimple(const Vector3 &origin, const Vector3 &dir, float distance, int group, Vector3 &collisionPoint)
{
	// Establecer parámetros del rayo
	PxVec3 orig = Vector3ToPxVec3(origin);   // origen     
	PxVec3 unitDir = Vector3ToPxVec3(dir);  // dirección normalizada   
	unitDir.normalize();
	PxReal maxDistance = distance;         // distancia máxima

	// Lanzar el rayo

	PxRaycastHit hits[64];
	PxRaycastBuffer buffer (hits, 64);

	//flags del rayo
	//PxHitFlags flags = PxHitFlag::eMESH_ANY;

	bool status = _scene->getScene()->raycast(orig, unitDir, maxDistance, buffer);
	
	if(!status)//si no colisiona con nada devolvemos la lista vacía
	{
		return NULL;
	}

	float minDist = FLT_MAX;
	Logic::CEntity *closest = 0;
	PxVec3 minPoint = PxVec3(0,0,0);

	for (PxU32 i = 0; i < buffer.nbTouches; i++)
	{
		PxRigidActor *actor = hits[i].shape->getActor();
		PxU32 gr1 = (1<<group);
		PxU32 gr2 = hits[i].shape->getSimulationFilterData().word0;
		//comprobamos que es del grupo que queremos
		if (gr2 == gr1) 
		{
			float dist = (hits[i].position - orig).magnitudeSquared();

			if(dist <= minDist)
			{
				minDist = dist;
				minPoint = hits[i].position;

				IPhysics *component = (IPhysics *) actor->userData;

				if (component) 
				{
					closest = component->getEntity();
				}
			}
		}
	}

	if(minPoint.x != 0 || minPoint.y != 0 || minPoint.z != 0)
	{
		collisionPoint = PxVec3ToVector3(minPoint);
	}
	
	return closest;

}//raycastSimple

Vector3 CServer::raycastSimpleTile(const Vector3 &origin, const Vector3 &dir, float distance)
{
	// Establecer parámetros del rayo
	PxVec3 orig = Vector3ToPxVec3(origin);   // origen     
	PxVec3 unitDir = Vector3ToPxVec3(dir);  // dirección normalizada   
	unitDir.normalize();
	PxReal maxDistance = distance;         // distancia máxima

	PxRaycastHit hits[64];
	PxRaycastBuffer buffer (hits, 64);

	bool status = _scene->getScene()->raycast(orig, unitDir, maxDistance, buffer);
	
	if(!status)//si no colisiona con nada devolvemos la lista vacía
	{
		return Vector3::ZERO; 
	}

	for (PxU32 i = 0; i < buffer.nbTouches; i++)
	{
		PxU32 gr1 = (1<<CollisionGroup::eTiles);
		PxU32 gr3 = (1<<CollisionGroup::eTileAtravesableAbajo);
		PxU32 gr4 = (1<<CollisionGroup::eTileAtravesableArriba);
		PxU32 grPlayer = (1<<CollisionGroup::ePlayer);
		PxU32 gr2 = hits[i].shape->getSimulationFilterData().word0;

		if(gr2 == grPlayer)
		{
			return Vector3::ZERO;
		}
		else if (gr2 == gr1 || gr2 == gr3 || gr2 == gr4) 
		{
			return PxVec3ToVector3(hits[i].position);
		}	
		
	}

	return Vector3::ZERO;
}

////--------------------------------------------------------
Logic::CEntity* CServer::raycastSimple(const Ray &ray, float distance, int group, Vector3 &collisionPoint)
{
	Vector3 orig = ray.getOrigin();
	Vector3 dir = ray.getDirection();
	
	return raycastSimple(orig, dir, distance, group, collisionPoint);
}//raycastSimple

////--------------------------------------------------------


Logic::CEntity* CServer::raycastSimple(const Vector3 &origin, const Vector3 &dir, float distance, FilterMask mask, Vector3 &collisionPoint)
{
	// Establecer parámetros del rayo
	PxVec3 orig = Vector3ToPxVec3(origin);   // origen     
	PxVec3 unitDir = Vector3ToPxVec3(dir);  // dirección normalizada   
	unitDir.normalize();
	PxReal maxDistance = distance;         // distancia máxima

	// Lanzar el rayo

	PxRaycastHit hits[64];
	PxRaycastBuffer buffer (hits, 64);

	//flags del rayo
	//PxHitFlags flags = PxHitFlag::eNORMAL;

	bool status = _scene->getScene()->raycast(orig, unitDir, maxDistance, buffer);
	
	if(!status)//si no colisiona con nada devolvemos la lista vacía
	{
		return NULL;
	}

	float minDist = FLT_MAX;
	Logic::CEntity *closest = 0;
	PxVec3 minPoint = PxVec3(0,0,0);

	for (PxU32 i = 0; i < buffer.nbTouches; ++i)
	{
		PxRigidActor *actor = hits[i].shape->getActor();
		//excluimos el plano de colisión
		PxU32 gr1 = hits[i].shape->getSimulationFilterData().word0;

		FilterMask mask1 = FilterMask(gr1);

		unsigned int res = mask & mask1;
		//comprobamos que es del grupo que queremos
		if (res > 1) 
		{
			float dist = (hits[i].position - orig).magnitudeSquared();

			if(dist <= minDist)
			{
				minDist = dist;
				minPoint = hits[i].position;

				IPhysics *component = (IPhysics *) actor->userData;

				if (component) 
				{
					closest = component->getEntity();
				}
			}
		}	
	}

	if(minPoint.x != 0 || minPoint.y != 0 || minPoint.z != 0)
	{
		collisionPoint = PxVec3ToVector3(minPoint);
	}
	
	return closest;

}//raycastSimple

////-------------------------------------------------------------------


//---------------------------

void CServer::setDebugVisualization(bool debug)
{
	if(debug)
	{
		_scene->getScene()->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		//_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);
		_scene->getScene()->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	}
	else
	{
		_scene->getScene()->setVisualizationParameter(PxVisualizationParameter::eSCALE, 0.0f);
		//_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 0.0f);
		_scene->getScene()->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 0.0f);
	}
}//setDebugVisualization

//--------------------------------------------------------

std::vector<std::pair<Vector3, Vector3>>* CServer::getDebugVisualizationInfo()
{
	
	std::vector<std::pair<Vector3, Vector3>>* res = new std::vector<std::pair<Vector3, Vector3>>();
	const PxRenderBuffer& rb = _scene->getScene()->getRenderBuffer();
	for(PxU32 i=0; i < rb.getNbLines(); i++)
	{
			const PxDebugLine& line = rb.getLines()[i];
			Vector3 pos0 = Physics::PxVec3ToVector3(line.pos0);
			Vector3 pos1 = Physics::PxVec3ToVector3(line.pos1);
			std::pair<Vector3, Vector3> aux = std::pair<Vector3, Vector3>(pos0, pos1);
			res->push_back(aux);
	}

	return res;
}//getDebugVisualizationInfo

//--------------------------------------------------------



