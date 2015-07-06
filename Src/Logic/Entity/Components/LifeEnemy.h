/**
@file LifeEnemy.h

Contiene la declaraci�n del componente que controla la vida de una entidad.

@see Logic::CLifeEnemy
@see Logic::IComponent

@author David Llans�
@date Octubre, 2010
*/
#ifndef __Logic_LifeEnemy_H
#define __Logic_LifeEnemy_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Components/Life.h"

//declaraci�n de la clase
namespace Logic
{
	class CBarraLifeController;
	class CLife;
}
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el da�o a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	@todo  Si la vida pasa a ser 0 que la entidad muera (poner animaci�n de muerte?)
	y si es el jugador habr� que terminar el juego. Si la vida sigue siendo mayor 
	que 0 tr�s un golpe �poner la animaci�n de herido?.
	
	@ingroup logicGroup

	@author David Llans� Garc�a
	@date Octubre, 2010
*/
	class CLifeEnemy : public CLife
	{
		DEC_FACTORY(CLifeEnemy);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLifeEnemy() : CLife(), _tier(1), _level(1) {}
		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		void damageEntity(float damage, const std::string &armaAsesina);
		void healEntity(float heal);

		void exploteKami();

		void armaAsesina(const std::string &objeto);

	protected:

		//tier del enemigo
		int _tier;

		//nivel del enemigo
		int _level;
	}; // class CLifeEnemy

	REG_FACTORY(CLifeEnemy);

} // namespace Logic

#endif // __Logic_LifeEnemy_H
