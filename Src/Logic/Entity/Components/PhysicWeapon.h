/**
@file PhysicWeapon.h

Contiene la declaraci�n del componente encargado de representar entidades f�sicas simples,
que son aquellas representadas mediante un �nico actor de PhysX. Este componente no sirve
para representar character controllers.

@see Logic::CPhysicWeapon
@see Logic::IComponent
@see Logic::CPhysicController

@author Antonio S�nchez Ruiz-Granados
@date Noviembre, 2012
@refactor Alberto Mart�nez Villar�n
@date Enero, 2015
*/

#ifndef __Logic_PhysicWeapon_H
#define __Logic_PhysicWeapon_H

#include "PhysicEntity.h"
#include "Logic/Entity/Entity.h"

//// Predeclaraci�n de tipos
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
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysicWeapon();

		/**
		Este m�todo se invoca en cada ciclo de la simulaci�n y hace lo siguiente:
		<ul>
		<li> Si la entidad f�sica es de tipo est�tico no hace nada. </li>
		<li> Si la entidad f�sica es de tipo din�mico actualiza la posici�n y rotaci�n de 
		     la entidad l�gica usando la informaci�n proporcionada por el motor de f�sica. </li>
		<li> Si la entidad f�sica es de tipo cinem�tico, adem�s solicita al motor de f�sica
		     que mueva la entidad de acuerdo al �ltimo mensaje KINEMATIC_MOVE recibido. </li>
		</ul>
		*/
		virtual void onTick(unsigned int msecs) {};
 
		

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		La implementaci�n registrar� al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cron�metro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		virtual void deactivate();

		///M�todo de escucha de la posicion de la entidad
		void OnEntitySetTransform(const Matrix4 &transform);

	private:


		CWeaponController* _weaponController;


	}; // class CPhysicWeapon

	REG_FACTORY(CPhysicWeapon);

} // namespace Logic

#endif // __Logic_PhysicWeapon_H
