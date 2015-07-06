/*
@filename Server.h

Contiene la definici�n del servidor de Sonidos del motor de juego Eternal Sorrow

Este servidor contiene la inicializaci�n de FMOD Studio,�y funciona como punto de acceso para el resto del sistema al sistema de sonido

Es un singleton

@author Alberto Mart�nez Villar�n
@date Marzo, 2015
@refactor Alberto Mart�nez Villar�n
@date Mayo de 2015
*/

#ifndef __Sounds_Server_H
#define __Sounds_Server_H

#include <string>
//#include <map>
#include <set>
#include <vector>
#include <map>

namespace FMOD
{
    //class FMOD_RESULT;
	class System;

	namespace Studio
	{
		class System;
	}
}

namespace Sounds
{
	class CBank;
	class CListener;
	class CSound3D;
	class CSound;
}

namespace Sounds
{
	

	enum SoundType
	{
		MUSIC,
		EFFECT,
	};

	class CServer
	{
	public:
		/*
		Devuelve el puntero a la instancia del singleton
		*/
		static Sounds::CServer* getSingletonPtr() { return _instance; }

		/*
		Inicializa la instancia del singleton

		@return True si la inicializaci�n fue correcta
		*/
		static bool Init();

		/*
		Libera la instancia del singleto
		*/
		static void Release();

		/*
		Tick del servidor de Sonido. Se usa para actualizar las posiciones de los sonidos y EL Listener en FMOD

		@param msecs Incremento de tiempo en milisegundos desde el �ltimo frame
		*/
		void tick(unsigned int msecs);

		/*
		Devuelve el sistema de sonido de la api STUDIO de FMOD

		@return puntero al FMOD::Studio::System
		*/
		FMOD::Studio::System* getStudioSystem() { return _studioSystem; }

		/*
		Devuelve el sistema de sonido de la api LowLevel de FMOD

		@return puntero al FMOD::system
		*/
		FMOD::System* getLowSystem() { return _lowSystem; }

		/*
		Devuelve desde LUA un puntero a la ruta donde se localizan los bancos de sonido

		@return ruta de los bancos de sonidos
		*/
		std::string getSoundPath();

		/*
		A�ade a la instancia actual el banco seleccionado

		@param bankName nombre del banco que existe en FMOD para cargar en el servidor de sonido
		*/
		void AddBank (const std::string &bankName);

		/*
		M�todo llamado desde LUA una vez se han acabado de cargar los bancos de sonidos para que inicialice los managers auxiliares
		*/
		void InitManagers();

		/*
		Configura el volumen global de los sonidos etiquetados como MUSIC

		El volumen es un par�metro decimal que va de 0 a 1
		@param volume Volumen objetivo
		*/
		void setMusicMasterVolume(float volume);

		/*
		Configura el volumen global de los sonidos etiquetados como EEFECT

		El volumen es un par�metro decimal que va de 0 a 1
		@param volume Volumen objetivo
		*/
		void setEffectsMasterVolume(float volume);

		/*
		Devuelve el valor del volumen global de los sonidos etiquetados como MUSIC

		@return Volumen global de la m�sica
		*/
		float getMusicMasterVolume() { return _musicMasterVolume; };

		/*
		Devuelve el valor del volumen global de los sonidos etiquetados como MUSIC

		@return Volumen global de la m�sica
		*/
		float getEffectsMasterVolume() { return _effectsMasterVolume; };

	protected:
		/*
		Puntero a la instancia del singleton
		*/
		static CServer* _instance;

		/*
		Constructor por defecto del singleton
		*/
		CServer();

		/*
		Destructor del singletons
		*/
		virtual ~CServer();

		/*
		M�todo auxiliar para la inicializaci�n del singleton. Aqu� es donde se inicializan y configuran todos los sistemas de FMOD
		
		@return True Si la inicializaci�n fue correcta
		*/
		bool open();

		/*
		M�todo auxiliar para la liberaci�n de los recursos del singleton. Aqu� es donde se liberan todos los recursos de FMOD
		*/
		void close();

		/////FMOD
		/*
		Funci�n que libera los bancos de sonido cargados. Este m�todo es llamado al cerrarse la instancia del singleton
		*/
		void unloadBanks();

		/*
		Conjunto de los bancos de sonidos cargados. Crear y a�adir un banco de sonido no significa que se vaya a cargar todos sus datos en memoria
		*/
		std::set<CBank*> _banks;

		/*
		UNICO listener que admite la api de FMOD Studio
		*/
		CListener* _listener;

		/*
		Sonidos3D posicionales registrados
		*/
		std::set<CSound3D*> _sounds3D;

		/*
		Intancia del sistema Studio
		*/
		FMOD::Studio::System* _studioSystem;

		/*
		Instancia del sistema LowLevel
		*/
		FMOD::System* _lowSystem;

		typedef std::map<SoundType, std::set<CSound*>> TSoundTypeMap;

		TSoundTypeMap _soundTypeMap;

		float _musicMasterVolume;

		float _effectsMasterVolume;

		friend class CListener;
		friend class CSound3D;
		friend class CSound;
		/*
		M�todo usado para registrar un listener en el servidor
		OJO! la api de FMOD Studio solo admite un listener y este debe ser �nico

		@parama listener Listener a registrar
		*/
		void AddListener(CListener* listener);

		/*
		M�todo usado para registrar un sonido3D posicional en el mundo.

		@param sound Sonido 3D a registrar
		*/
		void AddSound3D(CSound3D* sound);

		/*
		M�todo que desregistra al listener actual del servidor

		@param listener Listener a desregistrar
		*/
		void RemoveListener(CListener* listener);

		/*
		M�todo que desrigstra un sonido 3D posicional

		@param sound Sonido a desregistrar
		*/
		void RemoveSound3D(CSound3D* sound);

		void AddSound(CSound* sound);

		void RemoveSound(CSound* sound);

	};//CServer

};//namespace Sounds

#endif //__Sounds_Server_H