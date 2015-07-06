

//---------------------------------------------------------------------------
// Camera.h
//---------------------------------------------------------------------------

/**
@file Camera.h

Contiene la declaraci�n de la clase que maneja la c�mara.

@see Graphics::CCamera

@author David Llans�
@date Julio, 2010
*/

#ifndef __Graphics_Camera_H
#define __Graphics_Camera_H

#include "BaseSubsystems/Math.h"
// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Ogre 
{
	class Camera;
	class SceneNode;
	class CompositorManager;
}
namespace Graphics 
{
	class CScene;
	class CServer;
}

namespace Graphics 
{
	/**
	Clase de la c�mara extendida basada en Ogre. Para simplificar su uso
	La c�mara extendida consta de dos nodos, uno es el propio de la 
	c�mara, que contiene la c�mara real de Ogre y otro es el nodo 
	objetivo, que representa la posici�n a la que la c�mara debe mirar.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Julio, 2010
	*/

	class CCamera 
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la c�mara.
		@param sceneMgr Gestor de la escena de Ogre a la que pertenece.
		*/
		CCamera(const std::string &name, CScene *scene);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CCamera();
		

		/**
		Devuelve la posici�n de la c�mara.

		@return Referencia a la posici�n del nodo que contiene la c�mara de Ogre.
		*/
		const Vector3 &getCameraPosition();
		
		/**
		Devuelve la posici�n a la que apunta la c�mara.

		@return Referencia a la posici�n del nodo _targetCamera.
		*/
		const Vector3 &getTargetCameraPosition();

		/**
		Devuelve la orientaci�n de la c�mara.

		@return Referencia al quaternion del nodo que contiene la c�mara de Ogre.
		*/
		const Quaternion &getCameraOrientation();

		/**
		Cambia la posici�n de la c�mara.

		@param newPosition Nueva posici�n para el nodo que contiene la c�mara 
		de Ogre.
		*/
		void setCameraPosition(const Vector3 &newPosition);

		/**
		Cambia la posici�n de la posici�n a la que apunta la c�mara.

		@param newPosition Nueva posici�n para el _targetNode.
		*/
		void setTargetCameraPosition(const Vector3 &newPosition);

		/**
		Configura la Dimensi�n-Y de Field of View(FOV) del frustum CON RADIANES.

		FOV es el angulo entre la posicion del frustum y los "bordes" de la pantalla, en los cuales la escena es proyectada.
		Valores altos (+90 grados) da un resultado a una vista "ojo de pez". Valores bajos (-30 grados) en una vista "telescopica".
		Los valores usuales son 45 o 60 grados.

		@param rads Este parametro indica el FOV vertical, el horizontal se calcula a partir de este.
		*/
		void setFOVyInRadians(float rads);
		
		/**
		Configura la Dimensi�n-Y de Field of View(FOV) del frustum CON GRADOS.

		FOV es el angulo entre la posicion del frustum y los "bordes" de la pantalla, en los cuales la escena es proyectada.
		Valores altos (+90 grados) da un resultado a una vista "ojo de pez". Valores bajos (-30 grados) en una vista "telescopica".
		Los valores usuales son 45 o 60 grados.

		@param degrees Este parametro indica el FOV vertical, el horizontal se calcula a partir de este.
		*/
		void setFOVyInDegrees(float degrees);

		/*
		Configura el Near Clip del frustum. Es decir el plano m�s cercano a la c�mara a partir del cual se pintan los objetos

		@param distance Distancia de dicho plano a la c�mara
		*/
		void setNearClipDistance(float distance);

		/*
		Configura el Far Clip del frustum. Es decir el plano m�s lejano a la c�mara a partir del cual se pintan los objetos

		@param distance Distancia de dicho plano a la c�mara
		*/
		void setFarClipDistance(float distance);

		void setCompositorEffect(const std::string &effect, bool activate);

		float getAspectRatio() const;

		Ogre::Radian getFovyY() const;

		Matrix4 getViewMatrix() const;
		 
		Matrix4 getProjectionMatrix() const;

		Vector2 getScreenCoord(const Vector3 &positionWorld);
	protected:
		
		/**
		Clase amiga. Solo la escena tiene acceso a la c�mara de Ogre para
		poder crear el viewport.
		*/
		friend class CScene;
		friend class CServer;

		/**
		Devuelve la c�mara de Ogre.

		@return puntero a la c�mara de Ogre.
		*/
		Ogre::Camera *getCamera() {return _camera;}

		/**
		Nodo que contiene la c�mara.
		*/
		Ogre::SceneNode *_cameraNode;

		/**
		Nodo que representa el punto a donde debe mirar la c�mara.
		*/
		Ogre::SceneNode *_targetNode;
		
		/**
		La c�mara de Ogre.
		*/
		Ogre::Camera *_camera;

		/**
		Controla todos los elementos de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la c�mara.
		*/
		std::string _name;

		Ogre::CompositorManager* _compositorManager;

		std::string _actualCameraEffect;

	}; // class CCamera

} // namespace Graphics

#endif // __Graphics_Camera_H
