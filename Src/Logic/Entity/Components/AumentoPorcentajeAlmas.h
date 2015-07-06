/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __Logic__AumentoPorcentajeAlmas__H
#define __Logic__AumentoPorcentajeAlmas__H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{

	class CPorcentajeAlmas : public IComponent
	{
		DEC_FACTORY(CPorcentajeAlmas);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CPorcentajeAlmas() : IComponent(){}
		
		virtual ~CPorcentajeAlmas();
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

	protected:
		int _porcentToAument;
	}; 

	REG_FACTORY(CPorcentajeAlmas);

} // namespace Logic

#endif 
