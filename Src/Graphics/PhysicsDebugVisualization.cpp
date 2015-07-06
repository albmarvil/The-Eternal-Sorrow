//---------------------------------------------------------------------------
// PhysicsDebugVisualization.cpp
//---------------------------------------------------------------------------

/**
@file PhysicsDebugVisualization.cpp


@see Graphics::CPhysicsDebugVisualization

@author Alberto Martínez
@date Febrero, 2010
*/

#include "PhysicsDebugVisualization.h"


#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include "Physics/Server.h"

#include "Scene.h"
#include "Server.h"

#include <OgreManualObject.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include <OgreMaterial.h>
#include <ctime>




namespace Graphics 
{
	CPhysicsDebugVisualization *CPhysicsDebugVisualization::_instance = 0;

	CPhysicsDebugVisualization::CPhysicsDebugVisualization() :  _drawPhysics(false), ID(0), limitSecs(0.6)
	{
		assert(!_instance && "Segunda inicialización de Graphics::CPhysicsDebugVisualization no permitida!");

		_instance = this;

	} // CPhysicsDebugVisualization

	//--------------------------------------------------------

	CPhysicsDebugVisualization::~CPhysicsDebugVisualization() 
	{
		assert(_instance);

		_instance = 0;

	} // ~CPhysicsDebugVisualization

	//--------------------------------------------------------

	bool CPhysicsDebugVisualization::Init() 
	{
		assert(!_instance && "Segunda inicialización de Graphics::CPhysicsDebugVisualization no permitida!");

		new CPhysicsDebugVisualization();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // Init

	//--------------------------------------------------------

	void CPhysicsDebugVisualization::Release()
	{
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	//--------------------------------------------------------

	bool CPhysicsDebugVisualization::open()
	{
		if(!BaseSubsystems::CServer::getSingletonPtr())
			return false;

		return true;

	} // open

	//--------------------------------------------------------

	void CPhysicsDebugVisualization::close() 
	{
		

	} // close

	//--------------------------------------------------------


	void CPhysicsDebugVisualization::tick(float secs) 
	{
		//clock_t timer = clock();

		acumSecs += secs;
		if(_drawPhysics && acumSecs>=limitSecs)//pintar debug
		{
			acumSecs = 0.0f;
			
			releasePhysObjects();

			/*clock_t timer1 = clock();
			double t1 = timer1 - timer;
			printf ("T1: %f\n", t1);*/


			std::vector<std::pair<Vector3, Vector3>>* list = Physics::CServer::getSingletonPtr()->getDebugVisualizationInfo();

			std::vector<std::pair<Vector3, Vector3>>::iterator it = list->begin();
			for(; it != list->end(); ++it)
			{
					
				//clock_t clk = clock();
					
				Vector3 pos0 = it->first;
				Vector3 pos1 = it->second;
				// render the line
				std::string name = "manual";
				std::stringstream sstm;
				sstm << name << ID;
				std::string resultname = sstm.str();

				CScene* activeScene = CServer::getSingletonPtr()->getActiveScene();

				Ogre::ManualObject* myManualObject =  activeScene->getSceneMgr()->createManualObject(resultname);
				Ogre::SceneNode* myManualObjectNode = activeScene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(resultname+std::string("_node")); 
 
 
				myManualObject->begin(std::string("default3"), Ogre::RenderOperation::OT_LINE_LIST); 
				myManualObject->position(pos0);
				myManualObject->position(pos1); 
				// etc 
				myManualObject->end(); 
 
				myManualObjectNode->attachObject(myManualObject);
				physObjects.insert(std::pair<Ogre::ManualObject*, Ogre::SceneNode*>(myManualObject, myManualObjectNode));
				++ID;

				/*clock_t clk1 = clock();
				double t3 = clk1 - clk;
				printf ("TINFOR: %f\n", t3);*/
			}

			delete list;
			/*clock_t timer2 = clock();
			double t2 = timer2 - timer1;
			printf ("TENDFOR: %f\n", t2);*/
	
		}
	} // tick


	void CPhysicsDebugVisualization::drawPhysics(bool draw)
	{ 
		_drawPhysics = draw;
		acumSecs = 0.0f;
		if(!draw)
		{
			releasePhysObjects();
		}
	}

	void CPhysicsDebugVisualization::releasePhysObjects()
	{
		std::map<Ogre::ManualObject*, Ogre::SceneNode*>::iterator it = physObjects.begin();
		for (; it != physObjects.end(); ++it)
		{
			(*it).first->setVisible(false);
			(*it).second->detachAllObjects();
			(*it).first->clear();
			//delete (*it).first;
			//delete (*it).second
		}
		physObjects.clear();
	}
	

} // namespace Graphics
