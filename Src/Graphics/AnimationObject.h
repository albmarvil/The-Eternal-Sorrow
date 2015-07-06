/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __Graphics__AnimationObject
#define __Graphics__AnimationObject

#include "BaseSubsystems/Math.h"

namespace Graphics
{
	class CEntity;
	
}

namespace Ogre
{
	class AnimationState;
	class SceneManager;
}
namespace Graphics
{
	enum tipoAnimacion
	{
		flotante = 0,
		rotacion = 1,
	};

	class CAnimationObject
	{
	public:
		CAnimationObject(const std::string &nameAnimation);
		~CAnimationObject();

		void tick(unsigned int msecs);
		
		void destroyAnimationState();
		
		void createAnimation(CEntity *_graphicEntity, int typeAnimation);

	private:

		void createAnimationFloating(CEntity *_graphicEntity);
		void createAnimationRotating(CEntity *_graphicEntity);

		Ogre::AnimationState *_animationState;
		Ogre::SceneManager *_sceneMgr;
		std::string _nameAnimation;
	};
}

#endif