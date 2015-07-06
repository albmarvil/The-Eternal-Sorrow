/**
@file SoulMenuGenerator.cpp

@author Alberto Martínez
@date Mayo, 2015
*/

#include "SoulMenuGenerator.h"
#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Logic/Maps/EntityFactory.h"

#define _USE_MATH_DEFINES
#include <math.h>


using namespace Logic;

IMP_FACTORY(CSoulMenuGenerator);

CSoulMenuGenerator::~CSoulMenuGenerator() 
{

}

bool CSoulMenuGenerator::OnSpawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("generator_time"))
	{
		_generatorTime = entityInfo->getFloatAttribute("generator_time") * 1000;
	}

	if(entityInfo->hasAttribute("generator_offset"))
	{
		_offset = entityInfo->getVector3Attribute("generator_offset");
	}

	return true;
} 

//---------------------------------------------------------

bool CSoulMenuGenerator::respawn(const Map::CEntity *entityInfo)
{
	if(entityInfo->hasAttribute("generator_time"))
	{
		_generatorTime = entityInfo->getFloatAttribute("generator_time") * 1000;
	}

	if(entityInfo->hasAttribute("generator_offset"))
	{
		_offset = entityInfo->getVector3Attribute("generator_offset");
	}
	return true;
}

//---------------------------------------------------------

bool CSoulMenuGenerator::activate()
{
	_timeAcum = _generatorTime + 1;

	return true;
	
}

//---------------------------------------------------------


//---------------------------------------------------------

void CSoulMenuGenerator::onTick(unsigned int msecs) 
{
	_timeAcum += msecs;

	if(_timeAcum >= _generatorTime)
	{
		_timeAcum = 0;

		//invocamos un alma

		int rnd = rand() % (int)_offset.x;

		int rnd2 = rand() % 2;

		Vector3 pos =_entity->getPosition() +  Vector3(rnd2? rnd : -rnd, 0, -20); 

		CEntityFactory::getSingletonPtr()->createEntityByType("SoulMenu", pos, _entity->getMap());

	}
}

//---------------------------------------------------------






