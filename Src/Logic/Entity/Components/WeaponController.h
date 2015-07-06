/*
@file WeaponController.h

Contiene la declaración de la clase CWeaponController

@author Alberto Martínez Villarán
@date Febrero 2015
@refactor Alberto Martínez Villarán
@date Abril 2015

*/
#ifndef __Logic_WeaponController_H
#define __Logic_WeaponController_H

#include "Logic/Entity/Component.h"


namespace Graphics
{
	class CSceneNode;
};

namespace Logic
{
	class CPhysicWeapon;
};
//declaración de la clase
namespace Logic 
{
	class CWeaponController : public IComponent
	{
		DEC_FACTORY(CWeaponController);
		public:

			/**
			Constructor por defecto; en la clase base no hace nada.
			*/
			CWeaponController(): IComponent(), _weaponTransform(Matrix4::ZERO), _node(0) {}

			virtual ~CWeaponController();
		
			/**
			Inicialización del componente usando la descripción de la entidad que hay en 
			el fichero de mapa.
			*/
			virtual bool OnSpawn(const Map::CEntity *entityInfo);

			virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

			virtual void process(const std::shared_ptr<Logic::IMessage> &message);

			bool respawn(const Map::CEntity *entityInfo);

			/**
			Método llamado en cada frame que actualiza el estado del componente.
			<p>
			Se encarga de mover la entidad en cada vuelta de ciclo cuando es
			necesario (cuando está andando o desplazándose lateralmente).

			@param msecs Milisegundos transcurridos desde el último tick.
			*/
			//virtual void onTick(unsigned int msecs);

			virtual bool activate();

			virtual void deactivate();

			Matrix4 getPivotTransform() { return _pivotTransform; }

			Graphics::CSceneNode* getPivotNode() { return _node; }


			void setTransformBone(const Matrix4 &transform);

		protected:

			//Nodo de escena del punto de pivote asociado al arma
			Graphics::CSceneNode* _node;

			std::string _attackType;

			Matrix4 _weaponTransform;

			Vector3 _initOffset;

			Vector3 _pivot;

			Matrix4 _pivotTransform;

			Vector3 _dirFromOrigin;

			/*
			Componente físico del arma asociado
			*/
			CPhysicWeapon* _componenteFisico;

	}; // class CBalaController

	REG_FACTORY(CWeaponController);

} // namespace Logic

#endif // __Logic_BalaController_H
