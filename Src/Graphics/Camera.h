

//---------------------------------------------------------------------------
// Camera.h
//---------------------------------------------------------------------------

/**
@file Camera.h

Contiene la declaración de la clase que maneja la cámara.

@see Graphics::CCamera

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_Camera_H
#define __Graphics_Camera_H

#include "BaseSubsystems/Math.h"
// Predeclaración de clases para ahorrar tiempo de compilación
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
	Clase de la cámara extendida basada en Ogre. Para simplificar su uso
	La cámara extendida consta de dos nodos, uno es el propio de la 
	cámara, que contiene la cámara real de Ogre y otro es el nodo 
	objetivo, que representa la posición a la que la cámara debe mirar.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/

	class CCamera 
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la cámara.
		@param sceneMgr Gestor de la escena de Ogre a la que pertenece.
		*/
		CCamera(const std::string &name, CScene *scene);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CCamera();
		

		/**
		Devuelve la posición de la cámara.

		@return Referencia a la posición del nodo que contiene la cámara de Ogre.
		*/
		const Vector3 &getCameraPosition();
		
		/**
		Devuelve la posición a la que apunta la cámara.

		@return Referencia a la posición del nodo _targetCamera.
		*/
		const Vector3 &getTargetCameraPosition();

		/**
		Devuelve la orientación de la cámara.

		@return Referencia al quaternion del nodo que contiene la cámara de Ogre.
		*/
		const Quaternion &getCameraOrientation();

		/**
		Cambia la posición de la cámara.

		@param newPosition Nueva posición para el nodo que contiene la cámara 
		de Ogre.
		*/
		void setCameraPosition(const Vector3 &newPosition);

		/**
		Cambia la posición de la posición a la que apunta la cámara.

		@param newPosition Nueva posición para el _targetNode.
		*/
		void setTargetCameraPosition(const Vector3 &newPosition);

		/**
		Configura la Dimensión-Y de Field of View(FOV) del frustum CON RADIANES.

		FOV es el angulo entre la posicion del frustum y los "bordes" de la pantalla, en los cuales la escena es proyectada.
		Valores altos (+90 grados) da un resultado a una vista "ojo de pez". Valores bajos (-30 grados) en una vista "telescopica".
		Los valores usuales son 45 o 60 grados.

		@param rads Este parametro indica el FOV vertical, el horizontal se calcula a partir de este.
		*/
		void setFOVyInRadians(float rads);
		
		/**
		Configura la Dimensión-Y de Field of View(FOV) del frustum CON GRADOS.

		FOV es el angulo entre la posicion del frustum y los "bordes" de la pantalla, en los cuales la escena es proyectada.
		Valores altos (+90 grados) da un resultado a una vista "ojo de pez". Valores bajos (-30 grados) en una vista "telescopica".
		Los valores usuales son 45 o 60 grados.

		@param degrees Este parametro indica el FOV vertical, el horizontal se calcula a partir de este.
		*/
		void setFOVyInDegrees(float degrees);

		/*
		Configura el Near Clip del frustum. Es decir el plano más cercano a la cámara a partir del cual se pintan los objetos

		@param distance Distancia de dicho plano a la cámara
		*/
		void setNearClipDistance(float distance);

		/*
		Configura el Far Clip del frustum. Es decir el plano más lejano a la cámara a partir del cual se pintan los objetos

		@param distance Distancia de dicho plano a la cámara
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
		Clase amiga. Solo la escena tiene acceso a la cámara de Ogre para
		poder crear el viewport.
		*/
		friend class CScene;
		friend class CServer;

		/**
		Devuelve la cámara de Ogre.

		@return puntero a la cámara de Ogre.
		*/
		Ogre::Camera *getCamera() {return _camera;}

		/**
		Nodo que contiene la cámara.
		*/
		Ogre::SceneNode *_cameraNode;

		/**
		Nodo que representa el punto a donde debe mirar la cámara.
		*/
		Ogre::SceneNode *_targetNode;
		
		/**
		La cámara de Ogre.
		*/
		Ogre::Camera *_camera;

		/**
		Controla todos los elementos de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la cámara.
		*/
		std::string _name;

		Ogre::CompositorManager* _compositorManager;

		std::string _actualCameraEffect;

	}; // class CCamera

} // namespace Graphics

#endif // __Graphics_Camera_H
