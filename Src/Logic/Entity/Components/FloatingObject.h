/*
Clase que sirve para mover el objeto de arriba abajo

author Cesar Manuel Paz Guzman
date Febrero 2015
*/

#ifndef __Logic_FloatingObject_H
#define __Logic_FloatingObject_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

namespace Graphics
{
	class CAnimationObject;
}
//declaración de la clase
namespace Logic 
{
	class CFloatingObject : public IComponent
	{
		DEC_FACTORY(CFloatingObject);
	public:

		CFloatingObject() : IComponent(), _animationObject(NULL){}
		~CFloatingObject();

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

	protected:
		Graphics::CAnimationObject *_animationObject;
		int _tipoAnimacion;
	}; 

	REG_FACTORY(CFloatingObject);

} // namespace Logic

#endif 
