/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

//Este componente tiene que ir siempre con el componente timeToLive, que es el que hara desaparecer el objeto

#ifndef __Logic_TimeToDisappear_H
#define __Logic_TimeToDisappear_H

#include "Logic/Entity/Component.h"
#include <string>

namespace Logic
{
	class CTimeToDisappear : public IComponent
	{
		DEC_FACTORY(CTimeToDisappear);
	public:

		CTimeToDisappear(): IComponent(){}

		virtual ~CTimeToDisappear(){}

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

	private:

		unsigned int _timeStamp;

		//Tiempo en el que se empezara a parpadear
		unsigned int _time_to_start_to_disappear;

		//Tiempo que se empieza a contar desde el parpadeo en cestion
		unsigned int _timeStampParpadeo;

		//Tiempo que debe trascurrir en ese parpadeo para que se parpadee de nuevo
		unsigned int _time_between_parpadeo;

		//Indica si esta invisible la entidad o no
		bool _isVisible;

		//Tiempo que se empieza a contar cuando una entidad se pone invisible
		unsigned int _timeStampInvisible;

		//Proporcion parpadeo
		float _proporcion;

		unsigned int _timeInvisible;

	};
	REG_FACTORY(CTimeToDisappear);
}
#endif