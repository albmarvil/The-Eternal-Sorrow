//---------------------------------------------------------------------------
// LSpotlight.h
//---------------------------------------------------------------------------

/**
@file LSpotlight.h

Contiene la declaración de la clase que representa una entidad gráfica.

@see Graphics::CLSpotlight

@author Bárbara Domínguez de la Torre González
@date Julio, 2010
*/

#ifndef __Graphics_LSpotlight_H
#define __Graphics_LSpotlight_H

#include "BaseSubsystems/Math.h"
#include "Light.h"

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
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	luz (posición, orientación, escala...) se deberán hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la luz debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de éstas, que permitan
	modelos con distintas características.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Enero, 2015
	*/
	class CLSpotlight : public CLight
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
		CLSpotlight(const std::string &type, const Vector3 &position, const Vector3 &especular,
			const Vector3 &direction, const Vector2 &rang, const Vector3 &diffuse);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CLSpotlight();

		/**
		Cambia el rango de la luz
		*/
		void setRange(const Vector2 &rang);

		/**
		Cambia la dirección en la que actúa la luz.

		@param orientation Nueva dirección en la que actúa la lu.
		*/
		void setOrientation(const Vector3 &orientation);

	protected:

		// CScene es la única que puede añadir o eliminar luces de una 
		// escena y por tanto cargar o descargar luces.
		// Por otro lado cada luz debe pertenecer a una escena.
		friend class CScene;
		
		/**
		Carga la luz correspondiente. No hace  comprobaciónes de si la
		luz está ya cargada o de si pertenece a otra escena. Esto se debe
		hacer de manera externa.

		@return true si la luz pudo crear los objetos necesarios en Ogre
		o si la luz ya estaba cargada.
		*/
		bool load();
		
		/**
		Actualiza el estado de la luz cada ciclo.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float secs);

		/**
		Rango en la que actúa la luz
		*/
		Vector2 range;

		/**
		Dirección en la que apunta la luz
		*/
		Vector3 dir;

	}; // class CLSpotlight

} // namespace Graphics

#endif // __Graphics_LSpotlight_H
