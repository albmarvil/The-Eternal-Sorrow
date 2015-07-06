/**
@file Entity.cpp

Contiene la implementación de la clase Entity, que representa una entidad
de juego. Es una colección de componentes.

@see Logic::CEntity
@see Logic::IComponent

@author David Llansó
@date Julio, 2010
*/

#include "Entity.h"

// Componentes
#include "Component.h"

#include "Logic/Server.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/Graphics.h"

#include "GUI/Server.h"
#include "GUI/PlayerController.h"

namespace Logic 
{
	CEntity::CEntity(TEntityID entityID) : _entityID(entityID), 
				_map(0), _type(""), _name(""), _transform(Matrix4::IDENTITY),
				_isPlayer(false), _activated(false), _sleeping(false),  _inicialized(false), _tag(""), _transformObservers(0),
				_centerOffset(Vector3::ZERO)
	{

	} // CEntity
	
	//---------------------------------------------------------

	CEntity::~CEntity()
	{
		assert(!_map && "¡¡Antes de destruir la entidad debe desacoplarse del mapa!!");

		destroyAllComponents();

		if (isPlayer())
		{
			CServer::getSingletonPtr()->setPlayer(0);
		}

		_inicialized = false;

	} // ~CEntity
	
	//---------------------------------------------------------

	bool CEntity::spawn(CMap *map, /*const*/ Map::CEntity *entityInfo) 
	{
		_dead = false;

		//Si ya se habia inicializado
		if(_inicialized)
		{
			return respawn(map, entityInfo);
		}

		

		//Si no se ha inicializado
		// Leemos las propiedades comunes
		_map = map;
		_type = entityInfo->getType();
		_entityInfo = entityInfo;

		if(entityInfo->hasAttribute("name"))
			_name = entityInfo->getStringAttribute("name");
		
		_tag = "";
		if(entityInfo->hasAttribute("tag"))
			_tag = entityInfo->getStringAttribute("tag");

		if(entityInfo->hasAttribute("position"))
		{
			Vector3 position = entityInfo->getVector3Attribute("position");
			_transform.setTrans(position);
		}

		// Por comodidad en el mapa escribimos los ángulos en grados.
		if(entityInfo->hasAttribute("orientation"))
		{
			Vector3 orientation = entityInfo->getVector3Attribute("orientation");
			orientation.normalise();
			float angle = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("orientation_angle"));
			Math::rotate(orientation, angle, _transform);
		}

		if(entityInfo->hasAttribute("entity_offset"))
		{
			_centerOffset = entityInfo->getVector3Attribute("entity_offset");
		}

		if(entityInfo->hasAttribute("isPlayer"))
		{
			_isPlayer = entityInfo->getBoolAttribute("isPlayer");
			if (isPlayer())
			{
				CServer::getSingletonPtr()->setPlayer(this);
			}
		}
		

		// Inicializamos los componentes
		TComponentVector::const_iterator it;

		_inicialized = true;

	
		for( it = _componentVector.begin(); it != _componentVector.end() && _inicialized; ++it )
		{	
			_inicialized = (*it)->spawn(this,entityInfo) && _inicialized;
		}
		return _inicialized;


	} // spawn

	//---------------------------------------------------------

	bool CEntity::respawn(CMap *map, Map::CEntity *entityInfo)
	{
		//Seteamos la informacion de la entidad
		_entityInfo = entityInfo;

		//Seteamos el mapa en el que se registrara
		_map = map;
		
		bool correcto = true;

		//Seteamos la posicion de la entidad si esta tiene, ya que hay entidades ue no tienen posicion
		if(entityInfo->hasAttribute("position"))
		{
			Vector3 position = entityInfo->getVector3Attribute("position");
			_transform.setTrans(position);
		}

		_tag = "";
		if(entityInfo->hasAttribute("tag"))
			_tag = entityInfo->getStringAttribute("tag");

		if(entityInfo->hasAttribute("orientation"))
		{
			Vector3 orientation = entityInfo->getVector3Attribute("orientation");
			orientation.normalise();
			float angle = Math::fromDegreesToRadians(entityInfo->getFloatAttribute("orientation_angle"));
			//Math::rotate(orientation, angle, _transform);

			Quaternion q = Quaternion(Radian(angle), orientation);

			Matrix3 rotation;
			_transform.extract3x3Matrix(rotation);
			q.ToRotationMatrix(rotation);

			_transform = rotation;
		}

		//RESPAWN DE LOS COMPONENTES
		TComponentVector::const_iterator it;
		for( it = _componentVector.begin(); it != _componentVector.end() && correcto ; ++it )
		{
			correcto  = (*it)->respawn(entityInfo) && correcto ;
		}

		return correcto;
	}

	//---------------------------------------------------------

	bool CEntity::activate() 
	{
		
		if (isPlayer())
		{
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(this);
		}
		// Activamos los componentes
		TComponentVector::const_iterator it;

		// Solo si se activan todos los componentes correctamente nos
		// consideraremos activados.
		_activated = true;

		for( it = _componentVector.begin(); it != _componentVector.end(); ++it )
		{
			(*it)->_messages.clear();
			_activated = (*it)->activate() && _activated;
		}
		
		return _activated;

	} // activate



	//---------------------------------------------------------

	void CEntity::deactivate() 
	{
		if (isPlayer())
		{
			GUI::CServer::getSingletonPtr()->getPlayerController()->setControlledAvatar(0);
		}


		TComponentVector::const_iterator it;

		setPosition(Vector3(0,0,-200));

		// Desactivamos los componentes
		for( it = _componentVector.begin(); it != _componentVector.end(); ++it )
		{
			//Eliminamos primero su cola de mensajes, dado que los mensajes son punteros inteligentes, no hay que hacer delete de ellos
			(*it)->_messages.clear();
			(*it)->deactivate();
		}

		removeAllEntityTransfomrListeners();

		_activated = false;

		
	} // deactivate

	//---------------------------------------------------------
	
	void CEntity::tick(unsigned int msecs) 
	{	
		TComponentVector::const_iterator it;
		for( it = _componentVector.begin(); it != _componentVector.end(); ++it )
		{
			if ((*it)->getIsActive())
				(*it)->tick(msecs);
		}

	} // tick

	//---------------------------------------------------------

	void CEntity::addComponent(IComponent* component, bool dormido)
	{
		_components[component->getName()] = component;
		_componentVector.push_back(component);
		
		component->setEntity(this);
		component->setSleeping(dormido);

	} // addComponent

	//---------------------------------------------------------

	IComponent* CEntity::getComponent(std::string name)
	{
		TComponentMap::iterator find = _components.find(name);

		if(find != _components.end())//si lo encontramos devolvemos el componente
		{
			return find->second;
		}
		else
		{
			return NULL;
		}
	}

	bool CEntity::removeComponent(IComponent* component)
	{
		TComponentMap::const_iterator it = _components.begin();
		TComponentVector::const_iterator itv = _componentVector.begin();

		bool removed = false;
		// Buscamos el componente hasta el final, por si aparecía
		// más de una vez... (no tendría mucho sentido, pero por si
		// acaso).
		while (it != _components.end()) 
		{
			if ((*it).second == component)
			{
				it = _components.erase(it);
				removed = true;
			}
			else
				++it;
		}

		bool removedV = false;
		while( itv != _componentVector.end())
		{
			if((*itv) == component)
			{
				itv = _componentVector.erase(itv);
				removedV = true;
			}
			else
				++it;
		}

		if (removed && removedV)
			component->setEntity(0);
		return removed && removedV;

	} // removeComponent

	//---------------------------------------------------------

	void CEntity::destroyAllComponents()
	{
		TComponentVector::iterator it = _componentVector.begin();

		for(; it != _componentVector.end(); ++it)
		{
			delete (*it);
		}

		_componentVector.clear();
		_components.clear();

	} // destroyAllComponents

	//---------------------------------------------------------

	bool CEntity::emitMessage(const std::shared_ptr<Logic::IMessage> &message, IComponent* emitter)
	{
		// Interceptamos los mensajes que además de al resto de los
		// componentes, interesan a la propia entidad.
		/*if(message->getType().compare("SET_TRANSFORM") == 0)
		{
			_transform = dynamic_cast<SET_TRANSFORM*>(message.get())->getTransform();
		}*/

		TComponentMap::const_iterator it;
		// Para saber si alguien quiso el mensaje.
		bool anyReceiver = false;
		for( it = _components.begin(); it != _components.end(); ++it )
		{
			// Al emisor no se le envia el mensaje.
			if( emitter != (*it).second )
				anyReceiver = (*it).second->set(message) || anyReceiver;
		}
		return anyReceiver;

	} // emitMessage

	//---------------------------------------------------------

	void CEntity::setTransform(const Matrix4& transform) 
	{
		_transform = transform;

		// Avisamos a los componentes del cambio.
		/*std::shared_ptr<SET_TRANSFORM> m(new SET_TRANSFORM());
		m->setMatrix4(_transform);
		emitMessage(m);*/

		if(!_transformObservers.empty())
		{
			std::list<CEntityTransformListener*>::iterator it = _transformObservers.begin();

			for(; it != _transformObservers.end(); ++it)
			{
				(*it)->OnEntitySetTransform(_transform);
			}
		}

	} // setTransform

	//---------------------------------------------------------

	void CEntity::setPosition(const Vector3 &position, IComponent* invoker)  
	{
		_transform.setTrans(position);

		// Avisamos a los componentes del cambio.
		/*std::shared_ptr<SET_TRANSFORM> m(new SET_TRANSFORM());
		m->setMatrix4(_transform);
		emitMessage(m, invoker);*/

		if(!_transformObservers.empty())
		{
			std::list<CEntityTransformListener*>::iterator it = _transformObservers.begin();

			for(; it != _transformObservers.end(); ++it)
			{
				(*it)->OnEntitySetTransform(_transform);
			}
		}

	} // setPosition

	//---------------------------------------------------------

	void CEntity::setOrientation(const Matrix3& orientation) 
	{
		_transform = orientation;

		// Avisamos a los componentes del cambio.
		/*std::shared_ptr<SET_TRANSFORM> m(new SET_TRANSFORM());
		m->setMatrix4(_transform);
		emitMessage(m);*/

		if(!_transformObservers.empty())
		{
			std::list<CEntityTransformListener*>::iterator it = _transformObservers.begin();

			for(; it != _transformObservers.end(); ++it)
			{
				(*it)->OnEntitySetTransform(_transform);
			}
		}

	} // setOrientation
	
	//---------------------------------------------------------

	Matrix3 CEntity::getOrientation() const
	{
		Matrix3 orientation;
		_transform.extract3x3Matrix(orientation);
		return orientation;

	} // getOrientation

	//---------------------------------------------------------

	void CEntity::setYaw(float yaw) 
	{
		Math::setYaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		/*std::shared_ptr<SET_TRANSFORM> m(new SET_TRANSFORM());
		m->setMatrix4(_transform);
		emitMessage(m);*/

		if(!_transformObservers.empty())
		{
			std::list<CEntityTransformListener*>::iterator it = _transformObservers.begin();

			for(; it != _transformObservers.end(); ++it)
			{
				(*it)->OnEntitySetTransform(_transform);
			}
		}

	} // setYaw

	//---------------------------------------------------------

	void CEntity::yaw(float yaw) 
	{
		Math::yaw(yaw,_transform);

		// Avisamos a los componentes del cambio.
		/*std::shared_ptr<SET_TRANSFORM> m(new SET_TRANSFORM());
		m->setMatrix4(_transform);
		emitMessage(m);*/

		if(!_transformObservers.empty())
		{
			std::list<CEntityTransformListener*>::iterator it = _transformObservers.begin();

			for(; it != _transformObservers.end(); ++it)
			{
				(*it)->OnEntitySetTransform(_transform);
			}
		}

	} // yaw


	void CEntity::addEntityTransformListener(CEntityTransformListener* listener)
	{
		_transformObservers.push_back(listener);
	}//adEntityTransformListener

	//----------------------------------------------------------

	void CEntity::removeEntityTransformListener(CEntityTransformListener* listener)
	{
		_transformObservers.remove(listener);
	}//removeEntityTransformListener

	//--------------------------------------------------------------

	void CEntity::removeAllEntityTransfomrListeners()
	{
		_transformObservers.clear();
	}//removeAllEntityTransformListeners

	//------------------------------------------------------------

} // namespace Logic
