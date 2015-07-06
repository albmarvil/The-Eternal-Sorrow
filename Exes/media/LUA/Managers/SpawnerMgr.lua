--author: Bárbara Domínguez de la Torre González
--date: Marzo 2015

SpawnerMgr = {

	---------------------VARIABLES------------------------
	tiempoInvocacionGlobal = 2500,
	tiempoInvocacionEnSala = 0,
	tiempoParaReducir = 3000,
	tInvocacion = 0,
	tReductor = 0,
	paramReductor = 0.05,
	puntos_spawn = {},
	total_puntos = 1,
	puntos_cercanos = {},
	enemVivos = {},
	total_puntos_cercanos = 0,
	puntoInvocacionActual = 1,

	inicioSala = false, --flag que nos indica que estamos en el inicio de la sala

	tiers = {
		-- Ejemplo de como se construye esta tabla al vuelo al cargar los diferentes enemigos:
		
		-- Tier1 = {
		--     tier  = 1,
		-- 	   Enemigos = {
		-- 		   Dragonite, -->Es el puntero a la tabla del dragonite definida en Dragonite.lua
		-- 		   Mewtwo,
		--     },
		-- },
		-- ....
	},

	minions = {
		
	},

	--Esta tabla tiene los pesos acumulados de los tiers en orden T1, T2, T3, T4 etc
	pesos_tiers = {},
	peso_total_tiers = 0,
	
	
	------------------METODOS------------------------
	InitMgr = function()
		--método de inicialización del Manager
		SpawnerMgr.tiempoInvocacionEnSala = SpawnerMgr.tiempoInvocacionGlobal
	end,

	Release =  function ()
		SpawnerMgr.puntos_spawn = {}
		SpawnerMgr.total_puntos = 1
		SpawnerMgr.puntos_cercanos = {}
		SpawnerMgr.total_puntos_cercanos = 0
		SpawnerMgr.puntoInvocacionActual = 1

		SpawnerMgr.tiempoInvocacionGlobal = 2500
		SpawnerMgr.tiempoInvocacionEnSala = 0
		SpawnerMgr.tiempoParaReducir = 3000

		--borramos los enemigos vivos que quedaran
		SpawnerMgr.enemVivos = {}
	end,
	
	--Metodo llamado cada vez que entramos en una nueva sala
	InitValues = function()
		--ponemos a 0 los puntos de spawn del spawnerMgr
		SpawnerMgr.tInvocacion = SpawnerMgr.tiempoInvocacionGlobal
		SpawnerMgr.tReductor = 0

		--Calculamos los pesos de los tiers
		SpawnerMgr.ActualizarPesosTier(Reto.SpawnParams)

	end,

	FinSala = function ()
		---X ms por sala
		SpawnerMgr.tiempoInvocacionGlobal = SpawnerMgr.tiempoInvocacionGlobal - GameMgr.salas_totales * 50

		-- establecemos un mínimo para evitar que se invoquen cada 0 segundos
		if SpawnerMgr.tiempoInvocacionGlobal < 1000 then
			SpawnerMgr.tiempoInvocacionGlobal = 1000
		end

		SpawnerMgr.tiempoInvocacionEnSala = SpawnerMgr.tiempoInvocacionGlobal


		SpawnerMgr.puntos_spawn = {}
		SpawnerMgr.total_puntos = 1
		puntos_cercanos = {}
		total_puntos_cercanos = 0
		puntoInvocacionActual = 1

		--borramos los enemigos vivos que quedaran
		enemVivos = {}
	end,
	
	tick = function(msecs)

		--contamos cuantos segundos han pasado desde la última vez
		SpawnerMgr.tInvocacion = SpawnerMgr.tInvocacion + msecs
		SpawnerMgr.tReductor = SpawnerMgr.tReductor + msecs

		---DEBUG
		-- DebugHUD.updateTimepoSpawner(SpawnerMgr.tInvocacion, SpawnerMgr.tiempoInvocacionEnSala)

		-- cada cierto tiempo reducimos el delay necesario para invocar enemigos (válido para cada sala individualmente)
		if SpawnerMgr.tReductor >= SpawnerMgr.tiempoParaReducir then
			SpawnerMgr.tiempoInvocacionEnSala = SpawnerMgr.tiempoInvocacionEnSala - SpawnerMgr.tiempoInvocacionEnSala * SpawnerMgr.paramReductor
			
			-- evitamos que se invoquen cada tick poniendo un mínimo
			if SpawnerMgr.tiempoInvocacionEnSala < 100 then
				SpawnerMgr.tiempoInvocacionEnSala = 100
			end
			
			SpawnerMgr.tReductor = 0
			 
		end
		
		-- Si ha pasado suficiente tiempo y tenemos puntos de spawn, invocamos enemigos
		if SpawnerMgr.tInvocacion >= SpawnerMgr.tiempoInvocacionEnSala and table.getn(SpawnerMgr.puntos_spawn) > 0 then
			
			---Comprobamos si hay hueco en la mochila y mientras haya hueco libre sacamos enemigos
			-- while RetoMgr.tam_libre_mochila > 0 do --con este while rellenamos la mochila cada vez que se cumpla el tiempo, sin él solo sale un bicho
			if RetoMgr.tam_libre_mochila > 0 then
				local rnd = Random(SpawnerMgr.peso_total_tiers)
				local tier = 0

				for i=1,table.getn(SpawnerMgr.pesos_tiers) do
					if rnd <= SpawnerMgr.pesos_tiers[i] then
						tier = i
						break
					end
				end

				local tablaTier = SpawnerMgr.tiers["Tier"..tier]

				---------------------------------No comprobamos que el tier elegido entre en la mochila, si queda 1 hueco libre puede entrar un enemigo de Tier5

				-- Hacemos spawn del enemigo
				SpawnerMgr.SpawnEnemy(tablaTier)


			end  
			SpawnerMgr.tInvocacion = 0
		end
			
	end,
	
	ActualizarPesosTier = function(Param)
		--Obtenemos el peso de cada tier
		local T1, T2, T3, T4, T5 = Param(GameMgr.salas_totales)
		
		local pesoAcum = 0

		if SpawnerMgr.tiers["Tier1"] ~= nil then
			pesoAcum = pesoAcum + T1
			SpawnerMgr.pesos_tiers[1] = pesoAcum
		else
			SpawnerMgr.pesos_tiers[1] = 0
		end

		if SpawnerMgr.tiers["Tier2"] ~= nil then
			pesoAcum = pesoAcum + T2
			SpawnerMgr.pesos_tiers[2] = pesoAcum
		else
			SpawnerMgr.pesos_tiers[2] = 0
		end

		if SpawnerMgr.tiers["Tier3"] ~= nil then
			pesoAcum = pesoAcum + T3
			SpawnerMgr.pesos_tiers[3] = pesoAcum
		else
			SpawnerMgr.pesos_tiers[3] = 0
		end

		if SpawnerMgr.tiers["Tier4"] ~= nil then
			pesoAcum = pesoAcum + T4
			SpawnerMgr.pesos_tiers[4] = pesoAcum
		else
			SpawnerMgr.pesos_tiers[4] = 0
		end

		if SpawnerMgr.tiers["Tier5"] ~= nil then
			pesoAcum = pesoAcum + T5
			SpawnerMgr.pesos_tiers[5] = pesoAcum
		else
			SpawnerMgr.pesos_tiers[5] = 0
		end

		SpawnerMgr.peso_total_tiers = pesoAcum
	end,
	
	--Esta funcion hace spawn de un enemigo del tier adecuado
	SpawnEnemy = function (tier)
		--una vez sabemos el tier lanzamos un random sobre la tabla del tier correspondiente
		local enemyIdx = RandomElement(tier.Enemigos)
		local typeEnemy = tier.Enemigos[enemyIdx]	
	
		--sacamos el nivel que tendrán los enemigos	
		local level = SpawnerMgr.CalcularLevel()
		
		--Restamos al espacio de la mochila el tier
		RetoMgr.tam_libre_mochila = RetoMgr.tam_libre_mochila - typeEnemy.tier

		---DEBUG
		-- DebugHUD.updateMochila(RetoMgr.tam_total_mochila - RetoMgr.tam_libre_mochila, RetoMgr.tam_total_mochila)
				
		--cogemos los tres puntos más cercanos al jugador
		SpawnerMgr.SetPoints()

		--sacamos de forma aleatoria el punto de spawn del que sale el enemigo
		if SpawnerMgr.total_puntos_cercanos > 0 then
			-- Log.Debug("Invocado en Punto " .. SpawnerMgr.puntoInvocacionActual)

			local position = SpawnerMgr.puntos_cercanos[SpawnerMgr.puntoInvocacionActual]	
			
			SpawnerMgr.puntoInvocacionActual = SpawnerMgr.puntoInvocacionActual + 1

			if SpawnerMgr.puntoInvocacionActual > SpawnerMgr.total_puntos_cercanos then
				SpawnerMgr.puntoInvocacionActual = 1
			end

			--creamos un entityInfo con los nuevos datos
			-- Log.Debug("Enemigo nuevo: "..typeEnemy.Archetype)
			local entity = MapEntity(typeEnemy.Archetype..typeEnemy.GetNextID()) --esta creando un entity info

			entity:SetType(typeEnemy.Archetype)
					
			--asignamos los atributos de las stats (vida, ataque, velocidad de movimiento, velocidad de ataque) el tier y el nivel al entity Info
			for key,value in pairs(typeEnemy) do
				if type(value) ~= "function" then
					entity:SetAttrib(key, toString(value))
				elseif type(value) == "function" and key ~= "GetNextID" then
					entity:SetAttrib(key, toString(value(level)))
				end
			end
					
			entity:SetAttrib("level", toString(level))
			entity:SetAttrib("position", toString({position.x, position.y, position.z}))
					
			--Creamos al enemigo
			local logicEntity = NewEntity(entity, GameMgr.map)

			-- Aumentamos la tabla de enemigos vivos
			SpawnerMgr.enemVivos[logicEntity:getID()] = logicEntity

			------------------------Anhadimos un enemigo a que sea tenido en cuenta para la habilidad de la flecha

			addEnemyToArrow(logicEntity:getID())

			------ Creamos la partícula de invocacion de enemigo
			entityfactory:createEntityByType("SpawnEnem", position, GameMgr.map)
		end
	end,

	CalcularLevel = function()
		local bajo, medio, alto
		local lvlDiscretizado = math.floor(GameMgr.nivel_enemigos) --Hay que discretizar a la baja ya que si no se producen situaciones en las que el nivel medio no crece

		bajo = lvlDiscretizado
		medio = lvlDiscretizado + 1
		alto = lvlDiscretizado + 2
		
		local rndlevel = Random(100)
		local level = nil

		if 0 <= rndlevel and rndlevel <= 90 then
			level = bajo
		elseif 91 <= rndlevel and rndlevel <= 98 then
			level = medio
		elseif 98 <= rndlevel and rndlevel <= 100 then
			level = alto
		end

		return level
	end,
	
	SetPoints = function()
		SpawnerMgr.total_puntos_cercanos = 0

		local distanciasAux = {}
		local totalDistanciasAux = 0

		--Primero guardamos en la tabla distanciasAux todas las distancias.
		for i = 1, SpawnerMgr.total_puntos-1, 1 do
		
			-- Guardamos los primeros 3 puntos como más cercanos
			local x = SpawnerMgr.puntos_spawn[i].x - GameMgr.player:getPosition().x
			local y = SpawnerMgr.puntos_spawn[i].y - GameMgr.player:getPosition().y
			local z = SpawnerMgr.puntos_spawn[i].z - GameMgr.player:getPosition().z
			
			distanciasAux[i] = math.pow(x, 2) + math.pow(y, 2) + math.pow(z, 2)

			totalDistanciasAux = totalDistanciasAux + 1

			-- Si hay más, recorremos los siguientes y comprobamos si alguno es más cercano que los ya guardados
		end

		--Queremos almacenar los tres mas cercanos
		for x = 1,3,1 do 

			local menorDistancia = 0
			local posMenorDistancia = 0

			--Recorremos el vector de las distancias y obtenemos las mas cercana en este momento
			for i = 1, totalDistanciasAux, 1 do 
				if menorDistancia > distanciasAux[i] or menorDistancia == 0 then
					menorDistancia = distanciasAux[i]
					posMenorDistancia = i
				end
			end

			--Almacenamos en los putnos mas cercanos, el punto correspondiente a esa distancia
			SpawnerMgr.total_puntos_cercanos = SpawnerMgr.total_puntos_cercanos + 1
			SpawnerMgr.puntos_cercanos[x] = SpawnerMgr.puntos_spawn[posMenorDistancia]	

			--Elimino esa distancia del vector a seguir comprobando
			table.remove(distanciasAux, posMenorDistancia)
			totalDistanciasAux = totalDistanciasAux -1
		end
	end,
}

function SpawnerMgr.GetNearestEnemyID(posOri)
	local distMinim = math.huge
	local idFinal = -1

	-- Buscamos el enemigo mÃ¡s cercano siempre que no sea uno mismo y devolvemos su id
	for id, ent in pairs(SpawnerMgr.enemVivos) do

		if posOri.x ~= ent:getPosition().x or posOri.y ~= ent:getPosition().y then
			
			local distX = ent:getPosition().x - posOri.x
			local distY = ent:getPosition().y - posOri.y
			local dist = distX*distX + distY*distY

			if dist < distMinim then
				distMinim = dist
				idFinal = id
			end
		end
	end

	return idFinal
end

function SpawnerMgr.EnemyIsDead(id)
	if SpawnerMgr.enemVivos[id] ~= nil then
		SpawnerMgr.enemVivos[id] = nil

		removeEnemyToArrow(id)

		GameMgr.estadisticasPartida.enemigosMuertosTotales = GameMgr.estadisticasPartida.enemigosMuertosTotales +1

		FuncionesTrophyManager.aumentarValor("MatarEnemigosMismoNivel")
		FuncionesTrophyManager.aumentarValor("MatarEnemigosTotales")
		FuncionesTrophyManager.aumentarValor("MatarEnemigosMismaPartida")

		if EventoAvalancha.iniciado == true then
			FuncionesTrophyManager.aumentarValor("MatarEnemigosMismaPartidaAvalancha")
			FuncionesTrophyManager.aumentarValor("MatarEnemigosAvalanchaTotal")
		end
	end

	---------------------------------
end

function SpawnerMgr.SummonMinion(leaderName, typeMinion, levelMinion, tierMinion, posMinion)

	-- coger con el tier y type (en la tabla de arriba) el puntero al minion
	local punteroMinion = nil

	-- Log.Debug("Tier" .. tierMinion .. ".Enemigos:")

	for key,value in pairs(SpawnerMgr.minions["Tier" .. tierMinion].Enemigos) do

		-- Log.Debug("Archetype: " .. value.Archetype .. ", type: " .. typeMinion)

		if value.Archetype == typeMinion then
			punteroMinion = value
			break
		end
	end

	local entity = MapEntity(typeMinion..punteroMinion.GetNextID()) --esta creando un entity info

	entity:SetType(typeMinion)
	
	--asignamos los atributos de las stats (vida, ataque, velocidad de movimiento, velocidad de ataque) el tier y el nivel al entity Info
	for key,value in pairs(punteroMinion) do
		if type(value) ~= "function" then
			entity:SetAttrib(key, toString(value))
		elseif type(value) == "function" and key ~= "GetNextID" then
			entity:SetAttrib(key, toString(value(levelMinion)))
		end
	end
			
	entity:SetAttrib("level", toString(levelMinion))
	entity:SetAttrib("position", toString({posMinion.x, posMinion.y, posMinion.z}))
			
	-- Creamos al enemigo
	local entityCreated = NewEntity(entity, GameMgr.map)
	-- Se lo enviamos de vuelta a quien lo solicitó
	SendMinionCreated(entityCreated, leaderName)
end

---------------------------CARGA DE DATOS-------------------------------------
--cargamos las tablas de los enemigos al abrir el spawner manager
scriptMgr:loadScript(Paths.luaEnemyPath.."Wompy.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."SkullShoot.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."SpiderDodge.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."Chulu.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."SpiderKami.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."SkullSummon.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."MinionSummonUniDir.lua")

scriptMgr:loadScript(Paths.luaEnemyPath.."WompyElite.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."SkullShootElite.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."SpiderDodgeElite.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."ChuluElite.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."SpiderKamiElite.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."SkullSummonElite.lua")
scriptMgr:loadScript(Paths.luaEnemyPath.."MinionSummonUniDirElite.lua")

------------- DE PRUEBA -----------------
--scriptMgr:loadScript(Paths.luaEnemyPath.."Dragonite.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."Mewtwo.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemAppr.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemApprDodge.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemApprMDSummon.lua")--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemPongShootPl.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemApprMDShootRay.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemSeekRayShootRay.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemApprWander.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemShootUniDir.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemApprKami.lua")
--scriptMgr:loadScript(Paths.luaEnemyPath.."EnemTeleport.lua")
