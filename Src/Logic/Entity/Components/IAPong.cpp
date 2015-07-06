#include "IAPong.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "AI/Movement.h"

namespace Logic 
{
	IMP_FACTORY(CIAPong);
	 
	bool CIAPong::OnSpawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxVelPong"))
			_maxVelPongOri = _maxVelPongX = _maxVelPongY = entityInfo->getFloatAttribute("maxVelPong");

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CIAPong::respawn(const Map::CEntity *entityInfo)
	{
		if(entityInfo->hasAttribute("maxVelPong"))
			_maxVelPongOri =_maxVelPongX = _maxVelPongY = entityInfo->getFloatAttribute("maxVelPong");

		return true;
	}

	//---------------------------------------------------------

	bool CIAPong::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		
		return message->getType() == "FALLING" ||
			   message->getType() == "UP_COLLISION" ||
			   message->getType() == "SIDE_COLLISION";
	} // accept

	//---------------------------------------------------------

	void CIAPong::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		// Solo realizará el cambio de dirección si el anterior cambio que hizo no fue el
		// mismo que va a realizar (de modo que no esté entrando constántemente al mismo y "tiemble)
		if (message->getType() == "FALLING" && _lastCollision != "DOWN")
		{
			bool isFalling = dynamic_cast<FALLING*>(message.get())->getBool();

			if (!isFalling)
			{
				/*int random = rand() % (int(_maxVelPongOri * 100));

				_maxVelPongX = random / 100.f;
				std::cout << "Suelo, maxvelX: " << _maxVelPongX << std::endl;*/

				_direction.y *= -1;
				_lastCollision = "DOWN";
			}
		}

		else if (message->getType() == "UP_COLLISION" && _lastCollision != "UP")
		{
			/*int random = rand() % (int(_maxVelPongOri * 100));

			_maxVelPongX = random / 100.f;
			std::cout << "Techo, maxvelX: " << _maxVelPongX << std::endl;*/

			_direction.y *= -1;
			_lastCollision = "UP";
		}
		else if (message->getType() == "SIDE_COLLISION")
		{
			std::string thisCollision;

			int normal = dynamic_cast<SIDE_COLLISION*>(message.get())->getNormalCollision();

			if (normal == 1)
				thisCollision = "RIGHT";
			else if (normal == -1)
				thisCollision = "LEFT";

			if (thisCollision != _lastCollision)
			{
				/*int random = rand() % (int(_maxVelPongOri * 100));

				_maxVelPongY = random / 100.f;
				std::cout << "Lateral, maxvelY: " << _maxVelPongY << std::endl;*/

				_lastCollision = thisCollision;
				_direction.x *= -1;
			}
		}

	} // process

	//---------------------------------------------------------

	CIAPong::~CIAPong()
	{
		
	} // ~Controller

	bool CIAPong::activate()
	{
		_velMovement = 0;
		_direction = 1;
		_lastCollision = "";

	 
		return true;

	} // activate

	void CIAPong::deactivate()
	{
	} // deactivate

	void CIAPong::onTick(unsigned int msecs)
	{
		_velMovement = _direction * Vector3(_maxVelPongX, _maxVelPongY, 0) * msecs;

		std::shared_ptr<AVATAR_WALK> m(new Logic::AVATAR_WALK());
		m->setVector3(_velMovement);
		_entity->emitMessage(m);

	} // tick

} // namespace Logic

