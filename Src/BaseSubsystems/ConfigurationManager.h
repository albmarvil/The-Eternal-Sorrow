/**
@author César Manuel Paz Guzman
@date Mayo, 2015
*/

#ifndef __ConfigurationManager__H
#define __ConfigurationManager__H

#include <map>
#include <string>
#include <OgreRenderWindow.h>

// Predeclaración de clases para ahorrar tiempo de compilación
namespace Ogre 
{
	class RenderWindow;
	class Root;
}

namespace Application
{
	class COptionState;
}
namespace BaseSubsystems
{
	class CConfigurationManager
	{
	public:

		/**
		Devuelve la única instancia de la clase ConigurationManager.
		
		@return Única instancia de la clase ConigurationManager.
		*/
		static CConfigurationManager* getSingletonPtr() {return _instance;}

		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de ConigurationManager. 
		*/
		static void Release();

		//Cambia el estado del vsync
		void vsyncActive();

		//Esta funcion sirve para indicarle desde cegui la nueva resolucion marcada
		void setIdResolucion(int idR){ id = idR;}

		//Funcion llamada cuando en el menu de opciones se marca la opcion de pantalla completa
		void setFullMode(bool isFullMode){ fullMode = isFullMode;}

		//Funcion llamada cuando en el menu de opciones se marca la opcion de vsync
		void setVSync(bool isVsync){ vsync = isVsync;}
		
		//Funcion que devuelve la resoucion actual
		std::pair<std::string,std::string> getResolucionActual()const { return resolucionActual;}

		//Funcion que obtiene la resolucion maxima soportada por el monitor
		Ogre::Rect getResolucionMaxima() const { return resolucionMaxima;}

		//Devuelve si estamos en pantalla completa o no
		bool isFullMode() const { return fullMode;}

		//Devuelve si se ha activado el vsync o no
		bool isVsync() const { return vsync;}

		//Actualiza la resolucion de pantalla
		void updateResolution();

		//Cambia el estado del modo de pantalla
		void changeFullMode();

		//Metodo para setear atributos de sistema de renderizado (openGL o DirectX)
		void saveConfig();

		//Funcion que devuelve el valor de cualquier opcion de configuracion
		std::string getValueConfiguration(const std::string &clave); 

		//setea un valor de las opciones de configuracion
		void setValueConfiguration(const std::string &clave, const std::string &value){ _mapConfigurationFile[clave] = value;}
	
		//Funcion llamada cuando se cambia el volumen de musica
		void updateVolumeMusic();

		//Funcion llamada cuando se cambia el volumen de los efectos
		void updateVolumeEffects();

		//Funcion llamada cuando se actualiza el antialiasing
		void updateFsaa();

		//Devuelve el valor actual de antialiasing
		int getFSAA() const { return _fsaa;}

	protected:

		/**
		Constructor.
		*/
		CConfigurationManager ();

		/**
		Destructor.
		*/
		~CConfigurationManager();

		/**
		Segunda fase de la construcción del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		estáticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucción del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberación de los recursos 
		estáticos se hace en Release().
		*/
		void close();

		//Metodo que debe ser llamado despues de cambiar las opciones de renderiado
		void changeResolution(int width, int height);

		void addResolucion(int width, int height, int pos);

	private:

		friend class Application::COptionState;

		//Metodo que lee del fichero de cofiguracion
		void readFileConfiguration();

		//Metodo que escribe en el fichero de configuracion
		void writeFileConfiguration();

		//Metodo que inicializa la configuracion de Ogre
		void initializeConfigurationOgre();

		//El id se corresponde con el id del combobox correspondiente a la resolucion Actual, para setearle la correcta
		int id;

		//Indica las resoluciones disponibles a escoger
		std::map<int,std::pair<int,int>> resoluciones;

		//Indica si estamos en modo pantalle completa
		bool fullMode;

		//Inidica la resolucionActual
		std::pair<std::string,std::string> resolucionActual;

		//Indica si el vsync esta activado o no
		bool vsync;

		//Indica el valor del FSAA
		int _fsaa;
		//Indica la resolucion maxima soportada por el monitor
		Ogre::Rect resolucionMaxima;

		/**
		Única instancia de la clase.
		*/
		static CConfigurationManager* _instance;

		/** 
		Ventana de renderizado 
		*/
		Ogre::RenderWindow *_renderWindow;

		Ogre::Root *_root;

		//Map que guarda el contenido del fichero de configuracion
		std::map<std::string, std::string> _mapConfigurationFile;

		///Flags para marcar la inicialización de los sliders de volumen
		bool _effectsInitialised;		

	}; // class CConigurationManager

} 

#endif 
