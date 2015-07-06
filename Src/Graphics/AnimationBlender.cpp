//---------------------------------------------------------------------------
// AnimationBlender.cpp
//---------------------------------------------------------------------------

/**
@file AnimationBlender.cpp

Contiene la implementación de la clase CAnimationBlender. Esta clase se encarga de encapsular una animación
dandole la funcionalidad necesaria para hacer blending de animaciones cuando se transita de una animación a otra

@see Graphics::CAnimationBlender


@author Alberto Martínez
@date Marzo, 2015
*/

#include "BaseSubsystems/Log.h"

#include "AnimationBlender.h"
#include "AnimatedEntity.h"

#include "LUA\ScriptManager.h"

#include <assert.h>

#include <OgreAnimationState.h>

namespace Graphics 
{
	
	/*
	Constructor que crea la animación y la inicializa
	*/
	CAnimationBlender::CAnimationBlender(const std::string &name, CAnimatedEntity *entity, bool loop, bool restart, float speed, std::vector<std::pair<float, float>> animationMask, int skeletonSize)
	{
		_skeletonSize = skeletonSize;
		_currentName = name;
		_animatedEntity = entity;
		_loop = loop;
		_currentSpeed = speed;
		_currentWeight = 1.0f;
		_currentAnimationMask = animationMask;

		_targetState = 0;
		_targetName = "";
		_loopTarget = false;
		_targetWeight = 0.0f;

		_transitionTime = 0.0f;
		_timeCount = 0.0f;

		_currentState = _animatedEntity->getAnimationState(_currentName);

		assert(_currentState && "La animación indicada no existe");

		_currentState->setEnabled(true);
		_currentState->setWeight(_currentWeight);
		_currentState->setLoop(_loop);
		
		///Creamos la máscara con un número de entradas igual al de huesos del esqueleto
		_currentState->createBlendMask(_skeletonSize, 0);
		setAnimationMask(_currentState, _currentAnimationMask);
	
		if(restart)
		{
			_currentState->setTimePosition(0);
		}

		///Si la animación current no tiene loop, sacamos su transición por defecto

		/*std::string defaultTarget = 0;

		if(!LUA::CScriptManager::GetPtrSingleton()->executeFunction("getDefaultTransition", "ANIMATIONCONFIG", _currentName, defaultTarget))
		{
			defaultTarget = "-";
		}

		float duration = 0;

		if(!LUA::CScriptManager::GetPtrSingleton()->executeFunction("getDefaultTransition",))
		{
			duration = 0;
		}*/
		
	}//CAnimation

	//-------------------------------------------------------------------------ç

	CAnimationBlender::~CAnimationBlender ()
	{
		/*if (_currentState != NULL)
		{
			_currentState->setEnabled(false);
			_currentState->setTimePosition(0);
		}*/
		_currentState = 0;
		_currentName = "";

		/*if(_targetState != NULL)
		{
			_targetState->setEnabled(false);
			_targetState->setTimePosition(0);
		}*/
		_targetState = 0;
		_targetName = "";

		_animatedEntity = 0;
	}//~CAnimation

	//-------------------------------------------------------------------


	void CAnimationBlender::tick(float secs)
	{

		if(_currentState != NULL)
		{
			_currentState->addTime(secs*_currentSpeed);

			//Si hemos acabado avisamos a la entidad Animada
			if(_currentState->hasEnded())
			{
				//BaseSubsystems::Log::Debug("Tick-> END CURRENT");
				_animatedEntity->animationFinished(_currentName);


				////OJO vamos a suponer que cuando se avisa de que una animacion acaba (no tiene loop) vas a enganchar otra con la que hacer blending

				/// si había un target la seteamos como current??
				/*_currentState->setTimePosition(0);
				_currentState->setEnabled(false);
				_currentState = 0;
				_loop = false;*/

				//if(_targetState != NULL)//transito directamente
				//{
				//	//if(!_loop)
				//	//{
				//	//	BaseSubsystems::Log::Debug("Vuelvo a lanzar anim sin loop para blending");
				//	//	//lanzo de nuevo la current y dejo que haga blending
				//	//	_currentState->setTimePosition(0);
				//	//	_currentState->setEnabled(true);
				//	//	_currentWeight = 1.0f;
				//	//	_targetWeight = 0.0f;
				//	//	//_timeCount = 0;
				//	//}
				//	//else
				//	//{
				//		BaseSubsystems::Log::Debug("Anim acabada y sin loop con trans directa");
				//		
				//		///transito directamente
				//		_currentSpeed = _targetSpeed;
				//		_currentState = _targetState;
				//		_currentName = _targetName;
				//		_currentAnimationMask = _targetAnimationMask;
				//		_targetWeight = 0;
				//		_currentWeight = 1;
				//		_currentState->setEnabled(true);
				//		_currentState->setWeight(_currentWeight);
				//		_loop = _loopTarget;
				//		_currentState->setLoop(_loop);
				//		setAnimationMask(_currentState, _currentAnimationMask);
				//	//}

				//}
			}


			if(_targetState != NULL)
			{
				_targetState->addTime(secs * _targetSpeed);

				_timeCount += secs;

				//Si no he acabado la transicion
				if(_timeCount < _transitionTime)
				{
					//actualizamos los pesos del current y del target

					float delta = secs / _transitionTime;

					_currentWeight -= delta;
					_targetWeight += delta;

					_currentState->setWeight(_currentWeight);
					_targetState->setWeight(_targetWeight);

					//BaseSubsystems::Log::Debug("TICK -> blending!");
					//BaseSubsystems::Log::Debug("TICK-> timeCount: "+std::to_string(_timeCount));
					//BaseSubsystems::Log::Debug("TICK-> delta: "+std::to_string(delta));
					//BaseSubsystems::Log::Debug("TICK-> current: "+_currentName+" - "+std::to_string(_currentWeight));
					//BaseSubsystems::Log::Debug("TICK-> target: "+_targetName+" - "+std::to_string(_targetWeight));


				}
				else
				{
					//BaseSubsystems::Log::Debug("Desactivo current: "+_currentName);
					_currentState->setEnabled(false);
					_currentState->setWeight(0);

					//avisamos de que la animacion current ha acabado
					_animatedEntity->animationFinished(_currentName);

					_currentSpeed = _targetSpeed;
					_currentState = _targetState;
					_currentName = _targetName;
					_currentAnimationMask = _targetAnimationMask;

					_currentState->setEnabled(true);
					_currentState->setWeight(1);
					setAnimationMask(_currentState, _currentAnimationMask);
					//BaseSubsystems::Log::Debug("Ahora target es current: "+_currentName);


					_loop = _loopTarget;
					_currentState->setLoop(_loop);

					_currentWeight = 1.0;
					_targetWeight = 0.0;

					_timeCount = 0;
					_transitionTime = 0;

					_targetState = 0;
					_targetName = "";
					//BaseSubsystems::Log::Debug("TICK2 -> End blend");
					//BaseSubsystems::Log::Debug("TICK2-> current: "+_currentName+" - "+std::to_string(_currentWeight));
					//BaseSubsystems::Log::Debug("TICK2-> target: "+_targetName+" - "+std::to_string(_targetWeight));
				}
			}
		}
	}//tick
	//--------------------------------------------------------------------

	bool CAnimationBlender::blend(const std::string &targetAnim, std::vector<std::pair<float, float>> animationMask, bool loop, bool restart, float duration, float speed)
	{
		//si había una animación de transición activada la cancelo antes de sustituirla por la nueva
		if(_targetState != NULL)
		{
			//BaseSubsystems::Log::Debug("Habia una animacion de transicion activada: "+_targetName+ " cancelada por: "+targetAnim);
			_transitionTime = 0;
			_timeCount = 0;
			_targetWeight = 0;
			_targetName = "";
			_targetState->setEnabled(false);
			_targetState->setWeight(_targetWeight);
			_targetState = 0;

			_currentWeight = 1;
			_currentState->setWeight(_currentWeight);
		}

		//Si no hay animación actual esta pasa a ser la animación principal
		if(_currentState == NULL)
		{
			//BaseSubsystems::Log::Debug("No teniamos current Animation. "+targetAnim+" pasa a ser la nueva current");
			_currentState = _animatedEntity->getAnimationState(targetAnim);

			if(_currentState == NULL)
			{
				return false;
			}

			_currentWeight = 1;
			_currentName = targetAnim;
			_loop = loop;
			_currentSpeed = speed;
			_currentAnimationMask = animationMask;
			_currentState->setEnabled(true);
			_currentState->setLoop(_loop);
			_currentState->setWeight(_currentWeight);
			_currentState->createBlendMask(_skeletonSize, 0);
			setAnimationMask(_currentState, _currentAnimationMask);


			if(restart)
			{
				_currentState->setTimePosition(0);
			}

			return true;
		}
		else if (_currentName != targetAnim)//transitamos solo si la animacion objetivo es diferente
		{
			//BaseSubsystems::Log::Debug("La animacion objetivo es diferente a la actual. Actual: "+_currentName + " Target: "+targetAnim);
			_targetState = _animatedEntity->getAnimationState(targetAnim);

			if(_targetState != NULL)
			{
				_transitionTime = duration;

				_targetWeight = 0.0;
				_currentWeight = 1.0;
				_currentState->setWeight(_currentWeight);

				_targetAnimationMask = animationMask;
				_targetName = targetAnim;
				_targetState->setEnabled(true);
				_targetState->setWeight(_targetWeight);
				_loopTarget = loop;
				_targetSpeed = speed;
				_targetState->setLoop(_loopTarget);
				_targetState->createBlendMask(_skeletonSize, 0);
				setAnimationMask(_targetState, _targetAnimationMask);

				if(restart)
				{
					_targetState->setTimePosition(0);
				}

				return  true;
			}
			else
			{
				return false;
			}

		}
	}//blend

	//-------------------------------------------------------------------------

	bool CAnimationBlender::stopAnimation()
	{
		if(_currentState != NULL)
		{
			
			//paramos la animacion
			_currentState->setEnabled(false);

			//notificar del fin de animación
			_animatedEntity->animationFinished(_currentName);
			
			//detenemos la transicion actual
			_currentWeight = 1.0;
			_currentState->setWeight(_currentWeight);
			_transitionTime = 0;
			_timeCount = 0;

			_currentState = 0;
			_currentAnimationMask.clear();

			_targetWeight = 0;
			if(_targetState != NULL)
			{
				_targetState->setEnabled(false);
				_targetState->setWeight(_targetWeight);
				_targetState = 0;
			}
			
			_targetState = 0;
			_targetName = "";
			_targetAnimationMask.clear();
			
			return true;
		}
		else
		{
			return false;
		}
	}//stopAniamtion

	//-------------------------------------------------------------------

	void CAnimationBlender::restartAnimation()
	{
		if(_currentState != NULL)
		{
			_currentState->setTimePosition(0);
		}
		
	}//restartAnimation

	///-----------------------------------------------------------------


	void CAnimationBlender::setAnimationMask(Ogre::AnimationState* anim, std::vector<std::pair<float, float>> mask)
	{
		//si la máscara tiene tamaño 0, añadimos todas las entradas con un 1 de peso
		if(mask.size() == 0)
		{
			for (int j = 0; j < _skeletonSize; ++j)
			{
				anim->setBlendMaskEntry((size_t) j, 1);
			}
		}
		else
		{
			for (int i = 0; i < mask.size(); ++i)
			{
				size_t boneHandle = (size_t) mask[i].first;
				float weight = mask[i].second;

				anim->setBlendMaskEntry(boneHandle, weight);
			}
		}

		
	}

} // namespace Graphics
