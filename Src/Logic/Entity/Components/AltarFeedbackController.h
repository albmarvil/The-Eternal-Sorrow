#ifndef __Logic__AltarFeedbackController__H
#define __Logic__AltarFeedbackController__H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{

	class CAltarFeedbackController : public IComponent
	{
		DEC_FACTORY(CAltarFeedbackController);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CAltarFeedbackController() : IComponent(){}
		
		virtual ~CAltarFeedbackController(){};
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);

		virtual void Config(Vector3 posDest);

	protected:
		Vector3 _pointDestino;
		Vector3 _direction;
		float _radio2;
		float _vel;
	}; 

	REG_FACTORY(CAltarFeedbackController);

} // namespace Logic

#endif 
