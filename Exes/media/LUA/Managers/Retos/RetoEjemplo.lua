----Author: Alberto MArtínez Villarán
--Date Junio de 2015

-- Este docuemnto sirve como ejemplo de los métodos y secciones que debe tener un reto


Reto =
{
	--variables del reto
	--Aqui puedes colocar las variables que te hagan falta para completar la lógica de tu reto

	informacionReto = "Info que sale en el cartelito",
	--Este flag es muy importante, nos dice si el reto se ha completado o no.
	completed = false,


	activarHUD = function()
		--METODO DE INICIALIZACION DEL RETO

		--Sirve para inicializar variables. así como para inicializar el GUI
		--EJ:
		-- scriptMgr:loadScript(Paths.luaGUIPath.."CartelCentralGUI.lua")
		-- CartelCentralHUD.Activate(2,Reto.informacionReto) --Dos objetivos, matar enemigos en tantos segundos, y la info a mostrar
		-- CartelCentralHUD.cambiarColorTexto("rojo")
		-- CartelCentralHUD.insertarTexto(1, "Tiempo", Reto.tiempo_objetivo)
		-- CartelCentralHUD.insertarTexto(2, "Enemigos", "0/"..Reto.num_enemigos)
	end,

	tick = function ( msecs )
		
		--ACTUALIZACION DE VARIABLES
		--actualizamos las variables y acciones de nuestro reto


		--FINALIZACION
		---COMPLETADO CON EXITO???
				--Reto.Success()
		---HA ACABADO MAL?
				--Reto.Fail()
	end,

	Success = function ()
		Reto.completed = true
		RetoMgr.is_reto = false --Indicamos al RetoMgr que ya no hay reto, para que pueda cargar otro

		--CODIGO ESPECIFICO

	end,
	
	Fail = function()
		Reto.completed = false
		RetoMgr.is_reto = false --Indicamos al RetoMgr que ya no hay reto, para que pueda cargar otro
		
		--CODIGO ESPECIFICO

	end,

	ActivarEventoAvalancha =function ()
		---Hay que devolver true o false dependiendo de las condiciones para activar el reto avalancha
		---Normalmente basta con decirle que se ha completado el reto
		return Reto.completed
	end,


	PorcentajeObjetivo = function ()
		--esta funcion devuelve un valor entre 0 y 1 que es el procentaje de consecución del objetivo
		--Se usa para el calculo global de tensión de la partida, se puede tener en cuenta el tiempo en sala
		-- los enemigos cuantificados del reto, ambas o ninguna
	end,



	--ESTO LO COPYPASTEAS

	-----------CONFIGURACION DE LA MOCHILA POR RETO-------------

	refreshTime = 15000, --cada 10s

	tam_inicial_mochila = 6, --tamaño inicial

	--función que devuelve los parámetros a, b y c de la fórmula atendiendo al número de salas
	parameters = function ( nSalas )
		-- body
		local a = 0.2
		local b = 0.3
		local c = 0

		if nSalas > 3 then
			a = 0.1
			b = 0.5
			c = 0
		end
		return a, b, c
	end,
	
	
	------------------CONFIGURACIÓN DEL SPAWNER POR RETO--------------------------
	
	SpawnParams = function(salas)
		local m1, m2, n1, n2, m3, n3, m4, n4, m5, n5
		m1 = -2
		n1 = 40
		m2 = 1
		n2 = 25
		m3 = 1
		n3 = 10
		m4 = 0.1
		n4 = 1
		m5 = 0.1
		n5 = 1
		

		----para las priemras 5 salas anulamos la prob de aparicion de los 3 ultimos tiers

		if salas < 5 then
			m2 = 0
			n2 = 0
			m3 = 0
			n3 = 0
			-- m4 = 0
			-- n4 = 0
			m5 = 0
			n5 = 0
		end
		
		local T1, T2, T3, T4, T5
		T1 = math.max(math.ceil(salas * m1 + n1), 10)
		T2 = math.ceil(salas * m2 + n2)
		T3 = math.ceil(salas * m3 + n3)
		T4 = math.ceil(salas * m4 + n4)
		T5 = math.ceil(salas * m5 + n5)
		return T1, T2, T3, T4, T5
	end,
}