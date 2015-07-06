Reto =
{
	--variables del reto


	informacionReto = "Destroy the door",
	completed = false,

	--tiempo total que se esta en la sala antes de que venga el evento de salida
	tiempo_total_sala = 123500,
	time_acum = 0,

	life = 1000,

	activarHUD = function()

		scriptMgr:loadScript(Paths.luaGUIPath.."CartelCentralGUI.lua")
		CartelCentralHUD.Activate(0, Reto.informacionReto)
		
	end,

	tick = function ( msecs )
		-- body
		--AQUI DEBERÍAMOS HACER LAS COSAS DEL RETO

		Reto.time_acum = Reto.time_acum + msecs
		
		if Reto.life <= 0 then
			Reto.Success()
		end
	end,

	--funcion de finalizacion del reto
	Fail = function ()
		RetoMgr.is_reto = false
		Reto.completed = false
	end,
	
	Success = function()
		RetoMgr.is_reto = false
		Reto.completed = true
		--abrimos la puerta
		ChangeCollisionGroup(GameMgr.exit_door)
	end,

	ActivarEventoAvalancha = function ()
		-- Log.Debug("Tiempo reto: "..Reto.time_acum.." / "..Reto.tiempo_total_sala)
		-- if Reto.completed then
		-- 	Log.Debug("reto.completed: true")
		-- else
		-- 	Log.Debug("reto.completed: false")
		-- end
		return Reto.time_acum >= Reto.tiempo_total_sala or Reto.completed
	end,

	PorcentajeObjetivo = function ()
		--vamos a devolver el % de tiempo que tenemos antes de que salga la avalancha
		return Reto.time_acum / Reto.tiempo_total_sala
	end,
	
	UpdateLife = function(life, damage)
		Reto.life = life - damage
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