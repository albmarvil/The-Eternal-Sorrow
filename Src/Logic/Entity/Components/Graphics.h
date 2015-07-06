/**
@file Graphics.h

Contiene la declaraci�n del componente que controla la representaci�n
gr�fica de la entidad.

@see Logic::CGraphics
@see Logic::IComponent

@author David Llans�
@date Agosto, 2010
*/
#ifndef __Logic_Graphics_H
#define __Logic_Graphics_H

#include "Logic/Entity/Component.h"
#include "Logic/Entity/Entity.h"
#include "Graphics/PrefabType.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
namespace Graphics 
{
	class CEntity;
	class CScene;
}

//declaraci�n de la clase
namespace Logic 
{
/**
	Componente que se encarga de la representaci�n gr�fica de una entidad.
	En concreto se encarga de las entidades con representaciones gr�ficas
	no animadas. Para otros tipos de representaciones hay otros componentes
	que especializan �ste (CAnimatedGraphics y CStaticGraphics).
	<p>
	Acepta mensajes de cambio de posici�n y orientaci�n (matriz de 
	transformaci�n) mediante el mensaje SET_TRANSFORM.
	
    @ingroup logicGroup

	@author David Llans� Garc�a
	@date Agosto, 2010
*/
	class CGraphics : public IComponent, public CEntityTransformListener
	{
		DEC_FACTORY(CGraphics);
	public:

		/**
		Constructor por defecto; inicializa los atributos a su valor por 
		defecto.
		*/
		CGraphics() : IComponent(), _graphicsEntity(0) ,_scale(Vector3(1,1,1)), _needNode(true){}

		/**
		Destructor (virtual); Quita de la escena y destruye la entidad gr�fica.
		*/
		virtual ~CGraphics();
		
		/**
		Inicializaci�n del componente, utilizando la informaci�n extra�da de
		la entidad le�da del mapa (Maps::CEntity). Toma del mapa el atributo
		model con el modelo que se deber� cargar e invoca al m�todo virtual
		createGraphicsEntity() para generar la entidad gr�fica.

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

		/**
		Devuelve la entidad grafica
		*/
		Graphics::CEntity* getGraphicsEntity() {return _graphicsEntity;}

		///M�todo de escucha de la posicion de la entidad
		void OnEntitySetTransform(const Matrix4 &transform);

		Vector3 getScale() const { return _scale;}
	protected:

		/**
		M�todo virtual que construye la entidad gr�fica de la entidad. Otros
		componentes pueden sobreescribirlo para inicializar otro tipo de
		entidades gr�ficas (animadas, etc.).
		
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Entidad gr�fica creada, NULL si hubo alg�n problema.
		*/
		virtual Graphics::CEntity* createGraphicsEntity(const Map::CEntity *entityInfo);

		/**
		M�todo virtual que construye la entidad gr�fica de la entidad a partir de una forma b�sica. 
		
		@param entityInfo Informaci�n de construcci�n del objeto le�do del
			fichero de disco.
		@return Entidad gr�fica creada, NULL si hubo alg�n problema.
		*/
		virtual Graphics::CEntity* createPrefabGraphicsEntity(const Map::CEntity *entityInfo);

		/**
		Atributo con el nombre del modelo gr�fico de la entidad.
		*/
		std::string _model;
		
		/**
		Entidad gr�fica.
		*/
		Graphics::CEntity *_graphicsEntity;

		/**
		Escena gr�fica donde se encontrar�n las representaciones gr�ficas de
		las entidades. La guardamos para la destrucci�n de la entidad gr�fica.
		*/
		Graphics::CScene* _scene;

		/**
		Tipo de prefab de la forma b�sica. Cubo, plano o esfera
		*/
		Graphics::PrefabType _type;

		/**
		Nos indica cuando estamos ante una forma b�sica
		*/
		bool _isPrefab;

		/**
		Material usado en la entidad
		*/
		std::string _material;
		
		/**
		Dimensiones de escala del objeto
		*/
		Vector3 _scale;

		bool _isStatic;

		bool _needNode;

	}; // class CGraphics

	REG_FACTORY(CGraphics);

} // namespace Logic

#endif // __Logic_Graphics_H
