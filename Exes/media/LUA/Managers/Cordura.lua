
Cordura = {
	
	Init = function ( )
		Cordura.estadoActual = Cordura.Normal
	end,
	estadoActual = nil,

	tick = function ( msecs )
		
		local vidaTotal = Data_Player.player_stats.vida
		local vida = Data_Player.player_info.vidaActual

		local porcentaje = (vida/vidaTotal)*100

		local estadoATransitar = nil

		if 41 <= porcentaje and porcentaje <= 61 then
			estadoATransitar = Cordura.Intranquilo
		elseif 21 <= porcentaje and porcentaje <= 40 then
			estadoATransitar = Cordura.Alucinaciones
		elseif 0 <= porcentaje and porcentaje <= 20 then
			estadoATransitar = Cordura.Demente
		end

		if estadoATransitar ~= Cordura.estadoActual and estadoATransitar ~= nil then
			--transitamos
			Cordura.estadoActual.Release()
			Cordura.estadoActual = estadoATransitar
			Cordura.estadoActual.Init()
		end

		Cordura.estadoActual.tick(msecs)

	end,

	Normal = {
		Init = function ()
		end,

		Release = function ()
		end,

		tick = function (msecs)
		end,
	},

	Intranquilo = {


		Init = function ()
			Log.Debug("Cordura: Intranquilo")

		end,

		Release = function ()

		end,

		tick = function (msecs)

			
		end,

	},

	Alucinaciones = {


		Init = function ()
			Log.Debug("Cordura: Alucinaciones")
			
		end,

		Release = function ()
			
		end,

		tick = function (msecs)

		end,

	},

	Demente = {

		Init = function ()
			Log.Debug("Cordura: Demente")
			ActiveViewportEffect("Cordura2")
		end,

		Release = function ()
			DisableViewportEffect("Cordura2")
		end,

		tick = function (msecs)
		end,

	},


}