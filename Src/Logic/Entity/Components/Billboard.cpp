/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/


#include "Billboard.h"

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
	IMP_FACTORY(CBillBoard);
	
	//---------------------------------------------------------

	CBillBoard::~CBillBoard()
	{
		if(_billboard)
		{
			delete _billboard;
			_billboard = 0;
		}
	}

	bool CBillBoard::OnSpawn(const Map::CEntity *entityInfo) 
	{
		//Comprobamos primero que tiene todos los atributos obligatorios para que se spawnee este componente con exito
		bool ok =	entityInfo->hasAttribute("materialBillboard") &&
					entityInfo->hasAttribute("dimensionBillboard") &&
					entityInfo->hasAttribute("offsetBillboard");

		assert(ok && "Error en el OnSpawn del componente Billboard");
		
		_materialName = entityInfo->getStringAttribute("materialBillboard");

		_offset = entityInfo->getVector3Attribute("offsetBillboard");

		_dimensions = entityInfo->getVector3Attribute("dimensionBillboard");

		if(entityInfo->hasAttribute("lifeBar"))
			_lifeBar = entityInfo->getBoolAttribute("lifeBar");

		_billboard = new Graphics::CBillBoards(_entity->getType(), "_billboard"+_entity->getName(), _materialName, _dimensions, _entity->getPosition()  + _offset);
		
		return true;

	} // Onspawn
	
	//--------------------------------------------------------

    bool CBillBoard::respawn(const Map::CEntity *entityInfo)
	{
		assert(entityInfo->hasAttribute("materialBillboard") && "Fallo respawn componente Billboard");
		_materialName = entityInfo->getStringAttribute("materialBillboard");
		
		_lifeBar = false;
		if(entityInfo->hasAttribute("lifeBar"))
			_lifeBar = entityInfo->getBoolAttribute("lifeBar");

		_billboard->setMaterialName(_materialName);
		_billboard->setPosition(_entity->getPosition() + _offset);

		return true;
	}

	bool CBillBoard::activate()
	{

		_entity->getMap()->getScene()->addBillBoards(_billboard);
		
		if(_lifeBar)
		{
			_billboard->loadLifeBar();
		}

		return true;
	}

	void CBillBoard::deactivate()
	{
		_entity->getMap()->getScene()->removeBillBoards(_billboard);
	}

	void CBillBoard::updateLife(int life, int lifeMax)
	{
		_billboard->updateLifeBar(life, lifeMax);
	}

	void CBillBoard::loadLife()
	{
		_billboard->loadLifeBar();
	}

	void CBillBoard::setVisible(bool visible)
	{
		_billboard->setVisible(visible);
	}

} // namespace Logic

