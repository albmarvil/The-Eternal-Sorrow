/*
Clase que publica las funciones en LUA. Se implementan en un namespaces FuncionestoLUA las funciones que seran
publicadas en LUA. 

/**
@author Cesar Manuel Paz Guzman
@date Febrero, 2015
*/

#ifndef __LUA_FunctionsToLUA_H
#define __LUA_FunctionsToLUA_H

namespace Logic
{
	class CServer;
};
namespace Physics
{
	class CServer;
};

namespace LUA
{
	class CFunctionsToLUA
	{

	public:

		/*
		Metodo que publica todas las funciones implementadas en el namespace FuncionestoLUA
		*/
		bool publishAllFunctions();

		/*
		Constructor
		*/
		CFunctionsToLUA(){}

		/*
		Destructor
		*/
		virtual ~CFunctionsToLUA(){}

	private:
		
	};
}
#endif