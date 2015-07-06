--@author Alberto Martínez Villarán
--@date Marzo, 2015

--Tabla donde estan registradas todos los cofres del juego 

Cofres = {
	
	--TIPOS DE COFRE. Admitimos N tipos de cofres clasificados en 3 tiers diferentes *El nº de tiers si es fijo
	Tipos = {
		Tier1 = {

			--Ejemplo

			--{
			--  name = Nombre del cofre
			-- 	peso = Peso para tiradas de probabilidad. Solo se tienen en cuenta para el Tier en el que esta la habilidad
			-- 	type = tipo del arquetipo del cofre
			--	material = material del cofre
			--	model = modelo del cofre
			--	price = precio básico del cofre

			--},
			chest_level = 1,

			{
				name = "CofreNormal",
				peso = 20, 
				type = "Chest",
				price = 1000,  --precio base

				price_function = function (sala)
					if sala > 9 then
						return math.floor(0.5 * sala * sala + sala + 1000)
					else
						return 1000
					end
				end,

			},

			{
				name = "CofreNormal2",
				peso = 30, 
				type = "Chest",
				price = 1000,  --precio base

				price_function = function (sala)
					if sala > 9 then
						return math.floor(0.5 * sala * sala + sala + 1000)
					else
						return 1000
					end
				end,

			},

		},

		Tier2 = {
			chest_level = 2,
			{
				name = "CofreMedio",
				peso = 20, 
				type = "Chest2",
				price = 2500,  --precio base

				price_function = function (sala)
					return math.floor(0.8 * sala * sala + sala + 2500)
				end,

			},

		},

		Tier3 = {
			chest_level = 3,
			{
				name = "CofreGrande",
				peso = 20, 
				type = "Chest3",
				price = 5000,  --precio base

				price_function = function (sala)
					return math.floor(sala * sala + 9 * sala + 5000)
				end,

			},

		},
	},

	
	CofresActualizados = {},


	--Esta función acumula el peso de los cofres y vuelca la información en CofresActualizados, la cual se usará durante el resto de la partida
	Init = function (  )

		for key,value in pairs(Cofres.Tipos) do
			--acumulamos los pesos dentro de cada Tier
			if type(value) == "table" then

				Cofres.CofresActualizados[key] = {}
				Cofres.CofresActualizados[key].chest_level = value.chest_level
				local totalWeight = 0

				for k,v in pairs(value) do

					if type(v) == "table" then
						totalWeight = totalWeight + v.peso

						local aux = {}

						--copiamos el contenido en aux
						for i,j in pairs(v) do
							aux[i] = j
						end

						aux.pesoAcum = totalWeight
						--insertamos aux en el sitio adecuado
						table.insert(Cofres.CofresActualizados[key], aux)
					end
				end

				Cofres.CofresActualizados[key].totalWeight = totalWeight

			end
		end

		-- -- DEBUG. Acumulacion
		-- for k,v in pairs(Cofres.CofresActualizados) do
		-- 	print(k, v)
		-- 	if type(v) == "table" then
		-- 		for k1,v1 in pairs(v) do
		-- 			print(k1,v1)
		-- 			if type(v1) == "table" then
		-- 				for k2,v2 in pairs(v1) do
		-- 					print(k2,v2)
		-- 				end
		-- 			end
		-- 		end
		-- 	end
		-- end

	end,


	ChestID = 0,

	NextChestID = function ( )
		-- body
		Cofres.ChestID = Cofres.ChestID + 1
		return Cofres.ChestID
	end,


	--INFO Y FUNCIONES RELATIVAS A LA COLOCACION DE COFRES

	--límites en número de puntos de interes para obtener el MÁXIMO de cofres a colocar
	--El primer limite indica que hasta él habra como máximo 1 confre, el segundo indica que hasta él habra como máximo 2 cofres ...etc.
	chestLimits = {
		10,
		20,
		30,
		40,
		50,
		math.huge, ---este último limita el máx ptos de Interes
	},

	---Funcion de Parámetros de crecimiento de las probabilidades de los tipos de cofres
	--Hay tres tipos de cofres: Normales, medianos y grandes. Cada tipo de cofre tiene un par de parámetros mi y ni.
	--Se usan dado que el peso de cada tipo de cofre varía con el numero de salas superadas, con la siguiente fórmula:  mi * X + ni
	--Esta función define estos parámetros dependiendo del número de salas, para adaptar su crecimiento dependiendo de la profundidad
	ChestParameters = function ( numSalas )
		-- body
		local m1 = 0
		local n1 = 0
		local m2 = 0
		local n2 = 0
		local m3 = 0
		local n3 = 0

		if numSalas < 4 then
			m1 = 1
			n1 = 100
			m2 = 0
			n2 = 0
			m3 = 0
			n3 = 0
		elseif 4 <= numSalas and numSalas < 8 then
			m1 = -1
			n1 = 40
			m2 = 1
			n2 = 40
			m3 = 0
			n3 = 0
		elseif 8 <= numSalas then
			m1 = -1
			n1 = 30
			m2 = 1
			n2 = 60
			m3 = 1
			n3 = 40
		end

		return m1, n1, m2, n2, m3, n3
	end,

	--Esta función actualiza los precios de los cofres dependiendo de la sala en la que estemos, aplicando un incremento definido por 
	-- un porcentaje del precio que ya teníamos
	ActualizaPrecios = function (numSalas)
		-- Esta función puede cambiar, por ahora aumentamos un 1% del precio que ya existía a todos los cofres

		for key, value in pairs(Cofres.CofresActualizados) do
			if type(value) == "table" then
				for k,v in pairs(value) do
					if type(v) == "table" then
						v.price = v.price_function(numSalas)
					end
				end
			end
		end

		-- --DEBUG. Actualizacion de rpecios
		-- for k,v in pairs(Cofres.CofresActualizados) do
		-- 	print(k, v)
		-- 	if type(v) == "table" then
		-- 		for k1,v1 in pairs(v) do
		-- 			print(k1,v1)
		-- 			if type(v1) == "table" then
		-- 				print(v1.price)
		-- 			end
		-- 		end
		-- 	end
		-- end

	end,

	--Esta función devuelve el tipo de cofre a instancia, haciendo una tirada probabilística sobre los pesos de los tres TIERS de cofres.
	--Para ello primero se consultan los pares de valores configurados y después se calculan las probabilidades dependiendo de la profundidad y se tiran los dados
	GetTipoCofre = function ( FuncParameters, numSalas )
		-- body

		local m1, n1, m2, n2, m3, n3 = FuncParameters(numSalas)

		local t1Prob = m1*numSalas + n1
		local t2Prob = m2*numSalas + n2
		local t3Prob = m3*numSalas + n3

		local t1ProbAcum = t1Prob
		local t2ProbAcum = t1ProbAcum + t2Prob
		local t3ProbAcum = t2ProbAcum + t3Prob

		-- local rnd = math.random(t3ProbAcum)
		local rnd = Random(t3ProbAcum)
		-- Log.Debug("GetTipoCofre RND: "..rnd)
		if t1ProbAcum >= rnd then
			--Es del tipo 1cofre normal
			return Cofres.CofresActualizados.Tier1
		elseif t1ProbAcum < rnd and rnd <= t2ProbAcum then
			return Cofres.CofresActualizados.Tier2
		elseif t2ProbAcum < rnd and rnd <= t3ProbAcum then
			return Cofres.CofresActualizados.Tier3
		end

	end,

	GetCofre = function (tierCofre)
		-- print ("totalWeight = "..tierCofre.totalWeight)
		-- local rnd = math.random(tierCofre.totalWeight)
		local rnd = Random(tierCofre.totalWeight)
		-- Log.Debug("GetCofre RND: "..rnd)
		-- print("RND de GetCofre: "..rnd)

		for k,v in pairs(tierCofre) do
			
			if type(v) == "table" then
				-- print(k, v.pesoAcum)
				if v.pesoAcum >= rnd then
					return v
				end

			end
		end
	end,

	--Esta función obtiene el número MAXIMO de cofres posibles, el cual depende de los puntos de interes actuales del mapa y de la configuración dada a los límites
	--Esto límites se configuran en Cofres.chestLimits
	GetNumMaxChest = function ( numPuntosInteres )
		-- body
		for k,v in pairs(Cofres.chestLimits) do
			if numPuntosInteres <= v then
				--el numero máximo de cofres coincide con el indice y este coincide con la clave de la tabla
				return k
			end
		end
	end,

	--Esta función obtiene a partir del numero Máximo de Cofres el numero REAL de cofres a colocar en el mapa.
	--Sigue una idea parecida a la campana de Gauss o distribución normal donde los valores más cercanos a la media tienen más probabilidad o peso.
	--Si el numMax = 5, los posibles cofres a poner son: 0,1,2,3,4 o 5
	--Esta función les da el siguiente peso:
	-- 0 - +
	-- 1 - ++
	-- 2 - +++
	-- 3 - +++
	-- 4 - ++
	-- 5 - +
	--Después se lanza un dado para obtener el numero real de cofres
	GetNumChestFinal = function ( numMaxChest )
		-- body
		---Primero el casos base
		-- Log.Debug("POST: "..numMaxChest)
		if numMaxChest == 1 then
			--Si solo podemos tener 1 cofre lanzamos un random entre 1 y 0
			-- local rnd = math.random(0,1)
			local rnd = RandomRange(0,1)
			-- Log.Debug("GetNumChestFinal (caso base 0-1) RND: "..rnd)
			return rnd
		elseif numMaxChest >= 2 then
			--En este caso le aplicamos pesos siguiendo algo parecido a una distribucion normal, donde los valores más cercanos a la media tienen más peso

			local totalWeight = 0
			local weight = 1
			local auxTabla = {}

			local creciendo = true
			for i=0,numMaxChest do
				-- print("construccion de aux: "..(i+1).." real: "..i.." - "..weight)
				auxTabla[i+1] = weight
				totalWeight = totalWeight + weight


				if weight < numMaxChest*0.5  and creciendo then
					weight = weight + 1
				elseif weight == numMaxChest*0.5 and creciendo then
					weight = weight + 1
					creciendo = false
				elseif weight > numMaxChest*0.5 and creciendo then
					-- weight = weight - 1
					creciendo = false
				elseif not creciendo then
					weight = weight - 1
				end

					
			end
			-- print("totalWeight: "..totalWeight)
			-- math.random()
			--una vez dados los pesos, escogemos el numero de cofres lanzando un dado
			-- local rnd = math.random(totalWeight)
			local rnd = 0

			if FuncionesMapList.currentMap ~= "HabitacionTesoro" then
				rnd = Random(totalWeight)
			else
				rnd = RandomRange(math.floor(totalWeight*0.7),totalWeight) --Si estamos en la sala del tesoro, aseguramos un numero minimo de cofres
			end

			-- Log.Debug("GetNumChestFinal (problematico) RND: "..rnd)
			local pesoAnterior = 0
			-- print("RND: "..rnd)
			for k,v in pairs(auxTabla) do
				-- print("for interno: ")
				-- print(k,v)
				pesoAnterior = pesoAnterior + v
				if pesoAnterior >= rnd then
					-- print(v)
					-- este es el numero de cofres elegido
					-- dicho numero coindice con el indice -1
					return k - 1
				end
			end

		end
	end,


	---Función encargada de la creación de los cofres. Una vez sabe el numero real de cofres, hará tiradas por cada uno para averiguar su tipo.
	--Con el tipo obtenido, se hacen tiradas sobre los puntos de interés para ver donde se sitúa. (Los puntos tienen un peso y se acumulan al vuelo)
	--Una vez creado el entityInfo y situado en la lista de entidades que el mapa lee del parser (AddEntity)
	--Se borra el pto de interes de la lista, de forma que los siguientes elementos a colocar no lo hagan sobre las mismas posiciones.
	--Es por esto que cada vez que se hace una comprobacion sobre los ptos de interes se debe acumular al vuelo
	CreateChests = function ()
		-- body

		--Actualizamos los precios de los cofres, puesto que este método se llama cada vez que se carga una sala nueva
		Cofres.ActualizaPrecios(GameMgr.salas_totales)

		-- Log.Debug("PRE: "..Drop.numPuntosInteres)
		local numMaxChest = Cofres.GetNumMaxChest(Drop.numPuntosInteres)
		-- Log.Debug("PRE2: "..numMaxChest)
		local numChestFinal = Cofres.GetNumChestFinal(numMaxChest)
		
		
		-- numChestFinal = 5
		Log.Debug("COFRES A PONER: "..numChestFinal)
		-- Log.Debug("------------------------------------------ESTA HARDCODEADO QUE SEAN 2 PARA PROBAR COFRES --------------------------------")
		
		for i=1,numChestFinal do
			
			--elegimos el tier de cofre
			local tierCofre = Cofres.GetTipoCofre(Cofres.ChestParameters, GameMgr.salas_totales)

			--de ese tier sacamos un cofre con tirada probabilística
			local cofre = Cofres.GetCofre(tierCofre)

			--acumulamos los ptos de interes
			local totalWeight = 0
			for k,v in pairs(Drop.puntosInteres) do
				totalWeight = totalWeight + v.peso
				v.pesoAcum = totalWeight

				-- print(k,v.peso, v.pesoAcum)
			end

			--elegimos en qué punto colocar el cofre
			-- local rnd = math.random(totalWeight)
			local rnd = Random(totalWeight)
			-- Log.Debug("CreateChests RND: "..rnd)
			local punto = nil

			-- print ("RND: "..rnd)

			for key,value in pairs(Drop.puntosInteres) do
				if value.pesoAcum >= rnd then
					punto = key
					-- print(punto)
					--este es el pto de interes elegido para el cofre

					local entity = MapEntity(cofre.name..Cofres.NextChestID())

					entity:SetType(cofre.type)
					-- entity:SetAttrib("position", value.pos.x..","..value.pos.y..","..value.pos.z)

					--Colocamos los cofres en una pos retrasada
					entity:SetAttrib("position", value.pos.x..","..value.pos.y..", -15")

					--Añadimos el resto de atributos, que al ser variables se debe hacer con un for
					--Hay tres atributos que no se deben añadir: type, peso y pesoAcum
					for k,v in pairs(cofre) do
						if k ~= "type" and k ~= "peso" and k ~= "pesoAcum"  and  k ~= "price_function" then
							-- print(k, toString(v))
							if k == "price" and FuncionesMapList.currentMap == "HabitacionTesoro" then
								entity:SetAttrib(k, toString(math.floor(v*0.5)))
							else
								entity:SetAttrib(k, toString(v))
							end
						end
					end

					-- entity:SetAttrib("model", tipoCofre.model)
					-- entity:SetAttrib("price", toString(tipoCofre.price))
					-- entity:SetAttrib("material", tipoCofre.material)

					entity:SetAttrib("chest_level", toString(tierCofre.chest_level))
					AddEntity(entity)
					Log.Debug("Cofre sacado: "..value.pos.x..","..value.pos.y..","..value.pos.z)
					break
				end
			end

			-- --borramos este punto de interés de la tabla
			Drop.puntosInteres[punto] = nil
			Drop.numPuntosInteres = Drop.numPuntosInteres - 1
			-- for k,v in pairs(Drop.puntosInteres) do
			-- 	print(k,v.peso, v.pesoAcum)
			-- end
		end

		return result

	end,

}


