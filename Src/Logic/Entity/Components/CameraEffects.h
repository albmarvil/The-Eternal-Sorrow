/*
file CameraEffects.h

Este fichero contiene la declaraci�n del componente CameraEffects

Este componente se encarga de lanzar efectos de postprocesado en el viewport asociado a la escena de juego

@author Alberto Mart�nez Villar�n
@date Abril, 2015
*/
#ifndef __Logic_CameraEffects_H
#define __Logic_CameraEffects_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

namespace Graphics 
{
	class CServer;
	class CScene;
	class CCamera;
}

//declaraci�n de la clase
namespace Logic 
{
	
	class CCameraEffects : public IComponent
	{
		DEC_FACTORY(CCameraEffects);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CCameraEffects() : IComponent(){}
	
		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		Se coge el jugador del mapa, la entidad que se quiere "seguir".

		@return true si todo ha ido correctamente.
		*/
		//virtual bool activate();
		
		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		Se pone el objetivo a seguir a NULL.
		*/
		//virtual void deactivate();

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la c�mara siguiendo al jugador.

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		//virtual void onTick(unsigned int msecs);

		/**
		Acepta mensajes.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Procesa mensajes
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

	private:

		/*std::map<int, std::string> viewportEffects;*/
		std::string current_effect;

		/**
		Funci�n que cambia el vieport y actualiza el current_effect
		*/
		void ChangeEffect(const std::string &effect, bool activate);

	}; // class CCameraEffects

	REG_FACTORY(CCameraEffects);

} // namespace Logic

#endif // __Logic_CameraEffects_H
