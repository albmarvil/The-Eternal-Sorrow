//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaración de la clase que representa una entidad gráfica.

@see Graphics::CEntity

@author David Llansó
@date Julio, 2010
*/

#ifndef __Graphics_Entity_H
#define __Graphics_Entity_H

#include "BaseSubsystems/Math.h"
#include "PrefabType.h"
//#include "Logic/Entity/Entity.h"

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class Entity;
	class SceneNode;
	class TagPoint;
}

namespace Graphics 
{
	class CScene;
}

namespace Logic
{
	class CEntity;
	class CWeaponController;
}

namespace Graphics 
{
	/**
	Clase que representa una entidad gráfica. Contiene una referencia a
	una entidad de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gráfico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las entidades de Ogre. Para cargar la entidad en Ogre y que 
	sea reenderizada, ésta debe añadirse a una escenada de Ogre mediante el 
	método attachToScene(). Solo la clase CScene puede invocar los métodos 
	para añadir o eliminar entidades de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Esto deriva de que en Ogre la carga
	de entidades se hace por las escenas, por lo que en nuestra arquitectura
	no permitimos entidades cargadas que no pertenezcan a ninguna escena.
	Al añadir una entidad a una escena se fuerza su carga en Ogre invocando a 
	load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	entidad gráfica (posición, orientación, escala...) se deberán hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la entidad debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de éstas, que permitan
	modelos con distintas características. Por ejemplo, ésta clase
	no permite especificar la animación utilizada. Si queremos que podamos
	guardar un modelo gráfico, y poder cambiar la animación actual,
	se utilizará la clase derivada correspondiente.
	
	@ingroup graphicsGroup

	@author David Llansó
	@date Julio, 2010
	*/
	class CEntity 
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param mesh Nombre del modelo que debe cargarse.
		*/
		CEntity(const std::string &name, const std::string &mesh);
		/**
		Constructor de la clase.

		@param name Nombre de la entidad.
		@param type Tipo del prefab que va a cargarse.
		*/
		CEntity(const std::string &name, const PrefabType &type);

		/**
		Destructor de la aplicación.
		*/
		virtual ~CEntity();

		/**
		Cambia la posición y orientación de la entidad.

		@param transform Referencia a la matriz de transformación con la 
		que debe posicionarse y orientarse la entidad.
		*/
		void setTransform(const Matrix4 &transform);

		/**
		Cambia la posición de la entidad.

		@param position Nueva posición para el nodo que contiene la entidad 
		de Ogre.
		*/
		void setPosition(const Vector3& position);

		/**
		Cambia la orientación de la entidad.

		@param orientation Referencia a la matriz de rotación con la que debe 
		orientarse la entidad.
		*/
		void setOrientation(const Matrix3 &orientation);

		/**
		 Establece la propiedad visible de la entidad. Si
		 la entidad está puesta invisible, no debería dibujarse.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.
		 <p>
		 El método cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _entityNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no está relacionada con si la entidad está
		 dentro del campo de visión de la cámara o no.

		 @return Cierto si la entidad es visible (está activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		/**
		 Escala la entidad.

		 @param scale Valores de la escala en los diferentes ejes.
		 */
		void setScale(const Vector3 &scale);

		/**
		 Escala la entidad.

		 @param scale Valor de la escala para los 3 ejes.
		 */
		void setScale(float scale);

		/**
		 Devuelve el valor de la escala de la entidad.

		 @return Valores de la escala en los diferentes ejes.
		*/
		const Vector3 &getScale();

		/**
		Devuelve la entidad de Ogre.

		@return puntero a la entidad de Ogre.
		*/
		Ogre::Entity* getEntity() {return _entity;}

		/**
		Setea el material deseado en la entidad gráfica. Todas las subentidades tendrán este material por defecto

		@param material Material a setear en la entidad gráfica
		*/
		void setMaterial(std::string &material);


		/**
		Setea el material deseado en la subentidad gráfica de la entidad gráfica padre. Todas las subentidades tendrán este material por defecto

		@param material Material a setear en la subentidad gráfica
		@param subEntity Subentidad gráfica en la cual se setea el material
		*/
		void setMaterial(std::string &material, int subEntity);

		
		/**
		Permite establecer un nuevo nombre para dicha entidad
		@param name Nuevo nombre
		*/
		void setName(std::string name) {_name = name;}

		/**
		Devuelve si el nombre
		@return _name Nombre
		*/
		std::string getName() {return _name;}

		/**
		Indicamos si necesita un entityNode o no dicha entidad
		@param need Si necesita o no
		*/
		void setNeedNode(bool need) {_needNode = need;}

		/**
		Devuelve si necesita un entityNode o no dicha entidad
		@return _needNode Si necesita o no
		*/
		bool getNeedNode() {return _needNode;}

		/**
		Permite acoplar una entidad a un hueso del esqueleto
		@param name Nuevo nombre
		@param logicEntity entidad de tipo Logic::CEntity* de la cual se sacará la entidad tipo Ogre::Entity*
		*/
		void attachObjectToBone(std::string name, Logic::CEntity* logicEntity);

		/**
		Permite desacoplar una entidad de un hueso del esqueleto
		@param name Nuevo nombre
		@param logicEntity entidad de tipo Logic::CEntity* a desacoplar
		*/
		void deattachObjectFromBone(std::string name, Logic::CEntity* logicEntity);
		
		/**
		Función que permite añadir un observador
		*/
		void addObserver(std::string boneName, Logic::CEntity* ent);

		/**
		Función que permite eliminar un observador
		*/
		void deleteObserver(std::string boneName, Logic::CEntity* ent);

		void pauseBone(bool pausa);

		/*
		Devuelve el puntero a la entidad de Ogre

		@return Puntero a la entidad de ogre
		*/
		Ogre::Entity* getOgreEntity() { return _entity; };

		/*
		Devuelve el puntero a la escena en la cual está attachada esta entidad

		@return puntero a la escena de la entidad
		*/
		CScene *getScene(){ return _scene;}

		/*
		Devuelve el puntero al nodo de escena de Ogre en caso de tenerlo
		*/
		Ogre::SceneNode *getEntityNode();

		/*
		Método que configura un TagPoint (puntero a un bone) en el caso de que la entidad se attachee a un bone y no tenga nodo de escena propio

		@param tagPoint TagPoint del bone que hace de nodo de escena
		*/
		void setTagPoint(Ogre::TagPoint* tagpoint) { _tagPoint = tagpoint; }

		/*
		Devuelve el puntero al bone de la entidad en caso de estar attachado a un bone
		*/
		Ogre::TagPoint *getTagPoint() { return _tagPoint; }

	protected:

		typedef std::map<Ogre::Bone*, std::vector<Logic::CWeaponController*>> MapBoneObservers;

		MapBoneObservers _mapBoneObservers;

		// CScene es la única que puede añadir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		Añade la entidad al SceneManager pasado por parámetro. Si la entidad
		no está cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere añadir
		la entidad.
		@return true si la entidad se pudo cargar y añadir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y eliminó de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();
		
		/**
		Carga la entidad gráfica correspondiente al nombre _mesh. No hace 
		comprobaciónes de si la entidad está ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaciónes de si la entidad está o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs Número de segundos transcurridos desde la última llamada.
		*/
		virtual void tick(float secs);
		
		/**
		Nodo que contiene la entidad de Ogre.
		*/
		Ogre::SceneNode *_entityNode;

		/**
		Si no tiene nodo, tiene un tagPoint al bone que sigue
		*/
		Ogre::TagPoint *_tagPoint;

		/**
		Entidad de Ogre.
		*/
		Ogre::Entity *_entity;

		/**
		Quizás es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la lógica del juego sería el mapa o nivel. 
		*/
		CScene *_scene;

		/**
		Nombre de la entidad.
		*/
		std::string _name;

		/**
		Nombre del modelo de la entidad.
		*/
		std::string _mesh;

		/**
		Tipo del prefab de la entidad
		*/

		PrefabType _type;

		/**
		Indica si la entidad se contruye a partir de un prefab
		*/
		bool isPrefab;

		/**
		Indica si la entidad ha sido cargada en el motor gráfico.
		*/
		bool _loaded;

		/**
		Indica si la entidad necesita un entityNode o no
		*/
		bool _needNode;

		

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
