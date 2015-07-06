
Drop = 
{

	--INFO Y FUNCIONES GENERALES DEL DROP: Inicialización, tabla de drop general (GDD) y función DropObjectByType


	Contenedores = {

		ChestT1 = { --Cofres normales o de Tier 1
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
			pesos = {
				{name = "Nada", peso = 0}, --Esto sirve para indicar que no debe caer nada
				{name = "Almas", peso = 2}, --En los primeros cofres es raro que necesite vida, quiero abrir cofres para habilidades
				{name = "Dinero", peso = 2},
				{name = "Habilidad1", peso = 55},
				{name = "Habilidad2", peso = 15}, 
				{name = "Habilidad3", peso = 0},
				{name = "Potenciador", peso = 7},
				{name = "Arma1", peso = 5},
				{name = "Arma2", peso = 1},
				{name = "Arma3", peso = 0},
			},

			rangoPotenciadores = {min = 5, max = 7},
			rangoAlmas = {min = 25, max = 50}, --son porcentajes
			rangoDineroBase = {min = 1000, max = 2000},

			rangoDinero_function = function (sala)
				local min = math.floor(0.5 * sala * sala + sala + 1000)
				local max = math.floor(0.5 * sala * sala + sala + 2000)
				return min, max
			end,

		},

		ChestT2 = { --Cofres medios o de Tier 2
			pesos = {
				{name = "Nada", peso = 0},
				{name = "Almas", peso = 5}, --Dado que salen dos cosas, un poco mas de probabilidad que el anterior
				{name = "Dinero", peso = 3},
				{name = "Habilidad1", peso = 10},
				{name = "Habilidad2", peso = 45}, 
				{name = "Habilidad3", peso = 5}, 
				{name = "Potenciador", peso = 7},
				{name = "Arma1", peso = 3},
				{name = "Arma2", peso = 3}, 
				{name = "Arma3", peso = 1},
			},

			rangoPotenciadores = {min = 5, max = 7},
			rangoAlmas = {min = 50, max = 75}, --son porcentajes
			rangoDineroBase = {min = 2500, max = 5000},

			rangoDinero_function = function (sala)
				local min = math.floor(0.5 * sala * sala + sala + 2500)
				local max = math.floor(0.5 * sala * sala + sala + 5000)
				return min, max
			end,
		},

		ChestT3 = { --Cofres grandes o de Tier 3
			pesos = {
				{name = "Nada", peso = 0},
				{name = "Almas", peso = 8}, --Dado que salen tres cosas, un poco mas de probabilidad que el anterior
				{name = "Dinero", peso = 4},
				{name = "Habilidad1", peso = 10},
				{name = "Habilidad2", peso = 20}, --2
				{name = "Habilidad3", peso = 40}, --3
				{name = "Potenciador", peso = 7},
				{name = "Arma1", peso = 0},
				{name = "Arma2", peso = 1},
				{name = "Arma3", peso = 10},
			},

			rangoPotenciadores = {min = 1, max = 8},
			rangoAlmas = {min = 75, max = 100}, --son porcentajes
			rangoDineroBase = {min = 5000, max = 10000},

			rangoDinero_function = function (sala)
				local min = math.floor(sala * sala + 9 * sala + 5000)
				local max = math.floor(sala * sala + 9 * sala + 10000)
				return min, max
			end,
		},

		brokenObject = { --Cofres normales o de Tier 1
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
			pesos = {
				{name = "Nada", peso = 80}, --Esto sirve para indicar que no debe caer nada
				{name = "Almas", peso = 5},
				{name = "Dinero", peso = 10},
				{name = "Habilidad1", peso = 2},
				{name = "Habilidad2", peso = 0}, 
				{name = "Habilidad3", peso = 0},
				{name = "Potenciador", peso = 3},
				{name = "Arma1", peso = 0},
				{name = "Arma2", peso = 0},
				{name = "Arma3", peso = 0},
			},

			rangoPotenciadores = {min = 1, max = 1},
			rangoAlmas = {min = 1, max = 10}, --son porcentajes
			rangoDineroBase = {min = 10, max = 50},

			rangoDinero_function = function (sala)
				local min = math.floor(1.5 * sala * sala + sala + 10)
				local max = math.floor(1.5 * sala * sala + sala + 50)
				return min, max
			end,

		},

		brokenObjectsNoTile = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
			pesos = {
				{name = "Nada", peso = 0}, --Esto sirve para indicar que no debe caer nada
				{name = "Almas", peso = 40},
				{name = "Dinero", peso = 55},
				{name = "Habilidad1", peso = 5},
				{name = "Habilidad2", peso = 0}, 
				{name = "Habilidad3", peso = 0},
				{name = "Potenciador", peso = 5},
				{name = "Arma1", peso = 1},
				{name = "Arma2", peso = 0},
				{name = "Arma3", peso = 0},
			},

			rangoPotenciadores = {min = 1, max = 3},
			rangoAlmas = {min = 15, max = 25}, --son porcentajes
			rangoDineroBase = {min = 75, max = 150},

			rangoDinero_function = function (sala)
				local min = math.floor(1.5 * sala * sala + sala + 75)
				local max = math.floor(1.5 * sala * sala + sala + 150)
				return min, max
			end,

		},
	},

	Init = function ()
		--Esta inicialización sirve para rellenar los vectores de acumulación una vez en el juego
		Cofres.Init()
		--inicializamos las armas
		Armas.Init()
		--inicializamos los potenciadores 
		--***OJO estamos fijando el nº máximo de potenciadores diferentes que puede haber en la partida
		Potenciadores.Init(24)
		--inicializamos las habilidades
		Habilidades.Init()

		--Acumulamos la tabla de probabilidades de cada contenedor del juego
		-- Y aprovechamos para inicializar los valores rangoDinero a partir de los valores básicos
		for k,v in pairs(Drop.Contenedores) do
			
			local totalWeight = 0

			local acum = {}

			for key,value in pairs(v.pesos) do

				if value.peso > 0 then --si el peso es nulo, no lo meto en el vector de acumulación
					totalWeight = totalWeight + value.peso

					acum[key] = {name = value.name, peso = totalWeight}
				end
					
			end
			v.rangoDinero = {min = v.rangoDineroBase.min, max = v.rangoDineroBase.max}
			v.acum = acum
			v.totalWeight = totalWeight
		end

	end,

	--Multiplicador para controlar rápidamente la cantidad de dinero que puede contener un contenedor
	MultiplicadorDinero = 1,

	--Esta funcion actualiza el rango de dinero que puede tener un contenedor
	--Esta función debería depender del numero de salas en la que estamos.
	--Se hace uso de la funcion de cada contenedor (la de los cofres son calcadas a la del precio de los cofres)
	ActualizaDineroContenedores = function (numSalas)
		for k,v in pairs(Drop.Contenedores) do
			if type(v) == "table" then
				--solo actualizamos aquellos que tengan funciones de actualización
				if v["rangoDinero_function"] ~= nil then
					local min, max = v.rangoDinero_function(numSalas)
					v.rangoDinero.min = min
					v.rangoDinero.max = max
					-- Log.Debug("Min: "..min)
					-- Log.Debug("Max: "..max)
				end
			end
		end

	end,

	--INFO Y FUNCIONES RELATIVAS A LOS PUNTOS DE INTERES
	puntosInteres = {},

	numPuntosInteres = 0,


	CreateObjects = function()
		--Al entrar en una nueva sala, actualizamos la información de los contenedores
		--Por ejemplo actualizamos el dinero que pueden soltar los diferentes tipos de contenedores
		--Este dato depende del numero de salas que llevemos

		Drop.ActualizaDineroContenedores(GameMgr.salas_totales)

		--limpiamos valores de los puntos de interes de la sala anterior
		Drop.numPuntosInteres = 0
		Drop.puntosInteres = {}

		local puntos = getInterestingPoints()

		--guardamos los puntos en lua para no acceder a C++
		if puntos ~= nil then
			for i = 0, puntos:size()-1, 1 do
				local aux_tabla = {peso = puntos:at(i):weight(), pos = puntos:at(i):pos(), pesoAcum = 0}
				Drop.puntosInteres[i+1] = aux_tabla
				Drop.numPuntosInteres = Drop.numPuntosInteres + 1
			end
		end
		
		if Drop.numPuntosInteres > 0 then
			--Elegimos los cofres sobre la info de los ptos de interes
			Cofres.CreateChests()
			
			if FuncionesMapList.currentMap ~= "HabitacionTesoro" then --En la habitacion del tesoro no creamos trampas
				--Elegimos las trampas sobre la info restante de ptos de interes
				FuncionesTrampas.crearTrampas()
			end

			--Eelegimos de los puntos de interes que queden puntos para spawnear los objetos rompibles
			FuncionesObjRompibles.crearObjetosRompibles()

		end
	end,



	DropObjectByType = function (tipo, posicion)

		if tipo:find("Arma") ~= nil then --estamos ante un arma
			
			if tipo:find("1") ~= nil then --Arma Tier1
				Armas.CreateArma(1, posicion) --Esto devuelve un arma aleatoria del tier indicado
			elseif tipo:find("2") ~= nil then --Arma Tier2
				Armas.CreateArma(2, posicion) 
			elseif tipo:find("3") ~= nil then --Arma Tier3
				Armas.CreateArma(3, posicion) 
			end
		
		elseif tipo:find("Habilidad") ~= nil then --estamos ante una habilidad

			if tipo:find("1") ~= nil then --Habilidad Tier1
				Habilidades.CreateHabilidad(1, posicion) --Esto devuelve una Habilidad aleatoria del tier indicado
			elseif tipo:find("2") ~= nil then --Habilidad Tier2
				Habilidades.CreateHabilidad(2, posicion) 
			elseif tipo:find("3") ~= nil then --Habilidad Tier3
				Habilidades.CreateHabilidad(3, posicion) 
			end
		end

	end,
	

}

--CALLBACK LLAMADO CUANDO MATAMOS A UN JEFE
function Drop.JefeMuerto()
	--Drop del resto de objetos: Habilidades, armas, ALMAS, POTENCIADORES
end

--CALLBACK LLAMADO CUANDO ROMPEMOS UN OBJETO
function Drop.ObjetoRompible(positionObjeto)
	-- Log.Debug("Se ha roto un objeto rompible")

	--cogemos los valores de la tabla que necesitamos
	local tablaObjAcum = Drop.Contenedores.brokenObject.acum
	local totalWeight = Drop.Contenedores.brokenObject.totalWeight
	local tablaObj = Drop.Contenedores.brokenObject


	--sacamos un número aleatorio del peso
	local rnd = Random(totalWeight)
	
	--buscamos el drop que tenemos que sacar del objeto rompible
	for k,v in pairs(tablaObjAcum) do
		if v.peso >= rnd then
			-- Log.Debug("Tengo que sacar algo de: "..v.name)

			if v.name == "Potenciador" then
				
				--averiguamos cuantos potenciadore sacamos a la vez
				local numPotenciadores = RandomRange(tablaObj.rangoPotenciadores.min, tablaObj.rangoPotenciadores.max)
				
				-- Log.Debug("Saco "..numPotenciadores.." potenciadores")

				--se le pasa la posicion, un radio donde apareceran los potenciadores y el nº de potenciadores a instanciar
				Potenciadores.CreatePotenciador(positionObjeto, 10, 30, numPotenciadores)
				
			elseif v.name == "Almas" then
				--averiguamos la cantidad de almas que sacamos
				local cantidadAlmas = RandomRange(tablaObj.rangoAlmas.min, tablaObj.rangoAlmas.max)
				
				-- Log.Debug("Saco "..cantidadAlmas.." almas del cofre")

				Almas.CreateCantidadAlmas( positionObjeto, 10, 30, cantidadAlmas, 1) --por ahora le indicamos la granularidad a machete
				
			elseif v.name == "Dinero" then
				--averiguamos la cantidad de dinero que sacamos
				local cantidadDinero = RandomRange(tablaObj.rangoDinero.min, tablaObj.rangoDinero.max)
				
				-- Log.Debug("Saco "..cantidadDinero.." dinero del cofre")

				--Como queremos conseguir una lluvia de monedas al abrir un cofre, le damos una granularidad alta
				-- Por ejemplo: granularidad =10, usaremos 10 veces más monedas que el mínimo de cambio aproximador
				-- La granularidad tmb depende de la cantidad soltada y de los tipos de monedas disponibles
				Dinero.CreateCantidadDinero( positionObjeto, cantidadDinero, 1, 5)
			else
				local entityType = Drop.DropObjectByType(v.name, positionObjeto)
			end

			break
		end
	end
end

--CALLBACK LLAMADO CUANDO ROMPEMOS UN OBJETO
function Drop.brokenObjectsNoTile(positionObjeto)
	-- Log.Debug("Se ha roto un objeto rompible")

	--cogemos los valores de la tabla que necesitamos
	local tablaObjAcum = Drop.Contenedores.brokenObjectsNoTile.acum
	local totalWeight = Drop.Contenedores.brokenObjectsNoTile.totalWeight
	local tablaObj = Drop.Contenedores.brokenObjectsNoTile


	--sacamos un número aleatorio del peso
	local rnd = Random(totalWeight)
	
	--buscamos el drop que tenemos que sacar del objeto rompible
	for k,v in pairs(tablaObjAcum) do
		if v.peso >= rnd then
			-- Log.Debug("Tengo que sacar algo de: "..v.name)

			if v.name == "Potenciador" then
				
				--averiguamos cuantos potenciadore sacamos a la vez
				local numPotenciadores = RandomRange(tablaObj.rangoPotenciadores.min, tablaObj.rangoPotenciadores.max)
				-- Log.Debug("Saco "..numPotenciadores.." potenciadores")

				--se le pasa la posicion, un radio donde apareceran los potenciadores y el nº de potenciadores a instanciar
				Potenciadores.CreatePotenciador(positionObjeto, 10, 30, numPotenciadores)
				
			elseif v.name == "Almas" then
				--averiguamos la cantidad de almas que sacamos
				local cantidadAlmas = RandomRange(tablaObj.rangoAlmas.min, tablaObj.rangoAlmas.max)
				-- Log.Debug("Saco "..cantidadAlmas.." almas del cofre")

				Almas.CreateCantidadAlmas( positionObjeto, 10, 30, cantidadAlmas, 1) --por ahora le indicamos la granularidad a machete
				
			elseif v.name == "Dinero" then
				--averiguamos la cantidad de dinero que sacamos
				local cantidadDinero = RandomRange(tablaObj.rangoDinero.min, tablaObj.rangoDinero.max)
				-- Log.Debug("Saco "..cantidadDinero.." dinero del cofre")

				--Como queremos conseguir una lluvia de monedas al abrir un cofre, le damos una granularidad alta
				-- Por ejemplo: granularidad =10, usaremos 10 veces más monedas que el mínimo de cambio aproximador
				-- La granularidad tmb depende de la cantidad soltada y de los tipos de monedas disponibles
				Dinero.CreateCantidadDinero( positionObjeto, cantidadDinero, 1, 5)
			else
				local entityType = Drop.DropObjectByType(v.name, positionObjeto)
			end

			break
		end
	end
end

--CALLBACK QUE EJECUTAREMOS CUANDO SE ABRA UN TESORO
function Drop.CofreAbierto(nivelCofre, posicionCofre)

	-- Log.Debug("Se ha abierto un cofre de nivel: "..nivelCofre.." - En la posicion: "..posicionCofre.x..", "..posicionCofre.y)

	-- local posSpawn = posicionCofre
	-- posSpawn.z = 0

	--seteamos la z a 0 para spawnear los elementos
	posicionCofre.z = 0

	local tablaCofreAcum = nil
	local tablaCofre = nil
	local totalWeight = 0

	local numHabilidades = nivelCofre

	if nivelCofre == 1 then
		tablaCofreAcum = Drop.Contenedores.ChestT1.acum
		tablaCofre = Drop.Contenedores.ChestT1
		totalWeight = Drop.Contenedores.ChestT1.totalWeight
	elseif nivelCofre == 2 then
		tablaCofreAcum = Drop.Contenedores.ChestT2.acum
		tablaCofre = Drop.Contenedores.ChestT2
		totalWeight = Drop.Contenedores.ChestT2.totalWeight
	elseif nivelCofre == 3 then
		tablaCofreAcum = Drop.Contenedores.ChestT3.acum
		tablaCofre = Drop.Contenedores.ChestT3
		totalWeight = Drop.Contenedores.ChestT3.totalWeight
	end

	-- local rnd = math.random(totalWeight)
	local rnd = Random(totalWeight)
	-- Log.Debug("Drop.CofreAbierto (tipo drop) RND: "..rnd)

	for k,v in pairs(tablaCofreAcum) do
		if v.peso >= rnd then
			-- Log.Debug("Tengo que sacar algo de: "..v.name)

			-- v.name = "Habilidad3"

			if v.name == "Potenciador" then
				
				--averiguamos cuantos potenciadore sacamos a la vez
				-- local numPotenciadores = math.random(tablaCofre.rangoPotenciadores.min, tablaCofre.rangoPotenciadores.max)
				local numPotenciadores = RandomRange(tablaCofre.rangoPotenciadores.min, tablaCofre.rangoPotenciadores.max)
				-- Log.Debug("Drop.CofreAbierto (potenciadores) RND: "..numPotenciadores)
				-- Log.Debug("Saco "..numPotenciadores.." potenciadores")

				--se le pasa la posicion, un radio donde apareceran los potenciadores y el nº de potenciadores a instanciar
				Potenciadores.CreatePotenciador(posicionCofre, 15, 40, numPotenciadores)
				
			elseif v.name == "Almas" then
				--averiguamos la cantidad de almas que sacamos
				-- local cantidadAlmas = math.random(tablaCofre.rangoAlmas.min, tablaCofre.rangoAlmas.max)
				local cantidadAlmas = RandomRange(tablaCofre.rangoAlmas.min, tablaCofre.rangoAlmas.max)
				-- Log.Debug("Drop.CofreAbierto (almas) RND: "..cantidadAlmas)
				-- Log.Debug("Saco "..cantidadAlmas.." almas del cofre")

				Almas.CreateCantidadAlmas( posicionCofre, 15, 40, cantidadAlmas, 1) --por ahora le indicamos la granularidad a machete
			elseif v.name == "Dinero" then
				--averiguamos la cantidad de dinero que sacamos
				-- local cantidadDinero = math.random(tablaCofre.rangoDinero.min, tablaCofre.rangoDinero.max)
				local cantidadDinero = RandomRange(tablaCofre.rangoDinero.min, tablaCofre.rangoDinero.max)
				-- Log.Debug("Drop.CofreAbierto (dinero) RND: "..cantidadDinero)
				-- Log.Debug("Saco "..cantidadDinero.." dinero del cofre")

				--Como queremos conseguir una lluvia de monedas al abrir un cofre, le damos una granularidad alta
				-- Por ejemplo: granularidad =10, usaremos 10 veces más monedas que el mínimo de cambio aproximador
				-- La granularidad tmb depende de la cantidad soltada y de los tipos de monedas disponibles
				Dinero.CreateCantidadDinero( posicionCofre, cantidadDinero, 2, 50)
			elseif v.name == "Habilidad1" or v.name == "Habilidad2" or v.name == "Habilidad3" then

				local offset = -20

				for i=1,numHabilidades do
					posicionCofre.x = posicionCofre.x + offset
					local entityType = Drop.DropObjectByType(v.name, posicionCofre)
					offset = offset + 20
				end
			else
				local entityType = Drop.DropObjectByType(v.name, posicionCofre)
			end

			break
		end
	end

end

--CALLBACK QUE EJECUTAREMOS CUANDO SE MUERA UN ENEMIGO
function Drop.EnemyDie(typeEnemy, nivelEnemy, positionEnemy)

	local radioAltar = true
	if RetoMgr.retoLoaded == "RetoAltar.lua" then
		radioAltar = Reto.distance
	end
	if EventoAvalancha.hordaInvocada == false and radioAltar then --Solo dropeamos cuando matamos a enemigos que no son del reto


		local tablaContendorAcum = Drop.Contenedores[typeEnemy].acum
		local tablaContenedor = Drop.Contenedores[typeEnemy]
		local totalWeight = Drop.Contenedores[typeEnemy].totalWeight

		-- local rnd = math.random(totalWeight)
		local rnd = Random(totalWeight)
		-- Log.Debug("Drop.CofreAbierto (tipo drop) RND: "..rnd)

		--Si estamos en el reto del altar calculamos si el enemigo ha muerto en su radio
		
		--Calculamos la cantidad de dinero que debemos dropear
		-- local minDinero, maxDinero = tablaContenedor.CrecimientoDinero(nivelEnemy)
		local minDinero = tablaContenedor.rangoDineroBase.min
		local maxDinero = tablaContenedor.rangoDineroBase.max

		local cantDinero = RandomRange(minDinero, maxDinero)
		Dinero.CreateCantidadDinero( positionEnemy, cantDinero, 1, 1)



		for k,v in pairs(tablaContendorAcum) do
			if v.peso >= rnd then
				-- Log.Debug("Tengo que sacar algo de: "..v.name)

				--Debug
				-- v.name = "Arma1"

				if v.name == "Potenciador" then
					
					--averiguamos cuantos potenciadore sacamos a la vez
					-- local numPotenciadores = math.random(tablaCofre.rangoPotenciadores.min, tablaCofre.rangoPotenciadores.max)
					local numPotenciadores = RandomRange(tablaContenedor.rangoPotenciadores.min, tablaContenedor.rangoPotenciadores.max)
					-- Log.Debug("Drop.CofreAbierto (potenciadores) RND: "..numPotenciadores)
					-- Log.Debug("Saco "..numPotenciadores.." potenciadores")

					--se le pasa la posicion, un radio donde apareceran los potenciadores y el nº de potenciadores a instanciar
					Potenciadores.CreatePotenciador(positionEnemy, 0, 30, numPotenciadores)
					
				elseif v.name == "Almas" then
					--averiguamos la cantidad de almas que sacamos
					-- local cantidadAlmas = math.random(tablaCofre.rangoAlmas.min, tablaCofre.rangoAlmas.max)
					local cantidadAlmas = RandomRange(tablaContenedor.rangoAlmas.min, tablaContenedor.rangoAlmas.max)
					-- Log.Debug("Drop.CofreAbierto (almas) RND: "..cantidadAlmas)
					-- Log.Debug("Saco "..cantidadAlmas.." almas del cofre")

					Almas.CreateCantidadAlmas( positionEnemy, 0, 30, cantidadAlmas, 1) --por ahora le indicamos la granularidad a machete
				
				else
					local entityType = Drop.DropObjectByType(v.name, positionEnemy)
				end

				break
			end
		end
	end
end


