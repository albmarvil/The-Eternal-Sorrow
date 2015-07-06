#ifndef __Logic_Message_H
#define __Logic_Message_H

#include <string>
#include "BaseSubsystems/Math.h"

//Extraigo el nombre de la clase, del mensaje pasado como parametro
#define nombreMensaje(mensaje) mensaje.substr(mensaje.find("::")+2, mensaje.find("*")-mensaje.find("::")-3)

// Predeclaraciones
namespace Logic {
	class CEntity;
};

/*
Jerarquia de clases, donde tenemos una clase Base que es de la cual heredaran todos los mensajes. Cada uno de los mensajes tiene
los atributos como privados (atributos publicos como antes = malo), con sus metodos getters y setters. Además, para extraer
el tipo de cada una de las clases, hago uso de la macro definida junto las cabeceras, que se encarga de extraer el nombre de la clase
de lo devuelto por el typeid. 

En cada uno de los parametros de los metodos accept y process de los componentes contiene un parametro de tipo
	const std::shared_ptr<Logic::IMessage> &message
que significa que es un puntero inteligente de tipo IMessage. Posteriormente para extraer el mensaje especifico que se esta recibiendo
se hace un dynamic_cast : 

	if(message->getType().compare("CONTROL") == 0)
		{
			if(!dynamic_cast<CONTROL*>(message.get())->getString().compare("walk"))

	Lo que hago es comparar el tipo almacenado por el mensaje. Y hago un dynamic cast de un puntero del mensaje especifico. Para extraer
	el mensaje se usa el metodo get() y luego ya podemos acceder a los metodos de las clases. 

*/
namespace Logic
{
	//Clase Base de la cual heredaran todos los tipos de mensajes. Se trata de una clase abstracta
	class IMessage 
	{
	public:

		IMessage(){}
		virtual ~IMessage(){}
		const std::string &getType(){return _type;}
	protected:
		std::string _type;
	};

	//MENSAJE SET_TRANSFORM: Sirve para 
	/*class SET_TRANSFORM : public IMessage
	{
	public:

		SET_TRANSFORM(): IMessage() { _type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_TRANSFORM(){};
		void setMatrix4(const Matrix4 &transform){_transform = transform;}
		Matrix4 getTransform(){ return _transform;}

	private:
		Matrix4 _transform;
	};*/

	//MENSAJE SET_TRANSFORM_BONE: Sirve para enviar la matriz4 de un hueso
	/*class SET_TRANSFORM_BONE : public IMessage
	{
	public:

		SET_TRANSFORM_BONE(): IMessage() { _type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_TRANSFORM_BONE(){};

		void setString(const std::string &string){ _string = string;}
		void setTransform(const Matrix4 &transform){_transform = transform;}
		Matrix4 getTransform(){ return _transform;}
		const std::string &getString(){ return _string;}

	private:
		Matrix4 _transform;
		std::string _string;
	};*/

	//MENSAJE ADD_PARTICLE_OBSERVER: Sirve para avisar de que ha de añadirse un observer
	class ADD_PARTICLE_OBSERVER : public IMessage
	{
	public:

		ADD_PARTICLE_OBSERVER(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~ADD_PARTICLE_OBSERVER(){};
	};

	//MENSAJE DELETE_PARTICLE_OBSERVER: Sirve para avisar de que ha de eliminarse un observer
	class DELETE_PARTICLE_OBSERVER : public IMessage
	{
	public:

		DELETE_PARTICLE_OBSERVER(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~DELETE_PARTICLE_OBSERVER(){};
	};

	//MENSAJE DE_ACTIVATE_PARTICLE: Sirve para avisar de que ha de des/activarse una particula
	class DE_ACTIVATE_PARTICLE : public IMessage
	{
	public:

		DE_ACTIVATE_PARTICLE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~DE_ACTIVATE_PARTICLE(){};

		void setString(const std::string &string){ _string = string;}
		void setBool(bool boolean) {_boolean = boolean;}
		bool getBool() {return _boolean;}
		const std::string &getString(){ return _string;}

	private:
		std::string _string;
		bool _boolean;

	};

	//MENSAJE CHANGE_ATTRIBUTE_PARTICLE: Sirve para que una entidad pegue al hueso otra entidad
	class CHANGE_ATTRIBUTE_PARTICLE : public IMessage
	{
	public:

		CHANGE_ATTRIBUTE_PARTICLE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CHANGE_ATTRIBUTE_PARTICLE(){};

		void setSource(const std::string &string){ _source = string;}
		void setAttribute(const std::string &string){ _attribute = string;}
		const std::string &getSource(){ return _source;}
		const std::string &getAttribute(){ return _attribute;}

	private:
		std::string _source;
		std::string _attribute;
	};

	//MENSAJE LASER_TO_LASERBALL: Envia un mensaje si el laser se ha tocado con la bola laser
	class LASER_TO_LASERBALL : public IMessage
	{
	public:

		LASER_TO_LASERBALL(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~LASER_TO_LASERBALL(){};

	};

	//MENSAJE SET_ANIMATION : Sirve para configurar una nueva animación haciendo interpolación si hay ya una animación activa
	/*class SET_ANIMATION : public IMessage
	{
	public:

		SET_ANIMATION(): IMessage() {
			_type = nombreMensaje(std::string(typeid(this).name()));
			_restart = false;
			_duration = 0.0f;
			_animSet = "default";
			_speed = 1.0f;
		}
		virtual ~SET_ANIMATION(){};

		const std::string &getAnimation(){ return _anim;}
		bool getLoop(){ return _loop;}
		void setAnimation(const std::string &animation){ _anim = animation;}
		void setLoop(bool loop){ _loop = loop;}

		bool getRestartFlag(){ return _restart;}
		void setRestartFlag(bool booleano){ _restart = booleano;}

		const std::string &getAnimationSet() { return _animSet; }
		void setAnimationSet(const std::string &animSet) { _animSet = animSet; }

		float getDuration() { return _duration; }
		void setDuration(float duration) { _duration = duration; }

		float getSpeed() { return _speed; }
		void setSpeed(float speed) { _speed = speed; }

	private:
		std::string _anim;
		std::string _animSet;
		bool _loop;
		bool _restart;
		float _duration;
		float _speed;
	};*/

	//MENSAJE STOP_ANIMATION: Sirve para 
	class STOP_ANIMATION : public IMessage
	{
	public:

		STOP_ANIMATION(): IMessage() 
		{
			_type = nombreMensaje(std::string(typeid(this).name()));
			_animSet = "default";
		}
		virtual ~STOP_ANIMATION(){};

		const std::string &getAnimation(){ return _anim;}
		void setAnimation(const std::string &animation){ _anim = animation;}

		const std::string &getAnimationSet() { return _animSet; }
		void setAnimationSet(const std::string &animSet) { _animSet = animSet; }

	private:
		std::string _anim;
		std::string _animSet;
	};

	//MENSAJE CONTROL: Sirve para 
	class CONTROL : public IMessage
	{
	public:

		CONTROL(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CONTROL(){};
		const std::string &getString(){ return _string;}
		int getInt(){ return _int;}
		bool getBool(){ return _bool;}
		void setString(const std::string &string){ _string = string;}
		void setInt(int entero){ _int = entero;}
		void setBool(bool booleano){ _bool = booleano;}
	private:
		std::string _string;
		int _int;
		bool _bool;
	};

	class START_COUNT : public IMessage
	{
	public:

		START_COUNT(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~START_COUNT(){};

		std::string getString(){return _action;}
		void setString(std::string action){_action = action;}

	private:
			std::string _action;
	};

	//MENSAJE FALLING: Sirve para 
	class FALLING : public IMessage
	{
	public:

		FALLING(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~FALLING(){};

		bool getBool(){ return _bool;}
		void setBool(bool booleano){ _bool = booleano;}

	private:
		bool _bool;
	};

	//MENSAJE UP_Collision: Sirve para avisar al AvatarController de que el personaje esta colisionando por la parte superior
	class UP_COLLISION : public IMessage
	{
	public:

		UP_COLLISION(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~UP_COLLISION(){};

		bool getBool(){ return _bool;}
		void setBool(bool booleano){ _bool = booleano;}

	private:
		bool _bool;
	};

	//MENSAJE SIDE_COLLISION: Sirve para avisar al AvatarController de que el personaje esta colisionando por los lados
	class SIDE_COLLISION : public IMessage
	{
	public:

		SIDE_COLLISION(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SIDE_COLLISION(){};

		bool getBool(){ return _bool;}
		void setBool(bool booleano){ _bool = booleano;}

		int getNormalCollision() {return _normalCollision; }
		void setNormalCollision (int normal) { _normalCollision = normal; }

	private:
		bool _bool;
		//-1 para derecha y 1 para izquierda
		int _normalCollision;
	};

	//MENSAJE ATTACK_ORDER:
	/*class ATTACK_ORDER : public IMessage
	{
	public:

		ATTACK_ORDER(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~ATTACK_ORDER(){};

		void setString(const std::string &string){ _string = string;}
		void setBool(bool boolean){ _boolean = boolean;}
		const std::string &getString(){ return _string;}
		bool getBool(){return _boolean;}
	private:
		std::string _string;
		bool _boolean;
	};*/


	//MENSAJE SPECIAL_ATTACK_ORDER: 
	/*class SPECIAL_ATTACK_ORDER : public IMessage
	{
	public:

		SPECIAL_ATTACK_ORDER(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SPECIAL_ATTACK_ORDER(){};

		void setString(const std::string &string){ _string = string;}
		void setBool(bool boolean){ _boolean = boolean;}
		const std::string &getString(){ return _string;}
		bool getBool(){return _boolean;}
	private:
		std::string _string;
		bool _boolean;
	};*/

	//MENSAJE SET_MATERIAL:  Cambia el material
	class SET_MATERIAL : public IMessage
	{
	public:

		SET_MATERIAL(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_MATERIAL(){};

		void setString(const std::string &string){ _string = string;}
		const std::string &getString(){ return _string;}
	private:
		std::string _string;
	};

	//MENSAJE CANIBAL: Sirve para indicar a un enemigo que tiene ese estado alterado activo
	class CANIBAL : public IMessage
	{
	public:

		CANIBAL(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CANIBAL(){};
		
		bool getBool(){ return _bool;}
		void setBool(bool booleano){ _bool = booleano;}
	private:
		bool _bool;
	};

	//MENSAJE CHANGE_TARGET: Sirve para que una entidad cambie su objetivo a otra
	class CHANGE_TARGET : public IMessage
	{
	public:

		CHANGE_TARGET(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CHANGE_TARGET(){};

		void setLogicEntity(Logic::CEntity* lEnt) { _lEntity = lEnt;}

		Logic::CEntity* getLogicEntity(){ return _lEntity;}

	private:
		Logic::CEntity* _lEntity;
	};

	//Este mensaje sirve para indicar que la tecla de usar esta activada
	class USE : public IMessage
	{
	public:

		USE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~USE(){};

		void setBool(bool boolean){ _boolean = boolean;}
		bool getBool(){return _boolean;}

	private:
		bool _boolean;
	};

	//MENSAJE CAN_MOVE: Sirve para mandar si se puede mover una entidad o no
	class CAN_MOVE : public IMessage
	{
	public:

		CAN_MOVE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CAN_MOVE(){};

		void setBool(bool boolean){ _boolean = boolean;}
		bool getBool(){return _boolean;}
	private:
		bool _boolean;
	};

	//MENSAJE CAN_TIME_TO_LIVE: Sirve para mandar si puede morir por tiempo o no
	class CAN_TIME_TO_LIVE : public IMessage
	{
	public:

		CAN_TIME_TO_LIVE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CAN_TIME_TO_LIVE(){};

		void setBool(bool boolean){ _boolean = boolean;}
		bool getBool(){return _boolean;}
	private:
		bool _boolean;
	};

	//MENSAJE CAN_IA: Sirve para mandar si se puede realizar sus IAs o no
	class CAN_IA : public IMessage
	{
	public:

		CAN_IA(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CAN_IA(){};

		void setBool(bool boolean){ _boolean = boolean;}
		bool getBool(){return _boolean;}
	private:
		bool _boolean;
	};


	//MENSAJE ATTACK: Sirve para indicar que ataque se ha hecho, cuanto quita y si es el especial o no
	/*class ATTACK_STATE : public IMessage
	{
	public:

		ATTACK_STATE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~ATTACK_STATE(){};

		void setMousePressed(bool boolean){ _mousePressed = boolean;}
		bool getMousePressed(){return _mousePressed;}

		void setBaseDamage(float damage) { _damage = damage; }
		float getBaseDamage() { return _damage; }

		void setSpecialDelay(float delay) {_specialDelay = delay;}
		float getSpecialDelay(){ return _specialDelay;}

		void setMainDelay(float delay){_mainDelay = delay;}
		float getMainDelay(){return _mainDelay; }

	private:
		bool _mousePressed;
		float _damage;
		float _specialDelay;
		float _mainDelay;
	};*/

	/*class SPECIAL_ATTACK_STATE : public IMessage
	{
	public:

		SPECIAL_ATTACK_STATE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SPECIAL_ATTACK_STATE(){};

		void setMousePressed(bool boolean){ _mousePressed = boolean;}
		bool getMousePressed(){return _mousePressed;}

		void setBaseDamage(float damage) { _damage = damage; }
		float getBaseDamage() { return _damage; }

		void setSpecialDelay(float delay) {_specialDelay = delay;}
		float getSpecialDelay(){ return _specialDelay;}

		void setMainDelay(float delay){_mainDelay = delay;}
		float getMainDelay(){return _mainDelay; }

	private:
		bool _mousePressed;
		float _damage;
		float _specialDelay;
		float _mainDelay;
	};*/

	//MENSAJE SET_VEL_MULT: Sirve para establecer un multiplicador de velocidad
	class SET_VEL_MULT : public IMessage
	{
	public:

		SET_VEL_MULT(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_VEL_MULT(){};

		void setFloat(float flo) { _float = flo; }
		float getFloat() { return _float; }
	private:
		float _float;
	};

	//MENSAJE ROTATE_ARMS: Sirve para rotar los brazos según el ratón
	class ROTATE_ARMS : public IMessage
	{
	public:

		ROTATE_ARMS(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~ROTATE_ARMS(){};

		void setVector3(Vector3 position) { _pos = position; }
		Vector3 getVector3() { return _pos; }
	private:
		Vector3 _pos;
	};

	//MENSAJE DESTINATION_POS: Sirve para indicar la posicion de destino 
	class DESTINATION_POS : public IMessage
	{
	public:

		DESTINATION_POS(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~DESTINATION_POS(){};

		void setVector3(Vector3 position) { _pos = position; }
		Vector3 getVector3() { return _pos; }
	private:
		Vector3 _pos;
	};

	//MENSAJE MOUSE_POS: Sirve para mandar la posición del cursor 
	class MOUSE_POS : public IMessage
	{
	public:

		MOUSE_POS(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~MOUSE_POS(){};

		void setVector3(Vector3 position) { _pos = position; }

		Vector3 getVector3() { return _pos; }
	private:
		Vector3 _pos;
	};
	
	//MENSAJE STOP_TARGETING_CAM: Sirve para que la camara se fije o no en un objetivo
	class STOP_TARGETING_CAM : public IMessage
	{
	public:

		STOP_TARGETING_CAM(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~STOP_TARGETING_CAM(){};

		void setBool(bool boolean) { _bool = boolean; }
		bool getBool() { return _bool; }

		void setVector3(Vector3 position) { _pos = position; }
		Vector3 getVector3() { return _pos; }
	
	private:
		bool _bool;
		Vector3 _pos;
	};

	//MENSAJE SHAKE: Sirve para mandar un temblor
	class SHAKE : public IMessage
	{
	public:

		SHAKE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SHAKE(){};

		void setMaxShake(float max) { _max = max; }
		void setMinShake(float min) { _min = min; }
		float getMaxShake() { return _max; }
		float getMinShake() { return _min; }
	private:
		float _max;
		float _min;
	};

	//MENSAJE SET_SCALE_PARTICLE: Sirve para escalar una particula
	class SET_SCALE_PARTICLE : public IMessage
	{
	public:

		SET_SCALE_PARTICLE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_SCALE_PARTICLE(){};

		void setScale(float scale) { _scale = scale; }
		void setOwner(CEntity* owner) { _owner = owner;}
		void setNameParticle(std::string name) {_namePart = name;}
		float getScale() { return _scale; }
		CEntity* getOwner() {return _owner;}
		std::string getSourceParticle() {return _namePart;}

	private:
		float _scale;
		CEntity* _owner;
		std::string _namePart;
	};

	//MENSAJE OPEN_CHEST: Sirve para abrir un cofre
	class OPEN_CHEST : public IMessage
	{
	public:

		OPEN_CHEST(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~OPEN_CHEST(){};
	};

	//MENSAJE OPEN_CHEST: Sirve para cancelar la apertura de un cofre
	class CANCEL_OPEN : public IMessage
	{
	public:

		CANCEL_OPEN(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CANCEL_OPEN(){};
	};

	////MENSAJE ATTACH: Sirve para que una entidad pegue al hueso otra entidad
	//class ATTACH : public IMessage
	//{
	//public:

	//	ATTACH(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
	//	virtual ~ATTACH(){};

	//	void setString(const std::string &string){ _string = string;}
	//	void setLogicEntity(Logic::CEntity* lEnt) { _lEntity = lEnt;}

	//	const std::string &getString(){ return _string;}
	//	Logic::CEntity* getLogicEntity(){ return _lEntity;}

	//private:
	//	std::string _string;
	//	Logic::CEntity* _lEntity;
	//};

	////MENSAJE DEATTACH: Sirve para que una entidad pegue al hueso otra entidad
	//class DEATTACH : public IMessage
	//{
	//public:

	//	DEATTACH(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
	//	virtual ~DEATTACH(){};

	//	void setString(const std::string &string){ _string = string;}
	//	void setLogicEntity(Logic::CEntity* lEnt) { _lEntity = lEnt;}

	//	const std::string &getString(){ return _string;}
	//	Logic::CEntity* getLogicEntity(){ return _lEntity;}

	//private:
	//	std::string _string;
	//	Logic::CEntity* _lEntity;
	//};

	//MENSAJE GRAB_WEAPON: Sirve para que un jugador obtenga un arma en el brazo indicado
	class GRAB_WEAPON : public IMessage
	{
	public:

		GRAB_WEAPON(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~GRAB_WEAPON(){};

		void setString(const std::string &string){ _string = string;}

		const std::string &getString(){ return _string;}
	private:
		std::string _string;
	};

	//MENSAJE GET_LOGIC_ENTITY: Sirve para enviar una entidad logica
	class GET_LOGIC_ENTITY : public IMessage
	{
	public:

		GET_LOGIC_ENTITY(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~GET_LOGIC_ENTITY(){};

		CEntity *getEntidad(){ return _entity;}
		void setEntity(CEntity *entidad){ _entity = entidad;}

	private:
		CEntity *_entity;
	};

	//MENSAJE START_PARTICLES: Sirve para iniciar o no particulas
	class START_PARTICLES : public IMessage
	{
	public:

		START_PARTICLES(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~START_PARTICLES(){};

		bool getBool(){ return _bool;}
		void setBool(bool boolean){ _bool = boolean;}

	private:
		bool _bool;
	};

	//MENSAJE IS_TOUCHED: Sirve para indicar si está siendo tocado o no
	class IS_TOUCHED : public IMessage
	{
	public:

		IS_TOUCHED(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~IS_TOUCHED(){};

		bool getBool(){ return _bool;}
		void setBool(bool boolean){ _bool = boolean;}

	private:
		bool _bool;
	};

	//MENSAJE TOUCHED: Sirve para indicar que se ha chocado contra un trigger. OJO la entidad puede ser NULL
	class TOUCHED : public IMessage
	{
	public:

		TOUCHED(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~TOUCHED(){};

		CEntity *getEntidad(){ return _entity;}
		void setEntity(CEntity *entidad){ _entity = entidad;}

	private:
		CEntity *_entity;
	};

	//MENSAJE UNTOUCHED: Sirve para indicar que se ha dejado de chocar contra un trigger. OJO la entidad puede ser NULL
	class UNTOUCHED : public IMessage
	{
	public:

		UNTOUCHED(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~UNTOUCHED(){};

		CEntity *getEntidad(){ return _entity;}
		void setEntity(CEntity *entidad){ _entity = entidad;}
	private:
		CEntity *_entity;
	};


	//MENSAJE TOUCHING_WEAPON: Sirve para indicar que se ha chocado contra un arma recogible
	class TOUCHING_WEAPON : public IMessage
	{
	public:

		TOUCHING_WEAPON(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~TOUCHING_WEAPON(){};

		CEntity *getEntidad(){ return _entity;}
		void setEntity(CEntity *entidad){ _entity = entidad;}

		void setStringWeaponType(const std::string &WeaponType){ _WeaponType = WeaponType;}
		const std::string &getStringWeaponType(){ return _WeaponType;}

		void setStringUsableObjectType(const std::string &usableObjectType){ _usableObjectType = usableObjectType;}
		const std::string &getStringUsableObjectType(){ return _usableObjectType;}
		
	private:
		CEntity *_entity;
		std::string _WeaponType;
		std::string _usableObjectType;
	};

	//MENSAJE SWAP_WEAPONS: Sirve para intercambiar de brazos las armas
	class SWAP_WEAPONS : public IMessage
	{
	public:

		SWAP_WEAPONS(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SWAP_WEAPONS(){};

	private:
		CEntity *_entity;
	};

	//MENSAJE SET_DAMAGE: Sirve para configurar el daño de los proyectiles disparados por un arma a distancia
	/*class SET_DAMAGE : public IMessage
	{
	public:

		SET_DAMAGE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_DAMAGE(){};
		
		void setDamage(float flotante){ _float = flotante;}
		float getDamage(){ return _float;}

	private:
		float _float;
	};*/

	//MENSAJE SET_ALTSTATE: Sirve para setear un estado alterado
	class SET_ALTSTATE : public IMessage
	{
	public:

		SET_ALTSTATE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_ALTSTATE(){};

		void setString(const std::string &string){ _string = string;}
		const std::string &getString(){ return _string;}

	private:
		std::string _string;
	};

	//MENSAJE DAMAGED: Sirve para 
	class DAMAGED : public IMessage
	{
	public:

		DAMAGED(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~DAMAGED(){};
		
		void setFloat(float flotante){ _float = flotante;}
		float getFloat(){ return _float;}
		void setString(std::string string){ _string = string;}
		std::string getString(){ return _string;}

	private:
		float _float;
		std::string _string;
	};

	//MENSAJE DAMAGED: Sirve para 
	class HEAL : public IMessage
	{
	public:

		HEAL(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~HEAL(){};

		void setFloat(float flotante){ _float = flotante;}
		float getFloat(){ return _float;}

	private:
		float _float;
	};

	//MENSAJE INVINCIBLE: Sirve para ser invencible o no
	class INVINCIBLE : public IMessage
	{
	public:

		INVINCIBLE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~INVINCIBLE(){};

		void setBool(bool boolean){ _boolean = boolean;}
		float getBool(){ return _boolean;}

	private:
		float _boolean;
	};

	//MENSAJE AVATAR_WALK: Sirve para 
	class AVATAR_WALK : public IMessage
	{
	public:

		AVATAR_WALK(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~AVATAR_WALK(){};
		
		void setVector3(Vector3 movimiento){ _vector3 = movimiento;}
		Vector3 getVector3(){ return _vector3;}

	private:
		Vector3 _vector3;;
	};

	//MENSAJE TELEPORT: Sirve para teletransportarse
	class TELEPORT : public IMessage
	{
	public:

		TELEPORT(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~TELEPORT(){};

		void setVector3(Vector3 movimiento){ _vector3 = movimiento;}
		Vector3 getVector3(){ return _vector3;}

	private:
		Vector3 _vector3;;
	};

	//MENSAJE KINEMATIC_MOVE: Sirve para 
	class KINEMATIC_MOVE : public IMessage
	{
	public:

		KINEMATIC_MOVE(): IMessage() {
			_type = nombreMensaje(std::string(typeid(this).name()));
			_transform = Matrix4::ZERO;
		}
		virtual ~KINEMATIC_MOVE(){};

		void setVector3(Vector3 movimiento){ _vector3 = movimiento;}
		Vector3 getVector3(){ return _vector3;}

		void setTransform(Matrix4 transform){ _transform = transform;}
		Matrix4 getTransform(){ return _transform;}
		

	private:
		Vector3 _vector3;
		Matrix4 _transform;
	};

	//MENSAJE SET_SIMULATION: Sirve para activar o desactivar la simulacion física desde el componente
	class SET_SIMULATION : public IMessage
	{
	public:

		SET_SIMULATION(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_SIMULATION(){};

		void setSimulation(bool simulate){ _simulate = simulate;}
		bool getSimulation(){ return _simulate;}
	private:
		bool _simulate;
	};

	//MENSAJE SET_PHYSIC_GROUP: Sirve para cambiar el grupo de colision del actor fisico controlado por un componente
	class SET_PHYSIC_GROUP : public IMessage
	{
	public:

		SET_PHYSIC_GROUP(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_PHYSIC_GROUP(){};

		void setGroup(int group){ _group = group;}
		int getGroup(){ return _group;}
	private:
		int _group;
	};

	//MENSAJE SET_ANIM_SPEED_MULT: Sirve para cambiar la velocidad d animacion
	class SET_ANIM_SPEED_MULT : public IMessage
	{
	public:

		SET_ANIM_SPEED_MULT(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_ANIM_SPEED_MULT(){};

		void setFloat(float speed){ _speed = speed;}
		float getFloat(){ return _speed;}
	private:
		float _speed;
	};

	//MENSAJE MOVE_TO: Sirve para mover una entidad (comúnmente usada para enemigos con IA)
	class MOVE_TO : public IMessage
	{
	public:

		MOVE_TO(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~MOVE_TO(){};

		void setVector3(Vector3 movimiento){ _vector3 = movimiento;}
		Vector3 getVector3(){ return _vector3;}

		void setInt(int entero){ _int = entero;}
		int getInt(){ return _int;}

	private:
		Vector3 _vector3; // objetivo de movimiento
		int _int; // tipo de movimiento (enumerado)
	};

	//MENSAJE FINISHED_MOVE: Sirve para indicar que ha terminado (con o sin éxito) un movimiento
	class FINISHED_MOVE : public IMessage
	{
	public:

		FINISHED_MOVE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~FINISHED_MOVE(){};

		bool getBool(){ return _bool;}
		void setBool(bool booleano){ _bool = booleano;}

	private:
		bool _bool; // ha llegado correctamente al destino o no
	};


	//MENSAJE TURN: Sirve para girar una entidad según la dirección en entero
	class TURN : public IMessage
	{
	public:

		TURN(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~TURN(){};

		float getInt(){ return _int;}
		void setInt(float entero){ _int = entero;}
	private:
		float _int;
	};


	//MENSAJE SET_ATTENUATION: Sirve para indicar la atenuacion de una luz puntual
	class SET_ATTENUATION : public IMessage
	{
	public:

		SET_ATTENUATION(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_ATTENUATION(){};

		Vector4 getAttenuation(){ return _attenuation;}
		void setAttenuation(Vector4 attenuation){ _attenuation = attenuation;}

	private:
		Vector4 _attenuation;
	};


	class CHANGE_VIEWPORT : public IMessage
	{
	public:
		CHANGE_VIEWPORT(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CHANGE_VIEWPORT(){}

		void setEffect(const std::string &effect){_effect = effect;}
		const std::string getEffect(){return _effect;}

		void setActivate(bool activate){_activate = activate;}
		bool getActivate(){return _activate;}

	private:
		std::string _effect;
		bool _activate;

	};
	
	//MENSAJES SET_HABILIDAD: Sirve para indicar la habilidad que se quiere despertar. 
	class SET_HABILIDAD : public IMessage
	{
	public:

		SET_HABILIDAD(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_HABILIDAD(){};

		const std::string & getString(){ return _tipoHabilidad;}
		void setString(const std::string &  Habilidad){ _tipoHabilidad = Habilidad;}

		int getInt(){return contador_habilidad;}
		void setInt(int habilidad){contador_habilidad = habilidad;}

	private:
		std::string _tipoHabilidad;
		int contador_habilidad;
	};

	//MENSAJES SET_HABILIDAD: Sirve para indicar la habilidad que se quiere despertar. 
	class SET_STAT : public IMessage
	{
	public:

		SET_STAT(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_STAT(){};

		const std::string & getString(){ return _tipoStat;}
		void setString(const std::string &  Stat){ _tipoStat = Stat;}

		float getFloat(){return _valorStat;}
		void setFloat(float stat){_valorStat = stat;}

	private:
		std::string _tipoStat;
		float _valorStat;
	};

	//MENSAJES AUMENTAR_SALTO sirve para aumentar en una unidad el numero de saltos máximos que puede dar el personaje
	class AUMENTAR_SALTO : public IMessage
	{
	public:

		AUMENTAR_SALTO(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~AUMENTAR_SALTO(){};
		int getInt(){return num_saltos;}
		void setInt(int saltos){num_saltos = saltos;}

	private:
		int num_saltos;

	};

	//MENSAJES ENEMY_JUMP sirve para notificar a un enemigo que debe saltar.
	class ENEMY_JUMP : public IMessage
	{
	public:

		ENEMY_JUMP(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~ENEMY_JUMP(){};

	private:

	};


	//MENSAJE CONTROLLER_TOUCHED: Sirve para indicar que se ha chocado contra un trigger. OJO la entidad puede ser NULL
	class CONTROLLER_TOUCHED : public IMessage
	{
	public:

		CONTROLLER_TOUCHED(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CONTROLLER_TOUCHED(){};

		CEntity *getEntidad(){ return _entity;}
		void setEntity(CEntity *entidad){ _entity = entidad;}

	private:
		CEntity *_entity;
	};

	//MENSAJE PHYSIC_ROTATION: Sirve para establecer una rotación a la física a partir de un vector y un angulo de rotacion. Angulos en radianes
	class PHYSIC_ROTATION : public IMessage
	{
	public:

		PHYSIC_ROTATION(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~PHYSIC_ROTATION(){};

		Vector3 getOrientation(){ return _orientation;}
		void setOrientation( Vector3 &orientation){ _orientation = orientation;}

		float getAngle() { return _angle; }
		void setAngle(float angle) {_angle = angle; }

	private:
		Vector3 _orientation;
		float _angle;
	};


	//MENSAJE PLAY_SOUND: Sirve para iniciar la reproducción de un sonido
	class PLAY_SOUND : public IMessage
	{
	public:

		PLAY_SOUND(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~PLAY_SOUND(){};

		std::string getSound(){ return _sound;}
		void setSound( std::string &sound){ _sound = sound;}

	private:
		std::string _sound;
	};

	//MENSAJE STOP_SOUND: Sirve para parar la reproducción de un sonido
	class STOP_SOUND : public IMessage
	{
	public:

		STOP_SOUND(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~STOP_SOUND(){};

		std::string getSound(){ return _sound;}
		void setSound( std::string &sound){ _sound = sound;}

	private:
		std::string _sound;
	};

	//MENSAJE SET_SOUND_PARAMETER: Sirve para cambiar el parámetro de un sonido
	class SET_SOUND_PARAMETER : public IMessage
	{
	public:

		SET_SOUND_PARAMETER(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_SOUND_PARAMETER(){};

		std::string getSound(){ return _sound;}
		void setSound( std::string &sound){ _sound = sound;}
		std::string getParameter(){ return _parameter;}
		void setParameter( std::string &parameter){ _parameter = parameter;}
		float getValue(){ return _value;}
		void setValue( float value){ _value = value;}

	private:
		std::string _sound;
		std::string _parameter;
		float _value;
	};


	//MENSAJE SET_ARMTOWEAPON: Sirve para cambiar configurar qué  brazo es el que sostiene un arma
	class SET_ARMTOWEAPON : public IMessage
	{
	public:

		SET_ARMTOWEAPON(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_ARMTOWEAPON(){};

		const std::string getArm(){ return _arm;}
		void setArm(const std::string &arm){ _arm = arm;}


		const bool getBool(){ return _bool;}
		void setBool(bool boolean){ _bool = boolean;}

	private:
		std::string _arm;
		bool _bool;
	};

	class SPECIAL_ATTACK_LAUNCHED : public IMessage
	{
	public:

		SPECIAL_ATTACK_LAUNCHED(): IMessage() { _type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SPECIAL_ATTACK_LAUNCHED(){};
	};

	class SET_ARMTOWEAPONHUD : public IMessage
	{
	public:

		SET_ARMTOWEAPONHUD(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_ARMTOWEAPONHUD(){};

		const std::string getArm(){ return _arm;}
		void setArm(const std::string &arm){ _arm = arm;}

	private:
		std::string _arm;
	};

	class NOTIFY_ARM : public IMessage
	{
	public:

		NOTIFY_ARM(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~NOTIFY_ARM(){};

		const std::string getArm(){ return _arm;}
		void setArm(const std::string &arm){ _arm = arm;}

	private:
		std::string _arm;
	};


	//Mensaje que sirve para modificar el zoom de la camara a partir de un offset
	class CAMERA_ZOOM : public IMessage
	{
	public:

		CAMERA_ZOOM(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~CAMERA_ZOOM(){};

		void setZoom(int zoom) { _zoom = zoom; }
		int getZoom() { return _zoom; }

	private:

		int _zoom;

	};

	///Mensaje que sirve para hacer visible o invisible el RibbonTrail de una entidad
	class SET_TRAIL_VISIBLE : public IMessage
	{
	public:

		SET_TRAIL_VISIBLE(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_TRAIL_VISIBLE(){};

		bool getVisible() { return _visible; }
		void setVisible(bool visible) { _visible = visible; }

	private:

		bool _visible;

	};

	class SET_TRAIL_LENGTH : public IMessage
	{
	public:

		SET_TRAIL_LENGTH(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~SET_TRAIL_LENGTH(){};

		float getLength() { return _length; }
		void setLength(float length) { _length = length; }

	private:

		float _length;

	};

	//Mensaje que avisa de que un tile fragil se ha roto
	////VAYA MENSAJE MAS GUARRO :3
	//Mensaje que sirve para indicar que se ha terminado los activates de todos los componentes
	class BROKEN : public IMessage
	{
	public:

		BROKEN(): IMessage() {_type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~BROKEN(){};
	};

	/*
	Este mensaje es usado para avisar al componente de retroceso de que debe aplicar un retorceso con la configuracion deseada
	*/
	class PUSH: public IMessage
	{
	public:
		PUSH() : IMessage() { _type = nombreMensaje(std::string(typeid(this).name()));}
		virtual ~PUSH(){};

		Vector3 getDirection() { return _direction; }
		void setDirection(const Vector3 &direction) { _direction = direction;  }

		float getSpeed() { return _speed; }
		void setSpeed(float speed) { _speed = speed; }

	private:

		Vector3 _direction;
		float _speed;

	};

} // namespace Logic

#endif // __Logic_Message_H