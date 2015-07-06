//---------------------------------------------------------------------------
// SceneNode.h
//---------------------------------------------------------------------------

/**
@file SceneNode.h

Contiene la declaración de la clase que representa un nodo de escena de Ogre. Es una clase sencilla que encapsula
el nodo de Ogre para poder ser usado en la parte de lógica

@see Graphics::CSceneNode

@author Alberto Martínez
@date Mayo, 2015
*/

#ifndef __Graphics_SceneNode_H
#define __Graphics_SceneNode_H

#include "BaseSubsystems\Math.h"

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
	class CSceneNode
	{
	public:

		CSceneNode(const std::string &name);

		virtual ~CSceneNode();

		///Devuelve el puntero al nodo de escena de Ogre
		Ogre::SceneNode* getOgreNode() { return _node; }

		//setea la posicion del nodo de escena
		void setPosition(const Vector3 &position);

		void attachToScene(CScene* scene);

		void deattachToScene(CScene* scene);

		std::string getName() { return _name; };
	protected:

		///Nombre del nodo de escena 
		std::string _name;

		//puntero a la escena en la cual se encuentra el nodo
		CScene* _scene;

		//Puntero al nodo de Ogre
		Ogre::SceneNode* _node;

	}; // class CSceneNode

} // namespace Graphics

#endif // __Graphics_SceneNode_H
