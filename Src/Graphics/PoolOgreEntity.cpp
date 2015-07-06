#include "PoolOgreEntity.h"
#include <assert.h>
#include "Server.h"
#include "Scene.h"
#include "OgreSceneManager.h"

namespace Graphics
{

	CPoolOgreEntity* CPoolOgreEntity::_instance = 0;

	//--------------------------------------------------------

	CPoolOgreEntity::CPoolOgreEntity()
	{
		/*std::string lista[9] = {"barril.mesh", "Mewtwo.mesh", "enemigo_basico.mesh","bala2.mesh","bala1.mesh","Katana.mesh","AK47.mesh",
		"Moneda.mesh","Runa.mesh"};*/
		
		/*for(int i = 0; i<9;++i)
		{
			_poolGraphicsEntity[lista[i]] = 0;
		}*/

		//_listMesh = std::list<std::string>(lista, lista + sizeof(lista)/sizeof(std::string));
		_instance = this;
	}

	//--------------------------------------------------------

	CPoolOgreEntity::~CPoolOgreEntity()
	{
		_poolGraphicsEntity.clear();
		_instance = 0;
	}

	//--------------------------------------------------------

	bool CPoolOgreEntity::Init()
	{
		assert(!_instance && "Segunda inicialización de Logic::CServer no permitida!");

		new CPoolOgreEntity();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;
	} 

	//--------------------------------------------------------

	void CPoolOgreEntity::Release()
	{
		assert(_instance && "Logic::CServer no está inicializado!");

		if(_instance)
		{
			_instance->close();
			delete _instance;
		}
	} 

	//--------------------------------------------------------

	bool CPoolOgreEntity::open()
	{
		return true;
	} 

	//--------------------------------------------------------

	void CPoolOgreEntity::close() 
	{

	}

	//---------------------------------------------------------------------------------------

	void CPoolOgreEntity::createGraphicsPool(Ogre::SceneManager *_sceneMgr)
	{
		std::map<std::string, Ogre::Entity*>::const_iterator it = _poolGraphicsEntity.begin();

		for(; it != _poolGraphicsEntity.end(); ++it)
		{
			_poolGraphicsEntity[(*it).first] = _sceneMgr->createEntity((*it).first);
		}

	}

	void CPoolOgreEntity::destroyColaActual(Ogre::SceneManager *_sceneMgr)
	{
		std::map<std::string, Ogre::Entity*>::const_iterator it = _poolGraphicsEntity.begin();

		for(; it != _poolGraphicsEntity.end(); ++it)
		{
			if(_poolGraphicsEntity[(*it).first] != 0)
			{
				_sceneMgr->destroyEntity((*it).second);
				_poolGraphicsEntity[(*it).first] = 0;
			}
		}
	}

	//--------------------------------------------------------

	Ogre::Entity * CPoolOgreEntity::getEntityOgre(const std::string &meshName)
	{
		std::map<std::string, Ogre::Entity*>::const_iterator it = _poolGraphicsEntity.find(meshName);

		if ( it != _poolGraphicsEntity.end())
		{
			return it->second;
		}

		return 0;
	}

}