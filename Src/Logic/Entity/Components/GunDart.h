#ifndef __Logic_GunDart_H
#define __Logic_GunDart_H

#include "Weapon.h"

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

//declaración de la clase
namespace Logic 
{
	class CGunDart : public Weapon
	{
		DEC_FACTORY(CGunDart);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CGunDart() : Weapon(){}
		
		virtual ~CGunDart() {};

	protected:


		/*
		Función virtual y sin implementación.
		Esta función lanzará el ataque principal del arma

		@param mousePos Posicion del raton en el mundo
		@param baseDamage Daño base al cual aplicamos el multiplicador
		*/
		virtual void mainAttack(Vector3 mousePos, float baseDamage);

		/*
		Función virtual y sin implementación.
		Esta función lanzará el ataque especial del arma

		@param mousePos Posicion del raton en el mundo
		@param baseDamage Daño base al cual aplicamos el multiplicador
		*/
		virtual void specialAttack(Vector3 mousePos, float baseDamage);

	}; // class CArmaD

	REG_FACTORY(CGunDart);

} // namespace Logic

#endif // __Logic_ArmaD_H
