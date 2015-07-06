--@author Alberto Martínez Villarán
--@date Marzo, 2015

--Tabla donde estan registradas todas las monedas de dinero del juego

Dinero = {
	
	---OJO todas las monedas tienen por ahora el mismo tamaño que esta seteado en el arquetipo, si se quiere modificar el tamaño por tipo de moneda, se puede setear aqui. Hay que tener en cuenta el tamaño de los componentes fisicos tmb
	Monedas = {
		{
			type = "Coin",
			name = "Moneda1",
			value = 1,
			-- model = "Moneda.mesh",
			-- material ="snow",
			-- scale = {0.6, 0.6, 0.6},
		},


		{
			type = "Coin2",
			name = "Moneda10",
			value = 10,
			-- model = "Moneda.mesh",
			-- material ="coin",
			-- scale = {1, 1, 1},
		},


		{
			type = "Coin3",
			name = "Moneda100",
			value = 100,
			-- model = "Moneda.mesh",
			-- material ="lava",
			-- scale = {1.5, 1.5, 1.5},
		},
	},

	MonedasID = 1,

	GetNextID = function (  )
		local id = Dinero.MonedasID + 1
		Dinero.MonedasID = id
		return id
	end,
		

	GetMonedas = function (cantidad, granularidad, maxMonedas, resultado)
		-- print(cantidad.." - "..granularidad.." - "..maxMonedas)
		-- body
		-- print(resultado)
		local size = table.getn(Dinero.Monedas)

		local monedaMasGrande = 0

		local minNumMonedas = 0

		repeat
			monedaMasGrande = Dinero.Monedas[size].value

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

		for k,v in pairs(Dinero.Monedas) do

			if v.value <= monedaOptima then --Solo podemos completar usando monedas iguales o menos a la monedaOptima
				local auxDiff = math.abs(monedaOptima - v.value)

				if auxDiff <= diff  then
					diff = auxDiff
					moneda = v
				end
			end
		end
		-- print("Mi moneda optima: "..moneda.value)


		numMonedasAUsar  = (cantidad/moneda.value)
		-- print("Monedas a usar: "..numMonedasAUsar)

		--Si el numero de monedas a usar estimado con esta granularidad sobrepasa el máximo establecido
		-- reducimos la granularidad hasta que se ajuste o la granularidad sea 1, si la granularidad alcanza 1, se puede sobrepasar el máximo establecido.
		if numMonedasAUsar > maxMonedas then
			repeat
				granularidad = math.max(1, granularidad - 1)
				
				numMonedasAUsar  = minNumMonedas * granularidad
				-- print("2Monedas a usar: "..numMonedasAUsar.." - Max: "..maxMonedas.." - "..granularidad)

				monedaOptima = math.floor( cantidad / numMonedasAUsar )
				--nos aseguramos que la moneda optima al menos sea la unitaria
				if monedaOptima == 0 then
					monedaOptima = 1
				end
				-- print("2monedaOptima: "..monedaOptima)

				diff = math.huge
				moneda = nil

				for k,v in pairs(Dinero.Monedas) do

					if v.value <= monedaOptima then --Solo podemos completar usando monedas iguales o menos a la monedaOptima
						local auxDiff = math.abs(monedaOptima - v.value)

						if auxDiff <= diff  then
							diff = auxDiff
							moneda = v
						end
					end
				end

				-- print("2Mi moneda optima: "..moneda.value)

				
			until numMonedasAUsar <= maxMonedas or granularidad == 1
		end


		--me copio la moneda
		local monedaCopia = {}

		for k,v in pairs(moneda) do
			monedaCopia[k] = v
		end

		local resto = cantidad
		local numMonedas = 0

		while resto >= moneda.value do
			resto = resto - monedaCopia.value
			numMonedas = numMonedas + 1
			table.insert(resultado, monedaCopia)
			-- print(moneda)
		end

		-- print("Resto al final: "..resto)
		-- print(numMonedas.." monedas usadas de "..monedaCopia.value)

		if resto == 0 then
			-- print("FIN")
			return resultado
		else
			-- print("llamada recursiva")
			return Dinero.GetMonedas(resto, granularidad, maxMonedas - numMonedas, resultado)
		end
	end,

	--Esta función crea una determinada cantidad de dinero en la posición indicada. Aquí es donde se deberían repartir las monedas de forma bonita.
	CreateCantidadDinero = function ( posicion, cantidad, granularidad, maxMonedas )
		-- body
		local monedasACrear = {}
		-- print(monedasACrear)
		Dinero.GetMonedas(cantidad, granularidad, maxMonedas, monedasACrear)

		--Debug
		for k,v in pairs(monedasACrear) do

			local entity = MapEntity(v.name..Dinero.GetNextID())

			entity:SetType(v.type)

			local z = math.random(-2,2)
			local z1 = math.random(0,9)
			entity:SetAttrib("position", toString({posicion.x, (posicion.y), z.."."..z1}))

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
