/**
@file PhysicsDebug.h

Contiene la declaración del componente que controla la representación
gráfica de la parte física de la entidad

@see Logic::CPhysicsDebug
@see Logic::IComponent

@author Alberto Martínez	
@date Febrero, 2015
*/
#ifndef __Logic_PhysicsDebug_H
#define __Logic_PhysicsDebug_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Entity.h"


// Predeclaración de clases para ahorrar tiempo de compilación
namespace Graphics 
{
	class CManualEntity;
	class CScene;
	class CEntity;
}

//declaración de la clase
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
		Destructor (virtual); Quita de la escena y destruye la entidad gráfica.
		*/
		virtual ~CPhysicsDebug();
		
		/**
		Inicialización del componente, utilizando la información extraída de
		la entidad leída del mapa (Maps::CEntity). 

		@param entity Entidad a la que pertenece el componente.
		@param map Mapa Lógico en el que se registrará el objeto.
		@param entityInfo Información de construcción del objeto leído del
			fichero de disco.
		@return Cierto si la inicialización ha sido satisfactoria.
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
		Método virtual que elige que mensajes son aceptados. Son válidos
		SET_TRANSFORM.

		@param message Mensaje a chequear.
		@return true si el mensaje es aceptado.
		*/
		virtual bool accept(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Método virtual que procesa un mensaje.

		@param message Mensaje a procesar.
		*/
		virtual void process(const std::shared_ptr<Logic::IMessage> &message);

		/**
		Método que activa el componente; invocado cuando se activa
		el mapa donde está la entidad a la que pertenece el componente.
		<p>
		La implementación registrará al componente en algunos observers en 
		los que pueda necesitar estar registrado (como el cronómetro del 
		sistema, etc.).

		@return true si todo ha ido correctamente.
		*/
		virtual bool activate();

		/**
		Método que desactiva el componente; invocado cuando se
		desactiva el mapa donde está la entidad a la que pertenece el
		componente. Se invocará siempre, independientemente de si estamos
		activados o no.
		<p>
		La implementación eliminará al componente de algunos observers en los 
		que pueda estar registrado (como el cronómetro del sistema, etc.).m
		*/
		virtual void deactivate();

		///Método de escucha de la posicion de la entidad
		void OnEntitySetTransform(const Matrix4 &transform);

	protected:

		void setTransform();

		void setTransform(Matrix4 trans);

		/**
		Atributo con el nombre del modelo gráfico de la entidad.
		*/
		std::string _model;
		
		/**
		Entidad gráfica del objeto manual.
		*/
		Graphics::CManualEntity *_PhysicsDebugEntity;

		/*
		Entidad Gráfica
		*/
		Graphics::CEntity* _graphicsEntity;
		/**
		Escena gráfica donde se encontrarán las representaciones gráficas de
		las entidades. La guardamos para la destrucción de la entidad gráfica.
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
		offset del centro físico
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
