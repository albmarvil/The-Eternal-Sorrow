--author: César Manuel Paz Gumán

Ambientaciones = 
{
	--0 castillo
	--1 mazmorra
	-- 2 nieve
	---3 lava
	Identificador = {0,1,2,3},
	SalasMinimasEntreAmbientaciones = 1,


	NumeroSalasActualesMismaAmbientacion = 0,
	AmbientacionActual = 0,
	ProbabilidadCambioAmbientacion = 50,

	--esta flag sirve para indicar que se ha pasado de castillo a mazmorra al menos una vez en el inicio
	MazmorraInicial = false,
}

function Ambientaciones.Init()
	Ambientaciones.AmbientacionActual = 0
	Ambientaciones.NumeroSalasActualesMismaAmbientacion = 0
end

function Ambientaciones.Release()
	Ambientaciones.AmbientacionActual = 0
	Ambientaciones.MazmorraInicial = false
end

function Ambientaciones.nextMap()

	Ambientaciones.NumeroSalasActualesMismaAmbientacion = Ambientaciones.NumeroSalasActualesMismaAmbientacion + 1
	Log.Debug("NumeroDeSalaActuales: "..toString(Ambientaciones.NumeroSalasActualesMismaAmbientacion))
	--Si pasamos un numero minimo de niveles en la misma ambientacion, tiramos un random para ver si podemos cambiarla
	if Ambientaciones.NumeroSalasActualesMismaAmbientacion > Ambientaciones.SalasMinimasEntreAmbientaciones then
		local rnd = Random(100)
		Log.Debug("Random cambio ambientacion: "..toString(rnd))
		if rnd <= Ambientaciones.ProbabilidadCambioAmbientacion then
			Ambientaciones.NumeroSalasActualesMismaAmbientacion = 0

			

			-- Log.Debug("AmbientacionActual: "..toString(Ambientaciones.AmbientacionActual))
			-- for key, value in pairs(Ambientaciones.Identificador) do
			-- 	Log.Debug("Numero: "..toString(value))
			-- end

			if not Ambientaciones.MazmorraInicial and Ambientaciones.AmbientacionActual == 0 then --si estamos en el castillo y no hemos pasado nunca a mazmorra, obligamos
				Ambientaciones.AmbientacionActual = 1
				Ambientaciones.MazmorraInicial = true
			else

				table.remove(Ambientaciones.Identificador, Ambientaciones.AmbientacionActual+1)

				Ambientaciones.AmbientacionActual = Ambientaciones.Identificador[RandomRange(1,3)]

				Ambientaciones.Identificador = {0,1,2,3}

			end

			LogicMapManager.getInstance():changeAmbientacion(Ambientaciones.AmbientacionActual)

			
		end
	end
end