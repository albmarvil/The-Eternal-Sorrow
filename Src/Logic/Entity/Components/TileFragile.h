
#ifndef __Logic_TileFragile_H
#define __Logic_TileFragile_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{
	class CTileFragile : public IComponent
	{
		DEC_FACTORY(CTileFragile);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CTileFragile() : IComponent(), _isTouched(false), _canDamageAgain(true), _canHit(false), _canTouch(false),
									   _numHitsLeft(0), _numTouchsLeft(0){};

		virtual ~CTileFragile(){};
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

	
		virtual bool respawn(const Map::CEntity *entityInfo);

		
		virtual bool activate();

	
		virtual void deactivate(){};

		/**
		Método llamado en cada frame que actualiza el estado del componente.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Este componente sólo acepta mensajes de tipo ATTACK.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo ATTACK lanzaremos el ataque correspondiente, especial o principal.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		void assignMaterial();

	protected:

		bool _isTouched;
		bool _canDamageAgain;
		int _numTouchsLeft;
		int _numHitsLeft;
		bool _canTouch;
		bool _canHit;
		std::string _matBreak1, _matBreak2, _matBreak3;
		
		
	}; // class Attack

	REG_FACTORY(CTileFragile);

} // namespace Logic

#endif // __Logic_AttackD_H
