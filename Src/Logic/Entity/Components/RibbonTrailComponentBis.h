/**
@file RibbonTrailComponentBis.h

@author Alberto Martínez Villarán
@date Mayo, 2015
*/

#ifndef __Logic_RibbonTrailComponentBis_H
#define __Logic_RibbonTrailComponentBis_H

#include "Logic/Entity/Component.h"

// Predeclaración de tipos
namespace Graphics {
	class CRibbonTrail;
	class CSceneNode;
};


// Los componentes pertenecen al namespace Logic
namespace Logic 
{	
	
	class CRibbonTrailComponentBis : public IComponent
	{
		DEC_FACTORY(CRibbonTrailComponentBis);
	public:
		
		//constructor
		CRibbonTrailComponentBis() : _trail(0), _startVisible(true), _trailMaterial(""), _width(10.0f), _widthChange(0.0f), _length(40.0f), _centerNode(0), _offset(Vector3::ZERO), _colorChange(Vector3::ZERO) {};

		///destructor
		virtual ~CRibbonTrailComponentBis();
		

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

		Graphics::CSceneNode* _centerNode;

		//material del trail
		std::string _trailMaterial;

		//ancho
		float _width;

		//gradiente de cambio del ancho
		float _widthChange;

		//Longitud del trail
		float _length;

		//Offset desde el centro de la entidad
		Vector3 _offset;

		Vector3 _colorChange;

		///Puntero al trail
		Graphics::CRibbonTrail* _trail;

	}; // class CRibbonTrailComponentBis

	REG_FACTORY(CRibbonTrailComponentBis);

} // namespace Logic

#endif // __Logic_RibbonTrailComponentBis_H
