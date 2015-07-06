#ifndef __Logic_IAApproach_H
#define __Logic_IAApproach_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CIAApproach : public IComponent
	{
		DEC_FACTORY(CIAApproach);

	public:

		CIAApproach() : IComponent() {}

		virtual ~CIAApproach();
		
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
		Método llamado en cada frame que actualiza el estado del componente.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		virtual bool onAwake();

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

		Vector3 calculateVelocity(unsigned int msecs, float &velMax);

		bool canMove() const { return _canMove;}
	protected:

		//virtual void SET_VEL(Vector3 vel, bool freno);

		//virtual void Stop();
		
		//bool noTileAbajo();

		CEntity *_target;
		float gravedad;
		int rangoDestino;
		float _accelMov;
		float _maxVel;
		float _velMov;
		int _direction;
		bool _isSlowdown;
		int _minDistSlowdown;
		bool _canMove;
		bool _canIA;
		bool _runningAway;

	}; // class Controller

	REG_FACTORY(CIAApproach);

} // namespace Logic

#endif // __Logic_Controller_H