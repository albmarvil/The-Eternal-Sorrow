//---------------------------------------------------------------------------
// AnimationBlender.h
//---------------------------------------------------------------------------

/**
@file AnimationBlender.h

Contiene la declaraci�n de la clase CAnimationBlender. Esta clase se encarga de encapsular una animaci�n
dandole la funcionalidad necesaria para hacer blending de animaciones cuando se transita de una animaci�n a otra

@see Graphics::CAnimationBlender


@author Alberto Mart�nez
@date Marzo, 2015
*/

#ifndef __Graphics_AnimationBlender_H
#define __Graphics_AnimationBlender_H


#include <string>
#include <vector>

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
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
	Esta clase encapsula de forma conceptual una animaci�n y la funcionalidad para poder transitar 
	de una animaci�n a otra haciendo interpolaci�n de las mismas.
	
	@ingroup graphicsGroup

	@author Alberto Mart�nez Villar�n
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
		Destructor de la aplicaci�n.
		*/
		virtual ~CAnimationBlender();

		/*
		Tick de la animaci�n. En este tick sumamos el tiempo a la animaci�n actual y actualizamos el estado de la interpolaci�n de animaciones
		si es que hay una transici�n entre animaciones

		@param msecs incremento de tiempo recibido desde el �ltimo frame
		*/
		void tick (float secs);

		/*
		M�todo usado para transitar de una animaci�n a otra. Para ello se requiere el nombre de la animaci�n a transitar, as� como la duraci�n de la transici�n.
		Si se indica una duraci�n igual a 0, la transici�n ser� inmediata y sin necesidad de hacer blend.

		@param targetAnim Animaci�n a la cual transitar
		@param loop True si la animaci�n ser� en loop
		@param restart True si la animaci�n empieza desde 0
		@param duration Duraci�n de la transici�n configurada
		@param speed velocidad de ejecucion dela nueva animaci�n

		@return True si la transici�n se inici� correctamente
		*/
		bool blend(const std::string &targetAnim, std::vector<std::pair<float, float>> animationMask, bool loop, bool restart, float duration = 0.0f, float speed = 1.0f);

		/*
		M�todo que para la animaci�n actual y si hab�a alguna transici�n en curso, esta queda anulada.

		@return True si se par� la animaci�n correctamente
		*/
		bool stopAnimation();

		/*
		*M�todo usado apra reiniciar la secuencia de la animaci�n
		*/
		void restartAnimation();

		/*
		M�todo consultor del nombre de la animaci�n actual
		*/

		std::string getAnimationName() { return _currentName; }

	protected:

		/*
		Puntero a la entidad animada que controla esta animaci�n
		*/
		CAnimatedEntity* _animatedEntity;

		/*
		Estado actual de la animaci�n que se esta ejecutando
		*/
		Ogre::AnimationState* _currentState;
		
		/*
		Estado de la animaci�n a la que se va a transitar. Puede ser NULL si no se transita a ninguna animaci�n
		*/
		Ogre::AnimationState* _targetState;

		/*
		Nombre de la animaci�n actual
		*/
		std::string _currentName;

		/*
		Nombre de la animaci�n a la que se quiere transitar. Puede ser vac�o si no se transita a ninguna animaci�n
		*/
		std::string _targetName;

		/*
		Peso de la animaci�n actual. se usa para hacer la transici�n de aniamciones
		Su valor va de 0 a 1
		*/
		float _currentWeight;

		/*
		Peso de la animaci�n target. Se usa para hacer la transicion de aniumaciones.
		Su valor va de 0 a 1
		*/
		float _targetWeight;

		/*
		Contador de tiempo usado para las transiciones de animaciones
		*/
		float _timeCount;

		/*
		Tiempo de transici�n configurado
		*/
		float _transitionTime;
		/*
		Flag que nos indica si la animaci�n actual es en loop o no
		*/
		bool _loop;

		/*
		Flag que nos indica si la animaci�n a la que transitamos es en loop o no
		*/

		bool _loopTarget;

		/*
		Factor de velocidad de la animaci�n current
		*/
		float _currentSpeed;

		/*
		Factor de velocidad de la animacion target
		*/
		float _targetSpeed;

		/*
		M�scaras de animaci�n
		*/
		std::vector<std::pair<float, float>> _currentAnimationMask;

		std::vector<std::pair<float, float>> _targetAnimationMask;

		int _skeletonSize;

		void setAnimationMask( Ogre::AnimationState *anim,  std::vector<std::pair<float, float>> mask);

	}; // class CAnimatedEntity

} // namespace Graphics

#endif // __Graphics_AnimatedEntity_H
