/**
@file SmartObjectJump.h

Contiene la declaración de la clase del componente encargado de definir el comportamiento de un
SmartObject de salto.

@see Logic::CSmartObjectJump
@see Logic::IComponent

@author Alberto Martínez
@date Febrero, 2015
*/
#ifndef __Logic_SmartObjectJump_H
#define __Logic_SmartObjectJump_H

#include "Logic/Entity/Component.h"


//declaración de la clase
namespace Logic 
{
/**
	Este componente se encarga de dar la orden de salto a los enemigos que pasen sobre el.
	Para ello escuchará los mensajes TOUCHED y procesará aquello que solo sean de enemigos.
	
    @ingroup logicGroup

	@author Alberto Martínez
	@date Febrero, 2015
*/
	class CSmartObjectJump : public IComponent
	{
		DEC_FACTORY(CSmartObjectJump);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CSmartObjectJump() : IComponent(), _jumpDirection(1){}

		/**
		Destructor (virtual); 
		*/
		virtual ~CSmartObjectJump();
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity).

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
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
		Método virtual que elige que mensajes son aceptados. Son válidos
		TOUCHED.

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
		<p>
		*/
		virtual void deactivate();

	protected:

		/*
		Dirección a la cual esta permitido saltar desde este gameObject
		*/
		int _jumpDirection;

	}; // class CSmartObjectJump

	REG_FACTORY(CSmartObjectJump);

} // namespace Logic

#endif // __Logic_SmartObjectJump_H
