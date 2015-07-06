/*
Componente que indica que la entidad dispone de scripts.lua para definir su comportamiento. 

author Cesar Manuel Paz Guzman
date Marzo 2015
*/

#ifndef __Logic_ScriptComponent_H
#define __Logic_ScriptComponent_H

#include "Logic/Entity/Component.h"

//declaración de la clase
#include <vector>

namespace Logic
{
	class CScriptEntity;
}

namespace Logic 
{

	class CScriptComponent : public IComponent
	{
		DEC_FACTORY(CScriptComponent);

	public:

		CScriptComponent() : IComponent() {}

		virtual ~CScriptComponent();
		
		virtual void onTick(unsigned int msecs);

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		virtual void deactivate();

		bool createScriptEntity(const std::string &name);

		bool accept(const std::shared_ptr<Logic::IMessage> &message);

		void process(const std::shared_ptr<Logic::IMessage> &message);

	protected:

		//Contiene un vector de referencias de tantos scriptEntity como scripts se le hayan indicado en el arquetipo
		std::vector<CScriptEntity *> _VectorScriptEntity;

	}; 

	REG_FACTORY(CScriptComponent);

} 

#endif 
