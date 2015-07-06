/**
@file PhysicController.cpp

Contiene la implementaci�n del componente que se utiliza para representar jugadores y enemigos en
el mundo f�sico usando character controllers.

@see Logic::CPhysicController
@see Logic::CPhysicEntity
@see Logic::IPhysics

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
@refactor Alberto Mart�nez
@date Enero, 2015
*/

#include "PhysicController.h"
#include "EnemyMeleeAttack.h"


#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "Physics/CapsuleController.h"

#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
using namespace physx; 

IMP_FACTORY(CPhysicController);

//---------------------------------------------------------

CPhysicController::CPhysicController() : IPhysics(), _controller(NULL), 
	_movement(0,0,0), _falling(false), _upCollision(false), _numTicksNoSideCollision(0)
{

}

//---------------------------------------------------------

CPhysicController::~CPhysicController() 
{
	if (_controller) {
		//_controller->release();
		delete _controller;
		_controller = NULL;
	}

} 

//---------------------------------------------------------

bool CPhysicController::OnSpawn(const Map::CEntity *entityInfo)
{
	// Crear el character controller asociado al componente
	_controller = createController(entityInfo);
	_scene = _controller->getScene();

	_controller->setSimulation(false);
	return true;
}

//---------------------------------------------------------


bool CPhysicController::respawn(const Map::CEntity *entityInfo)
{
	_movement = Vector3::ZERO;

	return true;
}
	
bool CPhysicController::activate()
{
	//_scene->addActor(_controller->getActor());
	_controller->setSimulation(true);
	_controller->setControllerPosition(_entity->getPosition());

	return true;
}

void CPhysicController::deactivate()
{
	_controller->setControllerPosition(_entity->getPosition());
	_controller->setSimulation(false);
	//_scene->removeActor(_controller->getActor());
}

//---------------------------------------------------------

bool CPhysicController::accept(const std::shared_ptr<Logic::IMessage> &message)
{
	return message->getType() == "AVATAR_WALK" ||
		   message->getType() == "TELEPORT" ||
		   message->getType() == "SET_PHYSIC_GROUP";
} 

//---------------------------------------------------------

void CPhysicController::process(const std::shared_ptr<Logic::IMessage> &message)
{
	if(message->getType().compare("AVATAR_WALK") == 0)
	{
		// Anotamos el vector de desplazamiento para usarlo posteriormente en 
		// el m�todo tick. De esa forma, si recibimos varios mensajes AVATAR_WALK
		// en el mismo ciclo s�lo tendremos en cuenta el �ltimo.

		// Cada tick se acumula por cada mensaje el movimiento recibido y tras aplicarlo se setea a 0.
		_movement += dynamic_cast<AVATAR_WALK*>(message.get())->getVector3();
	}

	else if(message->getType().compare("TELEPORT") == 0)
	{
		Vector3 posDest = dynamic_cast<TELEPORT*>(message.get())->getVector3();

		_controller->setControllerPosition(posDest);
		
	}
	else if(message->getType() == "SET_PHYSIC_GROUP")
	{
		_controller->setCollisionGroup(dynamic_cast<SET_PHYSIC_GROUP*>(message.get())->getGroup());
	}

	else if(message->getType().compare("SET_PHYSIC_GROUP") == 0)
	{
		_controller->setCollisionGroup(dynamic_cast<SET_PHYSIC_GROUP*>(message.get())->getGroup());
	}

} 

//---------------------------------------------------------

void CPhysicController::onTick(unsigned int msecs) 
{
	
	/*if(_controller == NULL)
		return;*/
	// Actualizar la posici�n y orientaci�n de la entidad l�gica usando la 
	// informaci�n proporcionada por el motor de f�sica	
	_entity->setPosition(_controller->getControllerPosition(), this);


	// Intentamos mover el controller a la posici�n recibida en el �ltimo mensaje 
	// de tipo AVATAR_WALK. 
	unsigned flags = _controller->moveController( _movement, msecs);
	
	// Actualizamos el flag que indica si estamos cayendo
	_falling =  !(flags & PxControllerFlag::eCOLLISION_DOWN);

	////Avisamos a la entidad de que estamos "en el aire"
	std::shared_ptr<FALLING> m(new FALLING());
	m->setBool(_falling);
	_entity->emitMessage(m);

	// Ponemos el movimiento a cero
	_movement = Vector3::ZERO;
}

//---------------------------------------------------------

CCapsuleController* CPhysicController::createController(const Map::CEntity *entityInfo)
{
	// Obtenemos la posici�n de la entidad. Inicialmente colocaremos el controller
	// un poco por encima del suelo, porque si lo ponemos justo en el suelo a veces
	// lo atraviesa en el primer ciclo de simulaci�n.
	Vector3 position = _entity->getPosition() + Vector3(0, 0.5f, 0);

	// Leer el volumen de colisi�n del controller. Por ahora s�lo admitimos c�psulas.
	std::string shape = "capsule";
	if (entityInfo->hasAttribute("physic_shape")) {
		shape = entityInfo->getStringAttribute("physic_shape");
		assert(shape == "capsule");
	}

	// Leer el radio de la c�psula
	assert(entityInfo->hasAttribute("physic_radius"));
	float radius = entityInfo->getFloatAttribute("physic_radius");

	// Leer la altura de la c�psula
	assert(entityInfo->hasAttribute("physic_height"));
	float height = entityInfo->getFloatAttribute("physic_height");

	assert(entityInfo->hasAttribute("physic_group"));
	int group = entityInfo->getIntAttribute("physic_group");

	Vector3 offset = Vector3::ZERO;

	if(entityInfo->hasAttribute("physic_offset"))
	{
		offset = entityInfo->getVector3Attribute("physic_offset");
	}

	// Crear el controller de tipo c�psula
	return new CCapsuleController(position, radius, height, offset, group, this);
} 

//---------------------------------------------------------

void CPhysicController::onTrigger(IPhysics *otherComponent, bool enter)
{
	// Construimos un mensaje de tipo TOUCHED o UNTOUCHED y lo enviamos a 
	// todos los componentes de la entidad. 

	//OJO! hay que tener cuidado, puede que nos haya llegado un componente NULL
	CEntity* otherEntity = NULL;
	if(otherComponent != NULL)
	{
		otherEntity = otherComponent->getEntity();
	}
	
	//En caso de haber chocado con algo que no tiene componentes
	//la referencia de la entidad ser� NULL
	if (enter) {
		std::shared_ptr<TOUCHED> m(new TOUCHED());
		m->setEntity(otherEntity);
		_entity->emitMessage(m);
	} else {
		std::shared_ptr<UNTOUCHED> m(new UNTOUCHED());
		m->setEntity(otherEntity);
		_entity->emitMessage(m);

	}
}

//---------------------------------------------------------

void CPhysicController::onShapeHit (const PxControllerShapeHit &hit)
{
	// Si chocamos contra una entidad est�tica no hacemos nada
	PxRigidDynamic* actor = hit.shape->getActor()->isRigidDynamic();

	/*if(!actor)
		return;*/
	if(actor)
	{
		bool kinematic = actor->getRigidDynamicFlags() & PxRigidDynamicFlag::eKINEMATIC;

		// Si chocamos contra una entidad cinem�tica no hacemos nada
		if (kinematic)
			return;
	
		// Aplicar una fuerza a la entidad en la direcci�n del movimiento
		actor->addForce(hit.dir * hit.length * 1000.0f);
	}
	

	#pragma region Flags de colisiones
	
	//En el eje de las X cogemos la normal de choque. si la normal es 1 o -1 estaremos ante un choque contra una pared lateral
	// 1 -> choque por la izquierda
	// -1 -> Choque por la derecha
	float x = hit.worldNormal.x;

	//Para la Y cogemos la direcci�n en la que ibamos al chocar (esta si es como el movimiento natural)
	// 1-> choque por la parte superior
	// -1 ->choque por la parte inferior
	float y = hit.dir.y;

	_sideCollision = x == -1.0 || x == 1.0;

	if(_sideCollision)
	{
		_numTicksNoSideCollision = 0;

		std::shared_ptr<SIDE_COLLISION> mSIDE(new SIDE_COLLISION());
		mSIDE->setBool(_sideCollision);
		mSIDE->setNormalCollision(x);
		_entity->emitMessage(mSIDE, this);
	}
	else
	{
		++_numTicksNoSideCollision;
		if(_numTicksNoSideCollision == 2)
		{
			std::shared_ptr<SIDE_COLLISION> mSIDE(new SIDE_COLLISION());
			mSIDE->setBool(_sideCollision);
			mSIDE->setNormalCollision(0.0f);
			_entity->emitMessage(mSIDE, this);
		}
	}

	_upCollision = y == 1.0;
	if(_upCollision)
	{
		std::shared_ptr<UP_COLLISION> mUP(new UP_COLLISION());
		mUP->setBool(_upCollision);
		_entity->emitMessage(mUP, this);
	}
	#pragma endregion

}

//---------------------------------------------------------

void CPhysicController::onControllerHit (CEntity* other)
{
	//Avisamos a la entidad de que ha chocado contra otro controller
	std::shared_ptr<CONTROLLER_TOUCHED> m(new CONTROLLER_TOUCHED());
	m->setEntity(other);
	_entity->emitMessage(m);
}

//---------------------------------------------------------



void CPhysicController::avatarWalk(const Vector3 &movement)
{
	_movement += movement;
}


//
//bool CPhysicController::filterControllerColision (const CPhysicController *other)
//{
//	CEntity* otherEntity = other->getEntity();
//	bool res= false;
//	if(otherEntity->getType() == "Player" && _entity->getTag() == "enemy")
//	{
//		//si el enemigo choca contra el player comprobamos si este puede atacarlo
//		CEnemyMeleeAttack* meleeComponent = (CEnemyMeleeAttack*) _entity->getComponent("CEnemyMeleeAttack");
//		if(meleeComponent == NULL)///el enemigo no tiene componente de ataque melee, lo atravesamos
//		{
//			res =  false;
//		}
//		res =  meleeComponent->canDoDamage();
//	}
//
//	//BaseSubsystems::Log::Debug("========== Filtro Colision con "+otherEntity->getName()+" - "+_entity->getName()+" resultado: "+std::to_string(res));
//	return res;
//}
//---------------------------------------------------------



