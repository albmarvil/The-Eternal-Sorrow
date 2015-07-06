#ifndef __Logic_GunLaser_H
#define __Logic_GunLaser_H

#include "Weapon.h"

#include "Logic/Entity/Component.h"


//declaraci�n de la clase
namespace Logic 
{
	class CGunLaser : public Weapon
	{
		DEC_FACTORY(CGunLaser);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CGunLaser();
		
		virtual ~CGunLaser();

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		
		//virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

	
		//virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		
		//virtual bool activate();

		//virtual void deactivate();

	protected:


		/*
		Funci�n virtual y sin implementaci�n.
		Esta funci�n lanzar� el ataque principal del arma

		@param mousePos Posicion del raton en el mundo
		@param baseDamage Da�o base al cual aplicamos el multiplicador
		*/
		virtual void mainAttack(Vector3 mousePos, float baseDamage);

		/*
		Funci�n virtual y sin implementaci�n.
		Esta funci�n lanzar� el ataque especial del arma

		@param mousePos Posicion del raton en el mundo
		@param baseDamage Da�o base al cual aplicamos el multiplicador
		*/
		virtual void specialAttack(Vector3 mousePos, float baseDamage);

		unsigned int _timeAcumBetweenAttacks;

		unsigned int _timeSpecialBetweenHits;

		CEntity* _specialLaserBeam;


	}; // class CArmaD

	REG_FACTORY(CGunLaser);

} // namespace Logic

#endif // __Logic_ArmaD_H
