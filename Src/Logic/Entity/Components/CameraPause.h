#ifndef __Logic_CameraPause_H
#define __Logic_CameraPause_H

#include "Logic/Entity/Component.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CCamera;
	class CScene;
}

//declaración de la clase
namespace Logic 
{
	class CCameraPause : public IComponent
	{
		DEC_FACTORY(CCameraPause);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CCameraPause() : IComponent(), _graphicsCamera(0), _targetPosition(Vector3::ZERO){}
		
	
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);
		
		virtual bool activate();
		
		
		virtual void deactivate();

		virtual void onTick(unsigned int msecs);

		void pulsarD(unsigned int msecs);
		void pulsarA(unsigned int msecs);
		void pulsarW(unsigned int msecs);
		void pulsarS(unsigned int msecs);
	protected:
		
		/**
		Cámara gráfica.
		*/
		Graphics::CCamera *_graphicsCamera;

		Vector3 _targetPosition;

		Vector3 _positionInitial;

		float _angle;

	};

	REG_FACTORY(CCameraPause);

} // namespace Logic

#endif 
