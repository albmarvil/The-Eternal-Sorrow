/**
@file StaticPlane.h

Contiene la declaraci�n de la encapsulaci�n del plano de f�sica estatico

@see Physics::CStaticPlane

@author Alberto Mart�nez Villar�n
@date Enero, 2015
*/

#ifndef __Physics_StaticPlane_H
#define __Physics_StaticPlane_H

#include "StaticActor.h"
#include "BaseSubsystems/Math.h"

// Predeclaraci�n de tipos


namespace physx {
	class PxPhysics;
	class PxScene;
	class PxMaterial;
};

namespace Logic
{
	class IPhysics;
};

// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	class CStaticPlane : public CStaticActor 
	{
	public:

		/*
		Este constructor crea un plano f�sico de forma que es est�tico

		@param point Punto del plano a crear
		@param normal Normal del plano a crear
		@param group GRupo de Colision al que pertenece el actor est�tico
		@param component Componente al cual se le debe hacer el callback
		@param material Material de comportamiento del actor est�tico
		@param physics Elemento principal de PhysX (Ver Physics::CServer)
		@param scene Escena f�sica donde crear el actor f�sico (Ver Physics::CServer)
		*/
		CStaticPlane::CStaticPlane(const Vector3 &point,
									const Vector3 &normal,
									int group,
									const Logic::IPhysics *component,
									physx::PxMaterial *material = Physics::CScene::getSingletonPtr()->getDefaultMaterial()
									);
	


		
	}; // class CStaticPlane

}; // namespace Physics

#endif 
