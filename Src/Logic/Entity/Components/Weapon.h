/**
@file Weapon.h

Contiene la declaración de la interfaz de ataque usada por el player

@see Logic::Weapon
@see Logic::IComponent

@author Alberto Martínez
@date Marzo, 2015
*/


#ifndef __Logic_Weapon_H
#define __Logic_Weapon_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

namespace Logic
{
	class CArms;
};

//declaración de la clase
namespace Logic 
{

	class Weapon : public IComponent
	{
		DEC_FACTORY(Weapon);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		Weapon() : IComponent(), _delayMainAtk(0), _delaySpecialAtk(0), _specialPressed(false), 
			_attackPressed(false), _baseDamage(0), _arm(""), _armComponent(0), _mainAttackSpeedAnimation(1.0f){};

		virtual ~Weapon(){};
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

	
		virtual bool respawn(const Map::CEntity *entityInfo);

		
		virtual bool activate();

	
		virtual void deactivate(){};

		/**
		Método llamado en cada frame que actualiza el estado del componente.

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Este componente sólo acepta mensajes de tipo ATTACK.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo ATTACK lanzaremos el ataque correspondiente, especial o principal.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);


		void attackOrder(bool attack, float baseDamage, float mainDelay, float specialDelay);

		void specialAttackOrder(bool attack, float baseDamage, float mainDelay, float specialDelay);

	
	protected:

		/*
		Función virtual y sin implementación.
		Esta función lanzará el ataque principal del arma

		@param mousePos Posicion del raton en el mundo
		@param baseDamage Daño base al cual aplicamos el multiplicador
		*/
		virtual void mainAttack(Vector3 mousePos, float baseDamage){};

		/*
		Función virtual y sin implementación.
		Esta función lanzará el ataque especial del arma

		@param mousePos Posicion del raton en el mundo
		@param baseDamage Daño base al cual aplicamos el multiplicador
		*/
		virtual void specialAttack(Vector3 mousePos, float baseDamage){};

		/*
		Multiplicador de daño que se aplica al daño base del arma
		*/
		float _mainDamageMultiplier;

		/*
		Multiplicador de daño que se aplica al daño base del arma
		*/
		float _specialDamageMultiplier;

		/*
		Tiempo acumulado entre ataques principales
		*/
		float _delayMainAtk;

		/*
		Tiempo acumulado entre ataques secundarios
		*/
		float _delaySpecialAtk;
		
		/*Tiempo entre ataques principales
		*/
		float _maxDelayMainAtk;

		/*Tiempo entre ataques secundarios
		*/
		float _maxDelaySpecialAtk;

		/* Indica si está pulsando un botón actualmente
		*/
		bool _isPressingAttackButton;

		/**
		variable que almacena el multiplicado del special delay
		*/
		float _multVelSpecialAttack;

		/**
		variable que almacena el multiplicador del main delay
		*/
		float _multVelMainAttack;
		
		
		/* Posición global del ratón
		*/
		Vector3 _posMouse;

		/* Indica si esta siendo pulsada la tecla de ataque especial
		*/
		bool _specialPressed;

		/* Indica que tecla de ratón está siendo pulsada
		*/
		bool _attackPressed;

		/* Indica el daño que produce actualmente el arma
		*/
		float _baseDamage;

		/* Indica si el ataque especial aun continua */
		bool _specialStillOn;
	
		/* Indica  la duración máxima del ataque especial */
		int _maxDurationSpecial;

		/* Indica cuanto tiempo lleva activo el special */
		int _timeSpecialActive;

		/*
		Indicador del brazo en el cual esta attachado el arma
		*/
		std::string _arm;

		CArms* _armComponent;

		/**
		parámetro que almacena la velocidad a la que se reproduce la animación
		*/
		float _mainAttackSpeedAnimation;
		
	}; // class Attack

	REG_FACTORY(Weapon);

} // namespace Logic

#endif // __Logic_AttackD_H
