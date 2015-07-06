/**
@file TriggerUsableWeapon.h

Contiene la declaraci�n del componente encargado de representar el trigger f�sico de las monedas.
Es un componente aparte ya que solo sirve para crear un actor kinematico y trigger que acompa�a
al actor dinamico de las monedas

@see Logic::CTriggerUsableWeapon
@see Logic::IComponent
@see Logic::CPhysicController


@author Alberto Mart�nez Villar�n
@date Abril, 2015
*/

#ifndef __Logic_TriggerUsableWeapon_H
#define __Logic_TriggerUsableWeapon_H

#include "PhysicEntity.h"


// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	class CTriggerUsableWeapon : public CPhysicEntity
	{
		DEC_FACTORY(CTriggerUsableWeapon);
	public:
	
		/**
		Constructor por defecto.
		*/
		CTriggerUsableWeapon(){};

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CTriggerUsableWeapon(){};

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message) { return false; };
		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		//virtual void deactivate();

	protected:

	}; // class CTriggerUsableWeapon

	REG_FACTORY(CTriggerUsableWeapon);

} // namespace Logic

#endif // __Logic_TriggerUsableWeapon_H
