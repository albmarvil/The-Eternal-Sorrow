#include "ParticleManager.h"

#include <assert.h>
#include "Scene.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/AnimatedGraphics.h"
#include "Logic/Maps/Map.h"

#include "Graphics/Particle.h"
#include "Graphics/Server.h"
#include "BaseSubsystems/Server.h"
#include "BaseSubsystems/Math.h"
#include "BaseSubsystems\Log.h"


namespace Graphics
{
	CParticleManager *CParticleManager::_instance = 0;

	CParticleManager::CParticleManager() : _particleIdCont(0)
	{
		assert(!_instance && "Segunda inicializaci�n de Graphics::CParticleManager no permitida!");

		_instance = this;
	}

	CParticleManager::~CParticleManager()
	{
				
	}

	bool CParticleManager::Init() 
	{
		assert(!_instance && "Segunda inicializaci�n de Grahpics::CParticleManager no permitida!");

		new CParticleManager();

		if (!_instance->open())
		{
			Release();
			return false;
		}

		return true;

	} // init

	bool CParticleManager::open()
	{
		_particleIdCont = 0;
		return true;

	} // open

	void CParticleManager::Release()
	{
		assert(_instance && "Grahpics::CParticleManager no est� inicializado!");
		if(_instance)
		{
			_instance->close();
			delete _instance;
		}

	} // Release

	void CParticleManager::close() 
	{
		//al cerrar la instancia del singleton borramos nuestro pool de part�culas
		TPoolParticles::iterator it = _poolParticles.begin();

		for (it; it != _poolParticles.end(); ++it)
		{
			std::vector<CParticle*> vecPart = (*it).second;

			for (int i = 0; i < vecPart.size(); ++i)
			{
				delete vecPart[i];
			}
			vecPart.clear();
		}
		_poolParticles.clear();


	} // close

	//--------------------------------------------------------
		
	void CParticleManager::tick(float secs)
	{
		TMapObservers::iterator it = _mapObservers.begin();

		// Recorremos todas las entidades observadoras y para cada una todas sus part�culas
		for (it; it != _mapObservers.end(); ++it)
		{
			for (int i = 0; i < (*it).second.size(); ++i)
			{
				Logic::CEntity* entity = (*it).first; // Alias
				ParticleInfo partInfo = (*it).second[i]; // Alias

				//Actualizamos la posici�n de la part�cula solo si esta es visible
				if (partInfo.particle->getVisible()) 
				{
					Vector3 posParticle;

					if (partInfo.boneObserved != "-")
					{
						// Establecemos como posicion la del hueso
						posParticle = partInfo.animEnt->getBoneWorldPosition(partInfo.boneObserved);
						partInfo.particle->setPosition(posParticle+Vector3(0,0,10));
					}

					else
					{
						// Establecemos como posici�n la de la entidad
						//posParticle = entity->getPosition();
						//partInfo.particle->setPosition(posParticle+Vector3(0,0,10));
						partInfo.particle->setTransform(entity->getTransform());
					}

					
				}
			}
		}

	} // tick

	std::vector<ParticleInfo> CParticleManager::addObserver(Logic::CEntity* entity, std::vector<std::string> vecSources, std::vector<std::string> vecBones)
	{
		std::vector<ParticleInfo> vecPartInfo;

		// Recorremos y a�adimos todas las part�culas de dicho observador a vecPartInfo
		for (int i = 0; i < vecSources.size() ;++i)
		{
			CAnimatedEntity* animEnt = 0;

			// Si el hueso que corresponde a dicho source no es "-" significa que tiene esqueleto, por lo que guardamos su AnimatedEntity
			if (vecBones[i] != "-")
			{
				Logic::CAnimatedGraphics* animComp = entity->getComponent<Logic::CAnimatedGraphics*>();
				animEnt = animComp->getAnimatedEntity();
			}

			// Creamos la part�cula, ya sea sac�ndola del pool o creando una nueva, y la ponemos inicialmente invisible
			CParticle* particle = addParticle(entity, vecSources.at(i), true);

			// ParticleInfo = {    source,      CParticula, huesoAsociado, animEnt} (vecSources.at(x) corresponde con vecBones.at(x))
			ParticleInfo pi = {vecSources[i], particle, vecBones[i], animEnt};

			vecPartInfo.push_back(pi);
		}

		// Creamos el observador con la entidad recibida y el vecPartInfo ya rellenado
		TPairObservers pair(entity, vecPartInfo);
		_mapObservers.insert(pair);


		return vecPartInfo;
	}

	void CParticleManager::deleteObserver(Logic::CEntity* entity)
	{
		/*
		Borramos todas las part�culas que estan observando a esta entidad l�gica
		Para ello recorremos la lista de ParticleInfo que tenemos apuntada para est entidad
		y borramos una a una todas sus part�culas

		Al final limpiamos el vector y la clave del mapa de observadores.
		*/
		std::vector<ParticleInfo>::iterator it2 = _mapObservers[entity].begin();

		for(; it2 != _mapObservers[entity].end(); ++it2)
		{
			deleteParticle((*it2).source, (*it2).particle);
		}

		_mapObservers[entity].clear();
		_mapObservers.erase(entity);

	}

	CParticle* CParticleManager::extractFromPool(std::string source)
	{
		/*
		Extraemos una part�cula del pool. 
		Para ello buscamos a partir del source de la particula en nuestro pool de part�culas.
		Si la encontramos, es la part�cula que devolvemos (consideramos el vector del pool como una pila, sacamos desde el final)

		En caso de no encontrarla instanciamos una nueva part�cula
		*/
		CParticle *particle = NULL;

		TPoolParticles::iterator find = _poolParticles.find(source);

		if( find != _poolParticles.end() && find->second.size() > 0)
		{
			particle = find->second.back();
			find->second.pop_back();
		}
		else
		{
			// Si creamos una nueva part�cula, aumentamos el contador de Id
			particle = new CParticle(source + std::to_string(_particleIdCont), source);
			++_particleIdCont;
		}

		return particle;
	}

	void CParticleManager::addToPool(CParticle* particle, std::string source)
	{
		/**
		A�adimos la part�cula al pool. Si ya exisit�a un source igual en el pool, simplemente hacemos un push a la pila de part�culas

		En caso de no existir inicializamos la pila de part�culas y hacemos una nueva inserci�n por clave en el mapa
		*/

		TPoolParticles::iterator find = _poolParticles.find(source);

		if( find != _poolParticles.end())
		{
			_poolParticles[source].push_back(particle);
		}
		else
		{
			std::vector<CParticle*> particles;
			particles.push_back(particle);

			TPoolPair pair(source, particles);
			_poolParticles.insert(pair);
		}
	}

	CParticle* CParticleManager::addParticle(Logic::CEntity* entity, std::string source, bool visible)
	{
		//Este m�todo a�ade una part�cula que observa a una entidad l�gica
		
		// Sacamos una particula del pool.
		CParticle* particle = extractFromPool(source);

		// A�adimos a la escena la part�cula
		entity->getMap()->getScene()->addParticle(particle, entity->getPosition()); 

		// Le establecemos si es visible y le asignamos un nombre nuevo
		particle->setVisible(visible);  

		return particle;
	}

	void CParticleManager::deleteParticle(std::string source, CParticle* particle)
	{
		// Hacemos invisible la part�cula
		particle->setVisible(false);
		particle->stop();
		
		// La sacamos de la escena activa
		CServer::getSingletonPtr()->getActiveScene()->removeParticle(particle);

		addToPool(particle, source);

	}

	void CParticleManager::activateParticle(Logic::CEntity* entity, std::string source)
	{
		for (int i = 0; i < _mapObservers[entity].size(); i++)
		{
			ParticleInfo pi = _mapObservers[entity].at(i);

			if (source == pi.source)
			{
				// La hacemos visible
				pi.particle->setVisible(true);
				pi.particle->start();

				// La metemos de la escena activa
				//CServer::getSingletonPtr()->getActiveScene()->addParticle(pi.particle);

				break;
			}
		}
	}

	void CParticleManager::deactivateParticle(Logic::CEntity* entity, std::string source)
	{
		for (int i = 0; i < _mapObservers[entity].size(); i++)
		{
			ParticleInfo pi = _mapObservers[entity].at(i);

			if (source == pi.source)
			{
				// La hacemos invisible
				pi.particle->setVisible(false);
				pi.particle->stop();

				// La sacamos de la escena activa
				//CServer::getSingletonPtr()->getActiveScene()->removeParticle(pi.particle);

				break;
			}
		}
	}

}