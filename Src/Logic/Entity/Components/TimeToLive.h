/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __Logic_TimeToLive_H
#define __Logic_TimeToLive_H
#include "Logic/Entity/Component.h"
#include <string>

namespace Logic
{
	class CTimeToLive : public IComponent
	{
		DEC_FACTORY(CTimeToLive);
	public:

		/**
		Constructor del componente
		**/
		CTimeToLive(): IComponent(){}

		/**
		Destructor del componente
		**/
		virtual ~CTimeToLive(){}

		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);
	
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);
		
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		virtual bool activate();

		virtual void deactivate();

		virtual void setTimeToLive(int _time);

	private:
		unsigned int _timeStamp;
		unsigned int _time_to_live;
		bool _canCountTime;
	};
	REG_FACTORY(CTimeToLive);
}
#endif