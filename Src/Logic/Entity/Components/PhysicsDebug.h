/**
@file PhysicsDebug.h

Contiene la declaraci�n del componente que controla la representaci�n
gr�fica de la parte f�sica de la entidad

@see Logic::CPhysicsDebug
@see Logic::IComponent

@author Alberto Mart�nez	
@date Febrero, 2015
*/
#ifndef __Logic_PhysicsDebug_H
#define __Logic_PhysicsDebug_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Entity.h"


// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CManualEntity;
	class CScene;
	class CEntity;
}

//declaraci�n de la clase
namespace Logic 
{

	class CPhysicsDebug : public IComponent , public CEntityTransformListener
	{
		DEC_FACTORY(CPhysicsDebug);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CPhysicsDebug() : IComponent(), _PhysicsDebugEntity(0) ,_scale(Vector3(1,1,1)){}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CPhysicsDebug();
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). 

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa L�gico en el que se registrar� el objeto.
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Cierto si la inicializaci�n ha sido satisfactoria.
		*/
		virtual bool OnSpawn(const Map::CEntity *entityInfo);

		/**
		Metodo que sirve para setear el entityInfo y el map en donde sera respawneada. No pongo solo la posicion, sino mas bien
		el entityInfo entero, porque puede ocurrir que queramos setear por ejemplo, la vida que tenga un enemigo, dado
		que los enemigos se haran mas fuertes. 

		@param map Mapa Logic en el que se registrara la entidad
		@param entity Informacion de construccion de la entidad leida del fichero
		@return Cierto si el respawn ha sido satisfatorio
		**/
		virtual bool respawn(const Map::CEntity *entity);

		/**
		M�todo virtual que elige que mensajes son aceptados. Son v�lidos
		SET_TRANSFORM.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		M�todo virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**
		M�todo que activa el componente; invocado cuando se activa
		el mapa donde est� la entidad a la que pertenece el componente.
		<p>
		La implementaci�n registrar� al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cron�metro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**
		M�todo que desactiva el componente; invocado cuando se
		desactiva el mapa donde est� la entidad a la que pertenece el
		componente. Se invocar� siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementaci�n eliminar� al componente de algunos observers en los 
		que pueda estar registrado (como el cron�metro del sistema, etc.).m
		*/
		virtual void deactivate();

		///M�todo de escucha de la posicion de la entidad
		void OnEntitySetTransform(const Matrix4 &transform);

	protected:

		void setTransform();

		void setTransform(Matrix4 trans);

		/**
		Atributo con el nombre del modelo gr�fico de la entidad.
		*/
		std::string _model;
		
		/**
		Entidad gr�fica del objeto manual.
		*/
		Graphics::CManualEntity *_PhysicsDebugEntity;

		/*
		Entidad Gr�fica
		*/
		Graphics::CEntity* _graphicsEntity;
		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades. La guardamos para la destrucci�n de la entidad gr�fica.
		*/
		Graphics::CScene* _scene;

		/**
		Material usado en la entidad
		*/
		std::string _material;
		
		/**
		Dimensiones de escala del objeto
		*/
		Vector3 _scale;

		/*
		offset del centro f�sico
		*/
		Vector3 _offset;

		/*
		Flag que nos indica que es un objeto manual
		*/
		bool _isManual;
		

	}; // class CPhysicsDebug

	REG_FACTORY(CPhysicsDebug);

} // namespace Logic

#endif // __Logic_PhysicsDebug_H
