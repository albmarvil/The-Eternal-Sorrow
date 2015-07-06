
/**
@author Cesar Manuel Paz Guzman
@date Enero, 2015
*/

#ifndef __Logic_ParticleController_H
#define __Logic_ParticleController_H

#include "Logic/Entity/Component.h"
#include <string>

namespace Graphics
{
	class CParticle;
	class CScene;
}
namespace Logic
{

	class CParticleController : public IComponent
	{
		DEC_FACTORY(CParticleController);
	public:

		/**
		Constructor del componente
		**/
		CParticleController(): IComponent(){}

		/**
		Destructor del componente
		**/
		virtual ~CParticleController();

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
		Este componente s�lo acepta mensajes de tipo TOUCHED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje TOUCHED se env�a otro mensaje de tipo DAMAGED a la
		entidad tocada.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		void setQuota(int quota);

		void startParticle();

		void stopParticle();

		void setVisible(bool visible);

	private:
		//Vector de sources
		std::vector<std::string> _vecSources;

		// Vector de huesos asociados a part�culas
		std::vector<std::string> _vecBoneParticles;

		std::vector<Graphics::CParticle*> _vectorParticulas;

		bool _activate;

		bool _startVisibleParticles;

	};
	REG_FACTORY(CParticleController);
}
#endif