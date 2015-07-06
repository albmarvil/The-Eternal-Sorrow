//---------------------------------------------------------------------------
// AnimatedEntity.h
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.h

Contiene la declaración de la clase que representa una entidad gráfica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
@refactor Alberto Martínez
@date Marzo, 2015
*/

#ifndef __Graphics_AnimatedEntity_H
#define __Graphics_AnimatedEntity_H

#include "Entity.h"


// Predeclaración de clases para ahorrar tiempo de compilación
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
	terminan las animaciones de ésta.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	@refactor Alberto Martinez
	@date Marzo, 2015
	*/
	class CAnimatedEntityListener 
	{
	public:

		/**
		Método que será invocado siempre que se termine una animación.
		Las animaciones en cíclicas no invocarán nunca este método.

		@param animation Nombre de la animación terminada.
		*/
		virtual void animationFinished(const std::string &animation) {}

		/*
		*Método que será invocado siempre que acabe la última animación de un animationSet

		@param animation Nombre de la animación terminada
		*/
		//virtual void animationSetFinished(const std::string &animation) {}
	}; // CAnimatedEntityListener

	/**
	Esta clase sirve para registrar observadores, los cuales heredarán de ella e implementarán
	su método setTransform.
	*/
	class IObserver
	{
	public:
		virtual void setTransform(Vector3 bonePos, Quaternion boneOri);
	};

	/**
	Clase que representa una entidad gráfica con animaciones. Especializa la clase
	Graphics::CEntity por lo que contiene una referencia a una entidad de 
	Ogre y al Nodo que la contiene. Añade la posibilidad de activar una animación 
	y reproducirla.
	<p>
	Esta clase es bastante sencilla y solo controlamos una animación activa.
	Podría ser más sofisticada permitiendo interpolación de animaciones o avisando
	mediante observers cuando una animación termina de reproducirse.
	<p>
	Oculta los detalles escabrosos del motor gráfico.
	
	@ingroup graphicsGroup

	@author David Llansó
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
		Destructor de la aplicación.
		*/
		virtual ~CAnimatedEntity();

		/**
		Activa una animación a partir de su nombre y el conjunto de animaciones correspondiente.
		Si no existe conjunto de animacion, se creará dicho conjunto y se seteará dicha animación.

		En caso de existir, se hará interpolación con la animación existente en dicho conjunto de animacion.

		@param anim Nombre de la animación a activar.
		@param animSet Conjunto donde activar la animación
		@param loop true si la animación debe reproducirse cíclicamente.
		@param restart true si queremos reiniciar la animación
		@param duration (OPCIONAL) Duración de la transición de animaciones, en caso de que se deba transitar
		@param speed (OPCIONAL) Velocidad de ejecución de la animación
		@return true si la animación solicitada fue correctamente activada.

		*/
		bool setAnimation(const std::string &anim, const std::string &animSet, bool loop, bool restart, float duration = 0.0f, float speed = 1.0f);
		
		/**
		Desactiva una animación a partir de su nombre y del conjunto de animación indicado

		@param anim Nombre de la animación a activar.
		@param animSet Conjunto de animación donde desactivar dicha animación

		@return true si la animación solicitada fue correctamente desactivada.
		*/
		bool stopAnimation(const std::string &anim, const std::string &animSet);
		
		/**
		Desactiva todas las animaciones de una entidad.
		*/
		void stopAllAnimations();

		/**
		Función que registra al oyente de la entidad gráfica. 
		*/
		void setObserver(CAnimatedEntityListener *observer, const std::string &animationName);

		/**
		Función que quita al oyente de la entidad gráfica.
		*/
		void removeObserver(CAnimatedEntityListener *observer, const std::string &animationName);

		/*
		Método para obtener el objeto AnimationState de un esqueleto a partir de su nombre.
		Si este no existe se devuelve NULL

		@param name Nombre de la animación

		@return AnimationState correspondiente. NULL si no existe
		*/
		Ogre::AnimationState* getAnimationState(const std::string &name);

		/*
		Método llamado desde CAnimation para notificarnos de la finalización de una animación.
		Nosotros notificaremos a todos sus observadores

		@param animation Animación que se notifica la finalización
		*/
		void animationFinished (const std::string &animation);

		/*
		Método llamado desde CAnimation para notificarnos de la finalización de una animación
		y que esta es la única restante en animationSet al que pertenece.

		@param animation Animacion que finaliza
		*/
		//void animationSetFinished(const std::string &animation);


		void setAnimationBlendMode(BlendMode mode);

		/////////////////////////////  METODOS DE OBSEVADORES DE HUESOS ///////////////////////////
		/*
		Obtiene la posición global de un hueso a partir de su nombre

		@param boneName Nombre del hueso a obtener la posición

		@return posición global del hueso
		*/
		Vector3 getBoneWorldPosition(std::string boneName);

		/*
		Obtiene la orientación global de un hueso a partir de su nombre

		@param boneName Nombre del hueso a obtener la orientación

		@return orientación global del hueso
		*/
		Quaternion getBoneWorldOrientation(std::string boneName);

		/*
		configura la posición global de un hueso a partir de su nombre

		@param boneName Nombre del hueso a configurar la posición
		@param worldPos Posición a la cual configurar el hueso
		*/
		void setBoneWorldPosition(std::string boneName, Vector3 worldPos);

		/*
		configura la orientación global de un hueso a partir de su nombre

		@param boneName Nombre del hueso a configurar la posición
		@param worldQuat Orientación a la cual configurar el hueso
		*/
		void setBoneWorldOrientation(std::string boneName, Quaternion worldQuat);


		/*
		configura la orientación local de un hueso a partir de su nombre

		@param boneName Nombre del hueso a configurar la posición
		@param localQuat Orientación a la cual configurar el hueso
		*/
		void setBoneLocalOrientation(std::string boneName, Quaternion localQuat);

		void yawBone(std::string boneName, float angle);
		void setBoneManuallyControlled(std::string boneName, bool mc);

	protected:

		/**
		Mapa de observadores de las animaciones. Actualmente este mapa de observadores se usa para notificar el fin de una animación concreta

		*/
		typedef std::map<std::string,std::set<CAnimatedEntityListener*>> TAnimationObservers;
		typedef std::pair<std::string, std::set<CAnimatedEntityListener*>> TAnimationObserverPair;
		TAnimationObservers _animationObservers;

		// Cada entidad debe pertenecer a una escena. Solo permitimos
		// a la escena actualizar el estado.
		friend class CScene;
		
		/**
		Actualiza el estado de la entidad cada ciclo.
		
		@param secs Número de milisegundos transcurridos desde la última 
		llamada.
		*/
		virtual void tick(float secs);

		/**
		Animaciónes que tiene la entidad activada.
		La clave corresponde al conjunto de animaciones. Cada conjunto tiene una animación activa
		*/
		typedef std::map<std::string, CAnimationBlender*> TAnimationSets;
		TAnimationSets _currentAnimations;

		/*
		Caché de máscaras de animacion, con el objetivo de no preguntar a LUA constantemente
		*/
		typedef std::map<std::string, std::vector<std::pair<float, float>>> TAnimationMasks;
		typedef std::pair<std::string, std::vector<std::pair<float, float>>> TAnimationMaskPair;

		TAnimationMasks _animationMasks;

		std::string _animationConfig;

		/*
		Método que crea la máscara de animación. Para crearla la consulta directamente a LUA. 
		En caso de tenerla cacheada la devuelve directamente

		@param animationSet Conjunto de Animación para crear la máscara
		*/
		std::vector<std::pair<float, float>> createAnimationMask(const std::string &animationSet);

	}; // class CAnimatedEntity

} // namespace Graphics

#endif // __Graphics_AnimatedEntity_H
