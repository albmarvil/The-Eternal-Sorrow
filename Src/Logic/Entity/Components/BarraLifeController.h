/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __Logic_BarraLifeController_H
#define __Logic_BarraLifeController_H

#include "Logic/Entity/Component.h"

namespace Graphics
{
	class CBillBoards;
}

namespace Logic
{
	class CEntity;
}

//declaración de la clase
namespace Logic 
{

	class CBarraLifeController : public IComponent
	{
		DEC_FACTORY(CBarraLifeController);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CBarraLifeController() : IComponent(){}
		
		virtual ~CBarraLifeController();
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

		void updateLifeBar(int life, int lifeMax);

	protected:

		/**
		Función que calcula la distancia máxima del rango de vision del jugador
		*/
		void SetRangoVision();

		Graphics::CBillBoards *_lifeBar;
		Vector3 _dimensions;
		std::string _materialName;

		/**
		nombre del compondente correspondiente al enemigo
		*/
		std::string nameComponent;

		CEntity *_player;

		//Distancia maxima de vision
		float _maxDistance;

		float _minDistance;
		bool _visible; //Flag que indica si la barra de vida del enemigo esta activada o no

		unsigned int _timeStamp;
	}; 

	REG_FACTORY(CBarraLifeController);

} // namespace Logic

#endif 
