#ifndef __Graphics__PoolOgreEntity
#define  __Graphics__PoolOgreEntity

#include <map>

namespace Ogre
{
	class Entity;
	class SceneManager;
}
namespace Graphics
{
	class CPoolOgreEntity
	{

	public:
		/**
		Devuelve la única instancia de la clase CPoolOgreEntity.
		
		@return Única instancia de la clase CPoolOgreEntity.
		*/
		static CPoolOgreEntity* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CPoolOgreEntity. Debe llamarse al finalizar la 
		aplicación.
		*/
		static void Release();

		Ogre::Entity *getEntityOgre(const std::string &meshName);

		void createGraphicsPool(Ogre::SceneManager *_sceneMgr);

		void destroyColaActual(Ogre::SceneManager *_sceneMgr);

	private:

		/**
		Única instancia de la clase.
		*/
		static CPoolOgreEntity* _instance;	

		std::map<std::string, Ogre::Entity*> _poolGraphicsEntity;

	protected:

		CPoolOgreEntity();

		virtual ~CPoolOgreEntity();

		bool open();

		void close();
	};
}
#endif