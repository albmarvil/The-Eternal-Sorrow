//---------------------------------------------------------------------------
// LSpotlight.h
//---------------------------------------------------------------------------

/**
@file LSpotlight.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica.

@see Graphics::CLSpotlight

@author B�rbara Dom�nguez de la Torre Gonz�lez
@date Julio, 2010
*/

#ifndef __Graphics_LSpotlight_H
#define __Graphics_LSpotlight_H

#include "BaseSubsystems/Math.h"
#include "Light.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
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
	Oculta los detalles escabrosos del motor gr�fico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las luces de Ogre. Para cargar la luz en Ogre y que 
	sea reenderizada, �sta debe a�adirse a una escena de Ogre mediante el 
	m�todo attachToScene(). Solo la clase CScene puede invocar los m�todos 
	para a�adir o eliminar luces de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Al a�adir una luz a una escena se 
	fuerza su carga en Ogre invocando a load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	luz (posici�n, orientaci�n, escala...) se deber�n hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la luz debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de �stas, que permitan
	modelos con distintas caracter�sticas.
	
	@ingroup graphicsGroup

	@author David Llans�
	@date Enero, 2015
	*/
	class CLSpotlight : public CLight
	{
	public:

		/**
		Constructor de la clase.

		@param name Tipo de la luz.
		@param pos Posici�n inicial de la luz.
		@param especular Color especular de la luz.
		@param direction Direcci�n en la que apunta la luz.
		@param range rango de actuaci�n de la luz.
		@param difuse Color difuso de la luz.
		*/
		CLSpotlight(const std::string &type, const Vector3 &position, const Vector3 &especular,
			const Vector3 &direction, const Vector2 &rang, const Vector3 &diffuse);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CLSpotlight();

		/**
		Cambia el rango de la luz
		*/
		void setRange(const Vector2 &rang);

		/**
		Cambia la direcci�n en la que act�a la luz.

		@param orientation Nueva direcci�n en la que act�a la lu.
		*/
		void setOrientation(const Vector3 &orientation);

	protected:

		// CScene es la �nica que puede a�adir o eliminar luces de una 
		// escena y por tanto cargar o descargar luces.
		// Por otro lado cada luz debe pertenecer a una escena.
		friend class CScene;
		
		/**
		Carga la luz correspondiente. No hace  comprobaci�nes de si la
		luz est� ya cargada o de si pertenece a otra escena. Esto se debe
		hacer de manera externa.

		@return true si la luz pudo crear los objetos necesarios en Ogre
		o si la luz ya estaba cargada.
		*/
		bool load();
		
		/**
		Actualiza el estado de la luz cada ciclo.
		
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
		*/
		virtual void tick(float secs);

		/**
		Rango en la que act�a la luz
		*/
		Vector2 range;

		/**
		Direcci�n en la que apunta la luz
		*/
		Vector3 dir;

	}; // class CLSpotlight

} // namespace Graphics

#endif // __Graphics_LSpotlight_H
