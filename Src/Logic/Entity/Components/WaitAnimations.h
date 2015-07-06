#ifndef __Logic_Arms_H
#define __Logic_Arms_H

#include "Logic/Entity/Component.h"
#include "Graphics/AnimatedEntity.h"


//declaraci�n de la clase
namespace Logic 
{

	class CWaitAnimations : public IComponent, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CWaitAnimations);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CWaitAnimations() : IComponent(), _timeStop(0),  _isCountAttackL(true), _isCountAttackR(true),
			_isCountTurn(true), _isCountWalk(true) {}

		bool CWaitAnimations::OnSpawn(const Map::CEntity *entityInfo);


		/**
		Este componente s�lo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		virtual bool activate();
		void deactivate();

		void sendSET_ANIMATION(char *animacion, bool loop, bool restart, float speed);

		/**
		M�todo que ser� invocado siempre que se termine una animaci�n.
		Las animaciones en c�clicas no invocar�n nunca este m�todo.

		@param animation Nombre de la animaci�n terminada.
		*/
		void animationFinished(const std::string &animation);

		/**
		Setea la animaci�n actual
		*/
		void SetCurrentAnimation(const std::string &animation);

	protected:
		/*
		Atributo que permite controlar la animaci�n actual
		*/
		std::string _animActual;

		/*
		Atributo que almacena el tiempo que el jugador lleva parado
		*/
		float _timeStop;

		/**
		Atributos que activan o desactivan los contadores
		*/
		bool _isCountTurn;
		bool _isCountWalk;
		bool _isCountAttackL;
		bool _isCountAttackR;

		/*
		Atributo que almacena el tiempo m�ximo que puede estar parado el jugador
		*/
		float _maxTimeStop;

		/**
		M�todo qeu activa o desactiva el contador
		*/
		void SetCount(std::string data, bool active);
	}; // class CWaitAnimations

	REG_FACTORY(CWaitAnimations);

} // namespace Logic

#endif // __Logic_ArmaD_H