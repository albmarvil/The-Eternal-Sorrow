/**
@author Eduardo Garcia Reillo
@date Marzo, 2015
*/


#ifndef __Graphics_ParticleManager_H
#define __Graphics_ParticleManager_H

#include <string>
#include "BaseSubsystems/Math.h"

namespace Graphics
{
	class CParticle;
	class CAnimatedEntity;
}

namespace Logic
{
	class CEntity;
}

namespace Graphics
{
	struct ParticleInfo
	{
		std::string source;
		CParticle* particle;
		std::string boneObserved;
		CAnimatedEntity* animEnt;
	};

	class CParticleManager
	{
	public:

		/**
		Constructor de la clase.
		*/
		CParticleManager();

		/**
		Destructor de las particulas
		**/
		virtual ~CParticleManager();

		/**
		Inicializa el particleManager.

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera el manager de particulas. Debe llamarse al finalizar la aplicación.
		*/
		static void Release();

		static CParticleManager *getSingletonPtr() { return _instance; }

		/**
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		void tick(float secs);

		/*
		Añade una entidad como observador, usando su informacion (vecSources y vecBones)
		*/
		std::vector<ParticleInfo> addObserver(Logic::CEntity* entity, std::vector<std::string> vecSources, std::vector<std::string> vecBones);

		/*
		Elimina una entidad observador, metiendo todas sus partículas en el pool
		*/
		void deleteObserver(Logic::CEntity* entity);

		/*
		Crea y devuelve una partícula en una entidad concreta con un source concreto
		*/
		CParticle* addParticle(Logic::CEntity* entity, std::string source, bool visible);

		/*
		Saca una partícula de la escena y la mete al pool
		*/
		void deleteParticle(std::string source, CParticle* particle);
		
		/*
		Extrae una partícula del pool con el source dado, si no la encuentra instancia una nueva partícula
		*/
		CParticle* extractFromPool(std::string source);

		/*
		Añade una partícula al pool, haciendo las insercciones correctamente
		*/
		void addToPool(CParticle* particle, std::string source);

		/*
		Activa una partícula de un observador
		*/
		void activateParticle(Logic::CEntity* entity, std::string source);

		/*
		Desactiva una partícula de un observador
		*/
		void deactivateParticle(Logic::CEntity* entity, std::string source);

	private:

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();

		/*
		Mapa de observadores, donde apuntamos las particulas que observan la posición y los huesos de una entidad logica
		*/
		typedef std::map<Logic::CEntity*, std::vector<ParticleInfo>> TMapObservers;
		typedef std::pair<Logic::CEntity*, std::vector<ParticleInfo>> TPairObservers;
		TMapObservers _mapObservers;

		/*
		Pool de partículas
		*/
		typedef std::map <std::string, std::vector<CParticle*>> TPoolParticles;
		typedef std::pair <std::string, std::vector<CParticle*>> TPoolPair;
		TPoolParticles _poolParticles;

		/**
		Instancia única de la aplicación.
		*/
		static CParticleManager* _instance;

		int _particleIdCont;
	};
}
#endif