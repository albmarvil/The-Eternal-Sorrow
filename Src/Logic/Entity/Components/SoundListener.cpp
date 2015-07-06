#include "SoundListener.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Maps/Map.h"
#include "Map/MapEntity.h"

#include "Sounds/Listener.h"

namespace Logic 
{
	IMP_FACTORY(SoundListener);

	SoundListener::SoundListener() : IComponent()
	{

	}
	//--------------------------------------------------------

	SoundListener::~SoundListener()
	{
		delete _listener;
	}

	//---------------------------------------------------------

	bool SoundListener::OnSpawn(const Map::CEntity *entityInfo) 
	{
		_listener = new Sounds::CListener();
		return true;

	} // spawn


	bool SoundListener::respawn(const Map::CEntity *entityInfo) 
	{
		
		return true;

	} // respawn
	//---------------------------------------------------------


	bool SoundListener::activate() 
	{
		//Nos registramos como observador de la posicion lógica de la entidad
		_entity->addEntityTransformListener(this);

		return true;

	} // activate
	//---------------------------------------------------------

	void SoundListener::deactivate() 
	{
		//mandamos al listener a tomar por saco
		//hay que investigar como hacer que un listener deje de actuar sin necesidad de hacerle un delete
		_listener->setPosition(Vector3(-1000,1000,1000));

		//nos desregistramos como observador de la posicion logica de la entidad
		_entity->removeEntityTransformListener(this);

	} // deactivate
	//--------------------------------------------------------

	bool SoundListener::accept(const std::shared_ptr<Logic::IMessage> &message)
	{
		return false;//message->getType() == "SET_TRANSFORM";

	} // accept
	
	//---------------------------------------------------------

	void SoundListener::process(const std::shared_ptr<Logic::IMessage> &message)
	{
		//if(message->getType() == "SET_TRANSFORM")
		//{
		//	Matrix4 trans = dynamic_cast<SET_TRANSFORM*>(message.get())->getTransform();
		//	//cambiamos la posicion a todos los sonidos emitidos desde esa entidad
		//	setListenerTransform(trans);
		//}
		
	} // process

	//---------------------------------------------------------------------------------

	void SoundListener::onTick(unsigned int msecs)
	{
		
	}

	void SoundListener::setListenerTransform(const Matrix4 &transform)
	{
		_listener->setPosition(transform.getTrans());
		//Por ahora solo seteamos la posicion y el vector forward hacia donde queremos
		//estamos dando por supuesto que este vector es desde la cámara
		//_listener->setForwardDirection(Vector3(0,0,-1));
	}

	void SoundListener::OnEntitySetTransform(const Matrix4 &transform)
	{
		setListenerTransform(transform);
	}

} // namespace Logic