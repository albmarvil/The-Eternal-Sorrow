//---------------------------------------------------------------------------
// AnimatedEntity.h
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author David Llans�
@date Julio, 2010
@refactor Alberto Mart�nez
@date Marzo, 2015
*/

#ifndef __Graphics_AnimatedEntity_H
#define __Graphics_AnimatedEntity_H

#include "Entity.h"


// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class AnimationState;
}

namespace Graphics 
{
	class CScene;
	class CAnimationBlender;
}
	
namespace Graphics 
{

	enum BlendMode
	{
		AVERAGE = Ogre::SkeletonAnimationBlendMode::ANIMBLEND_AVERAGE,
		CUMULATIVE = Ogre::SkeletonAnimationBlendMode::ANIMBLEND_CUMULATIVE,
	};

	/**
	Esta clase debe ser implementada por las clases que quieren
	registrarse en una entidad animada para enterarse de cuando
	terminan las animaciones de �sta.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Julio, 2010
	@refactor Alberto Martinez
	@date Marzo, 2015
	*/
	class CAnimatedEntityListener 
	{
	public:

		/**
		M�todo que ser� invocado siempre que se termine una animaci�n.
		Las animaciones en c�clicas no invocar�n nunca este m�todo.

		@param animation Nombre de la animaci�n terminada.
		*/
		virtual void animationFinished(const std::string &animation) {}

		/*
		*M�todo que ser� invocado siempre que acabe la �ltima animaci�n de un animationSet

		@param animation Nombre de la animaci�n terminada
		*/
		//virtual void animationSetFinished(const std::string &animation) {}
	}; // CAnimatedEntityListener

	/**
	Esta clase sirve para registrar observadores, los cuales heredar�n de ella e implementar�n
	su m�todo setTransform.
	*/
	class IObserver
	{
	public:
		virtual void setTransform(Vector3 bonePos, Quaternion boneOri);
	};

	/**
	Clase que representa una entidad gr�fica con animaciones. Especializa la clase
	Graphics::CEntity por lo que contiene una referencia a una entidad de 
	Ogre y al Nodo que la contiene. A�ade la posibilidad de activar una animaci�n 
	y reproducirla.
	<p>
	Esta clase es bastante sencilla y solo controlamos una animaci�n activa.
	Podr�a ser m�s sofisticada permitiendo interpolaci�n de animaciones o avisando
	mediante observers cuando una animaci�n termina de reproducirse.
	<p>
	Oculta los detalles escabrosos del motor gr�fico.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Julio, 2010
	*/
	class CAnimatedEntity : public CEntity
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CAnimatedEntity(const std::string &name, const std::string &mesh, const std::string &animationConfig);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CAnimatedEntity();

		/**
		Activa una animaci�n a partir de su nombre y el conjunto de animaciones correspondiente.
		Si no existe conjunto de animacion, se crear� dicho conjunto y se setear� dicha animaci�n.

		En caso de existir, se har� interpolaci�n con la animaci�n existente en dicho conjunto de animacion.

		@param anim Nombre de la animaci�n a activar.
		@param animSet Conjunto donde activar la animaci�n
		@param loop true si la animaci�n debe reproducirse c�clicamente.
		@param restart true si queremos reiniciar la animaci�n
		@param duration (OPCIONAL) Duraci�n de la transici�n de animaciones, en caso de que se deba transitar
		@param speed (OPCIONAL) Velocidad de ejecuci�n de la animaci�n
		@return true si la animaci�n solicitada fue correctamente activada.

		*/
		bool setAnimation(const std::string &anim, const std::string &animSet, bool loop, bool restart, float duration = 0.0f, float speed = 1.0f);
		
		/**
		Desactiva una animaci�n a partir de su nombre y del conjunto de animaci�n indicado

		@param anim Nombre de la animaci�n a activar.
		@param animSet Conjunto de animaci�n donde desactivar dicha animaci�n

		@return true si la animaci�n solicitada fue correctamente desactivada.
		*/
		bool stopAnimation(const std::string &anim, const std::string &animSet);
		
		/**
		Desactiva todas las animaciones de una entidad.
		*/
		void stopAllAnimations();

		/**
		Funci�n que registra al oyente de la entidad gr�fica. 
		*/
		void setObserver(CAnimatedEntityListener *observer, const std::string &animationName);

		/**
		Funci�n que quita al oyente de la entidad gr�fica.
		*/
		void removeObserver(CAnimatedEntityListener *observer, const std::string &animationName);

		/*
		M�todo para obtener el objeto AnimationState de un esqueleto a partir de su nombre.
		Si este no existe se devuelve NULL

		@param name Nombre de la animaci�n

		@return AnimationState correspondiente. NULL si no existe
		*/
		Ogre::AnimationState* getAnimationState(const std::string &name);

		/*
		M�todo llamado desde CAnimation para notificarnos de la finalizaci�n de una animaci�n.
		Nosotros notificaremos a todos sus observadores

		@param animation Animaci�n que se notifica la finalizaci�n
		*/
		void animationFinished (const std::string &animation);

		/*
		M�todo llamado desde CAnimation para notificarnos de la finalizaci�n de una animaci�n
		y que esta es la �nica restante en animationSet al que pertenece.

		@param animation Animacion que finaliza
		*/
		//void animationSetFinished(const std::string &animation);


		void setAnimationBlendMode(BlendMode mode);

		/////////////////////////////  METODOS DE OBSEVADORES DE HUESOS ///////////////////////////
		/*
		Obtiene la posici�n global de un hueso a partir de su nombre

		@param boneName Nombre del hueso a obtener la posici�n

		@return posici�n global del hueso
		*/
		Vector3 getBoneWorldPosition(std::string boneName);

		/*
		Obtiene la orientaci�n global de un hueso a partir de su nombre

		@param boneName Nombre del hueso a obtener la orientaci�n

		@return orientaci�n global del hueso
		*/
		Quaternion getBoneWorldOrientation(std::string boneName);

		/*
		configura la posici�n global de un hueso a partir de su nombre

		@param boneName Nombre del hueso a configurar la posici�n
		@param worldPos Posici�n a la cual configurar el hueso
		*/
		void setBoneWorldPosition(std::string boneName, Vector3 worldPos);

		/*
		configura la orientaci�n global de un hueso a partir de su nombre

		@param boneName Nombre del hueso a configurar la posici�n
		@param worldQuat Orientaci�n a la cual configurar el hueso
		*/
		void setBoneWorldOrientation(std::string boneName, Quaternion worldQuat);


		/*
		configura la orientaci�n local de un hueso a partir de su nombre

		@param boneName Nombre del hueso a configurar la posici�n
		@param localQuat Orientaci�n a la cual configurar el hueso
		*/
		void setBoneLocalOrientation(std::string boneName, Quaternion localQuat);

		void yawBone(std::string boneName, float angle);
		void setBoneManuallyControlled(std::string boneName, bool mc);

	protected:

		/**
		Mapa de observadores de las animaciones. Actualmente este mapa de observadores se usa para notificar el fin de una animaci�n concreta

		*/
		typedef std::map<std::string,std::set<CAnimatedEntityListener*>> TAnimationObservers;
		typedef std::pair<std::string, std::set<CAnimatedEntityListener*>> TAnimationObserverPair;
		TAnimationObservers _animationObservers;

		// Cada entidad debe pertenecer a una escena. Solo permitimos
		// a la escena actualizar el estado.
		friend class CScene;
		
		/**
		Actualiza el estado de la entidad cada ciclo.
		
		@param secs N�mero de milisegundos transcurridos desde la �ltima 
		llamada.
		*/
		virtual void tick(float secs);

		/**
		Animaci�nes que tiene la entidad activada.
		La clave corresponde al conjunto de animaciones. Cada conjunto tiene una animaci�n activa
		*/
		typedef std::map<std::string, CAnimationBlender*> TAnimationSets;
		TAnimationSets _currentAnimations;

		/*
		Cach� de m�scaras de animacion, con el objetivo de no preguntar a LUA constantemente
		*/
		typedef std::map<std::string, std::vector<std::pair<float, float>>> TAnimationMasks;
		typedef std::pair<std::string, std::vector<std::pair<float, float>>> TAnimationMaskPair;

		TAnimationMasks _animationMasks;

		std::string _animationConfig;

		/*
		M�todo que crea la m�scara de animaci�n. Para crearla la consulta directamente a LUA. 
		En caso de tenerla cacheada la devuelve directamente

		@param animationSet Conjunto de Animaci�n para crear la m�scara
		*/
		std::vector<std::pair<float, float>> createAnimationMask(const std::string &animationSet);

	}; // class CAnimatedEntity

} // namespace Graphics

#endif // __Graphics_AnimatedEntity_H
