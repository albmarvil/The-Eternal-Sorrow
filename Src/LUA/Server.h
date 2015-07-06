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
		Devuelve la única instancia de la clase CServer.
		
		@return Única instancia de la clase CServer.
		*/
		static CServer* getSingletonPtr() {return _instance;}
		
		/**
		Inicializa la instancia

		@return Devuelve false si no se ha podido inicializar.
		*/
		static bool Init();

		/**
		Libera la instancia de CServer. Debe llamarse al finalizar la 
		aplicación.
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

	private:

		/**
		Única instancia de la clase.
		*/
		static CServer* _instance;

		

	};

}

#endif