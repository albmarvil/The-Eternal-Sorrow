/**
@file SoundSystem.h

Contiene la declaraciñon del componente emisor de sonidos. Puede contener más de un sonido.

@see Logic::SoundSystem
@see Logic::IComponent

@author Alberto Martínez
@date Marzo, 2015
*/


#ifndef __Logic_SoundSystem_H
#define __Logic_SoundSystem_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

namespace Sounds 
{
	class CSound;
}

//declaración de la clase
namespace Logic 
{
	class SoundSystem : public IComponent
	{
		DEC_FACTORY(SoundSystem);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		SoundSystem();

		virtual ~SoundSystem();
		
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

	protected:

		void play(const std::string &sound);

		void stop(const std::string &sound);

		void setParameter(std::string &sound, std::string &parameter, float value);

		std::vector<std::string> _startedSounds;

		std::vector<std::string> _soundsStrings;

		typedef std::map<std::string, Sounds::CSound*> TSoundMap;
		TSoundMap _sounds;

	}; // class SoundSystem

	REG_FACTORY(SoundSystem);

} // namespace Logic

#endif // __Logic_SoundSystem_H