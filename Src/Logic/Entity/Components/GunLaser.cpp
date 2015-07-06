
#include "GunLaser.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Physics/Server.h"

#include "Logic/Maps/EntityFactory.h"
#include "LaserBeamController.h"
#include "LaserBallController.h"

namespace Logic 
{
	IMP_FACTORY(CGunLaser);
	

	CGunLaser::CGunLaser() : Weapon(), _specialLaserBeam(0) 
	{

	}
	//---------------------------------------------------------------------------------

	CGunLaser::~CGunLaser()
	{

	}


	bool CGunLaser::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(!Weapon::OnSpawn(entityInfo))
			return false;
		if(entityInfo->hasAttribute("numHitsSpecialAttack"))
		{
			_timeSpecialBetweenHits = (1000 / entityInfo->getIntAttribute("numHitsSpecialAttack"));
		}
		return true;
	}

	bool CGunLaser::respawn(const Map::CEntity *entityInfo)
	{
		if(!Weapon::OnSpawn(entityInfo))
			return false;
		if(entityInfo->hasAttribute("numHitsSpecialAttack"))
		{
			_timeSpecialBetweenHits = (1000 / entityInfo->getIntAttribute("numHitsSpecialAttack"));
		}
		return true;
	}
//--------------------------------------------------------

	void CGunLaser::onTick(unsigned int msecs)
	{
		if(_specialStillOn)
		{
			_timeAcumBetweenAttacks += msecs;
		}

		Weapon::onTick(msecs);
	}

	///---------------------------------------------------

	void CGunLaser::mainAttack(Vector3 mousePos, float baseDamage)
	{
		_delayMainAtk = 0;

		///Lanzamos el sonido de disparo
		std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
		std::string s = "/Varios/LaserBeam";
		sound->setSound(s);
		_entity->emitMessage(sound, this);

		Vector3 orig = _entity->getPosition();
		orig.z = 0;
		Vector3 dir = mousePos - orig;

		// Comprobamos si lo vemos, lanzando un rayo
		Physics::FilterMask mask = Physics::FilterMask(Physics::FilterMask::eTilesMask | Physics::FilterMask::eDoorMask | Physics::FilterMask::eEnemyTMask | Physics::FilterMask::eEnemyVMask | Physics::FilterMask::eLaserBallMask | Physics::FilterMask::ePropsMask);

		Vector3 collisPoint = Vector3::ZERO;

		
		Logic::CEntity* ent = Physics::CServer::getSingletonPtr()->raycastSimple(orig, dir, 9999, mask, collisPoint);
		
		///movemos el nodo del trail hasta la posición de impacto
		if(collisPoint != Vector3::ZERO)
		{

			///Instanciamos la entidad de tipo LaserBeam
			CEntity *laserBeam = CEntityFactory::getSingletonPtr()->createEntityByType("LaserBeam", _entity->getPosition(), _entity->getMap());
			//configuramos el rayo
			CLaserBeamController* comp = (CLaserBeamController*) laserBeam->getComponent("CLaserBeamController");
			assert(comp);
			comp->setLaserConfig(_entity->getPosition(), collisPoint, _maxDelayMainAtk, 0.5f);
		}

		

		if (ent)
		{
			float finalDamage = baseDamage * _mainDamageMultiplier;

			std::shared_ptr<Logic::DAMAGED> m1(new Logic::DAMAGED());
			m1->setFloat(finalDamage);
			m1->setString(_entity->getType());
			ent->emitMessage(m1, this);

			int random = rand() % 101;

			if (random >= 85)
			{
				std::shared_ptr<Logic::SET_ALTSTATE> m2(new Logic::SET_ALTSTATE());
				m2->setString("Frozen");
				ent->emitMessage(m2, this);
			}

			if(ent->getTag() == "enemy")
			{
				Logic::CEntityFactory::getSingletonPtr()->createEntityByType("ChispaDanhoEnemy", collisPoint, _entity->getMap());
			}
			
			else
			{
				Logic::CEntityFactory::getSingletonPtr()->createEntityByType("Chispa", collisPoint, _entity->getMap());

				if (ent->getTag() == "laserBall")
				{
					std::shared_ptr<Logic::LASER_TO_LASERBALL> m2(new Logic::LASER_TO_LASERBALL());
					ent->emitMessage(m2);
				}
			}
		}
		else
		{
			// Partícula
			Logic::CEntityFactory::getSingletonPtr()->createEntityByType("Chispa", collisPoint, _entity->getMap());
		}
		
	}
	
	void CGunLaser::specialAttack(Vector3 mousePos, float baseDamage)
	{
		_delaySpecialAtk = 0;

		Logic::CEntity* laserBall = Logic::CEntityFactory::getSingletonPtr()->createEntityByType("LaserBall", _entity->getPosition(), _entity->getMap());
	
		CLaserBallController* laserBallComp = (CLaserBallController*)laserBall->getComponent("CLaserBallController");
		
		laserBallComp->setDirection(mousePos);
	}
	//------------------------------------------------------------------------

} // namespace Logic