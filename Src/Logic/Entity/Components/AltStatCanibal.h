#ifndef __Logic_AltStatCanibal_H
#define __Logic_AltStatCanibal_H

#include "Logic/Entity/Component.h"

namespace Logic 
{
	class CAltStatCanibal : public IComponent
	{
		DEC_FACTORY(CAltStatCanibal);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CAltStatCanibal() : IComponent(){}
		
		virtual ~CAltStatCanibal();
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		bool OnSpawn(const Map::CEntity *entityInfo);

		bool respawn(const Map::CEntity *entityInfo);

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
		
		void stopCanibal();

		void sendSET_MATERIAL(const std::string &material);

	protected:
		float _timeCanibal;
		float _maxTimeCanibal;
		bool _activeCanibal;
		CEntity* _target;

		std::string _material;

		std::string _canibalMaterial;
	}; 

	REG_FACTORY(CAltStatCanibal);

} // namespace Logic

#endif 
