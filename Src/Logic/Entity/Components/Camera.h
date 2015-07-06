

/**
@file Camera.h

Contiene la declaraci�n del componente que controla la c�mara gr�fica
de una escena.

@see Logic::CCamera
@see Logic::IComponent

@author David Llans�
@date Septiembre, 2010
*/
#ifndef __Logic_Camera_H
#define __Logic_Camera_H

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
	class CCamera : public IComponent
	{
		DEC_FACTORY(CCamera);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CCamera() : IComponent(), _graphicsCamera(0), _canTarget(true), _shakeTarget(Vector3::ZERO){}
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Se accede a los atributos 
		necesarios como la c�mara gr�fica y se leen atributos del mapa.

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

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
		C�mara gr�fica.
		*/
		Graphics::CCamera *_graphicsCamera;

		/**
		Entidad que se usa como objetivo
		*/
		CEntity *_target;

		/**
		Distancia del punto al que mirar� la c�mara respecto a la entidad 
		objetivo. Es distancia sobre el plano XZ, la altura tiene su propio 
		atributo.
		*/
		float _targetDistance;
		
		/**
		altura del punto al que mirar� la c�mara respecto del suelo o plano XZ. Atributo necesario
		ya que si usaramos solo la posicion del target, apuntar�amos constantemente a la base del modelo
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
		FOV de la c�mara en grados
		*/
		float _fov;

		float _fovHorizontal;

		/*
		Distancia m�s cercana a la cual se visualizar�n los objetos
		*/
		float _nearClipDistance;


		/*
		Distancia m�s lejana a la cual se visualizar�n los objetos
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
