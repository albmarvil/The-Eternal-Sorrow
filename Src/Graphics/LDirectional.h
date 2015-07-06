//---------------------------------------------------------------------------
// LDirectional.h
//---------------------------------------------------------------------------

/**
@file LDirectional.h

Contiene la declaración de la clase que representa una entidad gráfica.

@see Graphics::CLDirectional

@author Bárbara Domínguez de la Torre González
@date Julio, 2010
*/

#ifndef __Graphics_LDirectional_H
#define __Graphics_LDirectional_H

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
	Clase que representa una luz direccional. Contiene una referencia a
	una entidad de Ogre.
	*/

	class CLDirectional : public CLight
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
		CLDirectional(const std::string &nameLight, const Vector3 &position, const Vector3 &especular,
			const Vector3 &direction, const Vector3 &diffuse);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CLDirectional();

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
		virtual bool load();
		
		/**
		Actualiza el estado de la luz cada ciclo.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float secs);

		/**
		Dirección en la que apunta la luz
		*/
		Vector3 dir;

	}; // class CLDirectional

} // namespace Graphics

#endif // __Graphics_LDirectional_H
