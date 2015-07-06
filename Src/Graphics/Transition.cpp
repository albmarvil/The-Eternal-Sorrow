//---------------------------------------------------------------------------
// Transition.cpp
//---------------------------------------------------------------------------

/**
@file Transition.h

Contiene la implementación de la clase base de las transiciones. Además contiene las implementaciones e implementaciones de las
diferentes transiciones del juego. 

Una transición se subdivide a su vez en dos efectos
	-Efecto entrada
	-Efecto salida

Cada transición recibirá un tick que sirve para ejecutar el efecto, ya sea de entrada o de salida.

Además una transición tendrá configurado un parámetro de duración de ambos efectos.

@see Graphics::CTransition

@author Alberto Martínez
@date Abril, 2015
*/

#include "Entity.h"
#include "Scene.h"
#include "Server.h"
#include "Camera.h"
#include "Transition.h"

#include <OgreMaterial.h>
#include <OgreEntity.h>

#include "BaseSubsystems\Log.h"
#include "BaseSubsystems\Math.h"

namespace Graphics 
{
	CTransition::~CTransition()
	{
	}

	void CTransition::tick(unsigned int msecs)
	{
		switch (_actualEffect)
		{
		case Graphics::SALIDA:
			outEffectTick(msecs);
			break;
		case Graphics::ENTRADA:
			inEffectTick(msecs);
			break;
		}
	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////  FADEIN/OUT TRANSICION   //////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CFadeInOutTransition::activateTransitionEffect(TipoEfecto efecto)
	{
		_actualEffect = efecto;
		_actualEffectFinished = false;
		_timeAcum = 0.0f;
		switch (_actualEffect)
		{
		case Graphics::SALIDA:
			{
				_fade = 0.0f;
				_planoOutEffect = new CEntity("FadeOutPlane", PrefabType::PT_PLANE);
				_planoOutEffect->setNeedNode(true);

				///añadimos el plano en la escena actual
				CServer::getSingletonPtr()->getActiveScene()->addEntity(_planoOutEffect);
				///seteamos la posición un poco delante de la camara

				_planoOutEffect->setMaterial(std::string("Transicion/Fade"));

				_planoOutEffect->setPosition(CServer::getSingletonPtr()->getActiveScene()->getCamera()->getCameraPosition() + Vector3(0,0,-10));
				
				Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("Transicion/Fade");
				material->getTechnique(0)->getPass(0)->setDiffuse(0,0,0,_fade);
				_planoOutEffect->getOgreEntity()->setMaterial(material);
			}
			break;
		case Graphics::ENTRADA:
			{
				_fade = 1.0f;
				_planoInEffect = new CEntity("FadeInPlane", PrefabType::PT_PLANE);
				_planoInEffect->setNeedNode(true);

				///añadimos el plano en la escena actual
				CServer::getSingletonPtr()->getActiveScene()->addEntity(_planoInEffect);
				///seteamos la posición un poco delante de la camara

				_planoInEffect->setMaterial(std::string("Transicion/Fade"));

				_planoInEffect->setPosition(CServer::getSingletonPtr()->getActiveScene()->getCamera()->getCameraPosition() + Vector3(0,0,-10));

				
				Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("Transicion/Fade");
				material->getTechnique(0)->getPass(0)->setDiffuse(0,0,0,_fade);
				_planoInEffect->getOgreEntity()->setMaterial(material);
			}
			break;
		}
	}


	void CFadeInOutTransition::deactivateTransitionEffect(TipoEfecto efecto)
	{
		switch (efecto)
		{
		case Graphics::SALIDA:
			CServer::getSingletonPtr()->getActiveScene()->removeEntity(_planoOutEffect);
			delete _planoOutEffect;
			break;
		case Graphics::ENTRADA:
			CServer::getSingletonPtr()->getActiveScene()->removeEntity(_planoInEffect);
			delete _planoInEffect;
			break;
		}
	}

	void CFadeInOutTransition::outEffectTick(unsigned int msecs)
	{
		_timeAcum += msecs;

		float step = (msecs / _duration);

		_fade += step;

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("Transicion/Fade");
		material->getTechnique(0)->getPass(0)->setDiffuse(0,0,0,_fade);
		_planoOutEffect->getOgreEntity()->setMaterial(material);

		if(_fade >= 1.0f || _timeAcum >= _duration)
		{
			_actualEffectFinished = true;
			_actualEffect = TipoEfecto::ENTRADA;
		}
	}

	void CFadeInOutTransition::inEffectTick(unsigned int msecs)
	{
		_timeAcum += msecs;

		float step = (msecs / _duration);

		_fade -= step;

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("Transicion/Fade");
		material->getTechnique(0)->getPass(0)->setDiffuse(0,0,0,_fade);
		_planoInEffect->getOgreEntity()->setMaterial(material);

		if(_fade <= 0.0f || _timeAcum >= _duration)
		{
			_actualEffectFinished = true;
		}
	}

} // namespace Graphics
