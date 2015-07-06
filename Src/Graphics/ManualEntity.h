//---------------------------------------------------------------------------
// ManualEntity.h
//---------------------------------------------------------------------------

/**
@file ManualEntity.h

Contiene la declaración de la clase que representa una entidad gráfica construida de forma manual.

@see Graphics::CManualEntity

@author Alberto Martínez
@date Febrero, 2015
*/

#ifndef __Graphics_ManualEntity_H
#define __Graphics_ManualEntity_H

//#include "Entity.h"
#include "BaseSubsystems/Math.h"
#include "PrefabType.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Entity;
	class ManualObject;
	class SceneNode;
}

namespace Graphics 
{
	class CScene;
}

namespace Graphics 
{
	
	enum ManualType
	{
		Capsule,
	};

	class CManualEntity 
	{
	public:

		/**
		Constructor de la clase.
		Este constructor creará una entidad manual con la forma de una capsula

		@param name Nombre de la entidad.
		@param height Altura de la capsula
		@param radius Radio de la capsula
		*/
		CManualEntity(const std::string &name, float height, float radius);
		
		/**
		Destructor de la aplicación.
		*/
		virtual ~CManualEntity();

		/**
		Cambia la posición y orientación de la entidad.

		@param transform Referencia a la matriz de transformación con la 
		que debe posicionarse y orientarse la entidad.
		*/
		void setTransform(const Matrix4 &transform);

		/**
		Cambia la posición de la entidad.

		@param position Nueva posición para el nodo que contiene la entidad 
		de Ogre.
		*/
		void setPosition(const Vector3& position);

		/**
		Cambia la orientación de la entidad.

		@param orientation Referencia a la matriz de rotación con la que debe 
		orientarse la entidad.
		*/
		void setOrientation(const Matrix3 &orientation);

		/**
		 Establece la propiedad visible de la entidad. Si
		 la entidad está puesta invisible, no debería dibujarse.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.
		 <p>
		 El método cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _ManualEntityNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		/**
		 Escala la entidad.

		 @param scale Valores de la escala en los diferentes ejes.
		 */
		void setScale(const Vector3 &scale);

		/**
		 Escala la entidad.

		 @param scale Valor de la escala para los 3 ejes.
		 */
		void setScale(float scale);

		/**
		 Devuelve el valor de la escala de la entidad.

		 @return Valores de la escala en los diferentes ejes.
		*/
		const Vector3 &getScale();

		/**
		Devuelve la entidad de Ogre.

		@return puntero a la entidad de Ogre.
		*/
		Ogre::ManualObject* getManualEntity() {return _ManualEntity;}

		/**
		Setea el material deseado en la entidad gráfica. Todas las subentidades tendrán este material por defecto

		@param material Material a setear en la entidad gráfica
		*/
		void setMaterial(std::string &material);

		
		/**
		Permite establecer un nuevo nombre para dicha entidad
		@param name Nuevo nombre
		*/
		void setName(std::string name) {_name = name;}

		/**
		Devuelve si el nombre
		@return _name Nombre
		*/
		std::string getName() {return _name;}

		/**
		Añade la entidad al SceneManager pasado por parámetro. Si la entidad
		no está cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere añadir
		la entidad.
		@return true si la entidad se pudo cargar y añadir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y eliminó de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();

	protected:

		/**
		Carga la entidad gráfica correspondiente al nombre _mesh. No hace 
		comprobaciónes de si la entidad está ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaciónes de si la entidad está o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float secs);


		/*
		Metodo que crea la geometría gráfica de una capsula a partir de los datos de la clase
		*/
		void createCapsule();
		

		/*
		Este método dibuja un circulo en el plano XZ en el manual object que se le pasa

		@param circle ManualObject donde se dibuja el circulo
		@param center Centro del circulo a dibujar
		@param radius Radio del circulo a dibujar
		@param accuracy Precisión a la hora de dibujar el circulo
		@param startAngle Angulo de inicio para dibujar el circulo
		@param endAngle Angulo de fin para dibujar el circulo
		*/
		void drawCircleShapeXZ(Ogre::ManualObject * circle,
								Ogre::Vector3 center,
								float radius,
								float accuracy,
								float startAngle,
								float endAngle);
		
		/*
		Este método dibuja un circulo en el plano XY en el manual object que se le pasa

		@param circle ManualObject donde se dibuja el circulo
		@param center Centro del circulo a dibujar
		@param zRotation Posible rotacion en Z que le queramos dar al circulo
		@param radius Radio del circulo a dibujar
		@param accuracy Precisión a la hora de dibujar el circulo
		@param startAngle Angulo de inicio para dibujar el circulo
		@param endAngle Angulo de fin para dibujar el circulo
		*/
		void drawCircleShapeXY(Ogre::ManualObject * circle,
								Ogre::Vector3 center, 
								float zRotation,
								float radius,
								float accuracy,
								float startAngle,
								float endAngle);

		/**
		Nodo que contiene la entidad de Ogre.
		*/
		Ogre::SceneNode *_ManualEntityNode;

		/**
		Entidad de Ogre.
		*/
		Ogre::ManualObject *_ManualEntity;

		/**
		Quizás es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		bool _loaded;

		float _height;

		float _radius;

		ManualType _type;
		

	}; // class CManualEntity

} // namespace Graphics

#endif // __Graphics_ManualEntity_H
