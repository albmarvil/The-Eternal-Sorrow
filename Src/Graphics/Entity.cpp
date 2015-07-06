//---------------------------------------------------------------------------
// Entity.cpp
//---------------------------------------------------------------------------

/**
@file Entity.cpp

Contiene la implementación de la clase que representa una entidad gráfica.

@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "Entity.h"
#include "Scene.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Entity/Components/WeaponController.h"
#include "Logic/Entity/Entity.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSceneNode.h>
#include <OgreTagPoint.h>
#include <OgreSceneManager.h>
#include "PoolOgreEntity.h"

namespace Graphics 
{
	int byYES = 0;

	CEntity::CEntity(const std::string &name, const std::string &mesh)
		: _entity(0), _entityNode(0), _tagPoint(0), _scene(0), _loaded(false), _needNode(true)
	{
		_name = name;
		_mesh = mesh;
		isPrefab = false;

	} // CEntity

	//--------------------------------------------------------
	CEntity::CEntity(const std::string &name, const PrefabType &type)
		: _entity(0), _entityNode(0), _tagPoint(0), _scene(0), _loaded(false), _needNode(true)
	{
		_name = name;
		_type = type;
		isPrefab = true;

	} // CEntity

	//-------------------------------------------------------

	CEntity::~CEntity() 
	{
		assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
		
	} // ~CEntity
	
	//--------------------------------------------------------
		
	bool CEntity::attachToScene(CScene *scene)
	{
		assert(scene && "¡¡La entidad debe asociarse a una escena!!");
		// Si la entidad está cargada por otro gestor de escena.
		if(_loaded && (_scene != scene))
			return false;

		// Si no está cargada forzamos su carga.
		if (!_loaded)
		{
			_scene = scene;
			return load();
		}
		
		// Si ya estaba cargada en la escena se devuelve cierto.
		return true;

	} // attachToScene
	
	//--------------------------------------------------------
		
	bool CEntity::deattachFromScene()
	{
		// Si la entidad no está cargada no se puede quitar de
		// una escena. Ya que no pertenecerá a ninguna.
		if(!_loaded)
			return false;
		// Si la entidad está cargada forzamos su descarga.
		else
		{
			assert(_scene && "¡¡La entidad debe estar asociada a una escena!!");
			unload();
			_scene = 0;
		}

		return true;

	} // deattachFromScene
	
	//--------------------------------------------------------
		
	bool CEntity::load()
	{
		if(!isPrefab)
		{
			try
			{
				/*if(CPoolOgreEntity::getSingletonPtr()->getEntityOgre(_mesh) != 0)
				{
					_entity = CPoolOgreEntity::getSingletonPtr()->getEntityOgre(_mesh)->clone(_name);
				}
				else
				{*/
					_entity = _scene->getSceneMgr()->createEntity(_name, _mesh);
				//}
			}
			catch(std::exception e)
			{
				return false;
			}
		}
		else
		{
			try
			{
				_entity = _scene->getSceneMgr()->createEntity(_name, _type);
			}
			catch(std::exception e)
			{
				return false;
			}
		}
	
		
		if (_needNode)
		{
			_entityNode = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(_name + "_node");
			_entityNode->attachObject(_entity);
		}

		_loaded = true;

		return true;

	} // load
	
	//--------------------------------------------------------
		
	void CEntity::unload()
	{
		if(_entityNode)
		{
			// desacoplamos la entidad de su nodo
			_entityNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_entityNode);
			_entityNode = 0;
		}
		if(_entity)
		{
			_scene->getSceneMgr()->destroyEntity(_entity);
			_entity = 0;
		}
		_loaded = false;
	} // load

	//--------------------------------------------------------
		
	void CEntity::tick(float secs)
	{
		


	} // tick
	
	//--------------------------------------------------------
		
	void CEntity::setTransform(const Matrix4 &transform)
	{
		if(_entityNode)
		{
			_entityNode->setPosition(transform.getTrans());
			_entityNode->setOrientation(transform.extractQuaternion());
		}

	} // setTransform
	
	//--------------------------------------------------------
		
	void CEntity::setOrientation(const Matrix3 &orientation)
	{
		if(_entityNode)
			_entityNode->setOrientation(orientation);

	} // setOrientation
	
	//--------------------------------------------------------
		
	void CEntity::setVisible(bool visible)
	{
		if(_entityNode)
			_entityNode->setVisible(visible);

	} // setVisible
	
	//--------------------------------------------------------
		
	const bool CEntity::getVisible()
	{
		if(_entityNode)
			return _entity->isVisible();

		throw new std::exception("La entidad no ha sido cargada");

	} // getPosition
	
	//--------------------------------------------------------
		
	void CEntity::setPosition(const Vector3 &position)
	{
		if(_entityNode)
			_entityNode->setPosition(position);

	} // setPosition
	
	//--------------------------------------------------------
		
	void CEntity::setScale(const Vector3 &scale)
	{
		if(_entityNode)
			_entityNode->setScale(scale);

	} // setScale
	
	//--------------------------------------------------------
		
	void CEntity::setScale(const float scale)
	{
		if(_entityNode)
		{
			Vector3 scaleVector(scale,scale,scale);
			_entityNode->setScale(scaleVector);
		}

	} // setScale

	//-------------------------------------------------------------------------

	void CEntity::setMaterial(std::string &material)
	{
		if(_entity)
			_entity->setMaterialName(material);
	}//setMaterial

	//-------------------------------------------------------------------------

	void CEntity::setMaterial(std::string &material, int subEntity)
	{
		if(_entity)
			_entity->getSubEntity(subEntity)->setMaterialName(material);
	}//setMaterial

	void CEntity::attachObjectToBone(std::string name, Logic::CEntity* logicEntity)
	{
		// Sacamos la entidad gráfica a partir de la lógica para sacar la entidad de Ogre y enviársela a attachObjectToBone
		Logic::CAnimatedGraphics* animComp = (Logic::CAnimatedGraphics*) logicEntity->getComponent("CAnimatedGraphics");
		CEntity* graEnt = 0;
		if(animComp)
		{
			graEnt = animComp->getGraphicsEntity();
		}
		else
		{
			Logic::CGraphics* graComp = logicEntity->getComponent<Logic::CGraphics*>();
			graEnt = graComp->getGraphicsEntity();
		}

		

		Ogre::TagPoint *tp =_entity->attachObjectToBone(name, graEnt->getEntity());
		graEnt->setTagPoint(tp);
		
		addObserver(name, logicEntity);
	}

	void CEntity::deattachObjectFromBone(std::string name, Logic::CEntity* logicEntity)
	{
		// Sacamos la entidad gráfica a partir de la lógica para sacar la entidad de Ogre y enviársela a attachObjectToBone
		Logic::CAnimatedGraphics* animComp = (Logic::CAnimatedGraphics*) logicEntity->getComponent("CAnimatedGraphics");
		CEntity* graEnt = 0;
		
		if(animComp)
		{
			graEnt = animComp->getGraphicsEntity();
		}
		else
		{
			Logic::CGraphics* graComp = logicEntity->getComponent<Logic::CGraphics*>();
			graEnt = graComp->getGraphicsEntity();
		}

		_entity->detachObjectFromBone(graEnt->getEntity());
		graEnt->setTagPoint(NULL);

		deleteObserver(name, logicEntity);
	}

	void CEntity::addObserver(std::string boneName, Logic::CEntity* ent)
	{
		Ogre::Bone* bone = _entity->getSkeleton()->getBone(boneName);

		// Comprobamos si ya existe dicho hueso en el mapa, y de no ser así lo metemos
		if (_mapBoneObservers.find(bone) == _mapBoneObservers.end())
		{
			std::vector<Logic::CWeaponController*> vecEnt;
			_mapBoneObservers[bone] = vecEnt;
		}

		Logic::CWeaponController* cmp = (Logic::CWeaponController*) ent->getComponent("CWeaponController");
		assert(cmp);
		// Asociamos a dicho hueso la entidad que dependerá de él
		_mapBoneObservers.at(bone).push_back(cmp);
	} 

	void CEntity::deleteObserver(std::string boneName, Logic::CEntity* ent)
	{
		Ogre::Bone* bone = _entity->getSkeleton()->getBone(boneName);
		Logic::CWeaponController* cmp = (Logic::CWeaponController*) ent->getComponent("CWeaponController");
		assert(cmp);
		MapBoneObservers::iterator it = _mapBoneObservers.find(bone);

		// Si lo ha encontrado...
		if (it != _mapBoneObservers.end())
		{
			//... recorremos el vector de entidades...
			for (int i = 0; i < (*it).second.size(); i++)
			{
				//... hasta encontrarlo y sacarlo de ahí.
				if (cmp == (*it).second.at(i))
				{
					(*it).second.erase((*it).second.begin() + i);
					break;
				}
			}
		}
	} 

	void CEntity::pauseBone(bool pause)
	{
		MapBoneObservers::iterator it = _mapBoneObservers.begin();

		for(; it != _mapBoneObservers.end(); ++it)
		{
			(*it).first->setManuallyControlled(pause);
		}
	}

	Ogre::SceneNode * CEntity::getEntityNode()
	{
		return _needNode? _entityNode : NULL;
	}

} // namespace Graphics
