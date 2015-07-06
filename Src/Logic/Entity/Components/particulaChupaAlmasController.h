/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __Logic__particulaChupaAlmasController__H
#define __Logic__particulaChupaAlmasController__H

#include "Logic/Entity/Component.h"

namespace Logic
{
	class CEntity;
}
//declaración de la clase
namespace Logic 
{

	class CParticulaChupaAlmasController : public IComponent
	{
		DEC_FACTORY(CParticulaChupaAlmasController);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CParticulaChupaAlmasController() : IComponent(){}
		
		virtual ~CParticulaChupaAlmasController();
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

		void moveTo(CEntity *destino);

	protected:
		bool _moveTo;
		CEntity *_destino;
		float _speed;
		float _distanceToDisappear;
	}; 

	REG_FACTORY(CParticulaChupaAlmasController);

} // namespace Logic

#endif 
