/**
@author Cesar Manuel Paz Guzman
@date Abril, 2015
*/

#ifndef __Logic_StaticText_H
#define __Logic_StaticText_H

#include "Logic/Entity/Component.h"
#include "Logic/Maps/EntityFactory.h"


namespace Graphics
{
	class CMovableText;
}
//declaración de la clase

namespace Logic 
{
	class CStaticText : public IComponent
	{
		DEC_FACTORY(CStaticText);
	public:

		/**
		Constructor por defecto; en la clase base no hace nada.
		*/
		CStaticText() : IComponent(){}

		virtual ~CStaticText();
		
		/**
		Inicialización del componente usando la descripción de la entidad que hay en 
		el fichero de mapa.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		virtual bool respawn(const Map::CEntity *entityInfo);

		void deactivate();

		bool activate();

		virtual void onTick(unsigned int msecs);

		void createStaticText();

		void setText(const std::string &textoMostrar);

		void setColor(const std::string &color);

		void setScale(const Vector3 &scala);

		void setVisible(bool visible);
	protected:
		Graphics::CMovableText *_text;
		std::string *_textoMostrar;
		Vector3 _scaleStaticText;
		Vector3 _offsetStaticText;
	}; 

	REG_FACTORY(CStaticText);

} // namespace Logic

#endif 