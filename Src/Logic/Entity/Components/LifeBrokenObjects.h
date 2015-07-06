/**
@author Cesar Manuel Paz Guzman
@date Abril, 2015
*/

#ifndef __Logic_LifeBrokenObjects_H
#define __Logic_LifeBrokenObjects_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Components/Life.h"

//declaraci�n de la clase
namespace Logic
{
	class CBarraLifeController;
	class CLife;
	class CEntity;
}
namespace Logic 
{
/**
	Este componente controla la vida de una entidad. Procesa mensajes de tipo 
	DAMAGED (indican que la entidad ha sido herida) y resta el da�o a la vida de la
	entidad.
	<p>
	La vida de la entidad se especifica en el mapa con el atributo "life".
	
	@ingroup logicGroup

	@author B�rbara Dom�nguez de la Torre Gonz�lez
	@date Abril, 2015
*/
	class CLifeBrokenObjects : public CLife
	{
		DEC_FACTORY(CLifeBrokenObjects);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CLifeBrokenObjects() {CLife();}
		
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);
		virtual bool respawn(const Map::CEntity *entityInfo);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		bool accept(const std::shared_ptr<Logic::IMessage> &message);
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);
		bool activate();
		void deactivate();
	private:
		std::vector<std::string> _objetosRompiblesGenerar;

	}; 

	REG_FACTORY(CLifeBrokenObjects);

} // namespace Logic

#endif
