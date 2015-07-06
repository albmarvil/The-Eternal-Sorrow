#include "LaserBallController.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"

#include "Map/MapEntity.h"

namespace Logic 
{
	IMP_FACTORY(CLaserBallController);
	 
	bool CLaserBallController::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("velMov"))
		{
			_velMov = entityInfo->getFloatAttribute("velMov");
		}

		if(entityInfo->hasAttribute("timeUntilExplode"))
		{
			_maxTimeUntilExplode = entityInfo->getIntAttribute("timeUntilExplode");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CLaserBallController::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("velMov"))
		{
			_velMov = entityInfo->getFloatAttribute("velMov");
		}

		if(entityInfo->hasAttribute("timeUntilExplode"))
		{
			_maxTimeUntilExplode = entityInfo->getIntAttribute("timeUntilExplode");
		}

		return true;
	}

	//---------------------------------------------------------

	bool CLaserBallController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "DESTINATION_POS";
			

	} // accept

	//---------------------------------------------------------

	void CLaserBallController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "DESTINATION_POS")
		{
			Vector3 destPos = dynamic_cast<DESTINATION_POS*>(message.get())->getVector3();

			_direction = (destPos - _entity->getCenterPosition())* Vector3(1,1,0);  
			_direction.normalise();
		}

	} // process

	void CLaserBallController::setDirection(Vector3 destPos)
	{
		_direction = (destPos - _entity->getCenterPosition())* Vector3(1,1,0);  
		_direction.normalise();
	}
	//---------------------------------------------------------

	CLaserBallController::~CLaserBallController()
	{
		
	} // ~Controller

	bool CLaserBallController::activate()
	{
		_physicComponent = (CPhysicEntity*) _entity->getComponent("CPhysicEntity");

		_velFinal = 0;
		_direction = 0;
		_timeUntilExplode = 0;
		return true;

	} // activate

	bool CLaserBallController::onAwake()
	{
		_velFinal = 0;
		_direction = 0;

		return true;
	}

	void CLaserBallController::deactivate()
	{

	} // deactivate

	
	void CLaserBallController::onTick(unsigned int msecs)
	{
		_velFinal = _velMov * _direction * msecs; 

		_physicComponent->kinematicMove(_velFinal);

		
	} // tick

} // namespace Logic

