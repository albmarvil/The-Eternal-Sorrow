/**
@author Cesar Manuel Paz Guzman
@date Mayo, 2015
*/


#include "ArrowEnemy.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"
#include "LUA/ScriptManager.h"
#include "Logic/Server.h"

#include <CEGUI/CEGUI.h>
#include "cegui\System.h"
#include "CEGUI\GUIContext.h"

#include "Graphics\Scene.h"
#include "Graphics\Camera.h"
#include "Graphics\Server.h"
#include "Physics/Server.h"
namespace Logic 
{
	IMP_FACTORY(CArrowEnemy);
	
	//---------------------------------------------------------

	bool CArrowEnemy::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_numArrowsMax = 0;
		return true;

	} // spawn

	//---------------------------------------------------------

	bool CArrowEnemy::activate()
	{
		std::string nombre = LUA::CScriptManager::GetPtrSingleton()->getField("Nombre_Habilidad", "CArrowEnemy", "");
		LUA::CScriptManager::GetPtrSingleton()->executeFunction("HadHability", nombre, _numArrowsMax);
		
		if(_numArrowsMax > 0)
		{
			_isSleeping = false;
		}
		//Nos creamos las flechas oportunas y las ponemos a setvisible false
		for(int i = 0; i<_numArrowsMax; ++i)
		{
			CEGUI::Window* arrow = CEGUI::WindowManager::getSingleton().createWindow("Generic/Image","Arrow"+std::to_string(i));
			CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->
				getChild("Arrow")->addChild(arrow);

			//Hacemos invisible esta ventana
			arrow->setVisible(false);
			arrow->setProperty("Image", "NewHUD/Arrow");

			//Metemos en el map las flechas sin asignar
			_enemiesWithArrow[arrow] = 0;	
			
		}

		_enemiesOutView = std::list<CEntity*>();

		return true;
	}

	void CArrowEnemy::deactivate()
	{
		std::map<CEGUI::Window*,CEntity*>::iterator it = _enemiesWithArrow.begin();

		for(; it!=_enemiesWithArrow.end(); ++it)
		{
			it->first->destroy();
		}
	}

	bool CArrowEnemy::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		//Recibimos un mensaje de tipo HABILIDAD
		return message->getType() == "SET_HABILIDAD";
	} // accept
	
	//---------------------------------------------------------

	void CArrowEnemy::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("SET_HABILIDAD") == 0)
		{
			if(!dynamic_cast<SET_HABILIDAD*>(message.get())->getString().compare("ArrowEnemy"))
			{
				//Despertamos el componente, si no estaba despierto antes
				if(_isSleeping)
				{
					_isSleeping = false;
				}

				++_numArrowsMax;

				CEGUI::Window* arrow = CEGUI::WindowManager::getSingleton().createWindow("Generic/Image","Arrow"+std::to_string(_numArrowsMax));
				CEGUI::System::getSingletonPtr()->getDefaultGUIContext().getRootWindow()->
					getChild("Arrow")->addChild(arrow);

				//Hacemos invisible esta ventana
				arrow->setVisible(false);
				arrow->setProperty("Image", "NewHUD/Arrow");
				arrow->setProperty("Area","{{0,0},{0,0},{0.1,0},{0.1,0}}");

				//Metemos en el map las flechas sin asignar
				_enemiesWithArrow[arrow] = 0;	

				//Apuntamos en lua la habilidad que se ha recogido
				LUA::CScriptManager::GetPtrSingleton()->executeProcedure("AddHability", _name);
			}		
		}
	} // process
	//------------------------------------------------------------------------

	void CArrowEnemy::onTick(unsigned int msecs)
	{

		//Primero obtenemos los enemigos fuera del viewport
		obteneterEnemigosFueraViewport();

		//Comprobmos qe los enemigos asociados a las flechas siguen fuera del viewport
		checkEnemiesWithArrow();

		//Recorremos cada una de las flechas y le aplicamos su rotacion
		std::map<CEGUI::Window*, CEntity*>::iterator it = _enemiesWithArrow.begin();
		for(; it!= _enemiesWithArrow.end();++ it)
		{
			if((*it).second!=0)
			{
				rotateArrow((*it).first, (*it).second);
			}
			else
			{
				(*it).first->setVisible(false);
			}
		}
	}

	void CArrowEnemy::addEnemy(int id, CEntity *entity)
	{
		_enemies[id] = entity;
	}

	void CArrowEnemy::removeEnemy(int id)
	{
		_enemies.erase(id);

		std::map<CEGUI::Window*,CEntity*>::iterator it = _enemiesWithArrow.begin();
		for(;it != _enemiesWithArrow.end(); ++it)
		{
			if(_entity->getMap()->getEntityByID(id) == (*it).second)
			{
				_enemiesWithArrow[(*it).first] = 0;
				(*it).first->setVisible(false);
				break;
			}
		}
	}

	void CArrowEnemy::obteneterEnemigosFueraViewport()
	{
		std::map<int,CEntity *>::iterator it = _enemies.begin();
		Vector2 screenPosition = Vector2::ZERO;

		_enemiesOutView.clear();

		for(;it != _enemies.end(); ++it)
		{
			//Obtenemos la posicion en pantalla del enemigo
			screenPosition = CServer::getSingletonPtr()->getMap()->getScene()->getCamera()->getScreenCoord(it->second->getCenterPosition());
			
			//Si esta fuera del viewport, lo almacenamo en el vector
			if(isOutViewport(screenPosition))
			{
				bool almacenar = true;

				std::map<CEGUI::Window*,CEntity*>::iterator it2 = _enemiesWithArrow.begin();
				for(; it2 != _enemiesWithArrow.end(); ++it2 )
				{
					if(it2->second == it->second)
					{
						almacenar = false;
						break;
					}
				}

				if(almacenar)
					_enemiesOutView.push_back(it->second);
			}
		}
	}

	void CArrowEnemy::checkEnemiesWithArrow()
	{
		std::map<CEGUI::Window*,CEntity*>::iterator it = _enemiesWithArrow.begin();
		Vector2 screenPosition = Vector2::ZERO;
		
		for(;it != _enemiesWithArrow.end(); ++it)
		{
			//Si tiene una entidad asociada
			if(it->second != 0)
			{
				Vector2 screenPosition = CServer::getSingletonPtr()->getMap()->getScene()->getCamera()->getScreenCoord(it->second->getCenterPosition());
		
				//Si no esta fuera del viewport, lo quitamos
				if(!isOutViewport(screenPosition))
				{

					//Asignamos una flecha a otro enemigo que estea fuera
					asignArrow(it->first);
				}
			}
			else
			{
				asignArrow(it->first);
			}
		}
	}

	bool CArrowEnemy::isOutViewport(const Vector2 &screenPosition)
	{
		return(screenPosition.x > 1 || screenPosition.x < 0 || screenPosition.y > 1|| screenPosition.y < 0);
	}

	void CArrowEnemy::asignArrow(CEGUI::Window* arrow)
	{
		if(!_enemiesOutView.empty())
		{
			_enemiesWithArrow[arrow] = *_enemiesOutView.begin();
			_enemiesOutView.pop_front();
			arrow->setVisible(true);

		}
		else
		{
			_enemiesWithArrow[arrow] = 0;
			arrow->setVisible(false);
		}
	}

	void CArrowEnemy::rotateArrow(CEGUI::Window* arrow, CEntity *ent)
	{
		Vector3 dirInicial = Vector3::UNIT_X;

		float offset = 0.03;
		Vector2 screenPosition = CServer::getSingletonPtr()->getMap()->getScene()->getCamera()->getScreenCoord(ent->getCenterPosition());
		if(screenPosition.x < 0)
		{
			screenPosition.x = 0 + offset;
		}
		else if(screenPosition.x>1)
		{
			screenPosition.x = 1 - offset;
		}
		
		if(screenPosition.y < 0)
		{
			screenPosition.y = 0 + offset;
		}
		else if(screenPosition.y > 1)
		{
			screenPosition.y = 1 - offset;
		}
		
		arrow->setProperty("Area","{{"+std::to_string(screenPosition.x)+",0},{"+std::to_string(screenPosition.y)+",0},{"
			+std::to_string(screenPosition.x+0.02)+",0},{"+std::to_string(screenPosition.y+0.04)+",0}}");

		//A esta función le pasamos la posición relativa en x e y
		Ray mouseRay = Graphics::CServer::getSingletonPtr()->getCameraToViewportRay(screenPosition.x, screenPosition.y);

		//Definimos la distancia máxima a la que lanzaremos el rayo
		float max_distance = 600;
		Vector3 point = Vector3::ZERO;

		//lanzamos el rayo y guardamos en la variable point la posición actual en el mundo del ratón
		Physics::CServer::getSingletonPtr()->raycastSimple(mouseRay, max_distance, Physics::CollisionGroup::ePlanoColision, point);
		
		Vector3 dirDestino = point - _entity->getCenterPosition();
		dirDestino.normalise();

		//Obtengo el angulo en radianes
		Ogre::Radian angleArrow = dirInicial.angleBetween(dirDestino);
		float angle = angleArrow.valueDegrees();

		//BaseSubsystems::Log::Debug("Nombre: "+ent->getName()+" Angle: "+std::to_string(angle));

		if(ent->getPosition().y < _entity->getPosition().y)
		{
			arrow->setRotation(CEGUI::Quaternion::eulerAnglesDegrees(0.0f,0.0f,angleArrow.valueDegrees()));
		}
		else
		{
			arrow->setRotation(CEGUI::Quaternion::eulerAnglesDegrees(0.0f,0.0f,-angleArrow.valueDegrees()));
		}
		
		/*if (angleArrow.valueDegrees() >= 0.0f && angleArrow.valueDegrees() <= 180)
			arrow->setRotation(CEGUI::Quaternion::eulerAnglesDegrees(0.0f,0.0f,-angleArrow.valueDegrees()));
		else 
			arrow->setRotation(CEGUI::Quaternion::eulerAnglesDegrees(0.0f,0.0f,180+angleArrow.valueDegrees()));*/
	}
	
} // namespace Logi