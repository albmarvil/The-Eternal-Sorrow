#include "AumentoUnSalto.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "LUA/ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CAumentoUnSalto);
	
	//---------------------------------------------------------

	bool CAumentoUnSalto::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // spawn

	//---------------------------------------------------------

	bool CAumentoUnSalto::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Recibimos un mensaje de tipo HABILIDAD
		return message->getType() == "SET_HABILIDAD";
	} // accept
	
	//---------------------------------------------------------

	void CAumentoUnSalto::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_HABILIDAD") == 0)
		{
			if(!dynamic_cast<SET_HABILIDAD*>(message.get())->getString().compare("AumentarSalto"))
			{
				//Despertamos el componente, si no estaba despierto antes
				if(_isSleeping)
				{
					_isSleeping = false;
				}

				//MANDO MENSAJE DE AUMENTAR SALTO AL AVATAR CONTROLLER
  				std::shared_ptr<AUMENTAR_SALTO> m(new AUMENTAR_SALTO());
				m->setInt(1);
				
				_entity->emitMessage(m);

				//Apuntamos en lua la habilidad que se ha recogido
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("AddHability", _name);
			}		
		}
	} // process
	//------------------------------------------------------------------------

	void CAumentoUnSalto::onTick(unsigned int msecs)
	{
	}

} // namespace Logi