#ifndef __Logic_SwordSlashController_H
#define __Logic_SwordSlashController_H

#include "Logic/Entity/Component.h"

namespace Logic
{
	class CPhysicEntity;
};

namespace Logic 
{
	class CSwordSlashController : public IComponent
	{
		DEC_FACTORY(CSwordSlashController);
		public:

			/**
			Constructor por defecto; en la clase base no hace nada.
			*/
			CSwordSlashController() : IComponent(){}
		
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

			void setSlashConfiguration(Vector3 direction, float damage);

		protected:

			/**
			funci�n que calcula la direcci�n en la que debe ir la bala
			*/
			void setDirection(Vector3 realPos);

			/**
			Velocidad a la que se mueve la bala
			*/
			float _speed;

			/**
			atributo que almacena la direcci�n de la bala
			*/
			Vector3 _direction;

			/**
			Da�o que se produce cada vez que se toca.
			*/
			float _damage;

			//Componente f�sico de la entidad
			CPhysicEntity* _physicComponent;

	}; // class CSwordSlashController

	REG_FACTORY(CSwordSlashController);

} // namespace Logic

#endif // __Logic_BalaController_H
