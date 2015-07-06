//---------------------------------------------------------------------------
// LSpot.h
//---------------------------------------------------------------------------

/**
@file LPuntual.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica.

@see Graphics::CLPuntual

@author B�rbara Dom�nguez de la Torre Gonz�lez
@date Julio, 2010
*/

#ifndef __Graphics_LPuntual_H
#define __Graphics_LPuntual_H

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
	Clase que representa una luz puntual. Contiene una referencia a
	una luz de Ogre.
	*/

	class CLPuntual : public CLight
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
		CLPuntual(const std::string &type, const Vector3 &position, const Vector3 &especular,
			const Vector3 &diffuse);

		/**
		Destructor de la aplicaci�n.
		*/
		virtual ~CLPuntual();

		/*
		Cambia los par�metros del atenmuaci�n de la luz spot
		
		M�s info aqu�: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
		aqui esta la formula y la explicacion de que hacen estos parametros

		@param range Rango de efecto de la luz
		@param constant
		@param linear
		@param quadratic
		*/
		void setAttenuation(float range, float constant, float linear, float quadratic);

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

	}; // class CLPuntual

} // namespace Graphics

#endif // __Graphics_LPuntual_H
