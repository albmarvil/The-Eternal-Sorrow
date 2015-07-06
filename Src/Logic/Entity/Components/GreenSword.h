/*
@file GreenSword.h

Contiene la delcaraci�n del componente que controla la l�gica y los ataques del arma GreenSword

@author Alberto Mart�nez Villar�n
@date Junio de 2015
*/

#ifndef __Logic_GreenSword_H
#define __Logic_GreenSword_H

#include "Weapon.h"

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

#include "Graphics/AnimatedEntity.h"

//declaraci�n de la clase
namespace Logic 
{
	class CGreenSword : public Weapon, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CGreenSword);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CGreenSword() : Weapon(), _materialCount(0), _material1("-"), _material2("-"), _material3("-"), _material4("-"), _material5("-")
			, _slash1("-"), _slash2("-"), _slash3("-"), _slash4("-"), _slash5("-"), _numGolpesActuales(0){};
		
		virtual ~CGreenSword();
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

	
		virtual bool respawn(const Map::CEntity *entityInfo);

		
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

		void sendSET_MATERIAL(const std::string &material);

	protected:

		/*
		Metodo usado para comprobar si el numero de golpes actual requiere actualizar el material de la espada verde
		*/

		void updateMaterial();

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

		void sendSET_ANIMATION(char *animacion, bool loop, bool restart, float speed, std::string animationSet);
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

		int _materialCount;

		int _numGolpesActuales;

		std::string _material1;
		std::string _material2;
		std::string _material3;
		std::string _material4;
		std::string _material5;


		std::string _slash1;
		std::string _slash2;
		std::string _slash3;
		std::string _slash4;
		std::string _slash5;


	}; // class CArmaD

	REG_FACTORY(CGreenSword);

} // namespace Logic

#endif // __Logic_ArmaD_H
