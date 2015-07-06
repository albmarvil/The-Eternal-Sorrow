

/**
@file Camera.h

Contiene la declaración del componente que controla la cámara gráfica
de una escena.

@see Logic::CCamera
@see Logic::IComponent

@author David Llansó
@date Septiembre, 2010
*/
#ifndef __Logic_Camera_H
#define __Logic_Camera_H

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
	class CCamera : public IComponent
	{
		DEC_FACTORY(CCamera);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CCamera() : IComponent(), _graphicsCamera(0), _canTarget(true), _shakeTarget(Vector3::ZERO){}
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la cámara gráfica y se leen atributos del mapa.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();
		
		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		virtual void deactivate();

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la cámara siguiendo al jugador.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Acepta mensajes.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Procesa mensajes
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		//Metodo que indica si la camara se puede seguir moviendo a la izqueirda o derecha segun el movimiento
		//relativo del player
		bool canMoveHorizontal(Vector3 position);

		//Metodo que indica si la camara se puede seguir moviendo arriba o abajo segun el movimiento
		//relativo del player
		bool canMoveVertical(Vector3 position);
	protected:
		
		/**
		Cámara gráfica.
		*/
		Graphics::CCamera *_graphicsCamera;

		/**
		Entidad que se usa como objetivo
		*/
		CEntity *_target;

		/**
		Distancia del punto al que mirará la cámara respecto a la entidad 
		objetivo. Es distancia sobre el plano XZ, la altura tiene su propio 
		atributo.
		*/
		float _targetDistance;
		
		/**
		altura del punto al que mirará la cámara respecto del suelo o plano XZ. Atributo necesario
		ya que si usaramos solo la posicion del target, apuntaríamos constantemente a la base del modelo
		*/
		float _targetHeight;


		/**
		Cantidad de movimiento relativo con respecto a la entidad objetivo
		*/
		float _relativeMovement;

		/*
		Posicion del tick anterior del target. Usada para calcular la cantidad de movimiento
		*/
		Vector3 _targetLastPos;


		/*
		FOV de la cámara en grados
		*/
		float _fov;

		float _fovHorizontal;

		/*
		Distancia más cercana a la cual se visualizarán los objetos
		*/
		float _nearClipDistance;


		/*
		Distancia más lejana a la cual se visualizarán los objetos
		*/
		float _farClipDistance;

		bool _canTarget;
		Vector3 _shakeTarget;

		float _targetLastX;
		float _targetLastY;

	}; // class CCamera

	REG_FACTORY(CCamera);

} // namespace Logic

#endif // __Logic_Camera_H
