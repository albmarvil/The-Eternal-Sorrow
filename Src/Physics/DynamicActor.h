/**
@file DynamicActor.h

Contiene la declaraci�n de la encapsulaci�n del actor de f�sica Dinamico

Un actor din�mico a diferencia de uno est�tico puede moverse libremente por la escena.
Un actor din�mico puro esta destinado para objetos que se mueven de forma �nica y exclusivamente f�sica
Esto quiere decir que si queremos mover un actor din�mico este lo har� �nicamente reaccionando a las fuerzas que
se le apliquen sobre el.

se puede pero NO SE DEBE, mover un actor din�mico haciendo un setPosition del mismo, pero es recomendable hacer uso
de m�todos que le apliquen fuerzas. (ESTO FALTA EN EL ENCAPSULADO)

Por otro lado hay una especializaci�n de los actores din�micos conocidos como los actores kinem�ticos.
Estos actores son especiales, popularmente se les conoce como actores "bajo cuerda". Esto se debe a que un 
actor kinematico no interactua con nada del entorno, ni con la malla est�tica.

No interactua, no empuja ni es empujado, pero SI lanza y recibe CALLBACKS. Los actores kinem�ticos estan pensados
para que sean usados en entes movidos por la l�gica (Proyectiles, npcs, ...etc).

Un Actor kinematico NO SE MUEVE usando setPosition, sino usando MoveToTarget(). REciben un tick de simulacion especial,
ellos intentaran moverse hasta l anueva posicion indicada, lanzando los callbacks necesarios en el camino


@see Physics::CDynamicActor

@author Alberto Mart�nez Villar�n
@date Enero, 2015
*/

#ifndef __Physics_DynamicActor_H
#define __Physics_DynamicActor_H

#include "Actor.h"
#include "BaseSubsystems/Math.h"

// Predeclaraci�n de tipos

namespace physx {
	class PxPhysics;
	class PxScene;
	class PxGeometry;
	class PxMaterial;
	class PxRigidDynamic;
	class PxD6Joint;
};

namespace Logic
{
	class IPhysics;
};


// Namespace que contiene las clases relacionadas con la parte f�sica. 
namespace Physics {

	enum TipoFuerza 
	{
		IMPULSO,
		FUERZA
	};
	/**
	descripcion de la clase CDynamicActor
	@ingroup physicGroup

	@author Alberto Mart�nez Villar�n
	@date Enero, 2015
	*/
	class CDynamicActor : public CActor 
	{
	public:

		virtual ~CDynamicActor();

		CDynamicActor(){};

		/*
		Constructor PARA UN ACTOR CON GEOMETRICA EN FORMA DE CUBO. 

		@param position posicion en la cual se crea el actor
		@param dimensions Dimensiones fisicas del actor
		@param mass Masa del actor
		@param kinematic Flag que inica si es  kinematico o no
		@param trigger Flag que indica si es trigger o no
		@param group Grupo de colision
		@param component Componente al cual realizar los callbacks
		@param material Material de simulacion
		*/
		CDynamicActor(const Vector3 &position, const Vector3 &dimensions,  float mass, bool kinematic, bool trigger, int group, const Logic::IPhysics *component, Vector3 &offset = Vector3(0,0,0),const std::string &material = "");

		/*
		Constructor PARA UN ACTOR CON GEOMETRICA EN FORMA DE ESFERA. 

		@param position posicion en la cual se crea el actor
		@param radius Radio de la esfera
		@param mass Masa del actor
		@param kinematic Flag que inica si es  kinematico o no
		@param trigger Flag que indica si es trigger o no
		@param group Grupo de colision
		@param component Componente al cual realizar los callbacks
		@param material Material de simulacion
		*/
		CDynamicActor(const Vector3 &position, float radius,  float mass, bool kinematic, bool trigger, int group, const Logic::IPhysics *component, Vector3 &offset = Vector3(0,0,0), const std::string &material = "");
		
		/*
		Constructor PARA UN ACTOR CON GEOMETRICA EN FORMA DE CAPSULA. 

		@param position posicion en la cual se crea el actor
		@param radius Radio de la esfera del cilindro
		@param height Altura de la capsula
		@param mass Masa del actor
		@param kinematic Flag que inica si es  kinematico o no
		@param trigger Flag que indica si es trigger o no
		@param group Grupo de colision
		@param component Componente al cual realizar los callbacks
		@param material Material de simulacion
		*/
		CDynamicActor(const Vector3 &position, float radius, float height,  float mass, bool kinematic, bool trigger, int group, const Logic::IPhysics *component, Vector3 &offset = Vector3(0,0,0), const std::string &material = "");

		//FUNCION QUE CREA UN ACTOR DINAMICO A PARTIR DE UNA GEOMETRIA
		void createDynamicActor(const Vector3 &position, const physx::PxGeometry &geom, float density, const Vector3 &dimensions, bool kinematic, bool trigger, int group, const Logic::IPhysics *component, Vector3 &offset, physx::PxMaterial *material);
		
		/*
		Constructro a partir de un actor dinamico de physX

		@param dynActor Actor de dinamico de physX
		*/
		CDynamicActor (physx::PxRigidDynamic* dynActor);
		
		/*
		Este predicado nos indica si estamos ante un actor dinamico y kinematico
		Se hace utiliazando una m�scara de bits con un flag especifico de PhysX

		@return True si es un actor kinematico
		*/
		bool isKinematic();

		/*
		Metodo especifico para mover actores kinematicos

		@param transform matriz de transformacion a aplicar al actor kinematico
		*/
		void moveKinematicActor(const Matrix4 &transform);

		/*
		Metodo especifico para mover actores kinematicos

		@param displ Vector de desplazamiento a aplicar al actor kinematico
		*/
		void moveKinematicActor(const Vector3 &displ);


		/*
		Este m�todo sobreescribe al de la clase padre CActor ya que necesita un tratamiento
		especial para sacar los objetos de la simulacion f�sica.

		El tratamiento especial se reduce a "despertar" los objetos din�micos 
		antes de volver a setearlos como activos en la simulacion

		Esto se debe a que si un objeto deja la simulacion este se duerme automaticamente, y al volver
		seguir�a dormido y quiz�s necesitemos que al volver se despierte para analizar las posibilidades de interacci�n
		con el entorno.

		@param simulate True para activar la simulacion
		*/
		virtual void setSimulation(bool simulate);


		/*
		A�ade la restricci�n global de Z = 0 para este objeto
		*/
		virtual void addZ0Constraint();

		/*
		A�ade la restricci�n global para que el objeto no pierda la verticalidad
		*/
		virtual void addVerticalConstraint();


		void addForce(const Vector3 &force, TipoFuerza tipofuerza = TipoFuerza::FUERZA);

		void addTorque(const Vector3 &torque, TipoFuerza tipofuerza = TipoFuerza::FUERZA);

		protected:

			physx::PxD6Joint *_joint;
		
	}; // class CDynamicActor

}; // namespace Physics

#endif // __Physics_Actor_H
