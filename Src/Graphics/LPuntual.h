//---------------------------------------------------------------------------
// LSpot.h
//---------------------------------------------------------------------------

/**
@file LPuntual.h

Contiene la declaración de la clase que representa una entidad gráfica.

@see Graphics::CLPuntual

@author Bárbara Domínguez de la Torre González
@date Julio, 2010
*/

#ifndef __Graphics_LPuntual_H
#define __Graphics_LPuntual_H

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
	Clase que representa una luz puntual. Contiene una referencia a
	una luz de Ogre.
	*/

	class CLPuntual : public CLight
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
		CLPuntual(const std::string &type, const Vector3 &position, const Vector3 &especular,
			const Vector3 &diffuse);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CLPuntual();

		/*
		Cambia los parámetros del atenmuación de la luz spot
		
		Más info aquí: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
		aqui esta la formula y la explicacion de que hacen estos parametros

		@param range Rango de efecto de la luz
		@param constant
		@param linear
		@param quadratic
		*/
		void setAttenuation(float range, float constant, float linear, float quadratic);

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

	}; // class CLPuntual

} // namespace Graphics

#endif // __Graphics_LPuntual_H
