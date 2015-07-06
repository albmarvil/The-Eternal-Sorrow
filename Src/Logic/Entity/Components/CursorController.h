/**
@file CursorController.h

Contiene la declaración del componente que controla el movimiento del ratón y el giro del personaje

@see Logic::CCursorController
@see Logic::IComponent

@author Bárbara Domínguez
@date Enero, 2015
*/
#ifndef __Logic_CursorController_H
#define __Logic_CursorController_H

#include "Logic/Entity/Component.h"

namespace Graphics 
{
	class CServer;
}

namespace Physics
{
	class CServer;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla el movimiento del ratón, cuando recibe un mensaje de tipo turn anota la
	nueva posición relativa en X del ratón y en su tick lanza un rayo para calcular la posición en el
	mundo y si debe girar o no al personaje
	
	@ingroup logicGroup

	@author Bárbara Domínguez
	@date Enero, 2015
*/
	class CCursorController : public IComponent
	{
		DEC_FACTORY(CCursorController);
		public:

			/**
			Constructor por defecto; en la clase base no hace nada.
			*/
			CCursorController() : IComponent() {}

			/**
			Inicialización del componente usando la descripción de la entidad que hay en 
			el fichero de mapa.
			*/
			virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

			/**
			Método llamado en cada frame que actualiza el estado del componente.
			<p>
			Se encarga de mover la entidad en cada vuelta de ciclo cuando es
			necesario (cuando está andando o desplazándose lateralmente).

			@param msecs Milisegundos transcurridos desde el último tick.
			*/
			virtual void tick(unsigned int msecs);

			/**
			Este componente sólo acepta mensajes de tipo CONTROL turn.
			*/
			virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

			/**
			Al recibir un mensaje de tipo MOUSE_ACTION turn se apunta la posición relativa en x del ratón
			*/
			virtual void process(const std::shared_ptr<Logic::IMessage> &message);
		
			/**
			Provoca que la entidad gire. Números Positivos para	giro a 
			derechas, negativos para giro izquierdas.

			@param mousePosX Cantidad de giro. Posición actual del ratón
			*/
			void turn(int look);

			/**
			Esta función anota la posición actual del ratón
			*/
			void SetRelativePos(Vector2 relativePos);

		protected:

			/**
			Variable que guarda la posición relativa actual del ratón
			*/
			Vector2 posRel;

			


	}; // class CLife

	REG_FACTORY(CCursorController);

} // namespace Logic

#endif // __Logic_Life_H
