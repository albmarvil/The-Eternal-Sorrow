#ifndef __Logic_IAInvincible_H
#define __Logic_IAInvincible_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CIAInvincible : public IComponent
	{
		DEC_FACTORY(CIAInvincible);

	public:

		CIAInvincible() : IComponent() {}

		virtual ~CIAInvincible();
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la cámara gráfica y se leen atributos del mapa.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		
		@remarks ESTE MÉTODO NO DEBE SER MODIFICADO POR EL USUARIO
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		*/
		virtual void deactivate();

		/**
		Despierta una entidad dormida

		@return true si la entidad se desperto correctamente
		*/
		virtual bool onAwake();

		/**
		Duerme una entidad

		@return true si la entidad se durmio correctamente
		*/
		virtual bool onSleep();

		/**
		Método llamado en cada frame que actualiza el estado del componente.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Método virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Metodo que sirve para setear el entityInfo y el map en donde sera respawneada. No pongo solo la posicion, sino mas bien
		el entityInfo entero, porque puede ocurrir que queramos setear por ejemplo, la vida que tenga un enemigo, dado
		que los enemigos se haran mas fuertes. 

		@param map Mapa Logic en el que se registrara la entidad
		@param entity Informacion de construccion de la entidad leida del fichero
		@return Cierto si el respawn ha sido satisfatorio
		**/
		virtual bool respawn(const Map::CEntity* entityInfo);

	protected:

		CEntity* _target;
		bool _activated;
		int _rangeDefend;
		int _maxTimeInvincible;
		int _currentTimeInvincible;
	}; // class Controller

	REG_FACTORY(CIAInvincible);

} // namespace Logic

#endif // __Logic_Controller_H