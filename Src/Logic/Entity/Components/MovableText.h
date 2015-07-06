/**
@author Cesar Manuel Paz Guzman
@date Enero, 2015
*/

#ifndef __Logic_MovableText_H
#define __Logic_MovableText_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"


namespace Graphics
{
	class CMovableText;
}
//declaración de la clase

namespace Logic 
{
	class CMovableText2 : public IComponent
	{
		DEC_FACTORY(CMovableText2);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CMovableText2() : IComponent(){}

		virtual ~CMovableText2();
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);
		void deactivate();

		bool activate();
		/**
		Método llamado en cada frame que actualiza el estado del componente.
		<p>
		Se encarga de mover la entidad en cada vuelta de ciclo cuando es
		necesario (cuando está andando o desplazándose lateralmente).

		@param msecs Milisegundos transcurridos desde el último tick.
		*/
		virtual void onTick(unsigned int msecs);

		void createMovableText();

		void setText(const std::string &textoMostrar);

		void setColor(const std::string &color);

		void setTimeToLive(float _time);

		void setScale(const Vector3 &scala);

		void setSpeed(float speed){ _speed = speed;}
	protected:
		Graphics::CMovableText *_text;
		std::string *_textoMostrar;
		float _timeStamp;
		float _timeToLive;
		float _timeToDegradade;
		bool _transparente;
		float _speed;
		Vector3 _scaleMovableText;
	}; 

	REG_FACTORY(CMovableText2);

} // namespace Logic

#endif 