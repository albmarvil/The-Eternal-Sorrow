/*
Esta clase es la clase padre de todos los scripts.lua. Debe contener tanto el metodo para setear la entidad a la que pertenece
, y el metodo para obtenerla. De esta manera, podremos extraer la entidad desde LUA, y acceder a sus metodos, como el getPosition

author Cesar Manuel Paz Guzman
date Marzo 2015
*/

#ifndef __Logic_ScriptEntity_H
#define __Logic_ScriptEntity_H
#include <iostream>

namespace Logic
{
	class CEntity;
}

namespace Map
{
	class CEntity;
}

namespace Logic 
{

	class CScriptEntity
	{
	public:

		CScriptEntity(){}
		virtual ~CScriptEntity();
		
		virtual bool spawn(const Map::CEntity *entityInfo);

		virtual void tick(unsigned int msecs);

		virtual bool activate();

		virtual void deactivate();

		virtual bool respawn(const Map::CEntity *entityInfo);

		virtual void arm(const std::string &brazo){}

		virtual void specialAtkLaunched(){}

		//Metodo que sirve 
		void setOwner(CEntity* entity){ _entity = entity;}

		CEntity *getOwner(){ return _entity;}

		virtual void stopAnim();

		void setSleep(std::string nameComponent, bool sleep);

		void sendMessageSETHABILIDAD(const std::string &nombreHabilidad,Logic::CEntity* other);

		void setVisibleText(CEntity *entity, const std::string &texto, const std::string &color, bool visible);
		void sendMessageMovableText(const std::string &text, Logic::CEntity *movableText, bool _movable);

		/*
			@param _movable: true si es un texto que se mueve y false si es estatico
			@param movableText: entidad 
			@param color: color del texto a mostrar
		*/
		void sendMessageSetColorMovableText(const std::string &color, Logic::CEntity *movableText, bool _movable);

		/*METODOS USADOS PARA TRANSMITIR MENSAJES AL SCRIPT DE LUA
			
			para que el script ejecute/reciba el mensaje debe implementar esta función
		*/
		virtual void touched(Logic::CEntity* other);

		virtual void untouched(Logic::CEntity* other);

		virtual void openChest();

		virtual void cancelOpen();

		virtual void laserToLaserBall();

		virtual void use (bool pressed);

		virtual void broken();

		virtual void setArm(const std::string &brazo){}

		virtual void damaged(float damage);
	
	protected:

		CEntity *_entity;
	}; 

} 

#endif 
