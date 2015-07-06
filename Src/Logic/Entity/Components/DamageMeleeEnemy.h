#ifndef __Logic__DamageMeleeEnemy__H
#define __Logic__DamageMeleeEnemy__H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{

	class CDamageMeleeEnemy : public IComponent
	{
		DEC_FACTORY(CDamageMeleeEnemy);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CDamageMeleeEnemy() : IComponent(){}
		
		virtual ~CDamageMeleeEnemy();
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

		/**
		Este componente sólo acepta mensajes de tipo HABILIDAD.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo HABILIDAD mira si el tipo de habilidad es el de aumentar salto, y de ser asi aumento el salto y la dispierto
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

	protected:
		int _damageToAument;
	}; 

	REG_FACTORY(CDamageMeleeEnemy);

} // namespace Logic

#endif 
