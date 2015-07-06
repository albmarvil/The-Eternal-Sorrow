#ifndef __Graphics_MovableText_H
#define __Graphics_MovableText_H
#include <string>
#include "BaseSubsystems/Math.h"

namespace Graphics
{
	class CScene;
}
namespace Ogre
{
	class SceneNode;
	class MovableText;
}
namespace Graphics
{
	class CMovableText
	{
	public:

		/**
		Constructor de la clase.
		*/

		CMovableText(const std::string &name, const std::string &source, const std::string &fontName = "bluehigh");

		/**
		Destructor de las particulas
		**/
		virtual ~CMovableText();

		/**
		Carga la particula correspondiente.

		@return true si la particula pudo crear los objetos necesarios en Ogre
		o si la particula ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). 
		*/
		void unload();

		/**
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		void tick(float secs);

		void setName(std::string name, int emitter);

		std::string getName() { return _name;}

		void setPosition(const Vector3 &position);

		void setText(const std::string &textoMostrar);

		void setColor(const std::string &color);

		bool setAlpha(float constanteDegradacion);

		void setScale(const Vector3 &scale);

		void setVisible(bool visible);
	private:

		friend class CScene;

		/**
		Añade la particula al SceneManager pasado por parámetro. Si la particula
		no está cargada se fuerza su carga.

		@param scene escena de Ogre a la que se quiere añadir
		la particula.
		@return true si la entidad se pudo cargar y añadir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una particula de la escena en la que se encuentra cargada.

		@return true si la particula se descargo y eliminó de la escena
		correctamente. Si la particula no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();

		//Nombre unico de la particula
		std::string _name;

		//script de la particula
		std::string _text;

		//scene a la que pertenece la particula
		CScene *_scene;

		//flag para saber si se ha cargado la particula
		bool _loaded;

		//nodo al que pertenecera la particula
		Ogre::SceneNode *_sceneNode; 

		Ogre::MovableText *_movableText;

		const std::string _fontName;
	};
}
#endif*/