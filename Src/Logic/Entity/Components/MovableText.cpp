/**
@author Cesar Manuel Paz Guzman
@date Abril, 2015
*/

#include "MovableText.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "Graphics/Scene.h"
#include "Graphics/MovableTextGraphics.h"
namespace Logic 
{
	IMP_FACTORY(CMovableText2);
	
	//---------------------------------------------------------

	CMovableText2::~CMovableText2()
	{
		if(_text)
		{
			delete _text;
			_text = 0;
		}
	}

	bool CMovableText2::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_text = new Graphics::CMovableText(_entity->getName(), "-");

		//Hago esto en lugar de añadir un componente timeToLive, porque si quiero saber el 90 % del tiempo antes de que desaparezca
		//debo de saber el tiempo aqui

		_timeStamp = _timeToLive = 0;

		bool ok =	entityInfo->hasAttribute("time_to_live") &&
					entityInfo->hasAttribute("speed") &&
					entityInfo->hasAttribute("scaleMovableText");

		assert(ok && "Error en el onspawn del componente movabletext");

		_timeToLive = entityInfo->getFloatAttribute("time_to_live") * 1000;

		_speed = entityInfo->getFloatAttribute("speed");
		
		_scaleMovableText = entityInfo->getVector3Attribute("scaleMovableText");		

		_timeToDegradade = (50 * _timeToLive) * 0.01; //Tiempo en el cual comienza a degradarse

		_transparente = false;

		return true;
	} // spawn

	bool CMovableText2::respawn(const Map::CEntity *entityInfo)
	{
		//Hago esto en lugar de añadir un componente timeToLive, porque si quiero saber el 90 % del tiempo antes de que desaparezca
		//debo de saber el tiempo aqui
		_timeStamp = _timeToLive = 0;
		
		assert(entityInfo->hasAttribute("time_to_live") && "Error en el respawn del componente movabletext");
		_timeToLive = entityInfo->getFloatAttribute("time_to_live") * 1000;

		_timeToDegradade = (50 * _timeToLive) * 0.01;

		_transparente = false;
		return true;
	}
	//------------------------------------------------------------------------
	void CMovableText2::createMovableText()
	{
		_entity->getMap()->getScene()->addMovableText(_text);
		_text->setPosition(_entity->getPosition());
		this->setScale(_scaleMovableText);
	}

	bool CMovableText2::activate()
	{
		createMovableText();
		return true;
	}

	void CMovableText2::deactivate()
	{
		_entity->getMap()->getScene()->removeMovableText(_text);
	}

	void CMovableText2::onTick(unsigned int msecs)
	{
		_timeStamp += msecs;

		_entity->setPosition(Vector3(_entity->getPosition().x, _entity->getPosition().y + msecs*_speed, _entity->getPosition().z));

		_text->setPosition(_entity->getPosition());

		if (_timeStamp < _timeToLive && _timeStamp >= _timeToDegradade)
		{
			_transparente = _text->setAlpha(0.01);
		}
		else if(_timeStamp >= _timeToLive || _transparente)
		{
			
			_timeStamp = 0;
			Logic::CEntityFactory::getSingletonPtr()->deferredDeleteEntity(_entity);
		}

		//BaseSubsystems::Log::Debug("POSITION: "+std::to_string(_entity->getPosition().x)+","+std::to_string(_entity->getPosition().y));
	}

	void CMovableText2::setText(const std::string &textoMostrar)
	{
		_text->setText(textoMostrar);
	}

	void CMovableText2::setColor(const std::string &color)
	{
		_text->setColor(color);
	}

	void CMovableText2::setTimeToLive(float _time)
	{
		_timeToLive = _time * 1000;
		_timeToDegradade = (50 * _timeToLive) * 0.01;
	}

	void CMovableText2::setScale(const Vector3 &scala)
	{
		_text->setScale(scala);
	}


} // namespace Logi