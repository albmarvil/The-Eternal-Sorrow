//---------------------------------------------------------------------------
// PhysicsDebugVisualization.h
//---------------------------------------------------------------------------

/**
@file PhysicsDebugVisualization.h


Contiene la declaraci�n de la clase que se encarga de mostrar la informaci�n de debug de la f�sica.
Es un singleton.

@see Graphics::CPhysicsDebugVisualization
@see Graphics::CEntity

@author Alberto Mart�nez
@date Febrero, 2015
*/

#ifndef __Graphics_PhysicsDebugVisualization_H
#define __Graphics_PhysicsDebugVisualization_H

#include <map>

namespace Ogre
{
	class SceneNode;
	class ManualObject;
}
	
namespace Graphics 
{

	class CPhysicsDebugVisualization
	{
	public:

		
		/**
		Devuelve la �nica instancia de la clase.

		@return Puntero al servidor gr�fico.
		*/
		static Graphics::CPhysicsDebugVisualization *getSingletonPtr() { return _instance; }

		static bool Init();

		static void Release();

		void tick(float secs);


		void drawPhysics(bool draw);


	protected:

		CPhysicsDebugVisualization();
					

		virtual ~CPhysicsDebugVisualization();
		
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();

		static CPhysicsDebugVisualization *_instance;

		void releasePhysObjects();

		int ID;

		float limitSecs;

		float acumSecs;

		bool _drawPhysics;

		///memoria de dibujado fisico

		std::map<Ogre::ManualObject*, Ogre::SceneNode*> physObjects;

	}; // class CPhysicsDebugVisualization

} // namespace Graphics

#endif // __Graphics_PhysicsDebugVisualization_H
