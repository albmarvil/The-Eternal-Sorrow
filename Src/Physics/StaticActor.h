/**
@file StaticActor.h

Contiene la declaraci�n de la encapsulaci�n del actor de f�sica est�tico. 

La clase contiene un constructor por defecto vac�o, pero el que se usa
es el constructor que toma varios par�metros.

@see Physics::CStaticActor

@author Alberto Mart�nez Villar�n
@date Enero, 2015
*/

#ifndef __Physics_StaticActor_H
#define __Physics_StaticActor_H

#include "Actor.h"
#include "BaseSubsystems/Math.h"

// Predeclaraci�n de tipos

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


// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	/**
	descripcion de la clase CStaticActor
	@ingroup physicGroup

	@author Alberto Mart�nez Villar�n
	@date Enero, 2015
	*/
	class CStaticActor : public CActor 
	{
	public:

		CStaticActor(){}

		/*
		Este constructor crea un actor f�sico de forma que es est�tico

		Es el m�todo usado por los tiles a la hora de crearse en el preprocesado.
		El constructor esta preparado para crear cualquier tipo de geometr�a est�tica de physX que se
		le indique desde el Server de f�sica

		@param position Posici�n donde crear el objeto est�tico
		@param dimensions Dimensiones f�sicas del objeto a crear
		@param group GRupo de Colision al que pertenece el actor est�tico
		@param trigger Indica si el actor creado es un trigger o no
		@param component Componente al cual se le debe hacer el callback
		@param geom	 Geometr�a con la cual se crear� el actor est�atico
		@param material Material de comportamiento del actor est�tico
		@param physics Elemento principal de PhysX (Ver Physics::CServer)
		@param scene Escena f�sica donde crear el actor f�sico (Ver Physics::CServer)
		*/
		CStaticActor(const Vector3 &position, const Vector3 &dimensions, bool trigger, int group, const Logic::IPhysics *component, Vector3 &offset = Vector3(0,0,0), physx::PxMaterial *material = Physics::CScene::getSingletonPtr()->getDefaultMaterial());

		/*
		El destructor de esta clase es exactamente el mismo que el de la clase padre
		*/


	}; // class CStaticActor

}; // namespace Physics

#endif // __Physics_Actor_H
