/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "IAAvoid.h"
#include "Logic/Server.h"
#include "Logic/Entity/Entity.h"
#include "Physics/Server.h"
#include "Logic/Maps/Map.h"
#include "Logic/Server.h"
#include "AIIntermediate.h"
#include "Map\MapEntity.h"

namespace Logic
{
	IMP_FACTORY(CIAAvoid);
	bool CIAAvoid::OnSpawn(const Map::CEntity* entityInfo)
	{
		_weight = 0;
		_directionX = 0.0f;
		_directionY = 0.0f;
		_weigthMax = 10;
		_puntoColisionEntidadFisica = Vector3::ZERO;
		_timeStamp = 500;
		

		return true;
	}
	

	bool CIAAvoid::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CAN_MOVE" || 
			   message->getType() == "CAN_IA" ||
			   message->getType() == "CHANGE_TARGET";

	} // accept

	//---------------------------------------------------------

	void CIAAvoid::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if (message->getType() == "CAN_MOVE")
		{
			_canMove = dynamic_cast<CAN_MOVE*>(message.get())->getBool();
		}
		else if(message->getType() == "CAN_IA")
		{
			_canMove = dynamic_cast<CAN_IA*>(message.get())->getBool();
		}

		else if (message->getType() == "CHANGE_TARGET")
		{
			CEntity* ent = dynamic_cast<CHANGE_TARGET*>(message.get())->getLogicEntity();
			if(ent != NULL)
			{
				if(!ent->getIsDead())
				{
					_target = ent; 
				}
			}  
		}
	}

	bool CIAAvoid::activate()
	{
		//Registro el movimiento en el intermediario
		_entity->getComponent<CAIIntermediate*>()->addNewMovement(SteeringMovement::Avoid,this);

		_target = CServer::getSingletonPtr()->getPlayer();

		_contador = 0;

		_rebotando = false;

		_canMove = true;

		return true;

	}

	void CIAAvoid::deactivate()
	{
		_directionX = 0.0f;
		_directionY = 0.0f;
		_weight = 0;
		_puntoColisionEntidadFisica = Vector3::ZERO;
	}

	void CIAAvoid::onTick(unsigned int msecs)
	{

	}

	void CIAAvoid::throwRay()
	{
		Vector3 targetPos = _target->getCenterPosition();
		Vector3 pos = _entity->getCenterPosition();

		_direccion = targetPos - pos;
		
		CMap *mapa = CServer::getSingletonPtr()->getMap();

		_puntoColisionEntidadFisica = Physics::CServer::getSingletonPtr()->raycastSimpleTile(pos, _direccion, 60.0f);

		(_puntoColisionEntidadFisica != Vector3::ZERO ) ? _weight = _weigthMax : _weight = 0;
	}

	Vector3 CIAAvoid::calculateVelocity(unsigned int msecs)
	{
		//Si estamos cerca de un tile
		_timeStamp += msecs;

		if (!_canMove)
			return Vector3::ZERO;

		if(_timeStamp < 333) //
		{
			if(_weight != 0)
			{
				if(_contador >= 3 && _rebotando) //
				{
					_rebotando = false;
					_directionX = 0;
					_contador = 0;
					_directionY = 0;
				}

				//Miramos si a la derecha en una distancia de medio tile hay algo, para poder ir a la otra direccion
				Vector3 punto = Physics::CServer::getSingletonPtr()->raycastSimpleTile(_entity->getCenterPosition(), Vector3(1,0,0), 10.0f);
				if(punto != Vector3::ZERO)
				{
					_rebotando = true;
					_directionX = -1;
					calculateDirectionY();
				}

				//Miramos si a la izquierda en una distancia de medio tile hay algo, para poder ir a la otra direccion
				punto = Physics::CServer::getSingletonPtr()->raycastSimpleTile(_entity->getCenterPosition(), Vector3(-1,0,0), 10.0f);
				if(punto != Vector3::ZERO)
				{
					_rebotando = true;
					_directionX = 1;
					calculateDirectionY();
				}
				
				//Ahora obtenemos la direccion en la que se mueve, y aplicamos movimiento con una aceleracion de 0.0003 
				calculateDirectionX();

				//Aceleracion Avoid: 0.0003
				return Vector3(_directionX,_directionY,0) *_weight *0.0003;

			}
			else
			{
				_contador = 0;
				_directionX = 0;
				_directionY = 0;
				_rebotando = false;
			}
		}
		else
		{
			if(_rebotando) //Cuando comience a rebotar, que durante tres ticks de rebote siga la misma direccion, para no cambiar tan bruscamente
				++_contador;

			_timeStamp = 0;
			throwRay(); //Lanzamos rayo para ver si entre player y enemigo hay algo de por medio y asi poder esquivarlo
		}

		return Vector3::ZERO;
	}

	void CIAAvoid::calculateDirectionY()
	{
		if(_puntoColisionEntidadFisica != Vector3::ZERO && _directionY == 0)
		{
			//Hacemos un random para la direccion de y
			_directionY = (rand() % 10>5) ? -0.5f:0.5f;
		}
	}

	void CIAAvoid::calculateDirectionX()
	{
		
		int limiteDerecho = 0, limiteIzquierdo = 0;

		calculateLimits(limiteIzquierdo, limiteDerecho);

		if(limiteIzquierdo == 0)
		{
			_directionX = 1; //Direccion hacia la derecha
			//BaseSubsystems::Log::Debug("Derecha Zona Izquierda");
		}
		else if(limiteDerecho == CServer::getSingletonPtr()->getMap()->getMapTileWidth()-1 )
		{
			//BaseSubsystems::Log::Debug("Izquierda Zona derecha");
			_directionX = -1; //Direccion hacia la izquierda
		}
		else if(_puntoColisionEntidadFisica != Vector3::ZERO && _directionX == 0)
		{
			_directionX = (_entity->getCenterPosition().x > _target->getCenterPosition().x) ? -1:1;
			//BaseSubsystems::Log::Debug("DirectionX: "+std::to_string(_directionX));
		}
	}

	void CIAAvoid::calculateLimits(int &limiteIzquierdo, int &limiteDerecho)
	{
		//Obtenemos la tile posicion en la que se encuentra el player. 
		int tilePos = _entity->getCenterPosition().x * 0.05; //*0.05 == / 20

		//Obtenemos la tile posicion correspondiente al punto de impacto del rayo
		int posYTile = _direccion.y<0 //Si la direccion en y es hacia abajo
						? 
							_puntoColisionEntidadFisica.y * 0.05 -1 //entonces la posicion en tile posicion es la de mas abajo que la de la plataforma donde choco
						:   
							_puntoColisionEntidadFisica.y * 0.05; //Si no es la de la plataforma 
							
		int posXTile = _puntoColisionEntidadFisica.x * 0.05 +1;

		CMap *mapa = CServer::getSingletonPtr()->getMap();

		/////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////OBTENEMOS LOS LIMITES DERECHO E IZQUIERDO...YA QUE PODREMOS DETERMINAR DE SI SE TRATA DE UNA PLATAFORMA
		//////QUE NO TIENE SALIDA POR UN EXTREMO, Y ELEGIR PR EL CONTRARIO.

		int i= 0;
		//Recorremos hacia la izquierda
		for(i=1; posXTile-i>=0 && mapa->getTileColisionTP(posXTile-i, -posYTile) != 0; ++i){}

		//Obtenemos el limite izquierdo de la plataforma, restando la tile posicionActual con el numero de tiles que hay a la izquierda
		limiteIzquierdo = posXTile - (i-1);

		//Recorremos hacia la derecha y obtenemos la i final que es el numero de tiles que forma la plataforma hacia la derecha
		for(i=1;posXTile+i<mapa->getMapTileWidth() && mapa->getTileColisionTP(posXTile+i, -posYTile) != 0; ++i){}

		limiteDerecho = posXTile + (i-1);
	}
}