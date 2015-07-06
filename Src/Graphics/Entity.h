//---------------------------------------------------------------------------
// Entity.h
//---------------------------------------------------------------------------

/**
@file Entity.h

Contiene la declaraci�n de la clase que representa una entidad gr�fica.

@see Graphics::CEntity

@author David Llans�
@date Julio, 2010
*/

#ifndef __Graphics_Entity_H
#define __Graphics_Entity_H

#include "BaseSubsystems/Math.h"
#include "PrefabType.h"
//#include "Logic/Entity/Entity.h"

// Predeclaraci�n de clases para ahorrar tiempo de compilaci�n
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
	Clase que representa una entidad gr�fica. Contiene una referencia a
	una entidad de Ogre y al Nodo que la contiene.
	Oculta los detalles escabrosos del motor gr�fico.
	<p>
	Al construir la clase solo se almacenan los atributos necesarios para
	la carga de las entidades de Ogre. Para cargar la entidad en Ogre y que 
	sea reenderizada, �sta debe a�adirse a una escenada de Ogre mediante el 
	m�todo attachToScene(). Solo la clase CScene puede invocar los m�todos 
	para a�adir o eliminar entidades de una escena y por tanto forzar su 
	carga o descarga en el motor de Ogre. Esto deriva de que en Ogre la carga
	de entidades se hace por las escenas, por lo que en nuestra arquitectura
	no permitimos entidades cargadas que no pertenezcan a ninguna escena.
	Al a�adir una entidad a una escena se fuerza su carga en Ogre invocando a 
	load.
	<p>
	Todas las modificaciones que se deseen hacer sobre los atributos de la
	entidad gr�fica (posici�n, orientaci�n, escala...) se deber�n hacer una 
	vez que las estructuras de Ogre han sido cargadas. De la misma manera 
	la entidad debe estar cargada para poder acceder a ellos.
	<p>
	Pueden implementarse clases derivadas de �stas, que permitan
	modelos con distintas caracter�sticas. Por ejemplo, �sta clase
	no permite especificar la animaci�n utilizada. Si queremos que podamos
	guardar un modelo gr�fico, y poder cambiar la animaci�n actual,
	se utilizar� la clase derivada correspondiente.
	
	@ingroup graphicsGroup

	@author David Llans�
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
		Destructor de la aplicaci�n.
		*/
		virtual ~CEntity();

		/**
		Cambia la posici�n y orientaci�n de la entidad.

		@param transform Referencia a la matriz de transformaci�n con la 
		que debe posicionarse y orientarse la entidad.
		*/
		void setTransform(const Matrix4 &transform);

		/**
		Cambia la posici�n de la entidad.

		@param position Nueva posici�n para el nodo que contiene la entidad 
		de Ogre.
		*/
		void setPosition(const Vector3& position);

		/**
		Cambia la orientaci�n de la entidad.

		@param orientation Referencia a la matriz de rotaci�n con la que debe 
		orientarse la entidad.
		*/
		void setOrientation(const Matrix3 &orientation);

		/**
		 Establece la propiedad visible de la entidad. Si
		 la entidad est� puesta invisible, no deber�a dibujarse.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.
		 <p>
		 El m�todo cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _entityNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		/**
		 Devuelve el valor de la propiedad visible.
		 La propiedad indica si la entidad debe dibujarse o no,
		 es decir, no est� relacionada con si la entidad est�
		 dentro del campo de visi�n de la c�mara o no.

		 @return Cierto si la entidad es visible (est� activa 
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
		Setea el material deseado en la entidad gr�fica. Todas las subentidades tendr�n este material por defecto

		@param material Material a setear en la entidad gr�fica
		*/
		void setMaterial(std::string &material);


		/**
		Setea el material deseado en la subentidad gr�fica de la entidad gr�fica padre. Todas las subentidades tendr�n este material por defecto

		@param material Material a setear en la subentidad gr�fica
		@param subEntity Subentidad gr�fica en la cual se setea el material
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
		@param logicEntity entidad de tipo Logic::CEntity* de la cual se sacar� la entidad tipo Ogre::Entity*
		*/
		void attachObjectToBone(std::string name, Logic::CEntity* logicEntity);

		/**
		Permite desacoplar una entidad de un hueso del esqueleto
		@param name Nuevo nombre
		@param logicEntity entidad de tipo Logic::CEntity* a desacoplar
		*/
		void deattachObjectFromBone(std::string name, Logic::CEntity* logicEntity);
		
		/**
		Funci�n que permite a�adir un observador
		*/
		void addObserver(std::string boneName, Logic::CEntity* ent);

		/**
		Funci�n que permite eliminar un observador
		*/
		void deleteObserver(std::string boneName, Logic::CEntity* ent);

		void pauseBone(bool pausa);

		/*
		Devuelve el puntero a la entidad de Ogre

		@return Puntero a la entidad de ogre
		*/
		Ogre::Entity* getOgreEntity() { return _entity; };

		/*
		Devuelve el puntero a la escena en la cual est� attachada esta entidad

		@return puntero a la escena de la entidad
		*/
		CScene *getScene(){ return _scene;}

		/*
		Devuelve el puntero al nodo de escena de Ogre en caso de tenerlo
		*/
		Ogre::SceneNode *getEntityNode();

		/*
		M�todo que configura un TagPoint (puntero a un bone) en el caso de que la entidad se attachee a un bone y no tenga nodo de escena propio

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

		// CScene es la �nica que puede a�adir o eliminar entidades de una 
		// escena y por tanto cargar o descargar entidades.
		// Por otro lado cada entidad debe pertenecer a una escena. Solo 
		// permitimos a la escena actualizar el estado.
		friend class CScene;

		/**
		A�ade la entidad al SceneManager pasado por par�metro. Si la entidad
		no est� cargada se fuerza su carga.

		@param sceneMgr Gestor de la escena de Ogre a la que se quiere a�adir
		la entidad.
		@return true si la entidad se pudo cargar y a�adir a la escena.
		*/
		bool attachToScene(CScene *scene);

		/**
		Descarga una entidad de la escena en la que se encuentra cargada.

		@return true si la entidad se descargo y elimin� de la escena
		correctamente. Si la entidad no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();
		
		/**
		Carga la entidad gr�fica correspondiente al nombre _mesh. No hace 
		comprobaci�nes de si la entidad est� ya cargada o de si pertenece a
		otra escena. Esto se debe hacer de manera externa.

		@return true si la entidad pudo crear los objetos necesarios en Ogre
		o si la entidad ya estaba cargada.
		*/
		bool load();

		/**
		Elimina las estructuras creadas en Ogre mediante load(). No hace 
		comprobaci�nes de si la entidad est� o no cargada o de si pertenece
		a una escena. Esto se debe hacer de manera externa.
		*/
		void unload();
		
		/**
		Actualiza el estado de la entidad cada ciclo. En esta clase no se
		necesita actualizar el estado cada ciclo, pero en las hijas puede que
		si.
		
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
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
		Quiz�s es mejor meter la clase CScene

		Controla todos los elementos Ogre de una escena. Su equivalente
		en la l�gica del juego ser�a el mapa o nivel. 
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
		Indica si la entidad ha sido cargada en el motor gr�fico.
		*/
		bool _loaded;

		/**
		Indica si la entidad necesita un entityNode o no
		*/
		bool _needNode;

		

	}; // class CEntity

} // namespace Graphics

#endif // __Graphics_Entity_H
