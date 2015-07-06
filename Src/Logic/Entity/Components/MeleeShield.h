
#ifndef __Logic_MeleeShield_H
#define __Logic_MeleeShield_H

#include "Weapon.h"

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

#include "Graphics/AnimatedEntity.h"

//declaraci�n de la clase
namespace Logic 
{
	class CMeleeShield : public Weapon, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CMeleeShield);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CMeleeShield();
		
		virtual ~CMeleeShield();
		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

	
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

		virtual void onTick(unsigned int msecs);

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


		void setShieldVisible(bool visible);

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

		/**
		este par�metro nos permite saber cuando el escudo esta en el aire
		*/
		bool _shieldReleased;


		/*
		Material de sangre del escudo
		*/
		std::string _material;
		std::string _materialSangre;

		unsigned int _cleanUpTime;
		unsigned int _timeAcum;

		bool _countToClean;

		void sendSET_MATERIAL(const std::string &material);


	}; // class CArmaD

	REG_FACTORY(CMeleeShield);

} // namespace Logic

#endif // __Logic_ArmaD_H
