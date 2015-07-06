#ifndef __Logic_BarrilTrigger_H
#define __Logic_BarrilTrigger_H

#include "Logic/Entity/Component.h"

// Los componentes pertenecen al namespace Logic
namespace Logic 
{

	class CBarrilTrigger: public IComponent
	{
		DEC_FACTORY(CBarrilTrigger);
	public:

		/**
		Constructor por defecto.
		*/
		CBarrilTrigger() : IComponent(), _damage(20.f), touch(0) {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Este componente sólo acepta mensajes de tipo TOUCHED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje TOUCHED se envía otro mensaje de tipo DAMAGED a la
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

		/**
		Daño que se produce cada vez que se toca.
		*/
		float _damage;

		/**
		Variable que controla que solo se reciba un mensaje TOUCHED
		*/
		int touch;

	}; 

	REG_FACTORY(CBarrilTrigger);

} 

#endif 