//---------------------------------------------------------------------------
// Light.h
//---------------------------------------------------------------------------

/**
@file Light.h

Contiene la declaración de la clase que representa una Luz en Ogre.

@see Graphics::CLight

@author Bárbara Domínguez de la Torre González
@date Julio, 2010
*/

#ifndef __Graphics_Light_H
#define __Graphics_Light_H

#include "BaseSubsystems/Math.h"
#include "Server.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Light;
	class SceneNode;
}

namespace Graphics 
{
	class CScene;
}

namespace Graphics 
{
	/**
	Clase que representa una luz. Contiene una referencia a
	una entidad de Ogre.
	Oculta los detalles escabrosos del motor gráfico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las luces de Ogre. Para cargar la luz en Ogre y que 
	sea reenderizada, ésta debe añadirse a una escena de Ogre mediante el 
	método attachToScene(). Solo la clase CScene puede invocar los métodos 
	para añadir o eliminar luces de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Al añadir una luz a una escena se 
	fuerza su carga en Ogre invocando a load.

	Pueden implementarse clases derivadas de éstas, que permitan
	modelos con distintas características.
	*/
	class CLight 
	{
	public:
		/**
		Constructor de la clase.

		@param name Tipo de la luz.
		@param pos Posición inicial de la luz.
		@param especular Color especular de la luz.
		@param direction Dirección en la que apunta la luz.
		@param range rango de actuación de la luz.
		@param difuse Color difuso de la luz.
		*/
		CLight(const Vector3 &diffuse, const Vector3 &specular, const Vector3 &position, const std::string &nameLight);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CLight();

		/**
		Cambia la posición de la luz.

		@param position Nueva posición para luz de Ogre.
		*/
		virtual void setPosition(const Vector3& position);

		/**
		Cambia el color difuso de la luz
		*/
		virtual void setDiffuseColour(const Vector3 &diffuse);

		/**
		Metodo para poner la visibilidad al estado que se quiera

		@param state booleano que indica si se quiere hacer visible o no
		**/
		virtual void setVisible(bool state);

		/**
		Cambia el color especular de la luz
		*/
		virtual void setSpecularColour(const Vector3 &specular);

	protected:

		/**
		Luz de Ogre.
		*/
		Ogre::Light *light;

		// CScene es la única que puede añadir o eliminar luces de una 
		// escena y por tanto cargar o descargar luces.
		// Por otro lado cada luz debe pertenecer a una escena.
		friend class CScene;
		
		/**
		Añade la luz al SceneManager pasado por parámetro. Si la luz
		no está cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere añadir
		la luz.
		@return true si la luz se pudo cargar y añadir a la escena.
		*/
		virtual bool attachToScene(CScene *sc);

		/**
		Descarga una luz de la escena en la que se encuentra cargada.

		@return true si la luz se descargó y eliminó de la escena
		correctamente. Si la luz no estaba cargada se devuelve false.
		*/
		virtual bool deattachFromScene();

		/**
		Carga la luz correspondiente.

		@return true si la luz pudo crear los objetos necesarios en Ogre
		o si la luz ya estaba cargada.
		*/
		virtual bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load().
		*/
		virtual void unload();
		
		/**
		Actualiza el estado de la luz cada ciclo.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float secs);

		/**
		Quizás es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *scene;

		/**
		Valor del color difuso de la luz
		*/
		Vector3 diff;

		/**
		Indica si la entidad ha sido cargada en el motor gráfico.
		*/
		bool loaded;

		/**
		Nombre de la entidad.
		*/
		std::string name;

		/**
		Valor del color especular de la luz
		*/
		Vector3 spec;

		/**
		Posición de la luz
		*/
		Vector3 pos;

	}; // class CLight

} // namespace Graphics

#endif // __Graphics_Light_H
