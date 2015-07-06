/*
@filename GUISoundsManager.h

Contiene la definición del manager de música del motor de juego de The Eternal Sorrow

Este manager se encarga de registrar y lanzar las pistas correspondientes a efectos de interfaz y GUI

Es un singleton

@author Alberto Martínez Villarán
@date Mayo, 2015
*/

#ifndef __Sounds_GUISoundsManager_H
#define __Sounds_GUISoundsManager_H



#include <map>

namespace Sounds
{
	class CSound;
}

namespace Sounds
{
	

	
	class CGUISoundsManager
	{
	public:
		/*
		Devuelve el puntero a la instancia del singleton
		*/
		static Sounds::CGUISoundsManager* getSingletonPtr() { return _instance; }

		/*
		Inicializa la instancia del singleton

		@return True si la inicialización fue correcta
		*/
		static bool Init();

		/*
		Libera la instancia del singleto
		*/
		static void Release();

		void playSound(const std::string &sound);

		void stopSound(const std::string &sound);

		void stopAllSounds();

		void setSoundParameter(const std::string &sound, std::string &parameter, float value);

	protected:
		/*
		Puntero a la instancia del singleton
		*/
		static CGUISoundsManager* _instance;

		/*
		Constructor por defecto del singleton
		*/
		CGUISoundsManager();

		/*
		Destructor del singleton
		*/
		virtual ~CGUISoundsManager();

		/*
		Método auxiliar para la inicialización del singleton.
		
		@return True Si la inicialización fue correcta
		*/
		bool open();

		/*
		Método auxiliar para la liberación de los recursos del singleton. Aquí es donde se liberan todos los recursos de FMOD
		*/
		void close();

		CSound* findSound(const std::string &sound);

		typedef std::map<std::string, CSound*> TMapSounds;
		typedef std::pair<std::string, CSound*> TPairSounds;

		TMapSounds _mapSounds;


	};//CGUISoundsManager

};//namespace Sounds

#endif //__Sounds_GUISoundsManager_H