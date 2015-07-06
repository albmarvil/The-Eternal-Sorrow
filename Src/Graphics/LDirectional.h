//---------------------------------------------------------------------------
// LDirectional.h
//---------------------------------------------------------------------------

/**
@file LDirectional.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica.

@see Graphics::CLDirectional

@author B�rbara Dom�nguez de la Torre Gonz�lez
@date Julio, 2010
*/

#ifndef __Graphics_LDirectional_H
#define __Graphics_LDirectional_H

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
	Clase que representa una luz direccional. Contiene una referencia a
	una entidad de Ogre.
	*/

	class CLDirectional : public CLight
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
		CLDirectional(const std::string &nameLight, const Vector3 &position, const Vector3 &especular,
			const Vector3 &direction, const Vector3 &diffuse);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CLDirectional();

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
		virtual bool load();
		
		/**
		Actualiza el estado de la luz cada ciclo.
		
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
		*/
		virtual void tick(float secs);

		/**
		Direcci�n en la que apunta la luz
		*/
		Vector3 dir;

	}; // class CLDirectional

} // namespace Graphics

#endif // __Graphics_LDirectional_H
