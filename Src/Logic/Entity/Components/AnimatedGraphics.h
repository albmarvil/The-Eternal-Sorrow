/**
@file AnimatedGraphics.h

Contiene la declaraci�n del componente que controla la representaci�n
gr�fica de una entidad est�tica.

@see Logic::CAnimatedGraphics
@see Logic::CGraphics

@author David Llans�
@date Agosto, 2010
*/
#ifndef __Logic_AnimatedGraphics_H
#define __Logic_AnimatedGraphics_H

#include "Graphics.h"
#include "Graphics/AnimatedEntity.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CAnimatedEntity;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representaci�n gr�fica animada de una 
	entidad. Especializa a la clase CGraphics para que la entidad gr�fica
	creada sea animada.
	<p>
	Adem�s de los mensajes de cambio de posici�n y de cambio de orientaci�n
	que acepta CGraphics, acepta mensajes para poner o para una animaci�n
	(SET_ANIMATION y STOP_ANIMATION).
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Agosto, 2010
*/
	class CAnimatedGraphics : public CGraphics, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CAnimatedGraphics);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CAnimatedGraphics() : CGraphics(), _animatedGraphicsEntity(0),
			_defaultAnimation("") , _animationConfig(""), _speed(1.0f), _multSpeed(1.f) {}

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SET_ANIMATION y STOP_ANIMATION.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);
		
		////////////////////////////////////////
		// M�todos de CAnimatedEntityListener //
		////////////////////////////////////////
		/**
		M�todo que ser� invocado siempre que se termine una animaci�n.
		Las animaciones en c�clicas no invocar�n nunca este m�todo.

		@param animation Nombre de la animaci�n terminada.
		*/
		void animationFinished(const std::string &animation);

		Graphics::CAnimatedEntity* getAnimatedEntity(){return _animatedGraphicsEntity;}

		/**
		Metodo que sirve para setear el entityInfo y el map en donde sera respawneada. No pongo solo la posicion, sino mas bien
		el entityInfo entero, porque puede ocurrir que queramos setear por ejemplo, la vida que tenga un enemigo, dado
		que los enemigos se haran mas fuertes. 

		@param map Mapa Logic en el que se registrara la entidad
		@param entity Informacion de construccion de la entidad leida del fichero
		@return Cierto si el respawn ha sido satisfatorio
		**/
		//virtual bool respawn(const Map::CEntity *entity);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		La implementaci�n registrar� al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cron�metro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementaci�n eliminar� al componente de algunos observers en los 
		que pueda estar registrado (como el cron�metro del sistema, etc.).m
		*/
		virtual void deactivate();

		std::string getDefaultAnimation(){return _defaultAnimation;};
		std::string getActualAnimation(){return _actualAnimation;};

		void setAnimation(const std::string &animation, const std::string &animSet = "default", bool loop = false, bool restart = false, float duration = 0.0f, float speed = 1.0f, bool saveAnim = true);

		void setMultiSpeed(float ms) {_multSpeed = ms;};


	protected:

		/**
		M�todo virtual que construye la entidad gr�fica animada de la entidad. 
		Sobreescribe el m�todo de la clase CGraphics.
		
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Entidad gr�fica creada, NULL si hubo alg�n problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(const Map::CEntity *entityInfo);
		
		/**
		Entidad gr�fica animada.
		*/
		Graphics::CAnimatedEntity *_animatedGraphicsEntity;

		/**
		Animaci�n por defecto de una entidad gr�fica animada.
		*/
		std::string _defaultAnimation;

		/**
		Animaci�n actual
		*/
		std::string _actualAnimation;
		
		/*
		Multiplicador de velocidad para la animacion
		*/
		float _multSpeed;
		/**
		Configuraci�n de la entidad animada
		*/
		std::string _animationConfig;

		/**
		Speed por defecto de la animaci�n default
		*/
		float _speed;

	}; // class CAnimatedGraphics

	REG_FACTORY(CAnimatedGraphics);

} // namespace Logic

#endif // __Logic_AnimatedGraphics_H
