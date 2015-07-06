/**
@file SmartObjectJump.h

Contiene la declaraci�n de la clase del componente encargado de definir el comportamiento de un
SmartObject de salto.

@see Logic::CSmartObjectJump
@see Logic::IComponent

@author Alberto Mart�nez
@date Febrero, 2015
*/
#ifndef __Logic_SmartObjectJump_H
#define __Logic_SmartObjectJump_H

#include "Logic/Entity/Component.h"


//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente se encarga de dar la orden de salto a los enemigos que pasen sobre el.
	Para ello escuchar� los mensajes TOUCHED y procesar� aquello que solo sean de enemigos.
	
    @ingroup logicGroup

	@author Alberto Mart�nez
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
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity).

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
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
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		TOUCHED.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		*/
		virtual void deactivate();

	protected:

		/*
		Direcci�n a la cual esta permitido saltar desde este gameObject
		*/
		int _jumpDirection;

	}; // class CSmartObjectJump

	REG_FACTORY(CSmartObjectJump);

} // namespace Logic

#endif // __Logic_SmartObjectJump_H
