
/*
@file WeaponController.cpp

Contiene la implementacion de la clase CWeaponController

@author Alberto Martínez Villarán
@date Febrero 2015
@refactor Alberto Martínez Villarán
@date Abril 2015

*/

#include "WeaponController.h"

#include "PhysicWeapon.h"

#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Graphics/Scene.h"
#include "Graphics/SceneNode.h"

namespace Logic 
{
	IMP_FACTORY(CWeaponController);
	

	CWeaponController::~CWeaponController()
	{
		delete _node;
	}
	//---------------------------------------------------------

	bool CWeaponController::OnSpawn(const Map::CEntity *entityInfo) 
	{
		if(entityInfo->hasAttribute("attackType"))
		{
			_attackType = entityInfo->getStringAttribute("attackType");
		}

		_initOffset = Vector3::ZERO;
		if(entityInfo->hasAttribute("offset"))
		{
			_initOffset = entityInfo->getVector3Attribute("offset");
		}

		_node = new Graphics::CSceneNode(_entity->getName());

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CWeaponController::respawn(const Map::CEntity *entityInfo)
	{	
		if(entityInfo->hasAttribute("attackType"))
		{
			_attackType = entityInfo->getStringAttribute("attackType");
		}

		_initOffset = Vector3::ZERO;
		if(entityInfo->hasAttribute("offset"))
		{
			_initOffset = entityInfo->getVector3Attribute("offset");
		}
		return true;
	}

	bool CWeaponController::activate()
	{
		_componenteFisico =  _entity->getComponent<CPhysicWeapon*>();
		_pivot = _entity->getPosition() + _initOffset;
		_dirFromOrigin = _pivot - _entity->getPosition();
		_node->attachToScene(_entity->getMap()->getScene());
		return true;
	}

	void CWeaponController::deactivate()
	{
		_node->deattachToScene(_entity->getMap()->getScene());
	}

	//---------------------------------------------------------

	bool CWeaponController::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return false;//message->getType() == "SET_TRANSFORM_BONE";
	} // accept
	
	//---------------------------------------------------------

	void CWeaponController::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		/*if(message->getType().compare("SET_TRANSFORM_BONE") == 0)
		{
			_weaponTransform = dynamic_cast<SET_TRANSFORM_BONE*>(message.get())->getTransform();
		}*/
	} // process

	//---------------------------------------------------------

	//void CWeaponController::onTick(unsigned int msecs)
	//{
	//	
	//	Vector3 pos = _weaponTransform.getTrans();
	//	Quaternion q = _weaponTransform.extractQuaternion();

	//	Matrix4 playerTransform = CServer::getSingletonPtr()->getPlayer()->getTransform();
	//	//Vector3 playerScale = Vector3::ZERO;
	//	//playerTransform.getScale(playerScale);
	//	Quaternion playerOrientation = playerTransform.extractQuaternion();

	//	q = playerOrientation * q ;
	//	q.normalise();

	//	Vector3 newDir = q * _dirFromOrigin;

	//	_pivot = pos + newDir;

	//	Matrix3 ori;
	//	q.ToRotationMatrix(ori);

	//	_pivotTransform = ori;
	//	_pivotTransform.setTrans(_pivot);
	//	//_pivotTransform.setScale(playerScale);

	//	if(!_componenteFisico)//si no tenemos componente físico de arma, seteamos nostros la transformada de la entidad
	//	{
	//		_entity->setTransform(_pivotTransform);
	//	}
	//	//en caso contrario preparamos a la física para que lo haga ella

	//	///Al final de todo el proceso, seteo el nodo de escena asociado al pivote en su sitio
	//	_node->setPosition(_pivot);

	//}

	void CWeaponController::setTransformBone(const Matrix4 &transform)
	{
		_weaponTransform = transform;
		Vector3 pos = _weaponTransform.getTrans();
		Quaternion q = _weaponTransform.extractQuaternion();

		Matrix4 playerTransform = CServer::getSingletonPtr()->getPlayer()->getTransform();
		//Vector3 playerScale = Vector3::ZERO;
		//playerTransform.getScale(playerScale);
		Quaternion playerOrientation = playerTransform.extractQuaternion();

		q = playerOrientation * q ;
		q.normalise();

		Vector3 newDir = q * _dirFromOrigin;

		_pivot = pos + newDir;

		Matrix3 ori;
		q.ToRotationMatrix(ori);

		_pivotTransform = ori;
		_pivotTransform.setTrans(_pivot);
		//_pivotTransform.setScale(playerScale);

		//if(!_componenteFisico)//si no tenemos componente físico de arma, seteamos nostros la transformada de la entidad
		//{
		//	_entity->setTransform(_pivotTransform);
		//}
		_entity->setTransform(_pivotTransform);
		//en caso contrario preparamos a la física para que lo haga ella

		///Al final de todo el proceso, seteo el nodo de escena asociado al pivote en su sitio
		_node->setPosition(_pivot);
	}

} // namespace Logic

