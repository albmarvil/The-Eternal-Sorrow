/**
@file PhysicCoin.h

Contiene la declaración del componente encargado de representar el trigger físico de las monedas.
Es un componente aparte ya que solo sirve para crear un actor kinematico y trigger que acompaña
al actor dinamico de las monedas

@see Logic::CPhysicCoin
@see Logic::IComponent
@see Logic::CPhysicController


@author Alberto Martínez Villarán
@date Abril, 2015
*/

#ifndef __Logic_PhysicCoin_H
#define __Logic_PhysicCoin_H

#include "PhysicEntity.h"


// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	class CPhysicCoin : public CPhysicEntity
	{
		DEC_FACTORY(CPhysicCoin);
	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicCoin();

		/**
		Destructor. Elimina el objeto físico de la escena y lo destruye. 
		*/
		virtual ~CPhysicCoin();

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message) { return false; };
		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		//virtual void deactivate();

	protected:

		float _timeToActivateTrigger;

		float _timeAcum;

	}; // class CPhysicCoin

	REG_FACTORY(CPhysicCoin);

} // namespace Logic

#endif // __Logic_PhysicCoin_H
