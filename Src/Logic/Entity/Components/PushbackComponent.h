/**
@file CPushbackComponent.h

Contiene la declaración del componente de retroceso. Es un componente genérico que acepta el mensaje PUSH y en su tick tiene la logica
relacionada con el movimiento originado pro dicho empuje

@see Logic::CPushbackComponent
@see Logic::IComponent

@author Alberto Martínez
@date Junio, 2015
*/


#ifndef __Logic_CPushbackComponent_H
#define __Logic_CPushbackComponent_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"

namespace Logic
{
	class CPhysicController;
};

//declaración de la clase
namespace Logic 
{

	class CPushbackComponent : public IComponent
	{
		DEC_FACTORY(CPushbackComponent);
	public:

		CPushbackComponent() : IComponent(), _controller(0), _pushed(0), _pushDirection(Vector3::ZERO),
			_initSpeed(0.0f), _actualSpeed(Vector3::ZERO), _accel(Vector3::ZERO), _falling(false) {};

		virtual ~CPushbackComponent() {};
		

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

	
		virtual bool respawn(const Map::CEntity *entityInfo);

		
		virtual bool activate();

	
		virtual void deactivate(){};

		virtual void onTick(unsigned int msecs);


		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);


		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/*
		Este método configura el empuje sobre la entidad. Solo puede ser llamado una vez por tick.
		Si se llama mientras se esta siendo empujado se ignora. hasta que noa cabe el empuje actual no se
		puede configurar otro nuevo empuje

		0.3 de velocidad mola cantidubi
		*/
		void push(const Vector3 &direction, float speed);
	
	protected:

		CPhysicController* _controller;

		/*
		Falg que indica si estamos en caida
		*/
		bool _falling;

		/*
		Flag que indica si se esta siendo empujado o no
		*/
		bool  _pushed;

		/*
		Direccion en la que se es empujado
		*/
		Vector3 _pushDirection;

		/*
		Velocidad inicial de empuje
		*/
		float _initSpeed;

		/*
		Velocidad actual del empuje
		*/
		Vector3 _actualSpeed;

		/*
		Aceleracion con la que se disminuye la velocidad de empuje
		*/
		Vector3 _accel;

	}; // class Attack

	REG_FACTORY(CPushbackComponent);

} // namespace Logic

#endif // __Logic_AttackD_H
