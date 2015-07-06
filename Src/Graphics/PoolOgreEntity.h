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
		Devuelve la �nica instancia de la clase CPoolOgreEntity.
		
		@return �nica instancia de la clase CPoolOgreEntity.
		*/
		static CPoolOgreEntity* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CPoolOgreEntity. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		Ogre::Entity *getEntityOgre(const std::string &meshName);

		void createGraphicsPool(Ogre::SceneManager *_sceneMgr);

		void destroyColaActual(Ogre::SceneManager *_sceneMgr);

	private:

		/**
		�nica instancia de la clase.
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