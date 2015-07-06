#ifndef __Logic_BulletController_H
#define __Logic_BulletController_H

#include "Logic/Entity/Component.h"

namespace Logic
{
	class CPhysicEntity;
};

namespace Logic 
{
	class CBulletController : public IComponent
	{
		DEC_FACTORY(CBulletController);
		public:

			/**
			Constructor por defecto; en la clase base no hace nada.
			*/
			CBulletController() : IComponent() , _touch(false), _tipoEntidadColisionada(""){}
		
			/**
			Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
			el fichero de mapa.
			*/
			virtual bool OnSpawn(const Map::CEntity *entityInfo);

			/**
			Este componente s�lo acepta mensajes.
			*/
			virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

			/**
			
			*/
			virtual void process(const std::shared_ptr<Logic::IMessage> &message);

			bool respawn(const Map::CEntity *entityInfo);

			/**
			M�todo llamado en cada frame que actualiza el estado del componente.
			<p>
			Se encarga de mover la entidad en cada vuelta de ciclo cuando es
			necesario (cuando est� andando o desplaz�ndose lateralmente).

			@param msecs Milisegundos transcurridos desde el �ltimo tick.
			*/
			virtual void onTick(unsigned int msecs);

			virtual bool activate();

			void setBulletConfiguration(Vector3 direction, float damage);

			bool getRotationBullet() { return _rotateBullet;};

		protected:

			/**
			funci�n que calcula la direcci�n en la que debe ir la bala
			*/
			void setDirection(Vector3 realPos);

			/**
			Velocidad a la que se mueve la bala
			*/
			float _speed;

			/*
			Indica si la bala debe rotarse seg�n la direcci�n de disparo
			*/
			bool _rotateBullet;

			/**
			atributo que almacena la direcci�n de la bala
			*/
			Vector3 _direction;

			/**
			Atributo que almacena la distancia
			*/
			float distance;

			/**
			Da�o que se produce cada vez que se toca.
			*/
			float _damage;

			/**
			Variable que indica si se ha chocado o no
			*/
			bool _touch;

			/**
			Variable que indica si puede moverse o no
			*/
			bool _canMove;

			//Componente f�sico de la entidad
			CPhysicEntity* _physicComponent;

			std::string _tipoEntidadColisionada;

	}; // class CBulletController

	REG_FACTORY(CBulletController);

} // namespace Logic

#endif // __Logic_BalaController_H
