/**
@file CursorController.h

Contiene la declaraci�n del componente que controla el movimiento del rat�n y el giro del personaje

@see Logic::CCursorController
@see Logic::IComponent

@author B�rbara Dom�nguez
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

//declaraci�n de la clase
namespace Logic 
{
/**
	Este componente controla el movimiento del rat�n, cuando recibe un mensaje de tipo turn anota la
	nueva posici�n relativa en X del rat�n y en su tick lanza un rayo para calcular la posici�n en el
	mundo y si debe girar o no al personaje
	
	@ingroup logicGroup

	@author B�rbara Dom�nguez
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
			Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
			el fichero de mapa.
			*/
			virtual bool spawn(CEntity* entity, CMap *map, const Map::CEntity *entityInfo);

			/**
			M�todo llamado en cada frame que actualiza el estado del componente.
			<p>
			Se encarga de mover la entidad en cada vuelta de ciclo cuando es
			necesario (cuando est� andando o desplaz�ndose lateralmente).

			@param msecs Milisegundos transcurridos desde el �ltimo tick.
			*/
			virtual void tick(unsigned int msecs);

			/**
			Este componente s�lo acepta mensajes de tipo CONTROL turn.
			*/
			virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

			/**
			Al recibir un mensaje de tipo MOUSE_ACTION turn se apunta la posici�n relativa en x del rat�n
			*/
			virtual void process(const std::shared_ptr<Logic::IMessage> &message);
		
			/**
			Provoca que la entidad gire. N�meros Positivos para	giro a 
			derechas, negativos para giro izquierdas.

			@param mousePosX Cantidad de giro. Posici�n actual del rat�n
			*/
			void turn(int look);

			/**
			Esta funci�n anota la posici�n actual del rat�n
			*/
			void SetRelativePos(Vector2 relativePos);

		protected:

			/**
			Variable que guarda la posici�n relativa actual del rat�n
			*/
			Vector2 posRel;

			


	}; // class CLife

	REG_FACTORY(CCursorController);

} // namespace Logic

#endif // __Logic_Life_H
