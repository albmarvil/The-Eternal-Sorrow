#include "IAShootRaycast.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Logic/Server.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"
#include "BulletController.h"

namespace Logic 
{
	IMP_FACTORY(CIAShootRaycast);
	 
	bool CIAShootRaycast::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayShoot"))
			_maxDelayShoot = entityInfo->getIntAttribute("maxDelayShoot");

		if(entityInfo->hasAttribute("typeBullet"))
			_typeBullet = entityInfo->getStringAttribute("typeBullet");

		if(entityInfo->hasAttribute("rangeShoot"))
			_rangeShoot = entityInfo->getIntAttribute("rangeShoot");

		if(entityInfo->hasAttribute("bulletDmgShoot"))
			_bulletDmg = entityInfo->getIntAttribute("bulletDmgShoot");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAShootRaycast::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxDelayShoot"))
			_maxDelayShoot = entityInfo->getIntAttribute("maxDelayShoot");

		if(entityInfo->hasAttribute("typeBullet"))
			_typeBullet = entityInfo->getStringAttribute("typeBullet");

		if(entityInfo->hasAttribute("rangeShoot"))
			_rangeShoot = entityInfo->getIntAttribute("rangeShoot");

		if(entityInfo->hasAttribute("bulletDmgShoot"))
			_bulletDmg = entityInfo->getIntAttribute("bulletDmgShoot");

		return true;
	}

	//---------------------------------------------------------

	bool CIAShootRaycast::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CHANGE_TARGET" ||
			   message->getType() == "CAN_IA";
	} // accept

	//---------------------------------------------------------

	void CIAShootRaycast::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CHANGE_TARGET")
		{
			CEntity* ent = dynamic_cast<CHANGE_TARGET*>(message.get())->getLogicEntity();
			if(ent != NULL)
			{
				if(!ent->getIsDead())
				{
					_target = ent; 
			
					_targetID = _target->getEntityID();
				}
			}
			
		}

		else if (message->getType() == "CAN_IA")
		{
			_canIA = dynamic_cast<CAN_IA*>(message.get())->getBool();
		}

	} // process

	//---------------------------------------------------------

	CIAShootRaycast::~CIAShootRaycast()
	{
		
	} // ~Controller

	bool CIAShootRaycast::activate()
	{
		_delayShoot = 0;
		_canIA = true;
		_target = CServer::getSingletonPtr()->getPlayer();
		_targetID = _target->getEntityID();
		
		return true;

	} // activate

	void CIAShootRaycast::deactivate()
	{
	} // deactivate

	void CIAShootRaycast::onTick(unsigned int msecs)
	{
		if (_canIA)
		{
			_delayShoot += msecs;

			//BaseSubsystems::Log::Debug("Delay:" + std::to_string(_delayShoot));

			if (_delayShoot > _maxDelayShoot)
			{
				Vector3 posTarget = _target->getCenterPosition();

				Vector3 dir = (posTarget - _entity->getCenterPosition()) * Vector3(1,1,0);
				float distance = dir.squaredLength();

				if (distance <= (_rangeShoot*_rangeShoot))
				{	
					_delayShoot = 0;

					Vector3 offsetPosEnem = _entity->getCenterPosition();

					Vector3 dir = posTarget - offsetPosEnem;

					unsigned int groupsExcluded = (Physics::FilterMask::eEnemyTMask |Physics::FilterMask::eEnemyVMask  | Physics::ePlanoColisionMask);
					Physics::FilterMask mask = Physics::FilterMask(~groupsExcluded);

					Logic::CEntity* ent = Physics::CServer::getSingletonPtr()->raycastSimple(offsetPosEnem,dir,dir.length(), mask);

					if (ent)
					{
						if (ent->getEntityID() == _targetID)
						{
							//BaseSubsystems::Log::Debug("--------Entidad:" + ent->getType());
							Logic::CEntity* bullet = Logic::CEntityFactory::getSingletonPtr()->createEntityByType(_typeBullet, _entity->getPosition(), _entity->getMap());

							CBulletController* bController = bullet->getComponent<CBulletController*>();
							bController->setBulletConfiguration(posTarget,_bulletDmg);


						}
					}
				}
			}
		}

		else
			_delayShoot = 0;

	} // tick

} // namespace Logic

