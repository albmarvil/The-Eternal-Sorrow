/**
@author Cesar Manuel Paz Guzman
@date Enero, 2015
@refactor Alberto Mart�nez Villar�n
@date Marzo de 2015
*/


#ifndef __Graphics_Particulas_H
#define __Graphics_Particulas_H
#include <string>
#include "BaseSubsystems/Math.h"

/**
	Las particulas las tenemos definida en el script fuego.particle. Cada particula se conforma de emisores (por ahora solo tenemos uno)
	que son los encargados de emitir la particula, y afectores, que son los encargados de modificar el aspecto de las particulas. 

	Cada particula tiene su material, el cual se definen en fuego.material. Y otros parametros comoel tama�o de las particulas o
	el numero de particulas emitidas cada vez. Los emisores debemos de ponerle la posicion, la direccion, el angulo en el que emitiran
	las particulas, el color de ellas, la velocidad, el tiempo de vida, su duracion y dependiendo del tipo de emisor las dimensiones
	de la zona de actuacion. 

	Para crear las particulas hemos creado esta clase en Graphics, la cual sera llamada desde el componente CParticle. En ese componente
	se llama al constructor de esta clase, y se a�ade a la escena a traves de un metodo addParticle de la escena grafica. Ahi se llama
	al metodo atachtoscene, el cual crea la particula de ogre y la asocia a un sceneNode. Apartir de ahi, ser� mostrada en pantalla.
**/
namespace Graphics
{
	class CScene;
}
namespace Ogre
{
	class SceneNode;
	//class ParticleSystem;
	//class ParticleEmitter;
}

namespace ParticleUniverse
{
	class ParticleSystemManager;
	class ParticleSystem;
}
namespace Graphics
{
	class CParticle
	{
	public:

		/**
		Constructor de la clase.

		@param name Nombre de la particula.
		@param mesh Nombre del script de la particula que debe ser leido.
		*/
		CParticle(const std::string &name, const std::string &source);

		/**
		Destructor de las particulas
		**/
		virtual ~CParticle();

		/**
		Carga la particula correspondiente.

		@return true si la particula pudo crear los objetos necesarios en Ogre
		o si la particula ya estaba cargada.
		*/
		bool load(const Vector3 &pos);

		/**
		Elimina las estructuras creadas en Ogre mediante load(). 
		*/
		void unload();

		/**
		@param secs N�mero de segundos transcurridos desde la �ltima llamada.
		*/
		void tick(float secs);

		/**
		Cambia la posici�n y orientaci�n de la particula.

		@param transform Referencia a la matriz de transformaci�n con la 
		que debe posicionarse y orientarse la particula.
		*/
		void setTransform(const Matrix4 &transform);

		/**
		Cambia la orientaci�n de la particula.

		@param orientation Referencia a la matriz de rotaci�n con la que debe 
		orientarse la particula.
		*/
		void setOrientation(const Matrix3 &orientation);

		/**
		 Establece la propiedad visible de la entidad. Si
		 la entidad est� puesta invisible, no deber�a dibujarse.
		 <p>
		 El m�todo cambia la propiedad visible a todas las 
		 subentidades o nodos que contenga _particulaNode.

		 @param visible Nuevo valor de la propiedad visible.
		 */
		void setVisible(bool visible);

		//void setName(std::string name, int emitter);

		/**
		 Devuelve el valor de la propiedad visible.

		 @return Cierto si la particula es visible (est� activa 
		 para ser reenderizada).
		*/
		const bool getVisible();

		/**
		Setea la quoata de la particula. 

		@param quota numero de particulas que se emitiran de cada vez
		*/
		//int getQuota();

		std::string getName() { return _name;}

		std::string getSource() {return _source;}

		/**
		Cambia la posici�n de la particula.

		@param position Nueva posici�n para el nodo que contiene la particula 
		de Ogre.
		@param emisor Numero del emisor que conforma la particula. Por ahora solo tenemos uno.
		*/
		void setPosition(const Vector3 &position);

		/**
		Setea el material deseado en la particula.

		@param material Material a setear en la particula
		*/
		//void setMaterial(std::string &material);

		/**
		 Escala la particula.

		 @param scale Valor de la escala para los 3 ejes.
		 */
		void setScale(float scale);

		/*
		Este m�todo inicia la emisi�n de part�culas. El par�metro opcional hace que se inicie y emita particulas hasta 
		un numero determinado de segundos. Tras ese tiempo el sistema para y deja de emitir y elimina las particulas emitidas restantes

		@param duracion Parametro opcional
		*/
		void start(float duracion = 0.0f);

		/*
		Este m�todo inicia la emisi�n de particulas que tendr� una duraci�n predeterminada, despues para, pero el sistema no para de emitir de repente
		lo hace gradualmente, conservando las particulas ya emitidas

		@param duracion Duracion del sistema de particulas
		*/
		void startAndStopFade(float duracion);

		/*
		Este m�todo inicia detiene la emisi�n de particulas de forma INMEDIATA. Si se le indica un delay, lo har� despues de completar el tiempo
		indicado
		@param delay Opcional
		*/
		void stop(float delay = 0.0f);

		/*
		M�todo igual a stop() pero que para progrsivamente el sistema de particulas
		@param delay Opcional
		*/
		void stopFade(float delay = 0.0f);

		/*
		Pausa el sistema de aprticulas de forma INMEDIATA, si se le indica una duraci�n el sistema se reanudar� automaticamente tras ese periodo de tiempo
		@param duracion Opcional
		*/
		void pause(float duracion = 0.0f);

		/*
		Reinicia un sistema de aprticulas tras la pausa
		*/
		void resume();

		/**
		Setea la quoata de la particula. 

		@param quota numero de particulas que se emitiran de cada vez
		*/
		//void setQuota(int quota);

		/**
		Setea la altura de actuacion de las particulas
		**/
		//void setDimensions(float height, float width, int emisor);

		/**
		Cambia el color de las particulas que emite un emisor especifico
		**/
		//void setColourParticle(const Vector3 &colour_start, const Vector3 &colour_end);

		void setQuota(int quota);

	private:

		friend class CScene;

		/**
		A�ade la particula al SceneManager pasado por par�metro. Si la particula
		no est� cargada se fuerza su carga.

		@param scene escena de Ogre a la que se quiere a�adir
		la particula.
		@return true si la entidad se pudo cargar y a�adir a la escena.
		*/
		bool attachToScene(CScene *scene, const Vector3 &pos);

		/**
		Descarga una particula de la escena en la que se encuentra cargada.

		@return true si la particula se descargo y elimin� de la escena
		correctamente. Si la particula no estaba cargada se devuelve false.
		*/
		bool deattachFromScene();

		//Nombre unico de la particula
		std::string _name;

		//script de la particula
		std::string _source;

		//scene a la que pertenece la particula
		CScene *_scene;

		//flag para saber si se ha cargado la particula
		bool _loaded;

		//nodo al que pertenecera la particula
		Ogre::SceneNode *_particleNode; 

		//particula 
		//Ogre::ParticleSystem *_particle;

		ParticleUniverse::ParticleSystem *_particle;

		ParticleUniverse::ParticleSystemManager *_particleManager;

		// Hueso asociado si lo tuviese
		std::string _boneObserver;
	};
}
#endif