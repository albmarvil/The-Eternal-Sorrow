//---------------------------------------------------------------------------
// PhysicsDebugVisualization.h
//---------------------------------------------------------------------------

/**
@file PhysicsDebugVisualization.h


Contiene la declaración de la clase que se encarga de mostrar la información de debug de la física.
Es un singleton.

@see Graphics::CPhysicsDebugVisualization
@see Graphics::CEntity

@author Alberto Martínez
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
		Devuelve la única instancia de la clase.

		@return Puntero al servidor gráfico.
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
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
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
