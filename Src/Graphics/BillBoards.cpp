#include "BillBoards.h"
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreSceneManager.h>
#include <OgreBillboardSet.h>
#include <OgreBillboard.h>

#include "Camera.h"

namespace Graphics 
{
	CBillBoards::CBillBoards(const std::string &type, const std::string &nameLight, const std::string &material,const Vector3 &dimension, const Vector3 &position)
		: scene(0), loaded(false), name(nameLight), _type(type), _material(material), _dimensions(dimension), _position(position)
	{

	} // CLight

	//-------------------------------------------------------

	CBillBoards::~CBillBoards() 
	{
	} // ~CLight
	
	//--------------------------------------------------------
		
	bool CBillBoards::attachToScene(CScene *sc)
	{
		// Si no está cargada forzamos su carga.
		if (!loaded)
		{
			scene = sc;
			return load();
		}

		// Si ya estaba cargada en la escena se devuelve cierto.
		return true;

	} // attachToScene
	
	//--------------------------------------------------------
		
	bool CBillBoards::deattachFromScene()
	{
		// Si la luz no está cargada no se puede quitar de
		// una escena. Ya que no pertenecerá a ninguna.
		if(!loaded)
			return false;
		// Si la luz está cargada forzamos su descarga.
		else
		{
			assert(scene && "¡¡La vida debe estar asociada a una escena!!");
			unload();
			scene = 0;
		}

		return true;

	} // deattachFromScene
	
	//--------------------------------------------------------

	bool CBillBoards::load()
	{
		//Primero creamos un billboardSet, necesario para asociar billboards
		_bbSetBillBoards = scene->getSceneMgr()->createBillboardSet();

		//Le asociamos un material
		_bbSetBillBoards->setMaterialName(_material);

		//Creamos el billboard en la posicion indiciada
		_BillBoards = _bbSetBillBoards->createBillboard(_position);

		//Le damos sus dimensiones
		_BillBoards->setDimensions(_dimensions.x,_dimensions.y);

		//Buscamos el sceneNode al que asociar ese billboard y le atachamos el billboard en la posicion
		if (_type == "lifeBar")
		{
			Ogre::SceneNode * sceneNode = scene->getSceneMgr()->getSceneNode(name+ "_node");
			_BillBoardsNode = sceneNode->createChildSceneNode();
		}
		else
		{
			_BillBoardsNode = scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(name + "_node");
		}
		_BillBoardsNode->attachObject(_bbSetBillBoards);
		loaded = true;

		return true;
	}
	bool CBillBoards::loadLifeBar()
	{
		_BillBoards->setTexcoordRect(0.0f, 0.0f, 0.5f, 1.0f);
		//_BillBoardsNode->scale(1.5,3,1);
		return true;
	} // load

	
	//--------------------------------------------------------
		
	void CBillBoards::setVisible(bool visible)
	{
		_BillBoardsNode->setVisible(visible);
	}

	void CBillBoards::unload()
	{
		if(_BillBoardsNode)
		{
			// desacoplamos la entidad de su nodo
			_BillBoardsNode->detachAllObjects();
			scene->getSceneMgr()->destroySceneNode(_BillBoardsNode);
			_BillBoardsNode = 0;
		}
		if(_BillBoards)
		{
			_bbSetBillBoards->removeBillboard(_BillBoards);
			_BillBoards = 0;
		}
		if(_bbSetBillBoards)
		{
			scene->getSceneMgr()->destroyBillboardSet(_bbSetBillBoards);
			_bbSetBillBoards = 0;
		}

		loaded = false;

	} // load

	//--------------------------------------------------------
		
	void CBillBoards::updateLifeBar(int life, int lifeMax)
	{
		Ogre::Real ratio = Ogre::Real(life) / Ogre::Real(lifeMax);

		if (ratio < 0.0f)
			ratio = 0.0f;

		Ogre::Real u1 = (1.0f - ratio) * 0.5f;
		Ogre::Real u2 = 0.5f + (1.0f - ratio) * 0.5f;

		if(_BillBoards)
			_BillBoards->setTexcoordRect(u1, 0.0f, u2, 1.0f);
	}

	void CBillBoards::tick(float secs)
	{
	} // tick

} // namespace Graphics
