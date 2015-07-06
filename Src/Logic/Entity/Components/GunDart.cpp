#include "GunDart.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/BulletController.h"
#include "Graphics/Entity.h"

namespace Logic 
{
	IMP_FACTORY(CGunDart);
	
	//---------------------------------------------------------------------------------


	void CGunDart::mainAttack(Vector3 mousePos, float baseDamage)
	{
		_delayMainAtk = 0;

		// Giramos el dardo a la dirección en la que apuntamos
		Radian angle = (mousePos - _entity->getPosition()).angleBetween(Vector3::UNIT_Y);

		Vector3 cross =  (mousePos - _entity->getPosition()).crossProduct(Vector3::UNIT_Y);
		cross.normalise();
			
		Quaternion qgiro = Quaternion(angle, -cross);

		Matrix4 transform;
		transform.makeTransform(_entity->getPosition(), _entity->getComponent<CGraphics*>()->getScale(),qgiro);

		// Creamos el dardo
		std::string typeBullet = "BulletDart";

		int randBullet = rand() % 100 + 1;
		
		if (randBullet >= 95)
			typeBullet = "BulletDartCanibal";
		
		Logic::CEntity* bullet = Logic::CEntityFactory::getSingletonPtr()->createEntityByTypeTransform(typeBullet, transform, _entity->getMap());
		Logic::CBulletController *controller = (CBulletController*) bullet->getComponent("CBulletController");
		
		assert(controller);
		
		float finalDamage = baseDamage * _mainDamageMultiplier;

		controller->setBulletConfiguration(mousePos, finalDamage);

		///Lanzamos el sonido de disparo
		std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
		std::string s = "/Varios/Cerbatana1";
		sound->setSound(s);
		_entity->emitMessage(sound, this);

	}
	
	void CGunDart::specialAttack(Vector3 mousePos, float baseDamage)
	{
		_delaySpecialAtk = 0;

		// Giramos el dardo a la dirección en la que apuntamos
		Radian angle = (mousePos - _entity->getPosition()).angleBetween(Vector3::UNIT_Y);

		Vector3 cross =  (mousePos - _entity->getPosition()).crossProduct(Vector3::UNIT_Y);
		cross.normalise();
			
		Quaternion qgiro = Quaternion(angle, -cross);

		Matrix4 transform;
		transform.makeTransform(_entity->getPosition(), _entity->getComponent<CGraphics*>()->getScale(),qgiro);

		// Creamos el dardo
		Logic::CEntity* bullet = Logic::CEntityFactory::getSingletonPtr()->createEntityByTypeTransform("BulletDartCanibalSpecial", transform, _entity->getMap());
		Logic::CBulletController *controller = (CBulletController*) bullet->getComponent("CBulletController");

		assert(controller);
		
		float finalDamage = baseDamage * _specialDamageMultiplier;

		controller->setBulletConfiguration(mousePos, finalDamage);


		///Lanzamos el sonido de disparo
		std::shared_ptr<PLAY_SOUND> sound (new PLAY_SOUND());
		std::string s = "/Varios/Cerbatana1";
		sound->setSound(s);
		_entity->emitMessage(sound, this);

	}
	//------------------------------------------------------------------------

} // namespace Logic