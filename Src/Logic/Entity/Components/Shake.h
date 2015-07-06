#ifndef __Logic_Shake_H
#define __Logic_Shake_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CShake : public IComponent
	{
		DEC_FACTORY(CShake);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CShake() : IComponent() ,_activeShake(false), _shakeStarted(false), _maxShake(0), _minShake(0), _randAngle(0), _shakeRadius(0), _canTarget(true){}
		
		virtual ~CShake();
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		bool OnSpawn(const Map::CEntity *entityInfo);

		bool respawn(const Map::CEntity *entityInfo){ return true;}

		void onTick(unsigned int msecs);

		bool activate();

		void deactivate();

		/**
		Acepta mensajes
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Procesa mensajes.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		void newShake(float max, float min);

		void shake();
		

	protected:
		bool _activeShake;
		bool _shakeStarted;
		float _maxShake;
		float _minShake;
		float _shakeRadius;
		float _randAngle;
		Vector3 _offsetShake;
		bool _canTarget;
	}; 

	REG_FACTORY(CShake);

} // namespace Logic

#endif 
