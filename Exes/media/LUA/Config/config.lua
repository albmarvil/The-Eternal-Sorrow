--Variable que le diga a la aplicacion cual es el estado inicial de la aplicacion.
-- 0: estado del menu
-- 1: estado del juego
--initialState = 0 --Esto es lo mismo que initialState ="1

GaleonConfig = {
	initialState = "menu",
}	

--Leer una variable de lua en c++

--Toda la comuncacion entre c++ y LUA se hace a través de una pila de LUA. Puedo acceder a la cima y a los elementos del medio. En la pila puede haber todo el tipo de entradas, pero
--cada uno de los elementos que hay en la pila puedo tener tipos de LUA. Los numeros de acceso a la pila admiten numeros positivos y negativos
--El -1 me devuelve la cima, -2 la subcima, el 1 la parte de abajo y así. 