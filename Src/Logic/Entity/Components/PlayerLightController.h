

/**
@file PlayerLightController.h



@see Logic::CPlayerLightController
@see Logic::IComponent

@author alberto Mart�nez
@date Enero, 2015
*/
#ifndef __Logic_PlayerLightController_H
#define __Logic_PlayerLightController_H

#include "Logic/Entity/Component.h"


//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente setear� a la entidad en todo momento donde est� el player teniendo en cuenta un offset
	<p>

	
    @ingroup logicGroup

	@author Alberto Mart�nez
	@date Enero, 2015
*/
	class CPlayerLightController : public IComponent
	{
		DEC_FACTORY(CPlayerLightController);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CPlayerLightController() : IComponent(), _offset(Vector3::ZERO) {}
		
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
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		virtual void deactivate();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la c�mara siguiendo al jugador.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);

	protected:
		
		/**
		Offset con respecto al player
		*/
		Vector3 _offset;

		/**
		puntero al player, lo pedimos solo una vez cuando nos activamos
		*/
		CEntity *_player;

	}; // class CPlayerLightController

	REG_FACTORY(CPlayerLightController);

} // namespace Logic

#endif // __Logic_PlayerLightController_H
