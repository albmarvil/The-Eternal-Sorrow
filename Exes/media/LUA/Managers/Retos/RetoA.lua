Reto =
{
	--variables del reto

	--tiempo para completar el objetivo
	tiempo_objetivo = 124000 + (HadHability("AumentarTiempoReto") * 5000),

	num_enemigos = 10, ---Este valor es dependiente de la mochila pero se actualiza en activarHUD para coger valores inicializados de la mochila
	informacionReto = "Se modifica abajo en activarHUD",

	completed = false,


	activarHUD = function()
		--actualizacion de variables del reto antes de mostrar el GUI
		
		local multiplicador = 1
		if GameMgr.salas_totales >= 1 and GameMgr.salas_totales < 5 then
			multiplicador = 2
		elseif GameMgr.salas_totales < 8 then
			multiplicador = 3
		elseif GameMgr.salas_totales < 10 then
			multiplicador = 4
		else
			multiplicador = 5
		end



		local numEnemigosObjetivo = RetoMgr.tam_total_mochila * multiplicador

		--como minimo 10 enemigos como maximo 80
		numEnemigosObjetivo = math.min(math.max(10, numEnemigosObjetivo), 80)
		
		Reto.num_enemigos = math.floor(numEnemigosObjetivo)

		local minutos = math.floor(Reto.tiempo_objetivo *0.001 / 60)
		local segundos = (Reto.tiempo_objetivo * 0.001) - math.floor(Reto.tiempo_objetivo *0.001 )
		local strSeg = ""..segundos
		if segundos < 10 then
			strSeg = "0"..strSeg
		end

		Reto.informacionReto = "Kill "..Reto.num_enemigos.." enemies\nin "..minutos..":"..strSeg.." minutes"
		-- Reto.informacionReto = RetoMgr.tam_total_mochila.." - "..RetoMgr.tam_libre_mochila

		scriptMgr:loadScript(Paths.luaGUIPath.."CartelCentralGUI.lua")
		CartelCentralHUD.Activate(2,Reto.informacionReto) --Dos objetivos, matar enemigos en tantos segundos, y la info a mostrar
		CartelCentralHUD.cambiarColorTexto("rojo")
		CartelCentralHUD.insertarTexto(1, "Time", Reto.tiempo_objetivo)
		CartelCentralHUD.insertarTexto(2, "Deaths", "0/"..Reto.num_enemigos)
		
	end,

	tick = function ( msecs )
		-- body
		
		local minuto = CartelCentralHUD.transformarHora(math.floor((Reto.tiempo_objetivo - GameMgr.tiempo_en_sala)*0.001))
		CartelCentralHUD.insertarTexto(1, "Time", minuto)
		CartelCentralHUD.insertarTexto(2, "Deaths", GameMgr.enemigos_muertos.."/"..Reto.num_enemigos)
		
		--Comprobación de finalización del reto
		if GameMgr.enemigos_muertos >= Reto.num_enemigos then
			Reto.Success()
		end
		
		--Reglas de fin de reto
		if GameMgr.tiempo_en_sala >= Reto.tiempo_objetivo then
			Reto.Fail()
		end

		
	end,

	--funcion de finalizacion del reto
	Fail = function ()
		RetoMgr.is_reto = false
		Reto.completed = false
		app:push("gameOver")
	end,
	
	Success = function()
		RetoMgr.is_reto = false
		Reto.completed = true

		CartelCentralHUD.cambiarColorTexto("verde")
		GameMgr.enemigos_muertos = Reto.num_enemigos
			
		entityfactory:createEntityByType("ParticulaPuertaAbierta", GameMgr.exit_door:getPosition(), GameMgr.map)
		--hacemos que la puerta NO colisione con el player para que la pueda atravesar
		ChangeCollisionGroup(GameMgr.exit_door)
		
		local minuto = CartelCentralHUD.transformarHora(math.floor((Reto.tiempo_objetivo - GameMgr.tiempo_en_sala)*0.001))
		CartelCentralHUD.insertarTexto(1, "Time", minuto)
		CartelCentralHUD.insertarTexto(2, "Deaths", GameMgr.enemigos_muertos.."/"..Reto.num_enemigos)

	end,

	ActivarEventoAvalancha = function ()
		return Reto.completed
	end,

	PorcentajeObjetivo = function ()
		--vamos a devolver la media del % del tiempo y del % de enemigos cuantificados en el reto
		local pEnem = GameMgr.enemigos_muertos / Reto.num_enemigos
		local pTiempo = GameMgr.tiempo_en_sala / Reto.tiempo_objetivo

		return (pEnem + pTiempo) * 0.5
	end,




	-----------CONFIGURACION DE LA MOCHILA POR RETO-------------

	refreshTime = 15000, --cada 10s

	-- tam_inicial_mochila = 6, --tamaño inicial

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