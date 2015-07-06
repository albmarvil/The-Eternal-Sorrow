/**
@file Graphics.cpp

Contiene la implementación del componente que controla la representación
gráfica de la entidad.
 
@see Logic::CGraphics
@see Logic::IComponent

@author David Llansó
@date Agosto, 2010
*/

#include "Graphics.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Graphics/Scene.h"
#include "Graphics/Entity.h"
#include "Graphics/StaticEntity.h"
#include "Graphics/Server.h"

namespace Logic 
{
	IMP_FACTORY(CGraphics);
	
	//---------------------------------------------------------

	CGraphics::~CGraphics() 
	{
		if(_graphicsEntity)
		{
			//_scene->removeEntity(_graphicsEntity);
			delete _graphicsEntity;
			_graphicsEntity = 0;
		}
	} // ~CGraphics
	
	//---------------------------------------------------------

	bool CGraphics::OnSpawn(const Map::CEntity *entityInfo)
	{
		_scene = _entity->getMap()->getScene();

		if(entityInfo->hasAttribute("basic_shape"))
		{
			std::string shape = entityInfo->getStringAttribute("basic_shape");
			if(shape.compare("Prefab_Cube") == 0)
			{
				_type = Graphics::PrefabType::PT_CUBE;
				_isPrefab = true;
			}
			else if(shape.compare("Prefab_Sphere") == 0)
			{
				_type = Graphics::PrefabType::PT_SPHERE;
				_isPrefab = true;
			}
			else if(shape.compare("Prefab_Plane") == 0)
			{
				_type = Graphics::PrefabType::PT_PLANE;
				_isPrefab = true;
			}
			else if(shape.compare("Plane") == 0)
			{
				_isPrefab = false;
				std::string _meshName = _entity->getName() + "_Plane";

				//Comprobamos que el plano tenga todos los atributos
				bool requiredAttributes = true;
			    requiredAttributes &= entityInfo->hasAttribute("plane_triangles");
				requiredAttributes &= entityInfo->hasAttribute("plane_width");
				requiredAttributes &= entityInfo->hasAttribute("plane_heigth");
				requiredAttributes &= entityInfo->hasAttribute("plane_normal");
				

				if(requiredAttributes)
				{
					//Obtenemos todos los datos del plano

					//la distancia sobre la normal por defecto es 0
					float _dist = .0f;
					if(entityInfo->hasAttribute("plane_dist"))
					{
						_dist = entityInfo->getFloatAttribute("plane_dist");
					}

					int _nTriangles = entityInfo->getIntAttribute("plane_triangles");
					float _width = entityInfo->getFloatAttribute("plane_width");
					float _heigth = entityInfo->getFloatAttribute("plane_heigth");
					Vector3 _normal = entityInfo->getVector3Attribute("plane_normal");

					Graphics::CServer::getSingletonPtr()->createPlaneMesh(_meshName, _normal, _dist, _width, _heigth, _nTriangles);
				
					_model = _meshName;
				}
				else
				{
					return false; //no podemos construir el meshPlane
				}
				
			}
			
		}
		else if(entityInfo->hasAttribute("model"))
		{
			_model = entityInfo->getStringAttribute("model");
			_isPrefab = false;
		}
		
		_isStatic = false;
		if(entityInfo->hasAttribute("static"))
			_isStatic = entityInfo->getBoolAttribute("static");

		// Si no es nodo se seteará leyendo "false" desde fichero
		if(entityInfo->hasAttribute("needNode"))
		{ 
			_needNode = entityInfo->getBoolAttribute("needNode");
		}

		if(!_isPrefab)
		{
			_graphicsEntity = createGraphicsEntity(entityInfo);
		}
		else
		{
			_graphicsEntity = createPrefabGraphicsEntity(entityInfo);
		}

		if(!_graphicsEntity)
			return false;

		//Seteamos la posicion de la entidad grafica
		_graphicsEntity->setTransform(_entity->getTransform());

		//Seteamos el material en caso de tenerlo
		if(entityInfo->hasAttribute("material"))
		{
			_material = entityInfo->getStringAttribute("material");
			_graphicsEntity->setMaterial(_material);
		}

		//Seteamos la escala en caso de tener
		if(entityInfo->hasAttribute("scale"))
		{
			_scale = entityInfo->getVector3Attribute("scale");
			_graphicsEntity->setScale(_scale);
		}

		//Si existe
		if(_graphicsEntity)
		{
			//La hacemos invisible antes de eliminar el sceneNode y quitarla de la escena
			_graphicsEntity->setVisible(false);
			//Elimino la entidad segun sea estatica o dinamica
			!_isStatic
			?
				_scene->removeEntity(_graphicsEntity)
			:
				_scene->removeStaticEntity((Graphics::CStaticEntity*)_graphicsEntity);

		}

		return true;

	} // spawn
	
	//---------------------------------------------------------

	bool CGraphics::respawn(const Map::CEntity *entityInfo)
	{
		//Obtenemos la nueva escena
		_scene = _entity->getMap()->getScene();

		//Seteamos la escala en caso de tener
		if(entityInfo->hasAttribute("scale"))
		{
			_scale = entityInfo->getVector3Attribute("scale");
		}
		return true;
	}

	bool CGraphics::activate()
	{
		/*if(_entity->isPlayer())
		{
			BaseSubsystems::Log::Debug("ACTIVATE GRAPCHICS PLAYER");
		}*/
		

		if(!_isStatic)
		{
			//Añadimos la entidad a la escena grafica. Si falla el añadido, entonces igualo a cero la entidad grafica
			if(!_scene->addEntity(_graphicsEntity))
			{
				_graphicsEntity = 0;
				return false;
			}
		}
		else
		{
			if(!_scene->addStaticEntity((Graphics::CStaticEntity*)_graphicsEntity))
			{
				_graphicsEntity = 0;
				return false;
			}
		}

		//Seteamos el material en caso de tenerlo
		if(_entity->getEntityInfo()->hasAttribute("material"))
		{
			_material = _entity->getEntityInfo()->getStringAttribute("material");
			_graphicsEntity->setMaterial(_material);
		}

		_graphicsEntity->setTransform(_entity->getTransform());
		//Seteamos la escala en caso de tener
		
		_graphicsEntity->setScale(_scale);
		

		///Nos registramos como oyente de la posicion logica de la entidad
		if(_needNode)
			_entity->addEntityTransformListener(this);

		return true;
	}

	void CGraphics::deactivate()
	{
		//Si existe
		if(_graphicsEntity)
		{
			//La hacemos invisible antes de eliminar el sceneNode y quitarla de la escena
			_graphicsEntity->setVisible(false);
			//Elimino la entidad segun sea estatica o dinamica
			if(!_isStatic)
			{
				//La elimino de la escena grafica
				_scene->removeEntity(_graphicsEntity);
			}
			else
			{
				_scene->removeStaticEntity((Graphics::CStaticEntity*)_graphicsEntity);
			}
		}
		_scene = 0;

		//Nos borramos como oyente de la posicion logica de la entidad
		if(_needNode)
			_entity->removeEntityTransformListener(this);
	}

	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createGraphicsEntity(const Map::CEntity *entityInfo)
	{
		
		if(_isStatic)
		{
			_graphicsEntity = new Graphics::CStaticEntity(_entity->getName(),_model);
			if(!_scene->addStaticEntity((Graphics::CStaticEntity*)_graphicsEntity))
				return 0;
		}
		else
		{
			_graphicsEntity = new Graphics::CEntity(_entity->getName(),_model);
			_graphicsEntity->setNeedNode(_needNode);

			if(!_scene->addEntity(_graphicsEntity))
				return 0;
		}
		
		return _graphicsEntity;

	} // createGraphicsEntity
	
	//---------------------------------------------------------

	Graphics::CEntity* CGraphics::createPrefabGraphicsEntity(const Map::CEntity *entityInfo)
	{

		if(_isStatic)
		{
			_graphicsEntity = new Graphics::CStaticEntity(_entity->getName(),_type);
			if(!_scene->addStaticEntity((Graphics::CStaticEntity*)_graphicsEntity))
				return 0;
		}
		else
		{
			_graphicsEntity = new Graphics::CEntity(_entity->getName(),_type);
			_graphicsEntity->setNeedNode(_needNode);

			if(!_scene->addEntity(_graphicsEntity))
				return 0;
		}
		
		return _graphicsEntity;

	} // createPrefabGraphicsEntity
	
	//---------------------------------------------------------

	bool CGraphics::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "SET_MATERIAL";
		//return message->getType() == "SET_TRANSFORM";

	} // accept
	
	//---------------------------------------------------------

	void CGraphics::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_MATERIAL") == 0)
		{
			std::string nameMat = dynamic_cast<SET_MATERIAL*>(message.get())->getString();

			_graphicsEntity->setMaterial(nameMat);
		}

	} // process

	//------------------------------------------------------------

	void CGraphics::OnEntitySetTransform(const Matrix4 &transform)
	{
		_graphicsEntity->setTransform(transform);
	}

} // namespace Logic

