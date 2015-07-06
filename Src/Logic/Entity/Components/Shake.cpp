#include "Shake.h"

#include "Logic/Entity/Entity.h"

namespace Logic 
{
	IMP_FACTORY(CShake);
	
	//---------------------------------------------------------

	CShake::~CShake()
	{
	}

	bool CShake::OnSpawn(const Map::CEntity *entityInfo) 
	{
		return true;

	} // Onspawn
	
	//--------------------------------------------------------

	bool CShake::activate()
	{

		return true;
	}

	void CShake::deactivate()
	{
	}

	void CShake::onTick(unsigned int msecs)
	{
		//std::cout << "PosCam:" << _entity->getPosition() << std::endl;

		if (_activeShake)
		{
			shake();

			std::shared_ptr<STOP_TARGETING_CAM> m(new STOP_TARGETING_CAM());
			m->setVector3(_offsetShake);
			m->setBool(_canTarget);
			_entity->emitMessage(m);
		}

	}
	//---------------------------------------------------------

	bool CShake::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "SHAKE";
	} // accept

	//---------------------------------------------------------

	void CShake::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SHAKE") == 0)
		{
			float max = dynamic_cast<SHAKE*>(message.get())->getMaxShake();
			float min = dynamic_cast<SHAKE*>(message.get())->getMinShake();

			newShake(max, min);
		}
	} // process

	void CShake::newShake(float max, float min)
	{
		_maxShake = max;
		_minShake = min;

		_activeShake = true;
		_shakeStarted = false;
	}

	void CShake::shake()
	{
		if (_shakeStarted == false)
		{
			_canTarget = false;
			_offsetShake = 0;
			_shakeRadius = _maxShake;
			_randAngle = rand() % 360;
			
			_shakeStarted = true;
		}

		else
		{
			_shakeRadius *= 0.9; //reducimos el radio cada tick
			_randAngle += (150 + rand() % 60); //cogemos un nuevo ángulo // randomAngle +=(180 +\- rand()%60) 
		}

		_offsetShake.x = sin(_randAngle) * _shakeRadius;
		_offsetShake.y = cos(_randAngle) * _shakeRadius;
		_offsetShake.z = 0;

		Vector3 pos = _entity->getPosition();

		//_entity->setPosition(pos + _offsetShake);

		if (_shakeRadius < _minShake)
		{
			_shakeStarted = false;
			_activeShake = false;
			_offsetShake = Vector3::ZERO;
			_canTarget = true;
		}
	}
} // namespace Logic

