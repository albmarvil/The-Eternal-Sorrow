//---------------------------------------------------------------------------
// ManualEntity.cpp
//---------------------------------------------------------------------------

/**
@file ManualEntity.cpp

Contiene la implementación de la clase que representa una entidad gráfica creada manualmente

@see Graphics::CManualEntity

@author Alberto martínez
@date Febrero, 2015
*/

#include "ManualEntity.h"
#include "Scene.h"


#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"

#include <assert.h>

#include <OgreManualObject.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Graphics 
{
	CManualEntity::CManualEntity(const std::string &name, float height, float radius)
		: _ManualEntity(0), _ManualEntityNode(0), _scene(0), _loaded(false)
	{
		_name = name;
		_height = height;
		_radius = radius;
		_type = Capsule;

	} // CManualEntity

	//--------------------------------------------------------
	

	//-------------------------------------------------------

	CManualEntity::~CManualEntity() 
	{
		//assert(!_scene && "¡¡Para destruir una entidad esta no puede pertenecer a una escena!!");
		
	} // ~CManualEntity
	
	//--------------------------------------------------------
		
	bool CManualEntity::attachToScene(CScene *scene)
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
		
	bool CManualEntity::deattachFromScene()
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
		
	bool CManualEntity::load()
	{
		//TODO construir la capsula
		switch (_type)
		{
		case Graphics::Capsule:
			createCapsule();
			break;
		default:
			createCapsule();
			break;
		}
		_ManualEntityNode = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(_name + "_node");
		_ManualEntityNode->attachObject(_ManualEntity);
		_loaded = true;

		return true;

	} // load
	
	//--------------------------------------------------------
		
	void CManualEntity::unload()
	{
		if(_ManualEntityNode)
		{
			// desacoplamos la entidad de su nodo
			_ManualEntityNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_ManualEntityNode);
			_ManualEntityNode = 0;
		}
		if(_ManualEntity)
		{
			_scene->getSceneMgr()->destroyEntity(_ManualEntity->getName());
			_ManualEntity = 0;
		}
		_loaded = false;
	} // load

	//--------------------------------------------------------
		
	void CManualEntity::tick(float secs)
	{
		


	} // tick
	
	//--------------------------------------------------------
		
	void CManualEntity::setTransform(const Matrix4 &transform)
	{
		if(_ManualEntityNode)
		{
			_ManualEntityNode->setPosition(transform.getTrans());
			_ManualEntityNode->setOrientation(transform.extractQuaternion());
		}

	} // setTransform
	
	//--------------------------------------------------------
		
	void CManualEntity::setOrientation(const Matrix3 &orientation)
	{
		if(_ManualEntityNode)
			_ManualEntityNode->setOrientation(orientation);

	} // setOrientation
	
	//--------------------------------------------------------
		
	void CManualEntity::setVisible(bool visible)
	{
		if(_ManualEntityNode)
			_ManualEntityNode->setVisible(visible);

	} // setVisible
	
	//--------------------------------------------------------
		
	const bool CManualEntity::getVisible()
	{
		if(_ManualEntityNode)
			return _ManualEntity->isVisible();

		throw new std::exception("La entidad no ha sido cargada");

	} // getPosition
	
	//--------------------------------------------------------
		
	void CManualEntity::setPosition(const Vector3 &position)
	{
		if(_ManualEntityNode)
			_ManualEntityNode->setPosition(position);

	} // setPosition
	
	//--------------------------------------------------------
		
	void CManualEntity::setScale(const Vector3 &scale)
	{
		if(_ManualEntityNode)
			_ManualEntityNode->setScale(scale);

	} // setScale
	
	//--------------------------------------------------------
		
	void CManualEntity::setScale(const float scale)
	{
		if(_ManualEntityNode)
		{
			Vector3 scaleVector(scale,scale,scale);
			_ManualEntityNode->setScale(scaleVector);
		}

	} // setScale

	//-------------------------------------------------------------------------

	void CManualEntity::setMaterial(std::string &material)
	{
		int size = _ManualEntity->getNumSections();
		for(int i = 0; i < size; ++i)
		{
			_ManualEntity->setMaterialName(i,material);
		}
		
	}//setMaterial

	//-------------------------------------------------------------------------


	void CManualEntity::createCapsule()
	{

		_ManualEntity = _scene->getSceneMgr()->createManualObject(_name);
		float halfHeight = _height * 0.5;
		//accuracy debe ser potencia de 2
		float accuracy=8;

		#pragma region Dibujado del cuerpo del cilindro


		Vector3 centroCilindro = Vector3(0, halfHeight+_radius, 0);


		#pragma region Dibujado del circulo 1 del cilindro
		float startAngle = 0;
		float endAngle = 2*Math::PI;

		Vector3 centroCirculo = Vector3(centroCilindro.x, centroCilindro.y+halfHeight, centroCilindro.z);

		drawCircleShapeXZ(_ManualEntity, centroCirculo, _radius, accuracy, startAngle, endAngle); 
		#pragma endregion

		#pragma region Dibujado del circulo 2 del cilindro
		startAngle = 0;
		endAngle = 2*Math::PI;

		centroCirculo = Vector3(centroCilindro.x, centroCilindro.y-halfHeight, centroCilindro.z);

		drawCircleShapeXZ(_ManualEntity, centroCirculo, _radius, accuracy, startAngle, endAngle); 
		#pragma endregion


		_ManualEntity->begin("debug_draw", Ogre::RenderOperation::OT_LINE_LIST);
		
		unsigned point_index = 0;
		//divide el circulo entre (Math::PI / accuracy)
		for(float theta = 0; theta <= 2*Math::PI; theta += Math::PI / accuracy) {
			_ManualEntity->position(centroCilindro.x+(_radius * cos(theta)), centroCilindro.y+halfHeight,centroCilindro.z+(_radius * sin(theta)));
			_ManualEntity->position(centroCilindro.x+(_radius * cos(theta)), centroCilindro.y-halfHeight,centroCilindro.z+(_radius * sin(theta)));
			_ManualEntity->index(point_index++);
			_ManualEntity->index(point_index++);
		}
		_ManualEntity->end();
		

		
		#pragma endregion

		#pragma region Dibujado de las esferas superiro e inferior

		//draw the half sphere on top and bottom of the cilinder
		 for(float theta = 0; theta <= Math::PI; theta += Math::PI / accuracy) {
			 this->drawCircleShapeXY(_ManualEntity, Vector3(0, 2*halfHeight+_radius, 0), theta, _radius, accuracy, 0, Math::PI);
			 this->drawCircleShapeXY(_ManualEntity, Vector3(0, _radius, 0), theta, _radius, accuracy, Math::PI, 2*Math::PI);
		 }
		 #pragma endregion
	}


	void CManualEntity::drawCircleShapeXZ(Ogre::ManualObject * circle, Ogre::Vector3 center, float radius, float accuracy, float startAngle, float endAngle)
	{
		circle->begin("debug_draw", Ogre::RenderOperation::OT_LINE_STRIP);
 
		unsigned point_index = 0;
		//divide the circle into (Math::PI / accuracy) part and draw a segment in XZ axes with linked point
		for(float theta = startAngle; theta <= endAngle; theta += Math::PI / accuracy) {

			circle->position(center.x+(radius * cos(theta)), center.y,center.z+(radius * sin(theta)));
			circle->index(point_index++);
		}
		circle->index(0); // Rejoins the last point to the first.
		circle->end();
	}//drawCircleXZ




	void CManualEntity::drawCircleShapeXY(Ogre::ManualObject * circle, Ogre::Vector3 center, float zRotation, float radius, float accuracy, float startAngle, float endAngle)
	{
		circle->begin("debug_draw", Ogre::RenderOperation::OT_LINE_STRIP);
	
		Ogre::Vector3 p;
		unsigned point_index = 0;
		float r=radius;
		float dr=2*(radius/accuracy);
		//divide the circle into (Math::PI / accuracy) part and draw a segment in XY axes with zRotation in Z axe
		for(float theta = startAngle; theta <= endAngle; theta += Math::PI / accuracy) {
			p.x=center.x+(r*(cos(zRotation)));
			p.y=center.y+(radius * sin(theta));
			p.z=center.z+(r * sin(zRotation));
			r-=dr;
			circle->position(p);
			circle->index(point_index++);
		}
		circle->index(0); // Rejoins the last point to the first.
		circle->end();

	}//drawCircleXY


} // namespace Graphics
