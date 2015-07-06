/**
@file SoulMenuGenerator.h

@author Alberto Martínez Villarán
@date Mayo, 2015
*/

#ifndef __Logic_SoulMenuGenerator_H
#define __Logic_SoulMenuGenerator_H

#include "Logic/Entity/Component.h"

// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	
	class CSoulMenuGenerator : public IComponent
	{
		DEC_FACTORY(CSoulMenuGenerator);
	public:
		
		//constructor
		CSoulMenuGenerator() : _generatorTime(10000), _timeAcum(0), _offset(Vector3::ZERO) {};

		///destructor
		virtual ~CSoulMenuGenerator();
		

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		
		//virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

	
		//virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		//virtual void deactivate();

	protected:
		
		unsigned int _generatorTime;

		unsigned int _timeAcum;

		Vector3 _offset;

	}; // class CSoulMenuGenerator

	REG_FACTORY(CSoulMenuGenerator);

} // namespace Logic

#endif // __Logic_SoulMenuGenerator_H
