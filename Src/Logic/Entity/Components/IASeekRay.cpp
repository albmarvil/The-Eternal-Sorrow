#include "IASeekRay.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/EntityFactory.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"
#include "LUA/ScriptManager.h"
#include "Physics/Server.h"

namespace Logic 
{
	IMP_FACTORY(CIASeekRay);
	 
	bool CIASeekRay::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("velRotSeekRay"))
		{
			_velRotation = entityInfo->getFloatAttribute("velRotSeekRay");
		}

		if(entityInfo->hasAttribute("maxDistSeekRay"))
		{
			_maxDistRay = entityInfo->getIntAttribute("maxDistSeekRay");
		}

		if(entityInfo->hasAttribute("maxDelayTurnSeekRay"))
		{
			_maxDelayTurn = entityInfo->getIntAttribute("maxDelayTurnSeekRay");
		}

		if(entityInfo->hasAttribute("minAngleSeekRay"))
		{
			_minAngle = entityInfo->getIntAttribute("minAngleSeekRay");
		}

		if(entityInfo->hasAttribute("maxAngleSeekRay"))
		{
			_maxAngle = entityInfo->getIntAttribute("maxAngleSeekRay");
		}

		if(entityInfo->hasAttribute("initialDirSeekRay"))
		{
			_initialDirRay = entityInfo->getIntAttribute("initialDirSeekRay");
		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIASeekRay::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("velRotSeekRay"))
		{
			_velRotation = entityInfo->getFloatAttribute("velRotSeekRay");
		}

		if(entityInfo->hasAttribute("maxDistSeekRay"))
		{
			_maxDistRay = entityInfo->getIntAttribute("maxDistSeekRay");
		}

		if(entityInfo->hasAttribute("maxDelayTurnSeekRay"))
		{
			_maxDelayTurn = entityInfo->getIntAttribute("maxDelayTurnSeekRay");
		}

		if(entityInfo->hasAttribute("minAngleSeekRay"))
		{
			_minAngle = entityInfo->getIntAttribute("minAngleSeekRay");
		}

		if(entityInfo->hasAttribute("maxAngleSeekRay"))
		{
			_maxAngle = entityInfo->getIntAttribute("maxAngleSeekRay");
		}

		if(entityInfo->hasAttribute("initialDirSeekRay"))
		{
			_initialDirRay = entityInfo->getIntAttribute("initialDirSeekRay");
		}

		return true;
	}

	//---------------------------------------------------------

	bool CIASeekRay::accept(const std::shared_ptr<Logic::IMessage> &message)
	{ 
		return true;
	} // accept

	//---------------------------------------------------------

	void CIASeekRay::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CHANGE_TARGET")
		{
			if (message->getType() == "CHANGE_TARGET")
		{
			CEntity* ent = dynamic_cast<CHANGE_TARGET*>(message.get())->getLogicEntity();
			if(ent != NULL)
			{
				if(!ent->getIsDead())
				{
					_targetID = ent->getEntityID();
				}
			}
			
		}  
		}
	} // process

	//---------------------------------------------------------

	CIASeekRay::~CIASeekRay()
	{
		
	} // ~Controller

	bool CIASeekRay::activate()
	{
		_actualAngle = _minAngle;
		_targetFound = false;
		_targetID = CServer::getSingletonPtr()->getPlayer()->getEntityID();
		_waitDelay = false;
		_delayTurn = 0;
		_dirRot = _initialDirRay;

		return true;

	} // activate

	bool CIASeekRay::onAwake()
	{
		_actualAngle = _minAngle;
		_targetFound = false;
		_targetID = CServer::getSingletonPtr()->getPlayer()->getEntityID();
		_delayTurn = 0;
		_waitDelay = false;
		_dirRot = _initialDirRay;

		return true;
	}

	void CIASeekRay::deactivate()
	{

	} // deactivate

	
	void CIASeekRay::onTick(unsigned int msecs)
	{
		if (!_targetFound)
		{
			// Sacamos el vector director unitario a raiz del ángulo
			Vector3 dir = Math::getDirectionXY(_actualAngle);

			// Comprobamos si lo vemos, lanzando un rayo
			unsigned int groupsExcluded = (Physics::FilterMask::eEnemyTMask | Physics::FilterMask::eEnemyVMask | Physics::ePlanoColisionMask);
			Physics::FilterMask mask = Physics::FilterMask(~groupsExcluded);

			Logic::CEntity* ent = Physics::CServer::getSingletonPtr()->raycastSimple(_entity->getCenterPosition(), dir, _maxDistRay, mask);

			//std::cout << "Angulo:" << _actualAngle << std::endl;
			
			if (ent)
			{
				if (ent->getEntityID() == _targetID)
				{
					//std::cout << "----> PLAYER ENCONTRADO <----" << std::endl;
					_targetFound = true;
				}
			}

			// Solo vamos rotando para los casos en los que el mínino no sea igual que el máximo
			if (_minAngle != _maxAngle)
			{
				if (_waitDelay)
				{
					_delayTurn += msecs;

					if (_delayTurn >= _maxDelayTurn)
					{
						//std::cout << "SEGUIMOS!" << std::endl;
						_waitDelay = false;
						_delayTurn = 0;
					}
				}

				else
				{
					// Aumentamos el ángulo
					_actualAngle += _velRotation * _dirRot;

					// Si nos pasamos de 0º o 360º (según la dirección), lo corregimos
					if (_actualAngle > 360)
						_actualAngle = 0;

					if (_actualAngle < 0)
						_actualAngle = 360;

					// Si ha superado el mínimo o el máximo establecido, damos la vuelta y esperamos un delay
					if (_actualAngle > _maxAngle )
					{
						_actualAngle = _maxAngle;
						//std::cout << "CAMBIO DIRECCION!" << std::endl;
						_dirRot *= -1;
						_waitDelay = true;
					}

					else if (_actualAngle < _minAngle)
					{
						_actualAngle = _minAngle;
						//std::cout << "CAMBIO DIRECCION!" << std::endl;
						_dirRot *= -1;
						_waitDelay = true;
					}
				}
			}
		}

		if (_targetFound)
		{
			// TO_DO: Dispararle de vez en cuando
		}
	
	} // tick

} // namespace Logic

