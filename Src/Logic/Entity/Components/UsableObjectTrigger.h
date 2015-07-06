/*
Componente trigger de objetos usables. De esta forma, cuando el player choca contra el objeto de habilidad, se la manda un mensaje
de tipo SET_HABILIDAD a los componentes del player. Cuando una habilidad tenga el tipo de habilidad que le pasamos en el mensaje
este se despertara y hara lo que tenga que hacer


@author Cesar Manuel Paz Guzman
@date Mayo, 2015

*/

#ifndef __Logic_UsableObjectTrigger_H
#define __Logic_UsableObjectTrigger_H

#include "Logic/Entity/Component.h"

// Los componentes pertenecen al namespace Logic
namespace Logic 
{

	class CUsableObjectTrigger: public IComponent
	{
		DEC_FACTORY(CUsableObjectTrigger);
	public:

		/**
		Constructor por defecto.
		*/
		CUsableObjectTrigger() : IComponent() {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);
		/**
		Este componente sólo acepta mensajes de tipo TOUCHED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje TOUCHED se envía otro mensaje de tipo HABILIDAD a la
		entidad tocada.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		virtual bool activate();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

	protected:

		std::string _weaponType;
		
	}; 

	REG_FACTORY(CUsableObjectTrigger);

} // namespace Logic

#endif 
