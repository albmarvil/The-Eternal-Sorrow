/**
@file ArmaI.cpp

Contiene la implementación del componente que controla la vida de una entidad.
 
@see Logic::CArmaI
@see Logic::IComponent

@author David Llansó
@date Octubre, 2010
*/

#include "ArmaI.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

#include "Graphics/Server.h"
#include "Graphics/Scene.h"
#include "Physics/Server.h"
#include "OgreSceneNode.h"

namespace Logic 
{
	IMP_FACTORY(CArmaI);
	
	//---------------------------------------------------------

	bool CArmaI::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("gunI"))
		{
			typeGun = entityInfo->getIntAttribute("gunI");
		}

		if(entityInfo->hasAttribute("shoot"))
		{
			typeAttack = entityInfo->getBoolAttribute("shoot");
		}

		if(entityInfo->hasAttribute("cursorGroupCollision"))
		{
			GROUP_COLLISION = entityInfo->getIntAttribute("cursorGroupCollision");
		}

		return true;

	} // spawn

	//---------------------------------------------------------

	bool CArmaI::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "MOUSE_ACTION" ||
			message->getType() == "CONTROL";

	} // accept
	
	//---------------------------------------------------------

	void CArmaI::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("MOUSE_ACTION") == 0)
		{
			
		}
		else if(message->getType().compare("CONTROL") == 0)
		{
			if(!dynamic_cast<CONTROL*>(message.get())->getString().compare("gun"))
			{
				typeAttack = dynamic_cast<CONTROL*>(message.get())->getBool();
			}
		}

	} // process

	//---------------------------------------------------------------------------------

	void CArmaI::shoot(Vector3 pos, int arma)
	{
		realPos = pos;
		realPos.z = _entity->getPosition().z;

		std::string nameentity;
		//Vamos a suponer que el arma podrá estár en cualquiera de los dos brazos, que en el futuro será la realidad

		if((typeGun == 1) && (arma == typeGun))
		{
			//arma izquierda
			if(typeAttack)
			{
				//ataque especial
				nameentity = "BIzquierdaEspecial";
			}
			else
			{
				nameentity = "BIzquierdaNormal";
			}
		}
		else
		{
			if((typeGun == 2) && (arma == typeGun))
			{
				//arma derecha
				if(typeAttack)
				{
					//ataque especial
					nameentity = "BDerechaEspecial";
				}
				else
				{
					nameentity = "BDerechaNormal";
				}
			}
		}

		if(nameentity.compare("") != 0)
		{
			createbala(nameentity);
		}
	}
	

	//------------------------------------------------------------------------

	void CArmaI::onTick(unsigned int msecs)
	{
	}

	//------------------------------------------------------------------------

	void CArmaI::createbala(std::string nameentity)
	{
		//Instanciamos bien las balas no a pelo plis
		Vector3 initialpos = Vector3::ZERO;

		if(realPos.x > _entity->getPosition().x)
		{
			initialpos = Vector3(_entity->getPosition().x+15, _entity->getPosition().y + 20, _entity->getPosition().z);
		}
		else
		{
			if(realPos.x < _entity->getPosition().x)
			{
				initialpos = Vector3(_entity->getPosition().x-15, _entity->getPosition().y + 20, _entity->getPosition().z);
			}
		}

		Logic::CEntity* entity_bala = Logic::CEntityFactory::getSingletonPtr()->createEntityByType(nameentity, initialpos, _entity->getMap());
		//enviamos un mensaje de giro
		std::shared_ptr<Logic::DIRECTION> m(new Logic::DIRECTION());

		//enviamos la posición para que pueda calcular donde está el ratón
		m->setString("direction");
		m->setVector(realPos);
		entity_bala->emitMessage(m);
	}

} // namespace Logic