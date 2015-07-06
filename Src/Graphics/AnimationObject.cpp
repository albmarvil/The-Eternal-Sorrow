/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/

#include "AnimationObject.h"
#include "OgreAnimationState.h"
#include "OgreAnimation.h"
#include "OgreKeyFrame.h"
#include "Entity.h"
#include "OgreEntity.h"
#include "Scene.h"

namespace Graphics
{
	CAnimationObject::CAnimationObject(const std::string &nameAnimation): 
		_nameAnimation(nameAnimation), _animationState(NULL)
	{
	}

	CAnimationObject::~CAnimationObject()
	{

	}

	void CAnimationObject::createAnimationFloating(CEntity *_graphicEntity)
	{
		//Obtenemos el sceneNode asociado a la entidad grafica
		Ogre::SceneNode *entityNode = _graphicEntity->getEntityNode();
		_sceneMgr = _graphicEntity->getScene()->getSceneMgr();

		Ogre::Real duracion = 1.5f; //Duracion totl de la animacion
		unsigned int numPasos = 4; //Centro, Arriba, centro, abajo, centro
		Ogre::Real longitudPaso = duracion / (Ogre::Real)numPasos;
		Ogre::Real tamDesplazamiento = 3.5f; //Desplazamiento maximo

		//Establecemos la posicion inicial 
		Vector3 keyframePos = Vector3::ZERO;

		//Establece el actual transform del nodo para que el animation state sepa el estado inicial, y realice las operaciones necesarias
		entityNode->setInitialState();

		//Creo una nueva animacion
		Ogre::Animation *animation = _sceneMgr->createAnimation(_nameAnimation, duracion);

		//Crea un track de animacion para controlar un nodo de escena. El numero debe ser unico dentro de esta animacion (en caso de crear mas tracks)
		Ogre::NodeAnimationTrack *track = animation->createNodeTrack(0);

		//Añadimos los keyframes. Queremos que el item flctue desde su posicion inicial a una posicion mas alto, luego que vuelva a pasar
		//por la posicion inicial y vaya a la posicion opuesta, y finalmente vuelva al origen. 
		Ogre::TransformKeyFrame *kf;
		//Keyframe 0: Centro
		kf = track->createNodeKeyFrame(longitudPaso * 0);
		kf->setTranslate(keyframePos);

		//Keyframe 1: Arriba
		kf = track->createNodeKeyFrame(longitudPaso * 1);
		keyframePos += Vector3::UNIT_Y * tamDesplazamiento;
		kf->setTranslate(keyframePos);

		//Keyframe 2: Nos lo saltamos, centro

		//keyframe3: Abajo
		kf = track->createNodeKeyFrame(longitudPaso * 3);
		keyframePos += Vector3::UNIT_Y * tamDesplazamiento * -2;
		kf->setTranslate(keyframePos);

		//keyframe4: centro
		kf = track->createNodeKeyFrame(longitudPaso * 4);
		keyframePos += Vector3::UNIT_Y * tamDesplazamiento;
		kf->setTranslate(keyframePos);

		//Asociamos el nodo de escena a la animacion
		track->setAssociatedNode(entityNode);
		animation->setInterpolationMode(Ogre::Animation::IM_SPLINE);

		//Creamos el animationState asociado a la animacion.
		_animationState = _sceneMgr->createAnimationState(_nameAnimation);

		//Esta instancia no esta activada por defecto en su momento de creacion. 
		_animationState->setEnabled(true);
	}
	
	void CAnimationObject::tick(unsigned int msecs)
	{
		//Para que la animacion avance, es necesario indicale al AnimationState cuanto tiempo ha transcurrido en segundos. 
		if (_animationState)
		{
			_animationState->addTime((float)msecs * 0.001f);
		}
	}

	void CAnimationObject::createAnimationRotating(CEntity *_graphicEntity)
	{
		//Obtenemos el sceneNode asociado a la entidad grafica
		Ogre::SceneNode *entityNode = _graphicEntity->getEntityNode();
		_sceneMgr = _graphicEntity->getScene()->getSceneMgr();

		Ogre::Real duracion = 1.5f; //Duracion totl de la animacion
		unsigned int numPasos = 4; //Centro, Arriba, centro, abajo, centro
		Ogre::Real longitudPaso = duracion / (Ogre::Real)numPasos;
		Ogre::Quaternion quat;

		//Establece el actual transform del nodo para que el animation state sepa el estado inicial, y realice las operaciones necesarias
		entityNode->setInitialState();

		//Creo una nueva animacion
		Ogre::Animation *animation = _sceneMgr->createAnimation(_nameAnimation, duracion);

		//Crea un track de animacion para controlar un nodo de escena. El numero debe ser unico dentro de esta animacion (en caso de crear mas tracks)
		Ogre::NodeAnimationTrack *track = animation->createNodeTrack(0);

		//Añadimos los keyframes. Queremos que el item flctue desde su posicion inicial a una posicion mas alto, luego que vuelva a pasar
		//por la posicion inicial y vaya a la posicion opuesta, y finalmente vuelva al origen. 
		Ogre::TransformKeyFrame *kf;

		//Keyframe 0: 
		kf = track->createNodeKeyFrame(longitudPaso * 0);
		quat.FromAngleAxis(Ogre::Degree(0), Vector3::UNIT_Y);
		kf->setRotation(quat);

		//Keyframe 1: 
		kf = track->createNodeKeyFrame(longitudPaso * 1);
		quat.FromAngleAxis(Ogre::Degree(90), Vector3::UNIT_Y);
		kf->setRotation(quat);

		//Keyframe 2:  
		kf = track->createNodeKeyFrame(longitudPaso * 2);
		quat.FromAngleAxis(Ogre::Degree(180), Vector3::UNIT_Y);
		kf->setRotation(quat);

		//keyframe3: 
		kf = track->createNodeKeyFrame(longitudPaso * 3);
		quat.FromAngleAxis(Ogre::Degree(270), Vector3::UNIT_Y);
		kf->setRotation(quat);

		//keyframe4: 
		kf = track->createNodeKeyFrame(longitudPaso * 4);
		quat.FromAngleAxis(Ogre::Degree(360), Vector3::UNIT_Y);
		kf->setRotation(quat);

		//Asociamos el nodo de escena a la animacion
		track->setAssociatedNode(entityNode);

		//Creamos el animationState asociado a la animacion.
		_animationState = _sceneMgr->createAnimationState(_nameAnimation);

		//Esta instancia no esta activada por defecto en su momento de creacion. 
		_animationState->setEnabled(true);
	}

	void CAnimationObject::destroyAnimationState()
	{
		if(_sceneMgr->hasAnimationState(_nameAnimation))
			_sceneMgr->destroyAnimationState(_nameAnimation);

		if(_sceneMgr->hasAnimation(_nameAnimation))
			_sceneMgr->destroyAnimation(_nameAnimation);

		_animationState = NULL;
	}

	void CAnimationObject::createAnimation(CEntity *_graphicEntity, int typeAnimation)
	{
		switch(typeAnimation)
		{
		case tipoAnimacion::flotante :
			createAnimationFloating(_graphicEntity);
			break;
		case tipoAnimacion::rotacion:
			createAnimationRotating(_graphicEntity);
			break;
		default:
			break;
		}
	}
}