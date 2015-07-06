/**
@file StaticPlane.h

Contiene la declaración de la encapsulación del plano de física estatico

@see Physics::CStaticPlane

@author Alberto Martínez Villarán
@date Enero, 2015
*/

#ifndef __Physics_StaticPlane_H
#define __Physics_StaticPlane_H

#include "StaticActor.h"
#include "BaseSubsystems/Math.h"

// Predeclaración de tipos


namespace physx {
	class PxPhysics;
	class PxScene;
	class PxMaterial;
};

namespace Logic
{
	class IPhysics;
};

// Namespace que contiene las clases relacionadas con la parte física. 
namespace Physics {

	class CStaticPlane : public CStaticActor 
	{
	public:

		/*
		Este constructor crea un plano físico de forma que es estático

		@param point Punto del plano a crear
		@param normal Normal del plano a crear
		@param group GRupo de Colision al que pertenece el actor estático
		@param component Componente al cual se le debe hacer el callback
		@param material Material de comportamiento del actor estático
		@param physics Elemento principal de PhysX (Ver Physics::CServer)
		@param scene Escena física donde crear el actor físico (Ver Physics::CServer)
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
