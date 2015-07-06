/**
@author Cesar Manuel Paz Guzman
@date Enero, 2015
*/


#ifndef __Logic_AumentoUnSalto_H
#define __Logic_AumentoUnSalto_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

//declaraci�n de la clase
namespace Logic 
{
	class CAumentoUnSalto : public IComponent
	{
		DEC_FACTORY(CAumentoUnSalto);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CAumentoUnSalto() : IComponent(){}
		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		M�todo llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando est� andando o desplaz�ndose lateralmente).

		@param msecs Milisegundos transcurridos desde el �ltimo tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Este componente s�lo acepta mensajes de tipo HABILIDAD.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo HABILIDAD mira si el tipo de habilidad es el de aumentar salto, y de ser asi aumento el salto y la dispierto
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

	}; 

	REG_FACTORY(CAumentoUnSalto);

} // namespace Logic

#endif 
