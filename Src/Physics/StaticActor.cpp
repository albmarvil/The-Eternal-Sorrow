/**
@file StaticActor.cpp

Contiene la implementación de la encapsulación del actor estático de física

@see Physics::CStaticActor

@author Alberto Martínez Villarán
@date Enero, 2015
*/

#include "StaticActor.h"
#include "Server.h"
#include "Conversions.h"

#include <PxPhysicsAPI.h>

using namespace physx;

using namespace Logic;



namespace Physics
{
	//CONSTRUCTOR PARA ACTORES ESTATICOS CON GEOMETRIA EN CUBO, Y MATERIAL POR DEFECTO DEFINIDO EN EL .H
	CStaticActor::CStaticActor(const Vector3 &position, const Vector3 &dimensions, bool trigger, int group, const IPhysics *component, Vector3 &offset, PxMaterial *material)
	{
		//Escena fisica actual
		_scene = _server->getScene()->getScene();

		// Crear geometria de un cubo estático
		PxBoxGeometry geom(Vector3ToPxVec3(dimensions));

		_dynamic = false;

		/* 
		   Nota: PhysX coloca el sistema de coordenadas local en el centro de la caja, mientras
		   que la lógica asume que el origen del sistema de coordenadas está en el centro de la 
		   cara inferior. Para unificar necesitamos realizar una traslación en el eje Y.
		   Afortunadamente, el descriptor que se usa para crear el actor permite definir esta 
		   transformación local, por lo que la conversión entre sistemas de coordenadas es transparente. 
		*/

		// Crear un cubo estático
		PxTransform pose(Vector3ToPxVec3(position));

		//OJO!!!! por ahora solo se usa este método en los TILES que son PREFABS y estos tienen el la posicion en el CENTRO de la figura
		//Por eso el local pose por ahora es 0 0 0
		PxTransform localPose(Vector3ToPxVec3(offset)); // Transformación de coordenadas lógicas a coodenadas de PhysX
		
		_actor = PxCreateStatic(*_server->getSdk(), pose, geom, *material, localPose);

		// Transformarlo en trigger si es necesario
		setTrigger(trigger);

		// Anotar el componente lógico asociado a la entidad física
		_actor->userData = (void *) component;

		// Añadir el actor a la escena
		_scene->addActor(*_actor);

		//Indicamos al actor con quien debe colisionar
		_server->setupFiltering(_actor, CollisionGroup(group));

		#pragma region Config de la visualización de debug

				PxShape *shape;
				_actor->getShapes(&shape, 1, 0);
				//Si no es trigger no queremos verlo, solo nos interesa si es trigger
				shape->setFlag(PxShapeFlag::eVISUALIZATION, trigger);
		#pragma endregion

		
	}//CStaticActor

	//-------------------------------------------------------------

	
}
