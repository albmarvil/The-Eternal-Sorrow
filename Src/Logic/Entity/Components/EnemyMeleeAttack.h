/**
@file EnemyMeleeAttack.h

Contiene la declaración del componente que controla el ataque melee de los enemigos

@see Logic::CEnemyMeleeAttack
@see Logic::IComponent

@author Alberto Martínez
@date Febrero, 2015
*/
#ifndef __Logic_EnemyMeleeAttack_H
#define __Logic_EnemyMeleeAttack_H

#include "Logic/Entity/Component.h"

//declaración de la clase
namespace Logic 
{

	class CEnemyMeleeAttack : public IComponent
	{
		DEC_FACTORY(CEnemyMeleeAttack);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CEnemyMeleeAttack() : IComponent(), _timeAcum(0.0f), /*_count(false),*/ _timeBetweenAttacks(1000.0f){}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);
		/**
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo CONTROLLER_TOUCHED y somos un enemigo y el 
		contrario es el player, le aplicaremos daño
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/*
		Este componente básciamente contea tiempo en su tick para saber si el enemigo puede volver a ejercer daño.
		Una vez acaba este conteo primero averigua si se esta solapando con el enemigo. En caso afirmativo vuelve a aplicar el daño
		correspondiente.

		El solape se resuelve lanzando dos rayos a izquierda y derecha desde el centro de la capsula, a una distancia máxima del radio
		de la capsula del enemigo.
		*/
		virtual void onTick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();
		
		/*
		Predicado que nos indica si puede hacer daño al player
		*/
		bool canDoDamage();

	protected:

		/*
		Esta función manda un mensaje aplicando el daño correspondiente del enemigo a la entidad indicada.
		*/
		void ApplyDamage(CEntity *entity);

		void damageEnemy(Vector3 collisionPoint);
		/*
		Daño del enemigo
		*/
		float _damage;

		/*
		Tiempo entre ataques del enemigo. no podrá realizar dos ataques seguidos hasta que no se cumpla este tiempo
		*/
		float _timeBetweenAttacks;

		/*
		Timepo acumulado
		*/
		float _timeAcum;

		int _targetID;
		/*
		Falg que me dice si debo acumular el tiempo no durante el tick
		*/
		//bool _count;

		/*
		Vector con la dirección izquierda en la lógica (-1 0 0)
		*/
		//Vector3 _left;

		/*
		Vector con la dirección derecha en la lógica (1 0 0)
		*/
		//Vector3 _right;

		/*
		Offset usado para lanzar el rayo desde el centro de la capsula del enemigo
		*/
		float _upOffset;

		/*
		Distancia máxima a la que se lanza el rayo del enemigo
		*/
		float _maxDist;

		//El doble de la distancia maxima al cuadrado
		float _maxDist2;

		/*
		Puntero al player para claculos
		*/
		CEntity* _target;

	}; // class CEnemyMeleeAttack

	REG_FACTORY(CEnemyMeleeAttack);

} // namespace Logic

#endif // __Logic_EnemyMeleeAttack_H
