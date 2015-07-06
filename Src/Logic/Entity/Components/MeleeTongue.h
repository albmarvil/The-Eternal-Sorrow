#ifndef __Logic_MeleeTongue_H
#define __Logic_MeleeTongue_H

#include "Weapon.h"

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

#include "Graphics/AnimatedEntity.h"

//declaraci�n de la clase
namespace Logic 
{
	class CMeleeTongue : public Weapon, public Graphics::CAnimatedEntityListener
	{
		DEC_FACTORY(CMeleeTongue);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CMeleeTongue() : Weapon(){}
		
		virtual ~CMeleeTongue() {};

	protected:

		/**
		Inicializaci�n del componente usando la descripci�n de la entidad que hay en 
		el fichero de mapa.
		*/
		/*virtual bool OnSpawn(const Map::CEntity *entityInfo);
		virtual bool respawn(const Map::CEntity *entityInfo);*/

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

		void animationFinished(const std::string &animation);

		void ApplyDamage(CEntity* entity);

		void sendSET_ANIMATION(char *animacion, bool loop, bool restart, float multSpeed);

		/*
		Manda un mensaje de activacion o desactivacion de la f�sica

		@param activate true para activar la fisica del arma, false para desactivarla
		*/
		void switchWeaponPhysic (bool activate);

		/*
		Puntero a la entidad animada del player
		*/
		Graphics::CAnimatedEntity* _animEntity;

		/*
		Da�o que se aplicar� con e ataque actual
		*/
		float _actualDamage;

		/* Mapa logico actual*/
		CMap* _logicMap;

		Vector3 _mousePos;

		/*
		Atributo que permite controlar la animaci�n actual
		*/
		std::string _animActual;

		/*
		Atributo que indica el nombre del configurador de animaciones
		Usado para calcular el tiempo de las transiciones
		*/
		std::string _animationConfig;

		int _percentPoison;

	}; // class CArmaD

	REG_FACTORY(CMeleeTongue);

} // namespace Logic

#endif // __Logic_ArmaD_H
