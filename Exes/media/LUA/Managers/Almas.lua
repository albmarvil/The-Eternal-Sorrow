--@author Alberto Martínez Villarán
--@date Marzo, 2015

--Tabla donde estan registradas todas las almas del juego

Almas = {
	
	Tipos = {
		{
			type = "Soul",
			name = "Alma1",
			souls = 5,
			feedbackParticle = "AlmaAzulFeedback",
			sourceParticles = {"AlmaAzul"},
		},

		{
			type = "Soul2",
			name = "Alma5",
			souls = 10,
			feedbackParticle = "AlmaVerdeFeedback",
			sourceParticles = {"AlmaVerde"},
		},

		{
			type = "Soul3",
			name = "Alma10",
			souls = 25,
			feedbackParticle = "AlmaRojaFeedback",
			sourceParticles = {"AlmaRoja"},
		},
	},

	AlmasID = 1,

	GetNextID = function (  )
		local id = Almas.AlmasID + 1
		Almas.AlmasID = id
		return id
	end,
		

	GetAlmas = function (cantidadOriginal, granularidad, resultado)
		-- body

		---Capamos la cantidad pedida para que se aun múltiplo de el alma más pequeña, en este caso un 5%
		local cantidad = cantidadOriginal
		local minSoul = Almas.Tipos[1].souls

		-- print("minSoul: "..minSoul)

		if cantidad < minSoul then
			cantidad = minSoul
		end

		local int, frac = math.modf(cantidad/minSoul)
		while frac ~= 0 do
			if frac <= 0.5 then
				cantidad = cantidad - 1
			else
				cantidad = cantidad + 1
			end
			int, frac = math.modf(cantidad/minSoul)
		end

		-- print("Cantidad: "..cantidad.." / "..cantidadOriginal)

		local size = table.getn(Almas.Tipos)

		local monedaMasGrande = 0

		local minNumMonedas = 0

		repeat
			monedaMasGrande = Almas.Tipos[size].souls

			minNumMonedas = math.floor(cantidad / monedaMasGrande)

			size = size - 1
		until minNumMonedas >= 1

		-- print("minNumMonedas: "..minNumMonedas)

		--- minNumMonedas tiene un mini aproximado de las monedas que podemos usar

		local numMonedasAUsar = minNumMonedas * granularidad


		-- print("numMonedasAUsar: "..numMonedasAUsar)

		local monedaOptima = math.floor( cantidad / numMonedasAUsar )
		--nos aseguramos que la moneda optima al menos sea la unitaria
		if monedaOptima == 0 then
			monedaOptima = 1
		end
		-- print("monedaOptima: "..monedaOptima)
		--El numero de monedas a usar es un multiplicador que intentará usar ese numero de monedas, si la granularidad es 1, se intentará dar el minimo nº de monedas posible
		
		local diff = math.huge
		local moneda = nil

		for k,v in pairs(Almas.Tipos) do

			if v.souls <= monedaOptima then --Solo podemos completar usando monedas iguales o menos a la monedaOptima
				local auxDiff = math.abs(monedaOptima - v.souls)

				if auxDiff <= diff  then
					diff = auxDiff
					moneda = v
				end
			end
		end
		-- print("Mi moneda optima: "..moneda.souls)

		--me copio la moneda
		local monedaCopia = {}

		for k,v in pairs(moneda) do
			monedaCopia[k] = v
		end

		local resto = cantidad
		local numMonedas = 0

		while resto >= moneda.souls do
			resto = resto - monedaCopia.souls
			numMonedas = numMonedas + 1
			table.insert(resultado, monedaCopia)
			-- print(moneda)
		end

		-- print("Resto al final: "..resto)
		-- print(numMonedas.." monedas usadas de "..monedaCopia.souls)

		if resto == 0 then
			-- print("FIN")
			return resultado
		else
			-- print("llamada recursiva")
			return Almas.GetAlmas(resto, granularidad, resultado)
		end
	end,

	--Esta función crea una determinada cantidad de almas en la posición indicada. Aquí es donde se deberían repartir las almas de forma bonita.
	CreateCantidadAlmas = function ( posicion, minradio, maxradio, cantidad, granularidad )
		-- body
		local almasACrear = {}
		Almas.GetAlmas(cantidad, granularidad, almasACrear)

		--Debug
		for k,v in pairs(almasACrear) do

			local entity = MapEntity(v.name..Almas.GetNextID())

			entity:SetType(v.type)

			--calculamos los limites inferiores del radio donde no queremos que aparezca el objeto
			local minHorInf = posicion.x - minradio
			local minHorSup = posicion.x + minradio

			--calculamos la posicion en la cual va a ir el potenciador
			--para ello calculamos los límites de aparicion del objeto

			local horInf = posicion.x - maxradio
			local horSup = posicion.x + maxradio
			local verInf = posicion.y --Con esto forzamos que el objeto salga por arriba del contenedor
			local verSup = posicion.y + maxradio


			local x = 0
			local y = 0
			local tileID = -1

			repeat
				x = math.random(horInf, horSup)
				y = math.random(verInf, verSup)
				tileID = GameMgr.getTile(x, y)
				--repetir hasta que el tile sea viable Y
				-- la coordenada X no este dentro del radio minimo
			until tileID == 0 and (x >= minHorSup or x <= minHorInf)

			x, y = GameMgr.positionToTilePosition(x, y)
			x = (20 * x) + math.random(-5, 5)
			y = (-20 * y) + math.random(-5, 5)

			---LA nueva posicion admisible para este potenciador es:
			local newPosition = Vector3(x,y,0)

			entity:SetAttrib("position", toString({newPosition.x, (newPosition.y), newPosition.z}))

			--Añadimos el resto de atributos mientras no sea el nombre
			for key,value in pairs(v) do

				if key ~= "name" then
					entity:SetAttrib(key, toString(value))
				end
			end

			NewEntity(entity, GameMgr.map)
		end
	end,
	

}
