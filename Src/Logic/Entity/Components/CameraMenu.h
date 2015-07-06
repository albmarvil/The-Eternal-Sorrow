

/**
@file CameraMenu.h

Contiene la declaración del componente que controla la cámara gráfica
de una escena.

@see Logic::CCameraMenu
@see Logic::IComponent

@author Alberto Martínez Villarán
@date Mayo de 2015
*/
#ifndef __Logic_CameraMenu_H
#define __Logic_CameraMenu_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CCamera;
	class CScene;
}

//declaración de la clase
namespace Logic 
{
/**
	Componente que se encarga de mover la cámara gráfica de una escena, para que 
	ésta se pueda reenderizar, siguiendo al jugador.
	<p>
	La cámara se situará a una distancia del plano de juego. El movimiento de esta siempre dependerá
	del movimiento del jugador. Con los parámetros "targetDistance" y "targetHeight" podremos controlar
	la distancia de la cámara del plano de juego y la altura de la base del modelo del jugador, respectivamente.
	Además con el parámetro "relative_movement" podremos controlar el porcentaje de movimiento del jugador que corresponde
	a la cámara.De esta forma la cámara seguira al jugador en cuanto a movimiento y siempre apuntará hacia la posicion del player.
	<p>
	El componente irá recalculando en cada ciclo las posiciones de la cámara 
	en función de la posición y orientación del jugador.
	
    @ingroup logicGroup

	@author David Llansó García
	@date Septiembre, 2010
*/
	class CCameraMenu : public IComponent
	{
		DEC_FACTORY(CCameraMenu);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CCameraMenu() : IComponent(), _graphicsCamera(0), _targetPosition(Vector3::ZERO){}
		
	
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		
		virtual bool activate();
		
		
		//virtual void deactivate();

		
		//virtual void onTick(unsigned int msecs);

		/**
		Acepta mensajes.
		*/
		//virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Procesa mensajes
		*/
		//virtual void process(const std::shared_ptr<Logic::IMessage> &message);

	protected:
		
		/**
		Cámara gráfica.
		*/
		Graphics::CCamera *_graphicsCamera;

		Vector3 _targetPosition;

		/*
		FOV de la cámara en grados
		*/
		float _fov;

		/*
		Distancia más cercana a la cual se visualizarán los objetos
		*/
		float _nearClipDistance;


		/*
		Distancia más lejana a la cual se visualizarán los objetos
		*/
		float _farClipDistance;

	}; // class CCameraMenu

	REG_FACTORY(CCameraMenu);

} // namespace Logic

#endif // __Logic_CameraMenu_H
