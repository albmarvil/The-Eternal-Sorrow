/**
@file StaticActor.h

Contiene la declaración de la encapsulación del actor de física estático. 

La clase contiene un constructor por defecto vacío, pero el que se usa
es el constructor que toma varios parámetros.

@see Physics::CStaticActor

@author Alberto Martínez Villarán
@date Enero, 2015
*/

#ifndef __Physics_StaticActor_H
#define __Physics_StaticActor_H

#include "Actor.h"
#include "BaseSubsystems/Math.h"

// Predeclaración de tipos

namespace physx {
	class PxPhysics;
	class PxScene;
	class PxGeometry;
	class PxMaterial;
};

namespace Logic
{
	class IPhysics;
};


// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	/**
	descripcion de la clase CStaticActor
	@ingroup physicGroup

	@author Alberto Martínez Villarán
	@date Enero, 2015
	*/
	class CStaticActor : public CActor 
	{
	public:

		CStaticActor(){}

		/*
		Este constructor crea un actor físico de forma que es estático

		Es el método usado por los tiles a la hora de crearse en el preprocesado.
		El constructor esta preparado para crear cualquier tipo de geometría estática de physX que se
		le indique desde el Server de física

		@param position Posición donde crear el objeto estático
		@param dimensions Dimensiones físicas del objeto a crear
		@param group GRupo de Colision al que pertenece el actor estático
		@param trigger Indica si el actor creado es un trigger o no
		@param component Componente al cual se le debe hacer el callback
		@param geom	 Geometría con la cual se creará el actor estñatico
		@param material Material de comportamiento del actor estático
		@param physics Elemento principal de PhysX (Ver Physics::CServer)
		@param scene Escena física donde crear el actor físico (Ver Physics::CServer)
		*/
		CStaticActor(const Vector3 &position, const Vector3 &dimensions, bool trigger, int group, const Logic::IPhysics *component, Vector3 &offset = Vector3(0,0,0), physx::PxMaterial *material = Physics::CScene::getSingletonPtr()->getDefaultMaterial());

		/*
		El destructor de esta clase es exactamente el mismo que el de la clase padre
		*/


	}; // class CStaticActor

}; // namespace Physics

#endif // __Physics_Actor_H
