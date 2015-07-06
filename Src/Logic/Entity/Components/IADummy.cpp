#include "IADummy.h"

#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include "Logic/Server.h"

namespace Logic 
{
	IMP_FACTORY(CIADummy);
	 
	bool CIADummy::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("rangoDestino"))
			rangoDestino = entityInfo->getIntAttribute("rangoDestino");

		if(entityInfo->hasAttribute("maxVelX"))
			maxVelX = entityInfo->getFloatAttribute("maxVelX");

		if(entityInfo->hasAttribute("velMov"))
			velMov = entityInfo->getFloatAttribute("velMov");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIADummy::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("rangoDestino"))
			rangoDestino = entityInfo->getIntAttribute("rangoDestino");

		if(entityInfo->hasAttribute("maxVelX"))
			maxVelX = entityInfo->getFloatAttribute("maxVelX");

		if(entityInfo->hasAttribute("velMov"))
			velMov = entityInfo->getFloatAttribute("velMov");

		return true;
	}

	//---------------------------------------------------------

	bool CIADummy::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//return message->getType() == "KINEMATIC_MOVE";

		return true;
	} // accept

	//---------------------------------------------------------

	void CIADummy::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		/*if(message->getType().compare("CONTROL") == 0)
		{
			if(!dynamic_cast<CONTROL*>(message.get())->getString().compare("walk"))
		
		}*/

	} // process

	//---------------------------------------------------------

	CIADummy::~CIADummy()
	{
		
	} // ~Controller

	bool CIADummy::activate()
	{

		// Obtenemos la entidad a la que sigue, que es el jugador
		entityToFollow = CServer::getSingletonPtr()->getPlayer();

		posDestNew = entityToFollow->getCenterPosition(); // Posición objetivo
		posDestOld = -1; // para que se envie un mensaje en el primer tick que cambie el mov

		hasChanged = false;
		movActual = -1;
		return true;

	} // activate

	void CIADummy::deactivate()
	{
		// Obtenemos la entidad a la que sigue, que es el jugador
		entityToFollow = 0;
	} // deactivate

	void CIADummy::onTick(unsigned int msecs)
	{

		posDestOld = posDestNew; // Posición destino que lleva desde el tick anterior
		posDestNew = entityToFollow->getCenterPosition(); // Nueva posición destino en este tick

		if (_entity->getCenterPosition() > posDestNew - rangoDestino && _entity->getCenterPosition() < posDestNew + rangoDestino)
		{
			//printf("--> SI esta en rango. Pos: %g,%g\n", _entity->getPosition().x, _entity->getPosition().y);
			
			if (movActual != AI::IMovement::MOVEMENT_KINEMATIC_ARRIVE)
				hasChanged = true;

			movActual = AI::IMovement::MOVEMENT_KINEMATIC_ARRIVE;
		}

		else
		{
			//printf("<-- NO esta en rango. Pos: %g,%g\n", _entity->getPosition().x, _entity->getPosition().y);

			if (movActual != AI::IMovement::MOVEMENT_KINEMATIC_SEEK)
				hasChanged = true;

			movActual = AI::IMovement::MOVEMENT_KINEMATIC_SEEK;
		}

		if (posDestNew != posDestOld)
		{
			hasChanged = true;
		}
	
		if (hasChanged)
		{
			std::shared_ptr<Logic::MOVE_TO> m(new Logic::MOVE_TO());
			m->setVector3(posDestNew);
			m->setInt(movActual);
			_entity->emitMessage(m);

			hasChanged = false;
		}
	
		if (posDestNew.x >= _entity->getCenterPosition().x)
		{
			std::shared_ptr<Logic::TURN> m(new Logic::TURN());
			m->setInt(1);
			_entity->emitMessage(m);
		}

		else
		{
			std::shared_ptr<Logic::TURN> m(new Logic::TURN());
			m->setInt(-1);
			_entity->emitMessage(m);
		}
	} // tick

} // namespace Logic

