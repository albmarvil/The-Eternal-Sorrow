#include "GunAK47.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Logic/Entity/Components/Graphics.h"
#include "Logic/Entity/Components/BulletController.h"
#include "Graphics/Entity.h"

namespace Logic 
{
	IMP_FACTORY(CGunAK47);
	
	//---------------------------------------------------------------------------------

	void CGunAK47::mainAttack(Vector3 mousePos, float baseDamage)
	{
		_delayMainAtk = 0;

		Logic::CEntity* bullet = Logic::CEntityFactory::getSingletonPtr()->createEntityByType("NormalBullet", _entity->getPosition(), _entity->getMap());
		Logic::CBulletController *controller = (CBulletController*) bullet->getComponent("CBulletController");

		assert(controller);
		//dónde debe enviarse
		/*std::shared_ptr<Logic::MOUSE_POS> m(new Logic::MOUSE_POS());
		m->setVector3(mousePos);
		bullet->emitMessage(m);*/

		//TODO enviar mensaje de configuración de daño a la bala
		float finalDamage = baseDamage * _mainDamageMultiplier;
		/*std::shared_ptr<Logic::SET_DAMAGE> m2(new Logic::SET_DAMAGE());
		m2->setDamage(finalDamage);
		bullet->emitMessage(m2);*/

		controller->setBulletConfiguration(mousePos, finalDamage);

	}
	
	void CGunAK47::specialAttack(Vector3 mousePos, float baseDamage)
	{
		_delaySpecialAtk = 0;

		Logic::CEntity* bullet = Logic::CEntityFactory::getSingletonPtr()->createEntityByType("SpecialBullet", _entity->getPosition(), _entity->getMap());
		Logic::CBulletController *controller = (CBulletController*) bullet->getComponent("CBulletController");

		assert(controller);
		//enviamos un mensaje de movimiento
		/*std::shared_ptr<Logic::MOUSE_POS> m(new Logic::MOUSE_POS());
		m->setVector3(mousePos);
		bullet->emitMessage(m);*/

		///TODO enviar mensaje de configuración de daño a la bala
		float finalDamage = baseDamage * _specialDamageMultiplier;
		/*std::shared_ptr<Logic::SET_DAMAGE> m2(new Logic::SET_DAMAGE());
		m2->setDamage(finalDamage);
		bullet->emitMessage(m2);*/

		controller->setBulletConfiguration(mousePos, finalDamage);

	}
	//------------------------------------------------------------------------

} // namespace Logic