/*
Servidor de LUA. Es donde se inicializa el interprete, y donde se juntan las publicaciones de las funciones
y de las clases en LUA. Este servidor es inicializado en el metodo initLUA del server de BaseSubsystems.

@author Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#ifndef __LUA_Server_H
#define __LUA_Server_H

namespace ScriptManager
{
	class CScriptManager;
}

namespace LUA
{
	class CServer
	{
	public:

		/**
		Devuelve la �nica instancia de la clase CServer.
		
		@return �nica instancia de la clase CServer.
		*/
		static CServer* getSingletonPtr() {return _instance;}
		
		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicaci�n.
		*/
		static void Release();

		//Metodo que publica todas las funciones y todas las clases en LUA. 
		bool publishAll();

	protected:

		/**
		Constructor.
		*/
		CServer ();

		/**
		Destructor.
		*/
		~CServer();

		/**
		Segunda fase de la construcci�n del objeto. Sirve para hacer
		inicializaciones de la propia instancia en vez de inicializaciones 
		est�ticas.

		@return true si todo fue correctamente.
		*/
		bool open();

		/**
		Segunda fase de la destrucci�n del objeto. Sirve para hacer liberar 
		los recursos de la propia instancia, la liberaci�n de los recursos 
		est�ticos se hace en Release().
		*/
		void close();

	private:

		/**
		�nica instancia de la clase.
		*/
		static CServer* _instance;

		

	};

}

#endif