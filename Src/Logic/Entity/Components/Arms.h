#ifndef __Logic_Arms_H
#define __Logic_Arms_H

#include "Logic/Entity/Component.h"
#include "Graphics/AnimatedEntity.h"

namespace GUI
{
	class CServer;
	class CPlayerController;
}

namespace Logic
{
	class Weapon;
};

//declaración de la clase
namespace Logic 
{
	struct WeaponInArm
	{
		CEntity* entity;
		std::string attackType;
		std::string weaponType;
		std::string usableObjectType;
		Weapon* weaponComponent;
		bool attached;
	};

	class CArms : public IComponent
	{
		DEC_FACTORY(CArms);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CArms() : IComponent(),  _posMouse(Vector3::ZERO), _delayGrabWeapon(0), _leftArmBone(""), 
			_leftHandBone(""), _rightArmBone(""), _rightHandBone(""), _animationConfig(""),
			_baseDamage(0), _baseMainDelay(0), _baseSpecialDelay(0) {}
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		/**
		Este componente sólo acepta mensajes de tipo DAMAGED.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Al recibir un mensaje de tipo DAMAGED la vida de la entidad disminuye.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**

		*/
		virtual bool activate();

		
		void attackOrder(const std::string &arm, bool attack);

		void specialAttackOrder(const std::string &arm, bool attack);

		void const setWeaponComponent (const std::string &arm, Weapon* component);

		void removeWeapons();

		/*
		Inicializa el hueso de la mano indicandoles si ha de hacerlo de forma simetrica o no

		@param simetric Indica la simetria en los huesos de las manos
		*/
		void InitializeHandsBones(bool simetric);

		void changeHandBoneSimetry(const std::string &hand, const Quaternion &qGiro);

		void setWeaponVisible(const std::string &arm, bool visible);

	protected:

		/**
		Auxiliar para crear el arma en el cambio de pantallas
		*/
		bool is_weapon;

		/**
		Contador de delay que debe pasar para poder coger un arma nueva
		*/
		int _delayGrabWeapon;

		/**
		Delay maximo que puede llegar
		*/
		int _maxDelayGrabWeapon;

		/**
		Vector de armas que estamos tocando en cada momento
		*/
		std::vector<WeaponInArm> vecTouching;

		/**
		Armas de los brazos
		*/
		WeaponInArm _weaponR;
		WeaponInArm _weaponL;

		/*Huesos que se van a rotar*/
		std::string _leftArmBone;
		std::string _rightArmBone;

		/*Huesos donde se attachan las armas*/
		std::string _leftHandBone;
		std::string _rightHandBone;

		std::string _animationConfig;
		
		/*
		Entidad animada del player (útil para rotateArms)
		*/
		Graphics::CAnimatedEntity* _animEntity;

		/**
		variable que almacena el grupo de colisión
		*/
		int _collisionGroup;

		/**
		variable que almacena la posición del ratón
		*/
		Vector3 _posMouse;

		/**
		Permite recoger un arma y asignarla a un brazo
		@param arm Brazo en el cual poner el arma
		*/
		void grabWeapon(std::string arm);

		/**
		Permite intercambiar de brazo las armas que se lleven
		*/
		void swapWeapons();

		/**
		Permite elegir el brazo que va a rotar
		*/
		void chooseArmsToRotate();

		/**
		Permite rotar el brazo para que apunte al ratón
		*/
		void rotateArm(std::string boneName);

		/**
		Método que crea las armas al pasar de habitación
		*/
		void weaponPermanency();

		/**
		Función que envía un mensaje de SetAnimation para cambiar la animación de los brazos al coger un arma
		*/
		void SendAnimationMessage(std::string arm);

		/**
		variable que almacena el daño base, este se irá modificando con los potenciadores
		*/
		float _baseDamage;

		/**
		variable que almacena la base del tiempo de carga del ataque normal
		*/
		float _baseMainDelay;

		/**
		variable que almacena la base del tiempo de carga del ataque especial
		*/
		float _baseSpecialDelay;

		/**
		Método que envia un mensaje de control para indicar que el player ha hecho algo y se debe forzar la
		animación de idle
		*/
		void SendControl(std::string message, bool active);

		/**
		parámetro que almacena la distancia máxima a la que sale el disparo
		*/
		float _maxDistance;
	}; // class CArmaD

	REG_FACTORY(CArms);

} // namespace Logic

#endif // __Logic_ArmaD_H