#include "IAIdle.h"

#include "AI/Movement.h"

namespace Logic 
{
	IMP_FACTORY(CIAIdle);
	 
	bool CIAIdle::OnSpawn(const Map::CEntity *entityInfo)
	{
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAIdle::respawn(const Map::CEntity *entityInfo)
	{
		return true;
	}

	//---------------------------------------------------------

	bool CIAIdle::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return true;
	} // accept

	//---------------------------------------------------------

	void CIAIdle::process(const std::shared_ptr<Logic::IMessage> &message)
	{

	} // process

	//---------------------------------------------------------

	CIAIdle::~CIAIdle()
	{
		
	} // ~Controller

	bool CIAIdle::activate()
	{
		_posOld = _entity->getCenterPosition();
		return true;

	} // activate

	void CIAIdle::deactivate()
	{
	} // deactivate

	void CIAIdle::onTick(unsigned int msecs)
	{
		if (_entity->getCenterPosition() != _posOld)
		{
			std::shared_ptr<Logic::MOVE_TO> m(new Logic::MOVE_TO());
			m->setVector3(Vector3::ZERO);
			m->setInt(AI::IMovement::MOVEMENT_KINEMATIC_STOP);
			_entity->emitMessage(m);
		}

		_posOld = _entity->getCenterPosition();
		

	} // tick

} // namespace Logic

