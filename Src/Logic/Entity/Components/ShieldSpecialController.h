#ifndef __Logic_ShieldSpecialController_H
#define __Logic_ShieldSpecialController_H

#include "Logic/Entity/Component.h"

namespace Logic
{
	class CMeleeShield;
}

namespace Logic 
{
	class CShieldSpecialController : public IComponent
	{
		DEC_FACTORY(CShieldSpecialController);

	public:

		CShieldSpecialController() : IComponent(), _shieldComponent(0),_material("-"), _materialSangre("-"), _cleanUpTime(0), _timeAcum(0), _countToClean(false) {}

		virtual ~CShieldSpecialController();
		
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

		void setShieldControllerConfiguration(Vector3 mousePos, float damage, CMeleeShield* shieldComponent);

	protected:

		// Pong
		bool _canPong;
		Vector3 _dirPong;
		float _maxVelPongX;
		float _maxVelPongY;
		float _maxVelPongOri;
		int _timePong;
		int _maxTimePong;
		Vector3 _velPong;
		std::string _lastCollision;
		int _numHitsPong;
		int _maxHitsPong;

		// ApproachMD
		bool _canAppr;
		float _accelMovAppr;
		float _maxVelMovAppr;
		int _minDistStopAppr;
		Vector3 _dirAppr;
		Vector3 _velFinalAppr;
		CEntity* _player;

		float _angle;

		float _damage;

		CMeleeShield* _shieldComponent;


		
		/*
		Material de sangre del escudo
		*/
		std::string _material;
		std::string _materialSangre;

		unsigned int _cleanUpTime;
		unsigned int _timeAcum;

		bool _countToClean;

		void sendSET_MATERIAL(const std::string &material);

	}; // class Controller

	REG_FACTORY(CShieldSpecialController);

} // namespace Logic

#endif // __Logic_Controller_H