#include "IAShootUniDir.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "BulletController.h"

namespace Logic 
{
	IMP_FACTORY(CIAShootUniDir);
	 
	bool CIAShootUniDir::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayShoot"))
			_maxDelayShoot = entityInfo->getIntAttribute("maxDelayShoot");

		if(entityInfo->hasAttribute("typeBullet"))
			_typeBullet = entityInfo->getStringAttribute("typeBullet");

		if(entityInfo->hasAttribute("dirShoot"))
		{
			std::string nomDirShoot = entityInfo->getStringAttribute("dirShoot");

			if (nomDirShoot == "Up")
			{
				_dirShoot = Vector3(0,1,0);
			}
			else if (nomDirShoot == "Down")
			{
				_dirShoot = Vector3(0,-1,0);
			}

			else if (nomDirShoot == "Right")
			{
				_dirShoot = Vector3(1,0,0);
			}

			else if (nomDirShoot == "Left")
			{
				_dirShoot = Vector3(-1,0,0);
			}
		}
		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAShootUniDir::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayShoot"))
			_maxDelayShoot = entityInfo->getIntAttribute("maxDelayShoot");

		if(entityInfo->hasAttribute("typeBullet"))
			_typeBullet = entityInfo->getStringAttribute("typeBullet");

		if(entityInfo->hasAttribute("dirShoot"))
		{
			std::string nomDirShoot = entityInfo->getStringAttribute("dirShoot");

			if (nomDirShoot == "Up")
			{
				_dirShoot = Vector3(0,1,0);
			}
			else if (nomDirShoot == "Down")
			{
				_dirShoot = Vector3(0,-1,0);
			}

			else if (nomDirShoot == "Right")
			{
				_dirShoot = Vector3(1,0,0);
			}

			else if (nomDirShoot == "Left")
			{
				_dirShoot = Vector3(-1,0,0);
			}
		}

		return true;
	}

	//---------------------------------------------------------

	bool CIAShootUniDir::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		
		return message->getType() == "CAN_IA";
	} // accept

	//---------------------------------------------------------

	void CIAShootUniDir::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CAN_IA")
		{
			_canIA = dynamic_cast<CAN_IA*>(message.get())->getBool();
		}
	} // process

	//---------------------------------------------------------

	CIAShootUniDir::~CIAShootUniDir()
	{
		
	} // ~Controller

	bool CIAShootUniDir::activate()
	{
		_delayShoot = 0;

		return true;

	} // activate

	void CIAShootUniDir::deactivate()
	{
	} // deactivate

	void CIAShootUniDir::onTick(unsigned int msecs)
	{
		if (_canIA)
		{
			_delayShoot += msecs;

			//BaseSubsystems::Log::Debug("Delay:" + std::to_string(_delayShoot));

			if (_delayShoot > _maxDelayShoot)
			{
				_delayShoot = 0;

				Logic::CEntity* bullet = Logic::CEntityFactory::getSingletonPtr()->createEntityByType(_typeBullet, _entity->getCenterPosition(), _entity->getMap());

				CBulletController* bController = bullet->getComponent<CBulletController*>();
				bController->setBulletConfiguration(_entity->getCenterPosition() + _dirShoot,0);

			}
		}
		else
			_delayShoot = 0;

	} // tick

} // namespace Logic

