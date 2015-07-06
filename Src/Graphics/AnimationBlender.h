//---------------------------------------------------------------------------
// AnimationBlender.h
//---------------------------------------------------------------------------

/**
@file AnimationBlender.h

Contiene la declaración de la clase CAnimationBlender. Esta clase se encarga de encapsular una animación
dandole la funcionalidad necesaria para hacer blending de animaciones cuando se transita de una animación a otra

@see Graphics::CAnimationBlender


@author Alberto Martínez
@date Marzo, 2015
*/

#ifndef __Graphics_AnimationBlender_H
#define __Graphics_AnimationBlender_H


#include <string>
#include <vector>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class AnimationState;
}

namespace Graphics
{
	class CAnimatedEntity;
}
	
namespace Graphics 
{

	/**
	Esta clase encapsula de forma conceptual una animación y la funcionalidad para poder transitar 
	de una animación a otra haciendo interpolación de las mismas.
	
	@ingroup graphicsGroup

	@author Alberto Martínez Villarán
	@date Marzo, 2015
	*/
	class CAnimationBlender 
	{
	public:

		/**
		Constructor de la clase.
		*/
		CAnimationBlender(const std::string &name, CAnimatedEntity *entity, bool loop, bool restart, float speed, std::vector<std::pair<float, float>> animationMask, int skeletonSize);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CAnimationBlender();

		/*
		Tick de la animación. En este tick sumamos el tiempo a la animación actual y actualizamos el estado de la interpolación de animaciones
		si es que hay una transición entre animaciones

		@param msecs incremento de tiempo recibido desde el último frame
		*/
		void tick (float secs);

		/*
		Método usado para transitar de una animación a otra. Para ello se requiere el nombre de la animación a transitar, así como la duración de la transición.
		Si se indica una duración igual a 0, la transición será inmediata y sin necesidad de hacer blend.

		@param targetAnim Animación a la cual transitar
		@param loop True si la animación será en loop
		@param restart True si la animación empieza desde 0
		@param duration Duración de la transición configurada
		@param speed velocidad de ejecucion dela nueva animación

		@return True si la transición se inició correctamente
		*/
		bool blend(const std::string &targetAnim, std::vector<std::pair<float, float>> animationMask, bool loop, bool restart, float duration = 0.0f, float speed = 1.0f);

		/*
		Método que para la animación actual y si había alguna transición en curso, esta queda anulada.

		@return True si se paró la animación correctamente
		*/
		bool stopAnimation();

		/*
		*Método usado apra reiniciar la secuencia de la animación
		*/
		void restartAnimation();

		/*
		Método consultor del nombre de la animación actual
		*/

		std::string getAnimationName() { return _currentName; }

	protected:

		/*
		Puntero a la entidad animada que controla esta animación
		*/
		CAnimatedEntity* _animatedEntity;

		/*
		Estado actual de la animación que se esta ejecutando
		*/
		Ogre::AnimationState* _currentState;
		
		/*
		Estado de la animación a la que se va a transitar. Puede ser NULL si no se transita a ninguna animación
		*/
		Ogre::AnimationState* _targetState;

		/*
		Nombre de la animación actual
		*/
		std::string _currentName;

		/*
		Nombre de la animación a la que se quiere transitar. Puede ser vacío si no se transita a ninguna animación
		*/
		std::string _targetName;

		/*
		Peso de la animación actual. se usa para hacer la transición de aniamciones
		Su valor va de 0 a 1
		*/
		float _currentWeight;

		/*
		Peso de la animación target. Se usa para hacer la transicion de aniumaciones.
		Su valor va de 0 a 1
		*/
		float _targetWeight;

		/*
		Contador de tiempo usado para las transiciones de animaciones
		*/
		float _timeCount;

		/*
		Tiempo de transición configurado
		*/
		float _transitionTime;
		/*
		Flag que nos indica si la animación actual es en loop o no
		*/
		bool _loop;

		/*
		Flag que nos indica si la animación a la que transitamos es en loop o no
		*/

		bool _loopTarget;

		/*
		Factor de velocidad de la animación current
		*/
		float _currentSpeed;

		/*
		Factor de velocidad de la animacion target
		*/
		float _targetSpeed;

		/*
		Máscaras de animación
		*/
		std::vector<std::pair<float, float>> _currentAnimationMask;

		std::vector<std::pair<float, float>> _targetAnimationMask;

		int _skeletonSize;

		void setAnimationMask( Ogre::AnimationState *anim,  std::vector<std::pair<float, float>> mask);

	}; // class CAnimatedEntity

} // namespace Graphics

#endif // __Graphics_AnimatedEntity_H
