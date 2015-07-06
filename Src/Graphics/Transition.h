//---------------------------------------------------------------------------
// Transition.h
//---------------------------------------------------------------------------

/**
@file Transition.h

Contiene la declaración de la clase base de las transiciones. Además contiene las declaraciones e implementaciones de las
diferentes transiciones del juego. 

Una transición se subdivide a su vez en dos efectos
	-Efecto entrada
	-Efecto salida

Cada transición recibirá un tick que sirve para ejecutar el efecto, ya sea de entrada o de salida.

Además una transición tendrá configurado un parámetro de duración de ambos efectos.

@see Graphics::CTransition

@author Alberto Martínez
@date Abril, 2015
*/

#ifndef __Graphics_Transition_H
#define __Graphics_Transition_H

#include "BaseSubsystems/Math.h"

namespace Graphics
{
	class CEntity;
}

namespace Graphics 
{
	enum TipoEfecto
	{
		SALIDA,
		ENTRADA
	};

	class CTransition
	{
	public:

		CTransition() : _duration(0.0f), _actualEffect(TipoEfecto::SALIDA), _actualEffectFinished(false), _timeAcum(0.0f) {};

		CTransition(float duration) : _duration(duration), _actualEffect(TipoEfecto::SALIDA), _actualEffectFinished(false), _timeAcum(0.0f) {};

		CTransition(float duration, TipoEfecto initEffect) : _duration(duration), _actualEffect(initEffect), _actualEffectFinished(false), _timeAcum(0.0f) {};

		virtual ~CTransition();

		bool isEffectFinished() { return _actualEffectFinished; };

		void tick(unsigned int msecs);

		virtual void activateTransitionEffect(TipoEfecto efecto) = 0;

		virtual void deactivateTransitionEffect(TipoEfecto efecto) = 0;

	protected:

		virtual void outEffectTick(unsigned int msecs) = 0;

		virtual void inEffectTick(unsigned int msecs) = 0;

		float _duration;

		float _timeAcum;

		TipoEfecto _actualEffect;

		bool _actualEffectFinished;

		CEntity *_planoOutEffect;

		CEntity *_planoInEffect;
	};


	class CFadeInOutTransition : public CTransition
	{

	public:

		CFadeInOutTransition() : CTransition () {};

		CFadeInOutTransition(float duration) : CTransition(duration) {};

		CFadeInOutTransition(float duration, TipoEfecto initEffect) : CTransition(duration, initEffect) {};

		virtual ~CFadeInOutTransition(){};

		virtual void activateTransitionEffect(TipoEfecto efecto);

		virtual void deactivateTransitionEffect(TipoEfecto efecto);

	protected:

		virtual void outEffectTick(unsigned int msecs);

		virtual void inEffectTick(unsigned int msecs);

		float _fade;
	};


} // namespace Graphics

#endif // __Graphics_Transition_H
