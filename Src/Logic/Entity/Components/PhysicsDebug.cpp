/**
@file PhysicsDebug.cpp

Contiene la declaración del componente que controla la representación
gráfica de la parte física de la entidad
 
@see Logic::CPhysicsDebug
@see Logic::IComponent

@author Alberto Martínez
@date Febrero, 2015
*/

#include "PhysicsDebug.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/ManualEntity.h"
#include "Graphics/Server.h"
#include "Graphics/Entity.h"

namespace Logic 
{
	IMP_FACTORY(CPhysicsDebug);
	
	//---------------------------------------------------------

	CPhysicsDebug::~CPhysicsDebug() 
	{
		if(_isManual)
		{
			if(_PhysicsDebugEntity)
			{
				//_scene->removeEntity(_PhysicsDebugEntity);
				delete _PhysicsDebugEntity;
				_PhysicsDebugEntity = 0;
			}
		}
		else
		{
			if(_graphicsEntity)
			{
				//_scene->removeEntity(_PhysicsDebugEntity);
				delete _graphicsEntity;
				_graphicsEntity = 0;
			}
		}
		
	} // ~CPhysicsDebug
	
	//---------------------------------------------------------

	bool CPhysicsDebug::OnSpawn(const Map::CEntity *entityInfo)
	{
		_scene = _entity->getMap()->getScene();

		// Leer la forma (shape)
		assert(entityInfo->hasAttribute("physic_shape"));
		const std::string physicShape = entityInfo->getStringAttribute("physic_shape");

		_offset = Vector3::ZERO;
		if(entityInfo->hasAttribute("physic_offset"))
		{
			_offset = entityInfo->getVector3Attribute("physic_offset");
		}

		if (physicShape == "box") 
		{
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_dimensions"));
			Vector3 physicDimensions = entityInfo->getVector3Attribute("physic_dimensions");

			//TODO crear una entidad grafica que sea un prefab CUbo
			_isManual = false;
			_scale = (physicDimensions * 2)*0.01;
			_material = "debug_draw";

			_graphicsEntity = new Graphics::CEntity(_entity->getName() + std::string("_Debug"), Graphics::PrefabType::PT_CUBE);
			_graphicsEntity->setNeedNode(true);
			if(!_scene->addEntity(_graphicsEntity))
			{
				_graphicsEntity = 0;
				return false;
			}
		}	
		else if(physicShape == "sphere")
		{
			// Leer las dimensiones de la caja
			assert(entityInfo->hasAttribute("physic_radius"));
			float radius = entityInfo->getFloatAttribute("physic_radius");
				
			//TODO crear una entidad gráfica que sea un prefab SPHERE
			_isManual = false;
			_scale = Vector3(radius*2, radius*2, radius*2) * 0.01 ;
			_material = "debug_draw";

			_graphicsEntity = new Graphics::CEntity(_entity->getName() + std::string("_Debug"), Graphics::PrefabType::PT_SPHERE);
			_graphicsEntity->setNeedNode(true);
			if(!_scene->addEntity(_graphicsEntity))
			{
				_graphicsEntity = 0;
				return false;
			}
		}
		else if(physicShape == "capsule")
		{
			assert(entityInfo->hasAttribute("physic_radius"));
			float radius = entityInfo->getFloatAttribute("physic_radius");
			
			assert(entityInfo->hasAttribute("physic_height"));
			float height = entityInfo->getFloatAttribute("physic_height");

			_isManual = true;
			_graphicsEntity = 0;
			_PhysicsDebugEntity = new Graphics::CManualEntity(_entity->getName() + std::string("_Debug"), height, radius);
			
				
		}
			
		 //}
		// Leer el tipo de entidad: estáticos, dinámico o cinemático
	
		if(_isManual)
		{
			if(!_PhysicsDebugEntity)
			return false;

			//Seteamos la posicion de la entidad grafica
			setTransform();
		}
		else
		{
			if(!_graphicsEntity)
			return false;

			//Seteamos la posicion de la entidad grafica
			setTransform();
			_graphicsEntity->setScale(_scale);
			_graphicsEntity->setMaterial(_material);
		}


		//Si existe
		if(_graphicsEntity)
		{
			//La hacemos invisible antes de eliminar el sceneNode y quitarla de la escena
			_graphicsEntity->setVisible(false);
			//Elimino la entidad segun sea estatica o dinamica
			
			_scene->removeEntity(_graphicsEntity);
		}
		_scene = 0;

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CPhysicsDebug::respawn(const Map::CEntity *entityInfo)
	{

		return true;
	}

	bool CPhysicsDebug::activate()
	{
		//Obtenemos la nueva escena
		_scene = _entity->getMap()->getScene();

		
		if(_isManual)
		{
			if(!_PhysicsDebugEntity->attachToScene(_scene))
			{
				_PhysicsDebugEntity = 0;
				return false;
			}

			//Seteamos la posicion de la entidad grafica
			setTransform();
		}
		else
		{
			if(!_scene->addEntity(_graphicsEntity))
			{
				_graphicsEntity = 0;
				return false;
			}

			//Seteamos la posicion de la entidad grafica
			setTransform();
			_graphicsEntity->setScale(_scale);
			_graphicsEntity->setMaterial(_material);

		}

		//Nos registramos como observador de la posicion 
		_entity->addEntityTransformListener(this);
		return true;
	}

	void CPhysicsDebug::deactivate()
	{
		if(_isManual)
		{
			//Si existe
			if(_PhysicsDebugEntity)
			{
				//La hacemos invisible antes de eliminar el sceneNode y quitarla de la escena
				_PhysicsDebugEntity->setVisible(false);
				//Elimino la entidad segun sea estatica o dinamica
			
				_PhysicsDebugEntity->deattachFromScene();
			}
			_scene = 0;
		}
		else
		{
			//Si existe
			if(_graphicsEntity)
			{
				//La hacemos invisible antes de eliminar el sceneNode y quitarla de la escena
				_graphicsEntity->setVisible(false);
				//Elimino la entidad segun sea estatica o dinamica
			
				_scene->removeEntity(_graphicsEntity);
			}
			_scene = 0;
		}

		//nos desregistramos de observadores de la entidad
		_entity->removeEntityTransformListener(this);
		
	}

	//---------------------------------------------------------

	
	//---------------------------------------------------------

	bool CPhysicsDebug::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//return message->getType() == "SET_TRANSFORM" ||
		return	   message->getType() == "SET_SIMULATION";

	} // accept
	
	//---------------------------------------------------------

	void CPhysicsDebug::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		/*if(message->getType().compare("SET_TRANSFORM") == 0)
		{
			setTransform(dynamic_cast<SET_TRANSFORM*>(message.get())->getTransform());
		}
		else*/ if(message->getType().compare("SET_SIMULATION") == 0)
		{
			bool flag = dynamic_cast<SET_SIMULATION*>(message.get())->getSimulation();
			if(_isManual)
			{
				if(_PhysicsDebugEntity)
				{
					_PhysicsDebugEntity->setVisible(flag);
				}
			}
			else
			{
				if(_graphicsEntity)
				{
					_graphicsEntity->setVisible(flag);
				}
			}
		}

	} // process


	void CPhysicsDebug::setTransform()
	{
		Matrix4 trans = _entity->getTransform();
		Vector3 pos = trans.getTrans();
		pos += _offset;
		trans.setTrans(pos);
		if(_isManual)
		{
			if(_PhysicsDebugEntity)
			{
				_PhysicsDebugEntity->setTransform(trans);
			}
		}
		else
		{
			if(_graphicsEntity)
			{
				_graphicsEntity->setTransform(trans);
			}
		}
	}


	void CPhysicsDebug::setTransform(Matrix4 trans)
	{
		Vector3 pos = trans.getTrans();
		pos += _offset;
		trans.setTrans(pos);
		if(_isManual)
		{
			if(_PhysicsDebugEntity)
			{
				_PhysicsDebugEntity->setTransform(trans);
			}
		}
		else
		{
			if(_graphicsEntity)
			{
				_graphicsEntity->setTransform(trans);
			}
		}
	}


	void CPhysicsDebug::OnEntitySetTransform(const Matrix4 &transform)
	{
		setTransform(transform);
	}

} // namespace Logic

