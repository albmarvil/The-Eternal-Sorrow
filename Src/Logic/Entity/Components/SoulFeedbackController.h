/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __Logic__SoulFeedbackController__H
#define __Logic__SoulFeedbackController__H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{

	class CSoulFeedbackController : public IComponent
	{
		DEC_FACTORY(CSoulFeedbackController);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CSoulFeedbackController() : IComponent(){}
		
		virtual ~CSoulFeedbackController();
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

		/**
		Este componente sólo acepta mensajes de tipo HABILIDAD.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo HABILIDAD mira si el tipo de habilidad es el de aumentar salto, y de ser asi aumento el salto y la dispierto
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		//Funcion que lanza un rayo a posicion relativa de la camara y me devuelve la posicion en el mundo
		void getRealPos(Vector3 &point, float posX, float posY);

		float calculateSpeed();
	protected:
		Vector2 _destino;
		Vector2 _actual;

		Vector3 _pointDestino;
		Vector3 _pointActual;
	}; 

	REG_FACTORY(CSoulFeedbackController);

} // namespace Logic

#endif 
