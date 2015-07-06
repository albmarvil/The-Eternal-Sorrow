/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __Logic_Billboard_H
#define __Logic_Billboard_H

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

	class CBillBoard : public IComponent
	{
		DEC_FACTORY(CBillBoard);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CBillBoard() : IComponent(), _lifeBar(false) {}
		
		virtual ~CBillBoard();
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs){}

		virtual bool activate();

		virtual void deactivate();

		void loadLife();

		void updateLife(int life, int lifeMax);

		void setVisible(bool visible);

	protected:

		Vector3 _dimensions;
		std::string _materialName;
		Graphics::CBillBoards *_billboard;
		bool _lifeBar;
		Vector3 _offset;
	}; 

	REG_FACTORY(CBillBoard);

} // namespace Logic

#endif 
