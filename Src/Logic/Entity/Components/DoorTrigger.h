/**
@file DoorTriggerr.h

Contiene la declaración del componente que provoca la carga
de un nuevo mapa llamando al MapManager
cuando recibe un mensaje TOUCHED / UNTOUCHED.
 

@see Logic::DoorTrigger
@see Logic::IComponent

@author Alberto Martínez
@date Octubre, 2015
*/
#ifndef __Logic_DoorTrigger_H
#define __Logic_DoorTrigger_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

namespace Graphics 
{
	class CServer;
	class CScene;
	class CCamera;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente procesa mensajes de tipo TOUCHED o UNTOUCHED (indican que la 
	entidad ha sido tocada o dejada de ser tocada) para cargar a traves del MapManager
	el siguiente mapa.
	<p>

	
    @ingroup logicGroup

	@author Alberto Martínez
	@date Octubre, 2015
*/
	class DoorTrigger : public IComponent
	{
		DEC_FACTORY(DoorTrigger);
	public:

		/**
		Constructor por defecto.
		*/
		DoorTrigger() : IComponent() {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Método que se invoca para activar el componente.
		*/
		virtual bool activate();
		
		/**
		Método que se invoca al desactivar el componente.
		*/
		virtual void deactivate();

		/**
		Este componente sólo acepta mensaje de tipos TOUCHED y UNTOUCHED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir mensaje TOUCHED y UNTOUCHED emite mensajes SWITCH
		a la entidad objetivo para que cambie de posición.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

	protected:
		/*
		*/

	}; // class CSwitchTrigger

	REG_FACTORY(DoorTrigger);

} // namespace Logic

#endif // __Logic_SwitchTrigger_H
