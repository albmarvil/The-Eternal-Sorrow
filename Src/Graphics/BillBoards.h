/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __Graphics_BillBoards_H
#define __Graphics_BillBoards_H

#include "BaseSubsystems/Math.h"
#include "Server.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class SceneNode;
}

namespace Graphics 
{
	class CScene;
}

namespace Graphics 
{

	class CBillBoards
	{
	public:
		/**
		Constructor de la clase.

		@param name Tipo de la luz.
		@param pos Posición inicial de la luz.
		@param especular Color especular de la luz.
		@param direction Dirección en la que apunta la luz.
		@param range rango de actuación de la luz.
		@param difuse Color difuso de la luz.
		*/
		CBillBoards(const std::string &type, const std::string &_name, const std::string &material,const Vector3 &dimensions, const Vector3 &position);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CBillBoards();

		void updateLifeBar(int life, int lifeMax);
		
		bool loadLifeBar();

		void setVisible(bool visible);

		void setMaterialName(const std::string &materialName){ _material = materialName;}

		void setPosition(const Vector3 &pos){ _position = pos;}
	protected:

		friend class CScene;

		virtual bool attachToScene(CScene *sc);

		virtual bool deattachFromScene();

		virtual void unload();
		
		virtual void tick(float secs);

		virtual bool load();

		CScene *scene;

		bool loaded;

		Ogre::SceneNode *_BillBoardsNode; 
		Ogre::BillboardSet *_bbSetBillBoards;
		Ogre::Billboard *_BillBoards;
		/**
		Nombre de la entidad.
		*/
		std::string name;
		Vector3 _dimensions;
		std::string _material;
		std::string _type;
		Vector3 _position;
	}; 

} // namespace Graphics

#endif 
