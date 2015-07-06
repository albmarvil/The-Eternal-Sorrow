/*
@filename MusicManager.h

Contiene la definici�n del manager de m�sica del motor de juego de The Eternal Sorrow

Este manager se encarga de registrar y lanzar las pistas correspondientes a m�sicas de ambiente y banda sonora.

Es un singleton

@author Alberto Mart�nez Villar�n
@date Mayo, 2015
*/

#ifndef __Sounds_MusicManager_H
#define __Sounds_MusicManager_H



#include <map>

namespace Sounds
{
	class CSound;
}

namespace Sounds
{
	

	
	class CMusicManager
	{
	public:
		/*
		Devuelve el puntero a la instancia del singleton
		*/
		static Sounds::CMusicManager* getSingletonPtr() { return _instance; }

		/*
		Inicializa la instancia del singleton

		@return True si la inicializaci�n fue correcta
		*/
		static bool Init();

		/*
		Libera la instancia del singleto
		*/
		static void Release();

		void playSound(const std::string &sound);

		void stopSound(const std::string &sound);

		void stopAllSounds();

		void setSoundParameter(const std::string sound, std::string parameter, float value);

	protected:
		/*
		Puntero a la instancia del singleton
		*/
		static CMusicManager* _instance;

		/*
		Constructor por defecto del singleton
		*/
		CMusicManager();

		/*
		Destructor del singleton
		*/
		virtual ~CMusicManager();

		/*
		M�todo auxiliar para la inicializaci�n del singleton.
		
		@return True Si la inicializaci�n fue correcta
		*/
		bool open();

		/*
		M�todo auxiliar para la liberaci�n de los recursos del singleton. Aqu� es donde se liberan todos los recursos de FMOD
		*/
		void close();

		CSound* findSound(const std::string &sound);

		typedef std::map<std::string, CSound*> TMapSounds;
		typedef std::pair<std::string, CSound*> TPairSounds;

		TMapSounds _mapSounds;


	};//CMusicManager

};//namespace Sounds

#endif //__Sounds_MusicManager_H