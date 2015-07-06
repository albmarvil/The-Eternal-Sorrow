/**
@author Cesar Manuel Paz Guzman
@date Enero, 2015
*/


#ifndef __Logic_ArrowEnemy_H
#define __Logic_ArrowEnemy_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"
#include <map>
#include <list>

namespace CEGUI
{
	class Window;	
}

namespace Logic
{
	class CEntity;
}

//declaración de la clase
namespace Logic 
{
	class CArrowEnemy : public IComponent
	{
		DEC_FACTORY(CArrowEnemy);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CArrowEnemy() : IComponent(){}
		
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

		void addEnemy(int id, CEntity *entity);
		void removeEnemy(int id);

	private:
		//Vector con todos los enemigos vivos
		std::map<int,CEntity *> _enemies;

		//Vector con todos los enemigos ue estan fuera del viewport sin asociar flecha
		std::list<CEntity*> _enemiesOutView;

		//map con los enemigos asociados a las flechas fuera del viewport
		std::map<CEGUI::Window*,CEntity*> _enemiesWithArrow;

		//numero maximo actuales de flechas permitidas
		int _numArrowsMax;
		//Numero de flechas asignadas hasta el momento
		int _numArrowAsign;

		void obteneterEnemigosFueraViewport();

		bool isOutViewport(const Vector2 &screenPosition);

		void asignArrow(CEGUI::Window* arrow);

		void checkEnemiesWithArrow();

		void rotateArrow(CEGUI::Window*, CEntity *ent);
	}; 

	REG_FACTORY(CArrowEnemy);

} // namespace Logic

#endif 
