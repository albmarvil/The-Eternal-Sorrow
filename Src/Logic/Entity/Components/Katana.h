/*
@file Katana.h

Contiene la delcaraci�n del componente que controla la l�gica y los ataques del arma Katana

@author Alberto Mart�nez Villar�n
@date Febrero de 2015

@refactor Barbara Dom�nguez de la Torre
@date Mayo de 2015

@refactor Alberto Mart�nez Villar�n
@date Mayo de 2015
*/

#ifndef __Logic_Katana_H
#define __Logic_Katana_H

#include "Weapon.h"

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

#include "Graphics/AnimatedEntity.h"

//declaraci�n de la clase
namespace Logic 
{
	class CKatana : public Weapon, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CKatana);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CKatana();
		
		virtual ~CKatana();
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/

		//virtual bool OnSpawn(const Map::CEntity *entityInfo);

	
		//virtual bool respawn(const Map::CEntity *entityInfo);

		
		virtual bool activate();

		virtual void deactivate();
		/**
		Este componente s�lo acepta mensajes de tipo TOUCHED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo TOUCHED lanzaremos el ataque correspondiente, especial o principal.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/*
		Listener de finalizaci�n de animaci�n
		*/
		virtual void animationFinished(const std::string &animation);

		/*
		Setea la animaci�n deseada. Lo hace comparando primero con la animacion actual, solo la cambia si son diferentes
		Optimizado para no enviar un mensaje innecesario en cada tick

		@param animacion Cadena de texto que indica a que animacion cambiar
		*/
		void sendSET_ANIMATION(char* animacion, bool loop, bool restart, float speed);

	protected:

		/*
		Puntero a la entidad animada del player
		*/
		Graphics::CAnimatedEntity* _playerAnimatedEntity;

		/*
		Puntero al player
		*/
		CEntity* _player;
		/*
		Da�o que se aplicar� con e ataque actual
		*/
		float _actualDamage;

		/* Mapa logico actual*/
		CMap* _logicMap;


		/*
		Atributo que permite controlar la animaci�n actual
		*/
		std::string _animActual;

		/*
		Atributo que indica el nombre del configurador de animaciones
		Usado para calcular el tiempo de las transiciones
		*/
		std::string _animationConfig;

		/*
		Aplica el da�o configurado como actual al objetivo alcanzado
		*/
		void ApplyDamage(CEntity* entity);

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

		/*
		Manda un mensaje de activacion o desactivacion de la f�sica

		@param activate true para activar la fisica del arma, false para desactivarla
		*/
		void switchWeaponPhysic (bool activate);

		/*
		Manda un mensaje de activacion o desactivacion del trail de la entidad

		@param activate true para activar el trail del arma, false para desactivarlo
		*/
		void switchWeaponTrail (bool activate);

		/*
		Atributo que indica el nombre del configurador de animaciones
		Usado para calcular el tiempo de las transiciones
		*/
		//std::string _animationConfig;


	}; // class CArmaD

	REG_FACTORY(CKatana);

} // namespace Logic

#endif // __Logic_ArmaD_H
