/**
@file RibbonTrailComponent.h

@author Alberto Martínez Villarán
@date Mayo, 2015
*/

#ifndef __Logic_RibbonTrailComponent_H
#define __Logic_RibbonTrailComponent_H

#include "Logic/Entity/Component.h"

// Predeclaración de tipos
namespace Graphics {
	class CRibbonTrail;
	class CSceneNode;
};


// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	
	class CRibbonTrailComponent : public IComponent
	{
		DEC_FACTORY(CRibbonTrailComponent);
	public:
		
		//constructor
		CRibbonTrailComponent() : _trail(0), _startVisible(true), _needNode(true), _trailMaterial(""), _width(10.0f), _widthChange(0.0f), _length(40.0f),
			_followCenter(false), _centerNode(0), _colorChange(Vector3::ZERO) {};

		///destructor
		virtual ~CRibbonTrailComponent();
		

		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

	
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		
		virtual void onTick(unsigned int msecs);
 
		virtual bool respawn(const Map::CEntity *entity);

		virtual bool activate();

		virtual void deactivate();


		void setTrailWidthChange(float widthChange);

		void setTrailLength(float length);

		void setTrailVisible(bool visible);

	protected:
		///flag que nos sirve para iniciar el trail invisible y luego activarlo a necesidad
		bool _startVisible;

		///flag que nos indica si la entidad a la que sigue el trail es de las entidades que no necesitan nodo (Armas)
		bool _needNode;

		bool _followCenter;

		Graphics::CSceneNode* _centerNode;

		//material del trail
		std::string _trailMaterial;

		//ancho
		float _width;

		//gradiente de cambio del ancho
		float _widthChange;

		//Longitud del trail
		float _length;

		///Puntero al trail
		Graphics::CRibbonTrail* _trail;

		Vector3 _colorChange;

	}; // class CRibbonTrailComponent

	REG_FACTORY(CRibbonTrailComponent);

} // namespace Logic

#endif // __Logic_RibbonTrailComponent_H
