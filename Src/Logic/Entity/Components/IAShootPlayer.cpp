#include "IAShootPlayer.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "BulletController.h"

namespace Logic 
{
	IMP_FACTORY(CIAShootPlayer);
	 
	bool CIAShootPlayer::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayShoot"))
			_maxDelayShoot = entityInfo->getIntAttribute("maxDelayShoot");

		if(entityInfo->hasAttribute("typeBullet"))
			_typeBullet = entityInfo->getStringAttribute("typeBullet");

		if(entityInfo->hasAttribute("rangeShoot"))
			_rangeShoot = entityInfo->getIntAttribute("rangeShoot");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAShootPlayer::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayShoot"))
			_maxDelayShoot = entityInfo->getIntAttribute("maxDelayShoot");

		if(entityInfo->hasAttribute("typeBullet"))
			_typeBullet = entityInfo->getStringAttribute("typeBullet");

		if(entityInfo->hasAttribute("rangeShoot"))
			_rangeShoot = entityInfo->getIntAttribute("rangeShoot");

		return true;
	}

	//---------------------------------------------------------

	bool CIAShootPlayer::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CHANGE_TARGET" ||
			   message->getType() == "CAN_IA";
	} // accept

	//---------------------------------------------------------

	void CIAShootPlayer::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CHANGE_TARGET")
		{
		
			CEntity* ent = dynamic_cast<CHANGE_TARGET*>(message.get())->getLogicEntity();
			if(ent != NULL)
			{
				if(!ent->getIsDead())
				{
					_target = ent; 
				}
			}
	
		 
		}

		else if (message->getType() == "CAN_IA")
		{
			_canIA = dynamic_cast<CAN_IA*>(message.get())->getBool();
		}
	} // process

	//---------------------------------------------------------

	CIAShootPlayer::~CIAShootPlayer()
	{
		
	} // ~Controller

	bool CIAShootPlayer::activate()
	{
		_delayShoot = 0;
		_canIA = true;
		_target = CServer::getSingletonPtr()->getPlayer();

		return true;

	} // activate

	void CIAShootPlayer::deactivate()
	{
	} // deactivate

	void CIAShootPlayer::onTick(unsigned int msecs)
	{
		if (_canIA)
		{
			_delayShoot += msecs;

			if (_delayShoot > _maxDelayShoot)
			{
				Vector3 posTarget = _target->getCenterPosition();

				if (_entity->getCenterPosition().x > posTarget.x - _rangeShoot && _entity->getCenterPosition().x < posTarget.x + _rangeShoot &&
					_entity->getCenterPosition().y > posTarget.y - _rangeShoot && _entity->getCenterPosition().y < posTarget.y + _rangeShoot)
				{
					_delayShoot = 0;

					Logic::CEntity* bullet = Logic::CEntityFactory::getSingletonPtr()->createEntityByType(_typeBullet, _entity->getCenterPosition(), _entity->getMap());

					CBulletController* bController = bullet->getComponent<CBulletController*>();
					bController->setBulletConfiguration(posTarget,0);
				}
			}
		}

		else
		{
			_delayShoot = 0;
		}
	} // tick

} // namespace Logic

