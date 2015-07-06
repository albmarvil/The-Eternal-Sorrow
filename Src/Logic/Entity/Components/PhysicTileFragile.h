/**
@file PhysicDoor.h

Contiene la declaraci�n del componente encargado de representar el trigger f�sico de las monedas.
Es un componente aparte ya que solo sirve para crear un actor kinematico y trigger que acompa�a
al actor dinamico de las monedas

@see Logic::CPhysicDoor
@see Logic::IComponent
@see Logic::CPhysicController


@author B�rbara Dom�nguez de la Torre Gonz�lez
@date Abril, 2015
*/

#ifndef __Logic_PhysicTileFragile_H
#define __Logic_PhysicTileFragile_H

#include "PhysicEntity.h"


// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	class CPhysicTileFragile : public CPhysicEntity
	{
		DEC_FACTORY(CPhysicTileFragile);
	public:
	
		/**
		Constructor por defecto.
		*/
		CPhysicTileFragile();

		/**
		Destructor. Elimina el objeto f�sico de la escena y lo destruye. 
		*/
		virtual ~CPhysicTileFragile();

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message) { return false; };
		
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		virtual void onTrigger (IPhysics *otherComponent, bool enter);

		//virtual void deactivate();

	protected:

	}; // class CPhysicDoor

	REG_FACTORY(CPhysicTileFragile);

} // namespace Logic

#endif // __Logic_PhysicDoor_H
