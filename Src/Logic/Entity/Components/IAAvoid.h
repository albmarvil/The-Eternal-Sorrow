/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __AI__AVOID__
#define __AI__AVOID__

#include "Logic/Entity/Component.h"
#include <map>
#include <vector>
namespace Map
{
	class CEntity;
}

namespace Logic
{
	class CIAAvoid: public IComponent
	{
		DEC_FACTORY(CIAAvoid);
	public:

		CIAAvoid():IComponent(){}
		virtual ~CIAAvoid(){}

		bool OnSpawn(const Map::CEntity* entityInfo);
		bool activate();
		void deactivate();

		void onTick(unsigned int msecs);

		Vector3 calculateVelocity(unsigned int msecs);
		/**
		Método virtual que elige que mensajes son aceptados.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);
	private:

		//Funcion que me comprueba si estoy cerca de un tile y de ser asi me devuelve la distancia, sino me devuelve un 0
		void throwRay();

		//Funcion que obtiene la direccion en X que tomar
		void calculateDirectionX();

		void calculateLimits(int &limiteIzquierdo, int &limiteDerecho);

		void calculateDirectionY();

		//Direccion y que tomara el avoid
		float _directionY;

		//peso del movimiento avoid
		int _weight;
		
		//Direccion x qe tomara el avoid
		float _directionX;

		//peso maximo que podria tomar el avoid
		int _weigthMax;

		//si se puede mover la entidad entonces aplicamos el movimiento avoid. Este flag lo indica.
		bool _canMove;

		//Variable que indica el punto que choca la entidad con las plataformas y poder asi esquivarlas
		Vector3 _puntoColisionEntidadFisica;

		Vector3 _direccion;
		CEntity* _target;

		//Cada cierto tiempo se comprueba si se debe esquivar
		unsigned int _timeStamp;

		float _resultWeight;

		int _contador;
		bool _rebotando;
	};

	REG_FACTORY(CIAAvoid);
}

#endif