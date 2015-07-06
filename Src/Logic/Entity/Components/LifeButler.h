
#ifndef __Logic_LifeButler_H
#define __Logic_LifeButler_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{

	class CLifeButler : public IComponent
	{
		DEC_FACTORY(CLifeButler);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLifeButler(){}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

	protected:

	}; // class CLifeEnemy

	REG_FACTORY(CLifeButler);

} // namespace Logic

#endif // __Logic_LifeEnemy_H
