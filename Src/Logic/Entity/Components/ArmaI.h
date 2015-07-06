/**
@file ArmaI.h

Contiene la declaración del componente que controla la vida de una entidad.

@see Logic::CArmaI
@see Logic::IComponent

@author Bárbara Domínguez
@date Enero, 2015
*/
#ifndef __Logic_ArmaI_H
#define __Logic_ArmaI_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

namespace Graphics 
{
	class CServer;
	class CScene;
}

namespace Physics
{
	class CServer;
}

namespace Ogre
{
	class SceneNode;
}

//declaración de la clase
namespace Logic 
{
/**
	Este componente controla el disparo de un jugador, recibe mensajes cuando se hace click en el ratón y cuando
	se pulsa la tecla shift
	
	@ingroup logicGroup

	@author Bárbara Domínguez
	@date Enero, 2015
*/
	class CArmaI : public IComponent
	{
		DEC_FACTORY(CArmaI);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CArmaI() : IComponent(), realPos(Vector3::ZERO) {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

	protected:
		
		/**
		Variable que almacena que arma llevamos,
		la 1 será la correspondiente al botón izquierdo
		la 2 será la correspondiente al botón derecho
		*/
		int typeGun;

		/**
		Variable que indica si tenemos el ataque primario o el secundario
		false indica que tenemos el ataque normal
		true indica que tenemos el ataque secundario
		*/
		bool typeAttack;

		/**
		variable que almacena el grupo de colisión
		*/
		int GROUP_COLLISION;

		/**
		variable que almacena la posición real del ratón
		*/
		Vector3 realPos;

		/**
		método que se encarga de crear la bala al disparar
		*/
		void createbala(std::string nameentity);

		/**
		función que se encarga de crear el disparo del jugador, teniendo en cuenta el botón del ratón que se ha pulsado
		y si se está pulsando la tecla shift
		*/
		void shoot(Vector3 pos, int arma);

	}; // class CArmaI

	REG_FACTORY(CArmaI);

} // namespace Logic

#endif // __Logic_ArmaI_H
