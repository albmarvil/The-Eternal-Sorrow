/**
@file RibbonTrail.h

Contiene la declaración de la clase CRibbonTrail

Un RibbonTrail no es más que un sistema de particulas que emite "cuadrados" de forma encadenada. Esta cadena tiene un comienzo
y un fin. El comienzo siempre sigue un nodo de la escena de Ogre mientras que el final esta definido por la longitud del trail

Cuando el ultimo cuadrado sale del espacio visible se traslada automaticamente al inicio del trail.

Un trail se configura con material (Magia!! admite shaders y todas esas cosillas)
Una longitud
Un ancho inicial y un gradiente de cambio del ancho

@author Alberto Martínez Villarán
@date Mayo de 2015

*/


#ifndef __Graphics_RibbonTrail_H
#define __Graphics_RibbonTrail_H

#include <string>
#include "BaseSubsystems/Math.h"

namespace Graphics
{
	class CScene;
	class CEntity;
	class CSceneNode;
}
namespace Ogre
{
	class SceneNode;
	class RibbonTrail;
}

namespace Graphics
{
	class CRibbonTrail
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre del trail.
		*/
		CRibbonTrail(const std::string &name);

		/**
		Destructor del trail
		**/
		virtual ~CRibbonTrail();

		/**
		Carga el trail correspondiente.

		@return true si el trail pudo crear los objetos necesarios en Ogre
		o si el trail ya estaba cargado.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). 
		*/
		void unload();

		/*
		Este método hace que el trail siga el nodo de escena de la entidad grafica indicada
		*/
		void setEntityToFollow(CEntity *entity);

		/*
		Este método hace que el trail siga el TagPoint(si lo tiene) de la entidad gráfica indicada
		*/
		void setTagPointToFollow(CEntity *entity);

		/*
		Este método hace que el trail siga un nodo de escena (encapsulado de Ogre) genérico
		Se usa para seguir el nodo de escena virtual del pivote de las armas y que este aparezca en el centro de las armas
		*/
		void setNodeToFollow(CSceneNode* node);
		
		/*
		Configura el material del trail
		*/
		void setMaterial(const std::string &materialName);
		/**
		 Establece la propiedad visible del trail. Si
		 el trail está puesta invisible, no debería dibujarse.
		 <p>
		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		/*
		Devuelve el nombre del trail
		*/
		std::string getName() { return _name;}

		/*
		Establece la longitud máxima en la cual se emiten los quads del trail
		*/
		void setTrailLength(float lenght);

		/*
		Configura el ancho inicial del trail
		*/
		void setInitialWidth(float width);

		/*
		Configura el cambio del ancho que sufre el trail en cada segundo
		*/
		void setWidthChange(float change);

		/*
		Configura el cambio de color que decrementa el trail enc ada segundo
		*/
		void setColorChange(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);

		/*
		Configura el cambio de color que decrementa el trail enc ada segundo
		*/
		void setInitialColor(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);


	private:

		friend class CScene;

		/**
		Añade el trail al SceneManager pasado por parámetro. Si el trail
		no está cargado se fuerza su carga.

		@param scene escena de Ogre a la que se quiere añadir
		el trail.
		@return true si el trail se pudo cargar y añadir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga el trail de la escena en la que se encuentra cargado.

		@return true si el trail se descargo y eliminó de la escena
		correctamente. Si el trail no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();

		//Nombre unico del trail
		std::string _name;

		std::string _material;

		//scene a la que pertenece el trail
		CScene *_scene;

		//flag para saber si se ha cargado el trail
		bool _loaded;

		//nodo al que pertenecera el trail
		Ogre::SceneNode *_trailNode;

		//trail de Ogre
		Ogre::RibbonTrail *_trail;

		//Entidad a la que sigue el trail
		CEntity *_entToFollow;

		///nodo generico que puede seguir el trail
		CSceneNode *_nodeToFollow;

	};
}
#endif