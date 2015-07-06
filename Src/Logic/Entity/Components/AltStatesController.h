#ifndef __Logic_AltStatesController_H
#define __Logic_AltStatesController_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{

	struct AltState
	{
		std::string name;
		bool activeState;
		bool initState;
		int timeState;
	};

	class CAltStatesController : public IComponent
	{
		DEC_FACTORY(CAltStatesController);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CAltStatesController() : IComponent(){}
		
		virtual ~CAltStatesController();
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

		/**
		Este componente sólo acepta mensajes de tipo TOUCHED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo TOUCHED lanzaremos el ataque correspondiente, especial o principal.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		void sendSET_MATERIAL(const std::string &material);

		void stopAltState(std::string name);

		void enemyDie();
	protected:

		void PoisonState(AltState &state, unsigned int msecs);
		void CanibalState(AltState &state, unsigned int msecs);
		void SlowdownState(AltState &state, unsigned int msecs);
		void FrozenState(AltState &state, unsigned int msecs);

		std::string _materialDefault;
		std::string _materialCanibal;
		std::string _materialPoison;
		std::string _materialFrozen;

		std::vector<AltState> _vecAltStates;
		CEntity* _target;

		int _maxTimeCanibal;
		int _maxTimePoison;
		int _maxTimeSlowdown;
		int _maxTimeFrozen;

		int _tickDamagePoison;
		int _maxTickDamagePoison;
		float _percentDamagePoison;
	}; 

	REG_FACTORY(CAltStatesController);

} // namespace Logic

#endif 
