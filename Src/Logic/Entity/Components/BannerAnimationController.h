/**
@file BannerAnimationController.h

Contiene la declaraci�n del componente que provoca la carga
de un nuevo mapa llamando al MapManager
cuando recibe un mensaje TOUCHED / UNTOUCHED.
 

@see Logic::BannerAnimationController
@see Logic::IComponent

@author Alberto Mart�nez
@date Octubre, 2015
*/
#ifndef __Logic_BannerAnimationController_H
#define __Logic_BannerAnimationController_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"
#include "Graphics/AnimatedEntity.h"

namespace Graphics 
{
	class CServer;
	class CScene;
	class CCamera;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente procesa mensajes de tipo TOUCHED o UNTOUCHED (indican que la 
	entidad ha sido tocada o dejada de ser tocada) para cargar a traves del MapManager
	el siguiente mapa.
	<p>

	
    @ingroup logicGroup

	@author Alberto Mart�nez
	@date Octubre, 2015
*/
	class CBannerAnimationController : public IComponent, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CBannerAnimationController);
	public:

		/**
		Constructor por defecto.
		*/
		CBannerAnimationController() : IComponent() {}
		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		M�todo que se invoca para activar el componente.
		*/
		virtual bool activate();
		
		/**
		M�todo que se invoca al desactivar el componente.
		*/
		virtual void deactivate();

		/**
		Este componente s�lo acepta mensaje de tipos TOUCHED y UNTOUCHED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir mensaje TOUCHED y UNTOUCHED emite mensajes SWITCH
		a la entidad objetivo para que cambie de posici�n.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		void animationFinished(const std::string &animation);

	protected:
		/*
		Puntero a la entidad animada del player
		*/
		Graphics::CAnimatedEntity* _playerAnimatedEntity;

		/**
		animaci�n actual
		*/
		std::string current_animation;

		void sendSET_ANIMATION(char *animacion, bool loop, bool restart, float speed);

	}; // class CSwitchTrigger

	REG_FACTORY(CBannerAnimationController);

} // namespace Logic

#endif // __Logic_SwitchTrigger_H
