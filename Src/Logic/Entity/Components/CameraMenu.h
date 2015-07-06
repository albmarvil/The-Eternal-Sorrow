

/**
@file CameraMenu.h

Contiene la declaraci�n del componente que controla la c�mara gr�fica
de una escena.

@see Logic::CCameraMenu
@see Logic::IComponent

@author Alberto Mart�nez Villar�n
@date Mayo de 2015
*/
#ifndef __Logic_CameraMenu_H
#define __Logic_CameraMenu_H

#include "Logic/Entity/Component.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CCamera;
	class CScene;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de mover la c�mara gr�fica de una escena, para que 
	�sta se pueda reenderizar, siguiendo al jugador.
	<p>
	La c�mara se situar� a una distancia del plano de juego. El movimiento de esta siempre depender�
	del movimiento del jugador. Con los par�metros "targetDistance" y "targetHeight" podremos controlar
	la distancia de la c�mara del plano de juego y la altura de la base del modelo del jugador, respectivamente.
	Adem�s con el par�metro "relative_movement" podremos controlar el porcentaje de movimiento del jugador que corresponde
	a la c�mara.De esta forma la c�mara seguira al jugador en cuanto a movimiento y siempre apuntar� hacia la posicion del player.
	<p>
	El componente ir� recalculando en cada ciclo las posiciones de la c�mara 
	en funci�n de la posici�n y orientaci�n del jugador.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
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
		C�mara gr�fica.
		*/
		Graphics::CCamera *_graphicsCamera;

		Vector3 _targetPosition;

		/*
		FOV de la c�mara en grados
		*/
		float _fov;

		/*
		Distancia m�s cercana a la cual se visualizar�n los objetos
		*/
		float _nearClipDistance;


		/*
		Distancia m�s lejana a la cual se visualizar�n los objetos
		*/
		float _farClipDistance;

	}; // class CCameraMenu

	REG_FACTORY(CCameraMenu);

} // namespace Logic

#endif // __Logic_CameraMenu_H
