#include "IAApproachPosition.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"

namespace Logic 
{
	IMP_FACTORY(CIAApproachPosition);
	 
	bool CIAApproachPosition::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("accelMovApproach"))
		{
			_accelMov = entityInfo->getFloatAttribute("accelMovApproach");
		}

		if(entityInfo->hasAttribute("maxVelApproach"))
		{
			_velMov = entityInfo->getFloatAttribute("maxVelApproach");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAApproachPosition::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("accelMovApproach"))
		{
			_accelMov = entityInfo->getFloatAttribute("accelMovApproach");
		}

		if(entityInfo->hasAttribute("maxVelApproach"))
		{
			_velMov = entityInfo->getFloatAttribute("maxVelApproach");
		}

		return true;
	}

	//---------------------------------------------------------

	bool CIAApproachPosition::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CAN_MOVE" ||
			message->getType() == "DESTINATION_POS" ||
			message->getType() == "CAN_IA";

		return true;
	} // accept

	//---------------------------------------------------------

	void CIAApproachPosition::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CAN_MOVE")
		{
			_canMove = dynamic_cast<CAN_MOVE*>(message.get())->getBool();
		}

		else if (message->getType() == "DESTINATION_POS")
		{
			_destPos = dynamic_cast<DESTINATION_POS*>(message.get())->getVector3();

			_direction = (_destPos - _entity->getPosition()) * Vector3(1,1,0); // No nos interesa el z
			_direction.normalise();

			// Dirección a la que mirar
			if (_destPos.x >= _entity->getCenterPosition().x)
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
		}

		else if (message->getType() == "CAN_IA")
		{
			_canMove = dynamic_cast<CAN_IA*>(message.get())->getBool();

			if (_canMove)
				setSleeping(false);
			else
				setSleeping(true);
		}
	} // process

	//---------------------------------------------------------

	CIAApproachPosition::~CIAApproachPosition()
	{
		
	} // ~Controller

	bool CIAApproachPosition::activate()
	{
		_destPos = 0;
		_direction = 0;
		_velFinal = 0;
		_canMove = true;
		_runningAway = false;

		return true;

	} // activate

	bool CIAApproachPosition::onAwake()
	{
		_destPos = 0;
		_direction = 0;
		_velFinal = 0;
		_canMove = true;
		_runningAway = false;

		return true;
	}

	void CIAApproachPosition::deactivate()
	{

	} // deactivate

	
	void CIAApproachPosition::onTick(unsigned int msecs)
	{
		if (_canMove)
		{
			_velFinal = _velMov * _direction * msecs; 
			
			//La direccion ya viene normalizada		
			std::shared_ptr<AVATAR_WALK> m(new AVATAR_WALK());
			m->setVector3(_velFinal);
			_entity->emitMessage(m);
		}
	
	} // tick

} // namespace Logic

