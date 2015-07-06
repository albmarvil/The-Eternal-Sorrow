/*
file CameraEffects.cpp

Este fichero contiene la implementación del componente CameraEffects

Este componente se encarga de lanzar efectos de postprocesado en el viewport asociado a la escena de juego

@author Alberto Martínez Villarán
@date Abril, 2015
*/
#include "CameraEffects.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"
#include "Application/BaseApplication.h"

#include "Graphics/Scene.h"
#include "Graphics/Camera.h"



namespace Logic 
{
	IMP_FACTORY(CCameraEffects);
	


	//---------------------------------------------------------

	bool CCameraEffects::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return message->getType() == "CHANGE_VIEWPORT";
	} // accept
	
	//---------------------------------------------------------

	void CCameraEffects::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		if(message->getType().compare("CHANGE_VIEWPORT") == 0)
		{
			CHANGE_VIEWPORT* m = dynamic_cast<CHANGE_VIEWPORT*>(message.get());
			bool activate = m->getActivate();
			std::string effect = m->getEffect();

			ChangeEffect(effect, activate);

		}
	} // process

	//------------------------------------------------------------------------


	//------------------------------------------------------------------------

	void CCameraEffects::ChangeEffect(const std::string &effect, bool activate)
	{
		_entity->getMap()->getScene()->getCamera()->setCompositorEffect(effect, activate);
	}

} // namespace Logic