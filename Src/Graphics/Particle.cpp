#include "Particle.h"
#include <assert.h>
#include "Scene.h"

#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"
#include "BaseSubsystems/Log.h"

#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>


#include <ParticleUniverse\ParticleUniverseSystem.h>
#include <ParticleUniverse\ParticleUniverseSystemManager.h>

namespace Graphics
{
	CParticle::CParticle(const std::string &name, const std::string &source):
		_name(name), _source(source), _scene(0),_loaded(false)
	{
		_particleManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
	}

	CParticle::~CParticle(){
	}

	bool CParticle::attachToScene(CScene *scene, const Vector3 &pos)
	{
		assert(scene && "¡¡La particula debe asociarse a una escena!!");

		if(_loaded && (_scene != scene))
			return false;

		if (!_loaded)
		{
			_scene = scene;
			return load(pos);
		}

		return true;

	} // attachToScene
	
	//--------------------------------------------------------
		
	bool CParticle::deattachFromScene()
	{
		if(!_loaded)
			return false;
		else
		{
			assert(_scene && "¡¡La particula debe estar asociada a una escena!!");
			unload();
			_scene = 0;
		}

		return true;

	} // deattachFromScene
	
	//--------------------------------------------------------
		
	bool CParticle::load(const Vector3 &pos)
	{
		try
		{
			//Creo la particula con su nombre y su script
			//_particle = _scene->getSceneMgr()->createParticleSystem(_name, _source);
			_particle = _particleManager->createParticleSystem(_name, _source, _scene->getSceneMgr());
		}
		catch(std::exception e)
		{
			return false;
		}
		
		//Creo el nodo y lo asocio con la particula creada
		_particleNode = _scene->getSceneMgr()->getRootSceneNode()->createChildSceneNode(_name + "_particula");
		_particleNode->attachObject(_particle);
		setPosition(pos);

		_particle->prepare();
		_particle->start();

		_loaded = true;

		return true;

	} // load
	
	//--------------------------------------------------------
		
	void CParticle::unload()
	{
		if(_particleNode)
		{
			// desacoplamos la entidad de su nodo
			_particleNode->detachAllObjects();
			_scene->getSceneMgr()->destroySceneNode(_particleNode);
			_particleNode = 0;
		}
		if(_particle)
		{
			_particle->stop();
			_particleManager->destroyParticleSystem(_name, _scene->getSceneMgr());
		}

		_loaded = false;

	} // load

	//--------------------------------------------------------
		
	void CParticle::tick(float secs)
	{
	} // tick

	void CParticle::setTransform(const Matrix4 &transform)
	{
		assert(_particleNode && "La particula no ha sido cargada");

		_particleNode->setPosition(transform.getTrans());
		_particleNode->setOrientation(transform.extractQuaternion());
	}

	void CParticle::setOrientation(const Matrix3 &orientation)
	{
		assert(_particleNode && "La particula no ha sido cargada");

		_particleNode->setOrientation(orientation);

	} 
	
	//--------------------------------------------------------
		
	void CParticle::setVisible(bool visible)
	{
		assert(_particle && "La particula no ha sido cargada");

		_particle->setVisible(visible);

	} 
	
	//--------------------------------------------------------
		
	const bool CParticle::getVisible()
	{
		if(_particle)
			return _particle->isVisible();

		throw new std::exception("La particula no ha sido cargada");

	}
	

	//--------------------------------------------------------
		
	void CParticle::setPosition(const Vector3 &position)
	{
		assert(_particle && "La particula no ha sido cargada");

		_particleNode->setPosition(position);
		
	} 
	
	//--------------------------------------------------------
		
	void CParticle::setQuota(int quota)
	{
		if(_particle)
			_particle->getTechnique(0)->setEmittedEmitterQuota((size_t)quota);
	}

	void  CParticle::setScale(const float scale)
	{
		assert(_particleNode && "La particula no ha sido cargada");

		Vector3 scaleVector(scale,scale,scale);
		_particleNode->setScale(scaleVector);
		_particle->setScale(scaleVector);

	}


	void CParticle::start(float duracion)
	{
		_particle->prepare();

		duracion == 0.0f ? _particle->start() :  _particle->start(duracion);
	}

	void CParticle::startAndStopFade(float duracion)
	{
		_particle->prepare();

		_particle->startAndStopFade(duracion);
	}

	void CParticle::stop(float delay)
	{
		delay == 0.0f ? _particle->stop() : _particle->stop(delay);
	}

	void CParticle::stopFade(float delay)
	{
		delay == 0.0f ? _particle->stopFade() : _particle->stopFade(delay);
	}

	void CParticle::pause(float duracion)
	{
		duracion == 0.0f ? _particle->pause() : _particle->pause(duracion);
	}



}