/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/


#include "BarraLifeController.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Server.h"
#include "Graphics/BillBoards.h"
#include "Graphics/Scene.h"
#include "Logic/Entity/Components/Life.h"

#include "LUA/ScriptManager.h"

namespace Logic 
{
	IMP_FACTORY(CBarraLifeController);
	
	//---------------------------------------------------------

	CBarraLifeController::~CBarraLifeController()
	{
		if(_lifeBar)
		{
			delete _lifeBar;
			_lifeBar = 0;
		}
	}

	bool CBarraLifeController::OnSpawn(const Map::CEntity *entityInfo) 
	{
		//Comprobamos primero que tiene todos los atributos obligatorios para que se spawnee este componente con exito
		bool ok =	entityInfo->hasAttribute("materialLifeBar") && 
					entityInfo->hasAttribute("dimensionLifeBar") && 
					entityInfo->hasAttribute("componente_vida");

		assert(ok && "Error en el OnSpawn del componente BarraLifeController");

		_materialName = entityInfo->getStringAttribute("materialLifeBar");

		_dimensions = entityInfo->getVector3Attribute("dimensionLifeBar");

		nameComponent = entityInfo->getStringAttribute("componente_vida");

		_lifeBar = new Graphics::CBillBoards("lifeBar", _entity->getName(), _materialName, _dimensions, Vector3(0,25,0));

		_minDistance = 0.0f;
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("GetMinDistanceView", _minDistance);

		SetRangoVision();

		_timeStamp = 333;

		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CBarraLifeController::respawn(const Map::CEntity *entityInfo)
	{

		_minDistance = 0.0f;
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("GetMinDistanceView", _minDistance);

		SetRangoVision();
		_timeStamp = 333;
		return true;
	}

	bool CBarraLifeController::activate()
	{
		_player = CServer::getSingletonPtr()->getPlayer();

		_entity->getMap()->getScene()->addBillBoards(_lifeBar);
		_lifeBar->loadLifeBar();

		Logic::CLife* life = (Logic::CLife*)_entity->getComponent(nameComponent);
 		life->setLifeBar(this);
		life->ChangeLifeBar();

		(_player->getCenterPosition().squaredDistance(_entity->getCenterPosition()) > _maxDistance*_maxDistance)
		?
			_lifeBar->setVisible(_visible = false) //Con esto ademas pasamos el valor a la variable como false
		:
			_lifeBar->setVisible(_visible = true);	

		return true;
	}

	void CBarraLifeController::deactivate()
	{
		_entity->getMap()->getScene()->removeBillBoards(_lifeBar);
	}

	void CBarraLifeController::updateLifeBar(int life , int lifeMax)
	{
		if(_lifeBar)
			_lifeBar->updateLifeBar(life, lifeMax);
	}

	void CBarraLifeController::onTick(unsigned int msecs)
	{
		_timeStamp +=msecs;
		if(_timeStamp >= 100) //10 veces por segundo
		{
			SetRangoVision();

			_timeStamp = 0;

			if(!_visible && _player->getCenterPosition().squaredDistance(_entity->getCenterPosition()) <= _maxDistance*_maxDistance)
			{
				_lifeBar->setVisible(_visible = true);	
			}
			else if(_visible && _player->getCenterPosition().squaredDistance(_entity->getCenterPosition()) > _maxDistance*_maxDistance)
			{
				_lifeBar->setVisible(_visible = false);		
			}
		}
	}

	void CBarraLifeController::SetRangoVision()
	{
		//La máxima distancia será el contador de la habilidad CVerVidaEnemigos * el rango de vision
		//cogemos el dato del rango de vision
		float delta = 0.0f;
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("GetDistanceView", delta);

		//cogemos el contador de la habilidad
		int contador;
		std::string nombre = LUA::CScriptManager::GetPtrSingleton()->getField("Nombre_Habilidad", "CVerVidaEnemigos", "");
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("HadHability", nombre, contador);

		_maxDistance = 0.0f;
		//hacemos el producto
		if(contador > 0)
		{
			_maxDistance = _minDistance + delta * contador;
		}
	}

} // namespace Logic

