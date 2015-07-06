/**
@file Life.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CLife
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_Life_H
#define __Logic_Life_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic
{
	class CBarraLifeController;
}
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el daño a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".

	@todo  Si la vida pasa a ser 0 que la entidad muera (poner animación de muerte?)
	y si es el jugador habrá que terminar el juego. Si la vida sigue siendo mayor 
	que 0 trás un golpe ¿poner la animación de herido?.
	
	@ingroup logicGroup

	@author David Llansó García
	@date Octubre, 2010
*/
	class CLife : public IComponent
	{
		DEC_FACTORY(CLife);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLife() : IComponent(), _life(100.0f), _dead(false), _lifeBar(0), _invincible(false), _deathSound("-"){}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);
	
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

		float getLife(){ return _life;}
		void setLife(float life){ _life = life;}

		bool getInvincible(){return _invincible;}
		void setInvincible(bool invin){_invincible = invin;}

		virtual void setLifeBar(CBarraLifeController *lifeBar);

		//esta función la tendrán todos los componentes vida, pero solo la implementará el jugador
		virtual void ChangeLifeBar();
	protected:

		/**
		Vida de la entidad
		*/
		float _life;

		//Vida maxima
		float _lifeMax;

		bool _invincible;

		bool _dead;

		std::string _deathSound;

		CBarraLifeController *_lifeBar;
	}; // class CLife

	REG_FACTORY(CLife);

} // namespace Logic

#endif // __Logic_Life_H
