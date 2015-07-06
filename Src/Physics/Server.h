/**
@file Server.h

Contiene la declaración del servidor de física. 

@see Physics::CServer

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#ifndef __Physics_Server_H
#define __Physics_Server_H

#include "BaseSubsystems/Log.h"
#include "BaseSubsystems/Math.h"

// Predeclaración de tipos
namespace Logic {
	class CEntity;
	class CPhysicController;
	class CPhysicEntity;
	class IPhysics;
};

namespace Physics {
	class CCollisionManager;
	class CErrorManager;
	class CActor;
	class CStaticActor;
	class CStaticPlane;
	class CDynamicActor;
	class CCapsuleController;
	class CScene;
};

namespace physx {
	class PxActor;
	class PxAllocatorCallback;
	class PxCapsuleController;
	class PxController;
	class PxControllerManager;
	class PxCooking;
	class PxDefaultAllocator;
	class PxDefaultCpuDispatcher;
	class PxErrorCallback;
	class PxFoundation;
	class PxMaterial;
	class PxPhysics;
	class PxProfileZoneManager;
	class PxRigidActor;
	class PxRigidDynamic;
	class PxRigidStatic;
	class PxScene;
	class PxCudaContextManager;
	class PxTransform;
	class PxExtendedVec3;
	class PxGeometry;

	namespace debugger {
		namespace comm {
			class PvdConnection;
		};
	};
};



// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/*
	Enumerados que sirven para identificar los grupos de colision y sus máscaras.
	hay varias normas IMPORTANTES:

	-CollisionGroup::Init debe ser siempre igual a 0
	-CollisionGroup::ePlanoColision debe ser siempre el último
	-CollisionGroup debe estar siempre con valores contiguos
	-Cada grupo de colision debe tener su máscara de colision a partir del enumerado de colision
	-A ser posible que dichas máscaras se elaboren en el mismo orden que los grupos de colision
	*/

	enum CollisionGroup
	{
		ePlanoColision = 0,
		eTiles = 1,
		eNoTrigger = 2,
		eEnemyT = 3,
		ePlayer = 4,
		eTriggers = 5,
		eBulletEnemNoClip = 6, // Bala del enemigo que no choca con tiles
		eBulletEnemClip = 7,   // Bala del enemigo que choca con tiles
		eBulletPlayerNoClip = 8,  // Bala del player que no choca con tiles
		eBulletPlayerClip = 9,    // Bala del player que choca con tiles
		eCoin = 10,				//grupo de colisión de las monedas
		eDoor = 11,
		eDoorNoCol = 12,
		eEnemyV = 13, //Enemigos voladores, igual que los terrestres pero no chocan con las plataformas especiales
		eTileAtravesableArriba = 14, //Tile especial atravesable si el player esta encima
		eTileAtravesableAbajo = 15, //Tile especial atravesable si el player esta abajo
		eLaserBall = 16,
		eUsableWeapon = 17,
		eProps = 18,
		END = 19,//Siempre ultimo
	};

	enum FilterMask
	{
		ePlanoColisionMask = (1<<ePlanoColision),
		eTilesMask = (1<<eTiles),
		eNoTriggerMask = (1<<eNoTrigger),
		eEnemyTMask = (1<<eEnemyT),
		ePlayerMask = (1<<ePlayer),
		eTriggersMask = (1<<eTriggers),
		eBulletEnemNoClipMask = (1 << eBulletEnemNoClip),
		eBulletEnemClipMask = (1 << eBulletEnemClip),
		eBulletPlayerNoClipMask = (1 << eBulletPlayerNoClip),
		eBulletPlayerClipMask = (1 << eBulletPlayerClip),
		eCoinMask = (1<<eCoin),
		eDoorMask = (1<<eDoor),
		eLaserBallMask = (1<<eLaserBall),
		eDoorNoColMask = (1<<eDoorNoCol),
		eEnemyVMask = (1<<eEnemyV),
		eTileAtravesableArribaMask = (1<<eTileAtravesableArriba),
		eTileAtravesableAbajoMask = (1<<eTileAtravesableAbajo),
		eUsableWeaponMask = (1<<eUsableWeapon),
		ePropsMask = (1<<eProps),
	};

	/**
	Servidor de física. Se encarga de gestionar las entidades físicas y simular su 
	comportamiento. Cumple varias finalidades:
	
	<ul>
	<li> Proporciona una interfaz común con el resto de servidores de la aplicación (gráficos, GUI, etc). </li> 
	<li> Aísla el resto de la aplicación del motor de física concreto que usamos (hasta cierto punto).
	<li> Realiza las conversiones entre los tipos lógicos y los de PhysX (vectores, matrices, etc).
	</ul>
	<p>
	Las entidades físicas de PhysX y los componentes lógicos que proporcionan la representación física de la entidad
	están relacionados de dos formas:
	<ul>
	<li> Los componentes lógicos almacenan internamente sus actores de PhysX asociados. </li>
	<li> Usamos el atributo userData de los actores de PhysX para almacenar el componente lógico asociado. </li>
	</ul>
	De esta forma, la capa lógica pueden mover los objetos físicos (actores cinemáticos, character controllers, 
	aplicar fuerzas a actores dinámicos, etc) y la capa de física puede comunicar las colisiones a los componentes
	lógicos.
	<p>
	Es aconsejable utilizar los métodos del servidor para cambiar / recuperar las posiciones de las
	entidades físicas, ya que el servidor realiza las conversiones necesarias entre sistemas de coordenadas.
	Aunque la lógica y PhysX usan el mismo tipo de sistema de coordenadas, PhysX asume que el origen está 
	en medio del objeto mientras que la lógica asume que el origen está a los pies del objeto. Usando los 
	métodos que	proporciona el servidor, la lógica no tiene que preocuparse de estas conversiones.
	<p>
	Esta clase está implementada como un Singleton de inicialización explícita: es necesario 
	invocar al método Init() al principio de la aplicación y al método Release() al final.

	@ingroup physicGroup

	@author Antonio Sánchez Ruiz-Granados
	@date Noviembre, 2012
	*/
	class CServer 
	{
	public:

		/**
		Devuelve la única instancia de la clase.

		@return Puntero al servidor físico.
		*/
		static CServer *getSingletonPtr() { return _instance; }
		
		/**
		Inicializa el servidor físico. Esta operación sólo es necesario realizarla
		una vez durante la inicialización de la aplicación. 

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el servidor físico. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		/**
		Realiza la simulación física y actualiza la posición de todas las 
		entidades físicas. 

		@param secs Millisegundos transcurridos desde la última actualización.
		@return Valor booleano indicando si todo fue bien.
		*/
		bool tick(unsigned int msecs);

		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////         CONSULTAS DE ESCENA           ////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
		Este método lanza un rayo de PhysX y devuelve todas las entidades lógicas que atraviese dicho rayo, ordenadas de mayor a menor
		distancia. NOTA IMPORTANTE. El vector devuelto pertenece a memoria estática. Si deseas conservarlo debes copiar las referencias
		dinámicas de su interior.

		@param origin Origen del rayo
		@param dir Dirección del rayo
		@param distance Distancia de alcance del rayo
		@param numMaxTouches Numero máximo de entidades devueltas. MAXIMO 64!!!. Si devuelve 5 entidades y ha tocado 6, devuelve las 5 más lejanas

		@return Vector de entidades lógicas alcanzadas. Ordenado de mayor a menor distancia.
		*/
		std::vector<Logic::CEntity*> raycastMultiple(const Vector3 &origin, const Vector3 &dir, float distance, const int numMaxTouches);

		/*
		Este método lanza un rayo de PhysX y devuelve todas las entidades lógicas que atraviese dicho rayo, ordenadas de mayor a menor
		distancia. NOTA IMPORTANTE. El vector devuelto pertenece a memoria estática. Si deseas conservarlo debes copiar las referencias
		dinámicas de su interior.

		@param ray Rayo de ogre en el que nos basamos para lanzar este rayo
		@param distance Distancia de alcance del rayo
		@param numMaxTouches Numero máximo de entidades devueltas. MAXIMO 64!!!. Si devuelve 5 entidades y ha tocado 6, devuelve las 5 más lejanas

		@return Vector de entidades lógicas alcanzadas. Ordenado de mayor a menor distancia.
		*/
		std::vector<Logic::CEntity*> raycastMultiple(const Ray &ray, float distance, const int numMaxTouches);

		/*
		Este método lanza un rayo de PhysX y devuelve todas las entidades lógicas que atraviese dicho rayo, ordenadas de mayor a menor
		distancia. Este método solo devuelve las entidades del grupo indicado.
		NOTA IMPORTANTE. El vector devuelto pertenece a memoria estática. Si deseas conservarlo debes copiar las referencias
		dinámicas de su interior.

		@param origin Origen del rayo
		@param dir Dirección del rayo
		@param distance Distancia de alcance del rayo
		@param numMaxTouches Numero máximo de entidades devueltas. MAXIMO 64!!!. Si devuelve 5 entidades y ha tocado 6, devuelve las 5 más lejanas
		@param group Grupo de colisión al que deben pertenecer las entidades con las cuales vamos a colisionar

		@return Vector de entidades lógicas alcanzadas. Ordenado de mayor a menor distancia.
		*/
		std::vector<Logic::CEntity*> raycastMultiple(const Vector3 &origin, const Vector3 &dir, float distance, const int numMaxTouches, FilterMask mask, std::vector<Vector3> &pointsCollision);

		/*
		Este método lanza un rayo de PhysX y devuelve todas las entidades lógicas que atraviese dicho rayo, ordenadas de mayor a menor
		distancia. Este método solo devuelve las entidades del grupo indicado.
		NOTA IMPORTANTE. El vector devuelto pertenece a memoria estática. Si deseas conservarlo debes copiar las referencias
		dinámicas de su interior.

		@param ray Rayo de ogre en el que nos basamos para lanzar este rayo
		@param distance Distancia de alcance del rayo
		@param numMaxTouches Numero máximo de entidades devueltas. MAXIMO 64!!!. Si devuelve 5 entidades y ha tocado 6, devuelve las 5 más lejanas
		@param group Grupo de colisión al que deben pertenecer las entidades con las cuales vamos a colisionar

		@return Vector de entidades lógicas alcanzadas. Ordenado de mayor a menor distancia.
		*/
		std::vector<Logic::CEntity*> raycastMultiple(const Ray &ray, float distance, const int numMaxTouches, FilterMask mask, std::vector<Vector3> &pointsCollision);

		/*
		Este método lanza un rayo de PhysX y devuelve la entidad lógica más cercana. La primera contra la que impacta.
		Si queremos saber el punto de impacto basta con pasarle una referencia de un Vector3 al último parámetro

		@param origin Origen del rayo
		@param dir Dirección del rayo
		@param distance Distancia de alcance del rayo
		@param (out) collisionPoint Devuelve en esta referencia el punto de colision. Es opcional

		@return Entidad más cercana contra la que colisiona.
		*/
		Logic::CEntity* raycastSimple(const Vector3 &origin, const Vector3 &dir, float distance, Vector3 &collisionPoint = Vector3(0,0,0));

		/*
		Este método lanza un rayo de PhysX y devuelve la entidad lógica más cercana. La primera contra la que impacta.
		Si queremos saber el punto de impacto basta con pasarle una referencia de un Vector3 al último parámetro

		@param ray Rayo de ogre en el que nos basamos para lanzar este rayo
		@param distance Distancia de alcance del rayo
		@param (out) collisionPoint Devuelve en esta referencia el punto de colision. Es opcional

		@return Entidad más cercana contra la que colisiona.
		*/
		Logic::CEntity* raycastSimple(const Ray &ray, float distance, Vector3 &collisionPoint = Vector3(0,0,0));

		/*
		Este método lanza un rayo de PhysX y devuelve la entidad lógica más cercana. La primera contra la que impacta.
		Si queremos saber el punto de impacto basta con pasarle una referencia de un Vector3 al último parámetro.
		Este método filtra para que la entidad tenga el grupo de colisión deseado.

		@param origin Origen del rayo
		@param dir Dirección del rayo
		@param distance Distancia de alcance del rayo
		@param group Grupo de colision de la entidad que buscamos
		@param (out) collisionPoint Devuelve en esta referencia el punto de colision. Es opcional

		@return Entidad más cercana contra la que colisiona.
		*/
		Logic::CEntity* raycastSimple(const Vector3 &origin, const Vector3 &dir, float distance, int group, Vector3 &collisionPoint = Vector3(0,0,0));

		/*
		Este método lanza un rayo de PhysX y devuelve la entidad lógica más cercana. La primera contra la que impacta.
		Si queremos saber el punto de impacto basta con pasarle una referencia de un Vector3 al último parámetro.
		Este método filtra para que la entidad tenga el grupo de colisión deseado.

		@param ray Rayo de ogre en el que nos basamos para lanzar este rayo
		@param distance Distancia de alcance del rayo
		@param group Grupo de colision de la entidad que buscamos
		@param (out) collisionPoint Devuelve en esta referencia el punto de colision. Es opcional

		@return Entidad más cercana contra la que colisiona.
		*/
		Logic::CEntity* raycastSimple(const Ray &ray, float distance, int group, Vector3 &collisionPoint = Vector3(0,0,0));

		/*
		Este método lanza un rayo de PhysX y devuelve la entidad lógica más cercana. La primera contra la que impacta.
		Si queremos saber el punto de impacto basta con pasarle una referencia de un Vector3 al último parámetro

		@param origin Origen del rayo
		@param dir Dirección del rayo
		@param distance Distancia de alcance del rayo
		@param (out) collisionPoint Devuelve en esta referencia el punto de colision. Es opcional

		@return Entidad más cercana contra la que colisiona.
		*/
		Logic::CEntity* raycastSimple(const Vector3 &origin, const Vector3 &dir, float distance, FilterMask mask, Vector3 &collisionPoint = Vector3(0,0,0));
	

		Vector3 raycastSimpleTile(const Vector3 &origin, const Vector3 &dir, float distance);
		/*
		Metodo que crea un escena fisica.
		*/
		void createScene ();

		/*
		Metodo que destruye una escena fisica
		*/
		void destroyScene();

		/*
		Metodo que devuelve el sdk de physX
		*/
		physx::PxPhysics * getSdk(){ return _physics;}

		/*
		Obtiene una referencia a la escena fisica
		*/
		Physics::CScene *getScene(){ return _scene;}

		/*
		Obtiene una referencia al cooking
		*/
		physx::PxCooking *getCooking(){ return _cooking;}

		/*
		Obtiene una referencia al gestor de colisiones
		*/
		Physics::CCollisionManager *getCollisionManager(){ return _collisionManager;}

		////////DEBUG VISUAL DE LA ESCENA/////////////

		/*
		Setea la visualización de debug de la escena. Aqui se configura qué es lo que se ve y cómo se ve.

		@param debug Configura si se ha de visualizar o no la información física
		*/
		void setDebugVisualization(bool debug);

		/*
		Consultor de la información de debug de la escena. Este método se llamará desde el proyecto Graphics para dibujar todas las líneas.

		@return Lista de pares de puntos que definen las lineas a dibujar.
		*/
		std::vector<std::pair<Vector3, Vector3>>* getDebugVisualizationInfo();
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////    UTILIDADES DE GRUPOS DE COLISION   ////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
		Establece con que grupos de colision debe haber colisiones. 

		@param actor Actor a los que se le indicara los grupos con los que colisionara
		@param collisionGroup grupo del actor
		*/
		void setupFiltering(physx::PxRigidActor* actor, CollisionGroup collisionGroup);
		

		/*
		Metodo que obtiene la máscara de colisión a partir de un grupo

		@param group grupo por el cual busco la máscara de colision

		@return Mascara de colision (FilterMask) del grupo.
		*/
		FilterMask getGroupCollision(CollisionGroup group){ return _groupCollisionMap[group];}

		/*
		Método que habilita o deshabilita un grupo de colisión en una máscara en tiempo de ejecución.
		Este método no modifica la memoria de mascaras del servidor.

		@param filterMask Máscara a modificar (parametro In/out)
		@param group Grupo que se quiere habilitar/deshabilitar en la máscara
		@param add Flag que indica si habilitamos/deshabilitamos el grupo en la máscara
		*/
		void UpdateGroup(unsigned int &filterMask, CollisionGroup group, bool add);

	private:

		/**
		Establece si debe haber colisiones entre dos grupos de colisión. Al crear los objetos
		físicos se establece el grupo de colisión al que pertenecen. Usando este método es
		posible activar / desactivar las colisiones entre grupos.

		@param group1 Primer grupo de colisión.
		@param group2 Segundo grupo de colisión
		@param enable Activar o desactivar las colisiones entre los dos grupos anteriores.
		*/
		void setGroupCollisions(CollisionGroup group1, CollisionGroup group2, bool enable);


		/**
		Establece si debe haber colisiones entre dos grupos de colisión. Al crear los objetos
		físicos se establece el grupo de colisión al que pertenecen. Usando este método es
		posible activar / desactivar las colisiones entre grupos.

		Este método desactiva o activa las colisiones para TODO el grupo

		@param group Primer grupo de colisión.
		@param enable Activar o desactivar las colisiones entre los dos grupos anteriores.
		*/
		void setGroupCollisions(CollisionGroup group, bool enable);

		/*
		Función que inicializa los grupos de colisión. Por defecto todos los grupos colisionan con todos, 
		excepto el plano de colisión que no colisiona con nadie
		*/
		void InicializarGruposColision();

		/*
		Método llamado exclusivamente después de InicializarGruposColision().
		Este método agrupa la configuración de los grupos de colisión actvados/desactivados que se diferencian
		de la configuración por defecto de TODOS chocan con TODOS
		*/
		void collisionGroupConfig();

		/*
		Map cuya clave es el grupo de colision y su valor la máscara de colisiones del mismo
		*/
		std::map<CollisionGroup, FilterMask> _groupCollisionMap;

		/**
		Constructor de la clase.
		*/
		CServer();

		/**
		Destructor de la clase.
		*/
		virtual ~CServer();

		// Instancia única de la clase.
		static CServer *_instance;

		// Gestor de errores
		physx::PxErrorCallback *_errorManager;

		// Gestor de memoria
		physx::PxAllocatorCallback *_allocator;

		// Distintos atributos relacionados con PhysX
		physx::PxFoundation *_foundation;
		physx::PxProfileZoneManager *_profileZoneManager;
		physx::PxCudaContextManager *_cudaContextManager;
		physx::debugger::comm::PvdConnection *_pvdConnection;
		physx::PxCooking *_cooking;

		//Escena fisica en el que se realiza la simulacion fisica
		Physics::CScene *_scene;
		// SDK de PhysX
		physx::PxPhysics *_physics;

		// Gestion de colisiones
		CCollisionManager *_collisionManager;

		
	}; // class CServer

}; // namespace Physics

#endif // __Physics_Server_H
