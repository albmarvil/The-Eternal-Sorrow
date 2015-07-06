/**
@file PhysicEntity.cpp

Contiene la implementación del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicEntity
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
*/

#include "PhysicEntity.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"

#include "Physics/StaticPlane.h" //Incluye StaticActor -> Incluye Actor -> Incluye Scene fisica
#include "Physics/DynamicActor.h"
#include "Physics/Server.h"
#define _USE_MATH_DEFINES
#include <math.h>

//#include <PxPhysicsAPI.h>

using namespace Logic;
using namespace Physics;
//using namespace physx; 

IMP_FACTORY(CPhysicEntity);

//---------------------------------------------------------

CPhysicEntity::CPhysicEntity() : IPhysics(), _actor(NULL), _movement(0,0,0), _transform(Matrix4::ZERO), _physicMaterial(""), _z0Constraint(false), _verticalConstraint(false), _collisionGroup(0)
{

}

//---------------------------------------------------------

CPhysicEntity::~CPhysicEntity() 
{
	if (_actor) {//Si existe la entidad fisica
		delete _actor; //El servidor fisico la destruye
		_actor = NULL;
	}

} 

////---------------------------------------------------------

bool CPhysicEntity::OnSpawn(const Map::CEntity *entityInfo)
{
	// Crear el objeto físico asociado al componente
	_actor = createActor(entityInfo);
	_scene = _actor->getScene();

	_simulation = false;
	_actor->setSimulation(false);

	return true;
} 

//---------------------------------------------------------

bool CPhysicEntity::respawn(const Map::CEntity *entityInfo)
{
	_movement = Vector3::ZERO;
	_transform = Matrix4::ZERO;
	//Seteo su posicion
	//_actor->setPosition(_entity->getPosition());
	_actor->setTransform(_entity->getTransform());
	if(entityInfo->hasAttribute("physic_group"))
	{
		_collisionGroup = entityInfo->getIntAttribute("physic_group");
	}
	_actor->setCollisionGroup(Physics::CollisionGroup(_collisionGroup));
	return true;
}

//---------------------------------------------------------

bool CPhysicEntity::activate()
{
	//_scene->addActor(_actor->getActor());

	if(_physicType == "dynamic")
	{
		dynamic_cast<CDynamicActor*>(_actor)->setSimulation(true);
		if(_z0Constraint)
		{
			dynamic_cast<CDynamicActor*>(_actor)->addZ0Constraint();
		}
		if(_verticalConstraint)
		{
			dynamic_cast<CDynamicActor*>(_actor)->addVerticalConstraint();
		}
		
	}
	_simulation = true;
	_actor->setTransform(_entity->getTransform());
	_actor->setSimulation(true);
	return true;
}

//---------------------------------------------------------

void CPhysicEntity::deactivate()
{
	//_scene->removeActor(_actor->getActor());
	_actor->setPosition(_entity->getPosition());
	_simulation = false;
	_actor->setSimulation(false);
}

//---------------------------------------------------------

bool CPhysicEntity::accept(const std::shared_ptr<Logic::IMessage> &message)
{
	return message->getType() == "KINEMATIC_MOVE" ||
			message->getType() == "SET_SIMULATION" ||
			message->getType() == "SET_PHYSIC_GROUP";
}

//---------------------------------------------------------

void CPhysicEntity::process(const std::shared_ptr<Logic::IMessage> &message)
{
	if(message->getType().compare("KINEMATIC_MOVE") == 0) 
	{
		// Acumulamos el vector de desplazamiento para usarlo posteriormente en 
		// el método tick.
		_movement += dynamic_cast<KINEMATIC_MOVE*>(message.get())->getVector3();
		///La transformada que recibimos se considera como objetivo de movimiento único
		_transform = dynamic_cast<KINEMATIC_MOVE*>(message.get())->getTransform();
	}
	else if(message->getType().compare("SET_SIMULATION") == 0) 
	{
		_simulation = dynamic_cast<SET_SIMULATION*>(message.get())->getSimulation();
		if(_physicType == "dynamic")
		{
			dynamic_cast<CDynamicActor*>(_actor)->setSimulation(_simulation);
		}

		_actor->setSimulation(_simulation);
	}
	else if(message->getType().compare("SET_PHYSIC_GROUP") == 0) 
	{
		_actor->setCollisionGroup(Physics::CollisionGroup(dynamic_cast<SET_PHYSIC_GROUP*>(message.get())->getGroup()));
	}
}

//---------------------------------------------------------

void CPhysicEntity::changePhysicsGroup(int group)
{
	_actor->setCollisionGroup(Physics::CollisionGroup(group));
}

void CPhysicEntity::onTick(unsigned int msecs) 
{
	if(_simulation)
	{
		// Comprobamos si es la entidad es estática o dinámica
		CDynamicActor *dinActor = NULL;
		if(_actor->isRigidDynamic())
			dinActor = (CDynamicActor*) _actor;

		// Si es una entidad estática no hacemos nada
		if (!dinActor) 
			return;

		// Actualizar la posición y la orientación de la entidad lógica usando la 
		// información proporcionada por el motor de física	
		_entity->setTransform(dinActor->getTransform());

		// Si el objeto físico es cinemático intentamos moverlo de acuerdo 
		// a los mensajes KINEMATIC_MOVE recibidos 
		if (dinActor->isKinematic()) 
		{
			if(_transform != Matrix4::ZERO)
			{
				dinActor->moveKinematicActor(_transform);
			}
			else
			{
				dinActor->moveKinematicActor(_movement);
			}
			_transform = Matrix4::ZERO;
			_movement = Vector3::ZERO;
		}
	}
	 
}

//---------------------------------------------------------


CActor* CPhysicEntity::createActor(const Map::CEntity *entityInfo)
{
	// Leer el tipo de entidad
	assert(entityInfo->hasAttribute("physic_entity"));
	 const std::string physicEntity = entityInfo->getStringAttribute("physic_entity");
	//assert((physicEntity == "rigid") || (physicEntity == "plane") || (physicEntity == "fromFile"));

	// Crear el tipo de entidad adecuada
	if (physicEntity == "plane") 
		return createPlane(entityInfo);
	
	if (physicEntity == "rigid") 
		return createRigid(entityInfo);
	
	if (physicEntity == "fromFile")
		return createFromFile(entityInfo);

	return NULL;
}

CStaticActor* CPhysicEntity::createPlane(const Map::CEntity *entityInfo)
{
	// La posición de la entidad es un punto del plano
	const Vector3 point = _entity->getPosition();
	
	// Leer el vector normal al plano
	assert(entityInfo->hasAttribute("physic_normal"));
	const Vector3 normal = entityInfo->getVector3Attribute("physic_normal");

	// Leer el grupo de colisión (por defecto grupo 0)
	_collisionGroup = 0;
	if (entityInfo->hasAttribute("physic_group"))
		_collisionGroup = entityInfo->getIntAttribute("physic_group");
	
	// Crear el plano
	return new CStaticPlane(point, normal, _collisionGroup, this);
}

CActor* CPhysicEntity::createRigid(const Map::CEntity *entityInfo)
{
	// Leer la posición de la entidad
	const Vector3 position = _entity->getPosition();
	
	// Leer el tipo de entidad: estáticos, dinámico o cinemático
	assert(entityInfo->hasAttribute("physic_type"));
	_physicType = entityInfo->getStringAttribute("physic_type");
	assert((_physicType == "static") || (_physicType == "dynamic") || (_physicType == "kinematic"));

	// Leer la forma (shape)
	assert(entityInfo->hasAttribute("physic_shape"));
	const std::string physicShape = entityInfo->getStringAttribute("physic_shape");

	// Leer si es un trigger (por defecto no)
	bool trigger = false;
	if (entityInfo->hasAttribute("physic_trigger"))
		trigger = entityInfo->getBoolAttribute("physic_trigger");

	// Leer el grupo de colisión (por defecto 0)
	_collisionGroup = 0;
	if (entityInfo->hasAttribute("physic_group"))
		_collisionGroup = entityInfo->getIntAttribute("physic_group");

	Vector3 offset = Vector3::ZERO;
	if (entityInfo->hasAttribute("physic_offset"))
		offset = entityInfo->getVector3Attribute("physic_offset");

	if (_physicType == "static") 
	{
		if (physicShape == "box") 
		{
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja estática, proporcionando la posicion de la entidad, las dimensiones del cubo, si es un triger, el grupo de colision y una referencia al componente fisico
			return new CStaticActor(position, physicDimensions, trigger, _collisionGroup, this, offset);
		}
	} 
	else 
	{
		if(entityInfo->hasAttribute("z0_constraint"))
		{
			_z0Constraint = entityInfo->getBoolAttribute("z0_constraint");
		}

		if(entityInfo->hasAttribute("vertical_constraint"))
		{
			_verticalConstraint = entityInfo->getBoolAttribute("vertical_constraint");
		}

		// Leer la masa (por defecto 0)
		float mass = 0;
		if (entityInfo->hasAttribute("physic_mass"))
			mass = entityInfo->getFloatAttribute("physic_mass");

		// Leer la masa (por defecto 0)
		_physicMaterial = "";
		if (entityInfo->hasAttribute("physic_material"))
			_physicMaterial = entityInfo->getStringAttribute("physic_material");
		
		// Leer si se trata de un actor cinemático
		bool kinematic = (_physicType == "kinematic");

		if (physicShape == "box") 
		{
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			const Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");
			
			// Crear una caja dinámica
			return new CDynamicActor(position, physicDimensions, mass, kinematic, trigger, _collisionGroup, this, offset, _physicMaterial);
		}
		else if(physicShape == "sphere")
		{
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_radius"));
			float radius = entityInfo->getFloatAttribute("physic_radius");
			
			// Crear una esfera dinámica
			return new CDynamicActor(position, radius, mass, kinematic, trigger, _collisionGroup, this, offset, _physicMaterial);
		}
		else if(physicShape == "capsule")
		{
			assert(entityInfo->hasAttribute("physic_radius"));
			float radius = entityInfo->getFloatAttribute("physic_radius");
			
			assert(entityInfo->hasAttribute("physic_height"));
			float height = entityInfo->getFloatAttribute("physic_height");

			// Crear una capsula dinámica
			return new CDynamicActor(position, radius, height, mass, kinematic, trigger, _collisionGroup, this, offset, _physicMaterial);
		}
	}

	return NULL;
}

CActor* CPhysicEntity::createFromFile(const Map::CEntity *entityInfo)
{
	// Leer la ruta del fichero RepX
	assert(entityInfo->hasAttribute("physic_file"));
	const std::string file = entityInfo->getStringAttribute("physic_file");
	
	// Leer el grupo de colisión (por defecto 0)
	_collisionGroup = 0;
	if (entityInfo->hasAttribute("physic_group"))
		_collisionGroup = entityInfo->getIntAttribute("physic_group");

	// Leer si es un trigger (por defecto no)
	bool trigger = false;
	if (entityInfo->hasAttribute("physic_trigger"))
		trigger = entityInfo->getBoolAttribute("physic_trigger");

	// Crear el actor a partir del fichero RepX
	return new CActor(file, _collisionGroup,trigger, this);
}


void CPhysicEntity::onTrigger(IPhysics *otherComponent, bool enter)
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
	//la referencia de la entidad será NULL
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


void CPhysicEntity::onContact(IPhysics *otherComponent, bool enter)
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
	//la referencia de la entidad será NULL
	if (enter) {
		/*std::shared_ptr<TOUCHED> m(new TOUCHED());
		m->setEntity(otherEntity);
		_entity->emitMessage(m);*/
		BaseSubsystems::Log::Debug("onContact enter y soy: "+_entity->getName());
	} else {
		/*std::shared_ptr<UNTOUCHED> m(new UNTOUCHED());
		m->setEntity(otherEntity);
		_entity->emitMessage(m);*/

	}
}

void CPhysicEntity::createDynamicBox(Vector3 &position, Vector3 &dimensions, float mass, bool kinematic, bool trigger, int group, Vector3 &offset, const std::string &material)
{
	_actor = new CDynamicActor(position, dimensions, mass, kinematic, trigger, group, this, offset, material);
}

void CPhysicEntity::createStaticBox(Vector3 &position, Vector3 &dimensions, float mass, bool kinematic, bool trigger, int group, Vector3 &offset)
{
	_actor = new CStaticActor(position, dimensions, trigger, group, this, offset);
}


void CPhysicEntity::kinematicMove(Vector3 &movement, const Matrix4 &transform)
{ 
	_movement += movement;

	if(transform != Matrix4::ZERO)
	{
		_transform = transform;
	}
	
}

