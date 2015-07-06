/**
@file SoundListener.h

Contiene la declaración del componente oyente de los sonidos espacializados en 3D

@see Logic::SoundListener
@see Logic::IComponent

@author Alberto Martínez
@date Marzo, 2015
*/


#ifndef __Logic_SoundListener_H
#define __Logic_SoundListener_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Entity/Entity.h"
namespace Sounds 
{
	class CListener;
}

//declaración de la clase
namespace Logic 
{
	class SoundListener : public IComponent, public CEntityTransformListener
	{
		DEC_FACTORY(SoundListener);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		SoundListener();

		virtual ~SoundListener();
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

	
		virtual bool respawn(const Map::CEntity *entityInfo);

		
		virtual bool activate();

	
		virtual void deactivate();

		/**
		Método llamado en cada frame que actualiza el estado del componente.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Este componente acepta mensajes de tipo PLAY, STOP, SET_PARAMETER.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Procesamiento de mensajes
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		///Método de escucha de la posicion de la entidad
		void OnEntitySetTransform(const Matrix4 &transform);

	protected:

		void setListenerTransform(const Matrix4 &transfomr);
		
		Sounds::CListener* _listener;

	}; // class SoundSystem

	REG_FACTORY(SoundListener);

} // namespace Logic

#endif // __Logic_SoundListener_H
