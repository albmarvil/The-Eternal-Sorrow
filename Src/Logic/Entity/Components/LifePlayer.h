/**
@file LifePlayer.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CLifePlayer
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/
#ifndef __Logic_LifePlayer_H
#define __Logic_LifePlayer_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Components/Life.h"
#include "Graphics/AnimatedEntity.h"

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
	class CLifePlayer : public CLife, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CLifePlayer);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLifePlayer() : CLife(), _proporcionShake(1.0f), _dead(false), _gameOver(false) {}

		virtual bool activate();

		virtual void deactivate();

		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);
		
		/*
		Listener de finalización de animación
		*/
		virtual void animationFinished(const std::string &animation);

	private:

		/*
		Puntero a la entidad animada del player
		*/
		Graphics::CAnimatedEntity* _playerAnimatedEntity;

		/*
		Puntero al player
		*/
		CEntity* _player;

		bool _gameOver;

		bool _dead;

		float _proporcionShake;

		CEntity* _camera;
	}; // class CLifePlayer

	REG_FACTORY(CLifePlayer);

} // namespace Logic

#endif // __Logic_LifePlayer_H
