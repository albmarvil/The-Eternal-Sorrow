//---------------------------------------------------------------------------
// AnimatedEntity.cpp
//---------------------------------------------------------------------------

/**
@file AnimatedEntity.cpp

Contiene la implementación de la clase que representa una entidad gráfica 
con animaciones.

@see Graphics::CAnimatedEntity
@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#include "AnimatedEntity.h"
#include "AnimationBlender.h"
#include "Logic/Entity/Components/WeaponController.h"
#include "BaseSubsystems/Log.h"
#include "LUA/ScriptManager.h"

#include <assert.h>

#include <OgreEntity.h>
#include <OgreAnimationState.h>

namespace Graphics 
{
	
	CAnimatedEntity::CAnimatedEntity(const std::string &name, const std::string &mesh, const std::string &animationConfig) : CEntity(name, mesh)
	{
		//configuramos la forma de hacer el blending de animaciones
		//_entity->getSkeleton()->setBlendMode(Ogre::SkeletonAnimationBlendMode::ANIMBLEND_AVERAGE);
		_animationConfig = animationConfig;
	}

	CAnimatedEntity::~CAnimatedEntity()
	{
		TAnimationSets::iterator it = _currentAnimations.begin();
		TAnimationSets::iterator end = _currentAnimations.end();

		for(; it != end; ++it)
		{
			delete it->second;
		}

		TAnimationObservers::iterator it2 = _animationObservers.begin();
		TAnimationObservers::iterator end2 = _animationObservers.end();
		
		for(; it2 != end2; ++it2)
		{
			it2->second.clear();
		}

		_currentAnimations.clear();
		_animationObservers.clear();

		TAnimationMasks::iterator it3 = _animationMasks.begin();
		TAnimationMasks::iterator end3 = _animationMasks.end();


		for(; it3 != end3; ++it3)
		{
			it3->second.clear();
		}

		_animationMasks.clear();
	}


	bool CAnimatedEntity::setAnimation(const std::string &anim, const std::string &animSet, bool loop, bool restart, float duration, float speed)
	{
#pragma region método para sacar la info de los huesos a fichero

		/*_entity->getSkeleton()->setBlendMode(Ogre::SkeletonAnimationBlendMode::ANIMBLEND_CUMULATIVE);

		Ogre::Skeleton::BoneIterator it = _entity->getSkeleton()->getBoneIterator();

		for(; it.current() != it.end(); it.moveNext())
		{
			std::string name = (*it.current())->getName();
			int handle = (*it.current())->getHandle();

			BaseSubsystems::Log::Debug(name + " - "+std::to_string(handle));
		}*/
#pragma endregion


		//BaseSubsystems::Log::Debug("Set anim: "+anim+" conjunto: "+animSet+" duracion: "+std::to_string(duration));
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
		{
			return false;
		}
		
		std::vector<std::pair<float, float>> animationMask = createAnimationMask(animSet);

		TAnimationSets::iterator find = _currentAnimations.find(animSet);

		if ( find != _currentAnimations.end()) //si el conjunto de animación está en el mapa, transitamos desde la animación que tiene ahora el conjunto a la nueva
		{
			CAnimationBlender *animation = find->second;

			return animation->blend(anim, animationMask, loop, restart, duration, speed);
		}
		else
		{
			//Creamos la nueva entrada del conjunto con la nueva aniamción
			CAnimationBlender* animation = new CAnimationBlender(anim, this, loop, restart, speed, animationMask, _entity->getSkeleton()->getNumBones());
			_currentAnimations.insert(std::pair<std::string, CAnimationBlender*> (animSet, animation));
			return true;
		}

	} // setAnimation

	//--------------------------------------------------------
		
	bool CAnimatedEntity::stopAnimation(const std::string &anim, const std::string &animSet)
	{
		if(!_entity->getAllAnimationStates()->hasAnimationState(anim))
			return false;

		TAnimationSets::iterator find = _currentAnimations.find(animSet);

		if ( find != _currentAnimations.end()) 
		{
			CAnimationBlender *animation = find->second;

			return animation->stopAnimation();
		}
		else
		{
			return false;
		}
		
	} // stopAnimation

	//--------------------------------------------------------
		
	void CAnimatedEntity::stopAllAnimations()
	{
		if(_entity->getAllAnimationStates()->hasEnabledAnimationState())
		{
			TAnimationSets::iterator it = _currentAnimations.begin();
			TAnimationSets::iterator end = _currentAnimations.end();

			for (; it != end; ++it)
			{
				it->second->stopAnimation();
			}
		}

	} // stopAllAnimations

	//--------------------------------------------------------
		
	void CAnimatedEntity::tick(float secs)
	{
		// Aquí se actualizará de la lista de observadores
		MapBoneObservers::iterator it;

		for (it = _mapBoneObservers.begin(); it != _mapBoneObservers.end(); ++it)
		{
			// A traves de los datos, creamos un matrix4
			Vector3 bonePos =  getBoneWorldPosition((*it).first->getName());//(*it).first->getPosition();
			Vector3 boneScale =  (*it).first->getScale();
			Quaternion boneOri =   getBoneWorldOrientation((*it).first->getName());
			
			//BaseSubsystems::Log::Debug("Hueso Ori: "+std::to_string(boneOri.w)+" "+std::to_string(boneOri.x)+" "+std::to_string(boneOri.y)+" "+std::to_string(boneOri.z));
			//BaseSubsystems::Log::Debug("Bone Pos: "+std::to_string(bonePos.x)+" "+std::to_string(bonePos.y)+" "+std::to_string(bonePos.z));

			Matrix4 mat4;
			mat4.makeTransform(bonePos, boneScale, boneOri);

			for (unsigned int i = 0; i < (*it).second.size(); i++)
			{
				// Enviamos el mensaje de SetTransform
				/*std::shared_ptr<Logic::SET_TRANSFORM_BONE>m(new Logic::SET_TRANSFORM_BONE());
				m->setTransform(mat4);
				m->setString((*it).first->getName());
				(*it).second.at(i)->emitMessage(m);*/
				(*it).second.at(i)->setTransformBone(mat4);
			}
		}



		if(_currentAnimations.size() > 0)
		{
			TAnimationSets::iterator animIT = _currentAnimations.begin();
			TAnimationSets::iterator animEND = _currentAnimations.end();

			for (; animIT != animEND; ++animIT)
			{
				animIT->second->tick(secs);
			}
		}

		/*Ogre::AnimationStateIterator iterator = _entity->getAllAnimationStates()->getAnimationStateIterator();

		for(; iterator.current() != iterator.end(); iterator.moveNext())
		{
			std::string name = iterator.current()->second->getAnimationName();
			bool enabled = iterator.current()->second->getEnabled();

			if(enabled)
			{
				BaseSubsystems::Log::Debug(name + " esta activa - gEnt: "+_entity->getName());
			}
		}*/



	} // tick

	//--------------------------------------------------------------------

	std::vector<std::pair<float, float>> CAnimatedEntity::createAnimationMask(const std::string &animationSet)
	{
		TAnimationMasks::iterator find = _animationMasks.find(animationSet);

		if( find != _animationMasks.end())
		{
			return find->second;
		}
		else
		{
			std::vector<std::pair<float, float>> result;
			int maskSize = 0;
			LUA::CScriptManager::GetPtrSingleton()->executeFunction("getMaskLength", _animationConfig, animationSet, maskSize);


			if(maskSize == 0)
			{
				return result;
			}

			for (int i = 1; i <= maskSize; ++i)
			{
				float boneHandle = 0;
				float weight = 0;

				LUA::CScriptManager::GetPtrSingleton()->executeFunction("getAnimationMaskValue", _animationConfig, animationSet, i, boneHandle, weight);

				std::pair<float, float> pair(boneHandle, weight);

				result.push_back(pair);
			}

			TAnimationMaskPair p(animationSet, result);
			_animationMasks.insert(p);

			return result;
		}
	}

	void CAnimatedEntity::setObserver(CAnimatedEntityListener *observer, const std::string &animationName)
	{
		TAnimationObservers::iterator find =  _animationObservers.find(animationName);

		if( find != _animationObservers.end())
		{
			find->second.insert(observer);
		}
		else
		{
			std::set<CAnimatedEntityListener*> aux;
			aux.insert(observer);
			TAnimationObserverPair pair(animationName, aux);
			_animationObservers.insert(pair);
		}
	}// setObserver

	//-------------------------------------------------------------------

	void CAnimatedEntity::removeObserver(CAnimatedEntityListener *observer, const std::string &animationName)
	{
		TAnimationObservers::iterator find =  _animationObservers.find(animationName);

		if( find != _animationObservers.end())
		{
			find->second.erase(observer);
		}
	}// removeObserver

	//-----------------------------------------------------------------
	
	void CAnimatedEntity::animationFinished(const std::string &animation)
	{
		//BaseSubsystems::Log::Debug("Ha finalizado la animacion: "+animation);
		TAnimationObservers::iterator find = _animationObservers.find(animation);

		if( find != _animationObservers.end())
		{
			std::set<CAnimatedEntityListener*>::iterator it = find->second.begin();

			for(;  it != find->second.end(); ++it)
			{
				(*it)->animationFinished(find->first);
			}
		}

	}//animationFinished
	//--------------------------------------------------------------------


	Ogre::AnimationState* CAnimatedEntity::getAnimationState(const std::string &name)
	{
		if(_entity->getAllAnimationStates()->hasAnimationState(name))
		{
			return _entity->getAnimationState(name);
		}
		else
		{
			return 0;
		}
	}//getAnimationState
	//--------------------------------------------------------------------

	void CAnimatedEntity::setAnimationBlendMode(BlendMode mode)
	{
		_entity->getSkeleton()->setBlendMode(Ogre::SkeletonAnimationBlendMode(mode));
	}//getAnimationState
	//--------------------------------------------------------------------

	Vector3 CAnimatedEntity::getBoneWorldPosition(std::string boneName)
	{
		Ogre::Bone* bone = _entity->getSkeleton()->getBone(boneName);

		Ogre::Quaternion parentQuat = _entityNode->_getDerivedOrientation();
		Ogre::Vector3 parentPos = _entityNode->_getDerivedPosition();
		Ogre::Vector3 bonePos = bone->_getDerivedPosition();

		Ogre::Vector3 worldPos = parentQuat * bonePos + parentPos;

		return worldPos;
	}
	//--------------------------------------------------------------------

	Quaternion CAnimatedEntity::getBoneWorldOrientation(std::string boneName)
	{
		Ogre::Bone* bone = _entity->getSkeleton()->getBone(boneName);

		Ogre::SceneNode *parent = _entityNode; /*node of the entity that has the skeleton*/;
		Ogre::Quaternion parentQuat = _entityNode->_getDerivedOrientation();
		Ogre::Quaternion boneQuat = bone->_getDerivedOrientation();

		Ogre::Quaternion worldQuat = boneQuat * parentQuat; // actually, it might be parentQuat * boneQuat, I forgot

		return worldQuat;
	}
	//--------------------------------------------------------------------

	void CAnimatedEntity::setBoneWorldPosition(std::string boneName, Vector3 worldPos)
	{
		Ogre::Bone* bone = _entity->getSkeleton()->getBone(boneName);

		bone->setManuallyControlled(true);

		Ogre::Vector3 parentPos = _entityNode->_getDerivedPosition(); 
		Ogre::Vector3 parentQuatXbonePos = worldPos - parentPos;

		Ogre::Quaternion parentQuat = _entityNode->_getDerivedOrientation(); 
		Ogre::Vector3 bonePos = parentQuat.Inverse() * parentQuatXbonePos;
		Ogre::Vector3 inverseParentScale = 1.0 / _entityNode->_getDerivedScale();

		bone->_setDerivedPosition(bonePos * inverseParentScale);
	}
	//--------------------------------------------------------------------

	void CAnimatedEntity::setBoneWorldOrientation(std::string boneName, Quaternion worldQuat)
	{
		Ogre::Bone* bone = _entity->getSkeleton()->getBone(boneName);

		bone->setManuallyControlled(true);

		Ogre::Quaternion parentQuat = _entityNode->_getDerivedOrientation(); 
		Ogre::Quaternion boneQuat = worldQuat.Inverse() * parentQuat;

		bone->_setDerivedOrientation(boneQuat);
	}
	//-------------------------------------------------------------------
	
	void CAnimatedEntity::setBoneLocalOrientation(std::string boneName, Quaternion localQuat)
	{
		Ogre::Bone* bone = _entity->getSkeleton()->getBone(boneName);

		bone->setManuallyControlled(true);

		bone->setOrientation(localQuat);
	}
	//--------------------------------------------------------------------

	void CAnimatedEntity::yawBone(std::string boneName, float angle)
	{
		Ogre::Bone* bone = _entity->getSkeleton()->getBone(boneName);
		
		bone->yaw(Ogre::Radian(angle));
	}

	void CAnimatedEntity::setBoneManuallyControlled(std::string boneName, bool mc)
	{
		Ogre::Bone* bone = _entity->getSkeleton()->getBone(boneName);

		bone->setManuallyControlled(mc);
	}
	//--------------------------------------------------------------------

	

} // namespace Graphics
