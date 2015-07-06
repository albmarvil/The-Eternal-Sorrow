//---------------------------------------------------------------------------
// ManualEntity.h
//---------------------------------------------------------------------------

/**
@file ManualEntity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica construida de forma manual.

@see Graphics::CManualEntity

@author Alberto Mart�nez
@date Febrero, 2015
*/

#ifndef __Graphics_ManualEntity_H
#define __Graphics_ManualEntity_H

//#include "Entity.h"
#include "BaseSubsystems/Math.h"
#include "PrefabType.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
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
		Este constructor crear� una entidad manual con la forma de una capsula

		@param name Nombre de la entidad.
		@param height Altura de la capsula
		@param radius Radio de la capsula
		*/
		CManualEntity(const std::string &name, float height, float radius);
		
		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CManualEntity();

		/**
		Cambia la posici�n y orientaci�n de la entidad.

		@param transform Referencia a la matriz de transformaci�n con la 
		que debe posicionarse y orientarse la entidad.
		*/
		void setTransform(const Matrix4 &transform);

		/**
		Cambia la posici�n de la entidad.

		@param position Nueva posici�n para el nodo que contiene la entidad 
		de Ogre.
		*/
		void setPosition(const Vector3& position);

		/**
		Cambia la orientaci�n de la entidad.

		@param orientation Referencia a la matriz de rotaci�n con la que debe 
		orientarse la entidad.
		*/
		void setOrientation(const Matrix3 &orientation);

		/**
		 Establece la propiedad visible de la entidad. Si
		 la entidad est� puesta invisible, no deber�a dibujarse.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.
		 <p>
		 El m�todo cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _ManualEntityNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.

		 @return Cierto si la entidad es visible (est� activa 
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
		Setea el material deseado en la entidad gr�fica. Todas las subentidades tendr�n este material por defecto

		@param material Material a setear en la entidad gr�fica
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
		A�ade la entidad al SceneManager pasado por par�metro. Si la entidad
		no est� cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere a�adir
		la entidad.
		@return true si la entidad se pudo cargar y a�adir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y elimin� de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();

	protected:

		/**
		Carga la entidad gr�fica correspondiente al nombre _mesh. No hace 
		comprobaci�nes de si la entidad est� ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaci�nes de si la entidad est� o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
		*/
		virtual void tick(float secs);


		/*
		Metodo que crea la geometr�a gr�fica de una capsula a partir de los datos de la clase
		*/
		void createCapsule();
		

		/*
		Este m�todo dibuja un circulo en el plano XZ en el manual object que se le pasa

		@param circle ManualObject donde se dibuja el circulo
		@param center Centro del circulo a dibujar
		@param radius Radio del circulo a dibujar
		@param accuracy Precisi�n a la hora de dibujar el circulo
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
		Este m�todo dibuja un circulo en el plano XY en el manual object que se le pasa

		@param circle ManualObject donde se dibuja el circulo
		@param center Centro del circulo a dibujar
		@param zRotation Posible rotacion en Z que le queramos dar al circulo
		@param radius Radio del circulo a dibujar
		@param accuracy Precisi�n a la hora de dibujar el circulo
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
		Quiz�s es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
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
