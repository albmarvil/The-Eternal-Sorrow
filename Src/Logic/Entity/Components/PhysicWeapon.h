/**
@file PhysicWeapon.h

Contiene la declaración del componente encargado de representar entidades físicas simples,
que son aquellas representadas mediante un único actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicWeapon
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio Sánchez Ruiz-Granados
@date Noviembre, 2012
@refactor Alberto Martínez Villarán
@date Enero, 2015
*/

#ifndef __Logic_PhysicWeapon_H
#define __Logic_PhysicWeapon_H

#include "PhysicEntity.h"
#include "Logic/Entity/Entity.h"

//// Predeclaración de tipos
//namespace Physics {
//	class CActor;
//	class CStaticActor;
//	class CScene;
//};


namespace Logic
{
	class CWeaponController;
}
// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	class CPhysicWeapon : public CPhysicEntity, public CEntityTransformListener
	{
		DEC_FACTORY(CPhysicWeapon);
	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicWeapon();

		/**
		Destructor. Elimina el objeto físico de la escena y lo destruye. 
		*/
		virtual ~CPhysicWeapon();

		/**
		Este método se invoca en cada ciclo de la simulación y hace lo siguiente:
		<ul>
		<li> Si la entidad física es de tipo estático no hace nada. </li>
		<li> Si la entidad física es de tipo dinámico actualiza la posición y rotación de 
		     la entidad lógica usando la información proporcionada por el motor de física. </li>
		<li> Si la entidad física es de tipo cinemático, además solicita al motor de física
		     que mueva la entidad de acuerdo al último mensaje KINEMATIC_MOVE recibido. </li>
		</ul>
		*/
		virtual void onTick(unsigned int msecs) {};
 
		

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		La implementación registrará al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cronómetro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		virtual void deactivate();

		///Método de escucha de la posicion de la entidad
		void OnEntitySetTransform(const Matrix4 &transform);

	private:


		CWeaponController* _weaponController;


	}; // class CPhysicWeapon

	REG_FACTORY(CPhysicWeapon);

} // namespace Logic

#endif // __Logic_PhysicWeapon_H
