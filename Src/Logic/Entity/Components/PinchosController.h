#ifndef __Logic__PinchosController__H
#define __Logic__PinchosController__H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{

	class CPinchosController : public IComponent
	{
		DEC_FACTORY(CPinchosController);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CPinchosController() : IComponent(){}
		
		virtual ~CPinchosController();
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

		void downPinchos(){ _down = true; _up = false;}

		void upPinchos(){ _up = true; _down = false;}

	private:
		bool _down;
		bool _up;

		Vector3 _valorArriba;
		Vector3 _valorAbajo;
	}; 

	REG_FACTORY(CPinchosController);

} // namespace Logic

#endif 
