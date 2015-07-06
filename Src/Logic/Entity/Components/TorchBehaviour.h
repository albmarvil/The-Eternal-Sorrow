
#ifndef __Logic_TorchBehaviour_H
#define __Logic_TorchBehaviour_H

#include "Logic/Entity/Component.h"


//declaraci�n de la clase
namespace Logic 
{

	class CTorchBehaviour : public IComponent
	{
		DEC_FACTORY(CTorchBehaviour);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CTorchBehaviour() : IComponent(){}
		
		virtual ~CTorchBehaviour();
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Metodo que sirve para setear el entityInfo y el map en donde sera respawneada. No pongo solo la posicion, sino mas bien
		el entityInfo entero, porque puede ocurrir que queramos setear por ejemplo, la vida que tenga un enemigo, dado
		que los enemigos se haran mas fuertes. 

		@param map Mapa Logic en el que se registrara la entidad
		@param entity Informacion de construccion de la entidad leida del fichero
		@return Cierto si el respawn ha sido satisfatorio
		**/
		virtual bool respawn(const Map::CEntity *entity);

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

		/**
		Este componente s�lo acepta mensajes de tipo SET_POSITION. Estos mensajes  
		s�lo se utilizan para mover entidades de tipo cinem�tico.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Cuando recibe mensajes de tipo SET_POSITION cambia la posicion de la luz en la escena
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);


		virtual void onTick(unsigned int msecs);
	protected:

		float _baseRange;

		float _baseConstant;

		float _baseLinear;

		float _baseQuadratic;

		float _flickering;

		float _timeAcum;

		bool _changeTorch;

		float _minQuadraticMultiplier;

		float _maxQuadraticMultiplier;

		float _minRangeMultiplier;

		float _maxRangeMultiplier;

		float _minLinearMultiplier;

		float _maxLinearMultiplier;

	}; // class CTorchBehaviour

	REG_FACTORY(CTorchBehaviour);

} // namespace Logic

#endif // __Logic_TorchBehaviour_H
