--author: Bárbara Domínguez de la Torre González
--date: Marzo 2015

GameMgr = {
	--------------------------------------------------------------------
	----------------VARIABLES PROPIAS DEL GAME MANAGER------------------
	--------------------------------------------------------------------
	
	--Semilla de aleatoriedad del juego
	seed = 0,
	--referencia al player
	player = nil,
	
	--referencia a la puerta del nivel
	exit_door = nil,
	
	--referencia al mapa
	map = nil,
	---matriz de colision del mapa
	-- Lamatriz esta preparada para acceder con coordenadas [x][y]
	mapCollisionMatrix = {},
	--salas totales completadas
	salas_totales = 0,
	--enemgios muertos en sala
	enemigos_muertos = 0,
	--tiempo en la sala actual
	tiempo_en_sala = 0,
	--Velocidad de muertes
	velocidad_muertes = 0,
	--nivel medio de los enemigos en sala
	nivel_enemigos = 1,
	
	--bool auxiliar para que el mensaje de la puerta se envia una vez.
	open_door = false,

	estadisticasPartida = 
	{

	},
}

--Aparte de estas esta el dinero, el numero de salas totales completadas, y el numero de enemigos matados
function GameMgr.initEstadisticasPartida()
	GameMgr.estadisticasPartida.numeroCofresAbiertos = 0
	GameMgr.estadisticasPartida.numeroPotenciadoresCogidos = 0
	GameMgr.estadisticasPartida.numeroHabilidadesCogidas = 0
	GameMgr.estadisticasPartida.dineroGastado = 0
	GameMgr.estadisticasPartida.dineroConseguido = Data_Player.player_info.money
	GameMgr.estadisticasPartida.puntuacionTotal = 0
	GameMgr.estadisticasPartida.enemigosMuertosTotales = 0
end

--------------------------------------------------------------------------
--------------------------METODOS GAME MANAGER----------------------------
--------------------------------------------------------------------------
function GameMgr.InitMgr()
	-- Log.Debug("INIT GAMEMGR")
	--iniciamos las variables necesarias del Game Manager
	GameMgr.salas_totales = 0
	GameMgr.enemigos_muertos = 0
	GameMgr.tiempo_en_sala = 0
	GameMgr.nivel_enemigos = 1
	
	--inicializamos las variables del Reto Manager
	RetoMgr.InitMgr()

	--inicializamos el SpawnerMgr
	SpawnerMgr.InitMgr()
	
	--inicializamos las stats del pj, para ello llamamos al Data_Player
	Data_Player.InitPjValues()

	---INICIALIZACION DE STATS DEL PLAYER

	--Inicialización de la semilla!
	GameMgr.seed = os.time()
	setRandomSeed(GameMgr.seed)
	Log.Debug("RANDOMSEED: "..GameMgr.seed)


	GameMgr.initEstadisticasPartida()

	--Cargamos el manager de objetos
	Drop.Init()
	-- scriptMgr:executeProcedureFromTable("Drop", "Init")
	Cordura.Init()
	-- scriptMgr::executeProcedureFromTable("Cordura", "Init")

	Ambientaciones.Init()

	SoundtrackMgr.InitMgr()
	-- Log.Debug("FIN INIT")
end

function GameMgr.LoadReto()
	if(GameMgr.salas_totales == -1) then
		RetoMgr.LoadTutorial()
	else
		RetoMgr.LoadReto()
	end
end

function GameMgr.SetTutorial()
	GameMgr.Release()
	GameMgr.InitMgr()
	-- Log.Debug("setTutorial")
	GameMgr.salas_totales = -1

end

------COMPLETAR!!!
function GameMgr.Release()


	seed = 0
	player = nil
	
	exit_door = nil
	
	map = nil
	mapCollisionMatrix = {}
	salas_totales = 0
	enemigos_muertos = 0
	tiempo_en_sala = 0
	velocidad_muertes = 0
	nivel_enemigos = 1
	
	open_door = false

	estadisticasPartida = {}
	---Liberamos la info de partida del player
	Data_Player.DeletePjValues()

	SpawnerMgr.Release()
	RetoMgr.Release()

	Ambientaciones.Release()

	FuncionesEventoAvalancha.Release()

	SoundtrackMgr.Release()

end

function GameMgr.InitValues()
	--Sacamos la referencia del player y del mapa
	GameMgr.map = logicserver:getMap() 
	GameMgr.player = logicserver:getPlayer()
	
	--si el reto cargado es el reto de altar de sacrificio creamos el altar en un punto
	if RetoMgr.retoLoaded == "RetoAltar.lua" then
		Altar.CreateObjects()
	end

	--Inicializamos los valores del reto
	RetoMgr.InitValues()

	--Actualizamos la matriz de colisiones del mapa
	GameMgr.UpdateCollisionMatrix()

	--inicializamos a cero los contadores de la sala
	GameMgr.enemigos_muertos = 0
	GameMgr.tiempo_en_sala = 0
	GameMgr.open_door = false

	LifeHUD.ActualizarDinero(Data_Player.player_info.money)
	
	--Inicializamos las estadisticas de C++
	for key, value in pairs(Data_Player.player_stats) do
		if key ~= "velocidad_ataque_normal" and key ~= "velocidad_ataque_especial" then 
			Data_Player.ChangeCValues(key, value)
		end
	end
	if GameMgr.salas_totales < 1 then ---En sala inicial o tutorial cargamos datos del player por primera vez
		if GameMgr.salas_totales == -1 then --Solo en el tutorial le forzamos la vida a 60

			GameMgr.updateLifePj(60)
		end
		Data_Player.UpdateCValues()
	else
		--En cualquier otra sala
		--Reiniciamos los parametros del SpawnerMgr
		SpawnerMgr.InitValues()
		--inicializamos los valores del evento de avalancha
		FuncionesEventoAvalancha.InitValues()
	end

	SoundtrackMgr.InitValues()

end



-----AAAAAAAAAAAAAAAA
-- function GameMgr.InitStatsPj()
-- 	--Inicializmaos los valores de los stats del personaje, para ello se llama a la función correspondiente de Data_Player
-- 	Data_Player.InitPjValues()
-- end


--Tick del Game Manager, simplemente acumula tiempo en la sala y le pasa el tick al reto (por ahora)
--En un futuro deberá hacer más calculos
function GameMgr.tick(msecs)
	--acumulo los segundos pasados en el contador de tiempo de la sala
	GameMgr.tiempo_en_sala = GameMgr.tiempo_en_sala + msecs

	--se llama al tick del reto
	RetoMgr.tick(msecs)

	HUD.tick(msecs)

	Cordura.tick(msecs)

	--se llama al tick del spawner manager, a partir de la segunda sala
	if GameMgr.salas_totales > 0 then
		SpawnerMgr.tick(msecs)
		FuncionesEventoAvalancha.tick(msecs)
		SoundtrackMgr.tick(msecs)
	end

end

function GameMgr.updateLifePj(life)
	Data_Player.player_info.vidaActual = life
	--actualizamos el HUD con la vida actual
	LifeHUD.setLife(Data_Player.player_info.vidaActual)
end

function GameMgr.EnemyDie(tier, level, id, pos)
	GameMgr.enemigos_muertos = GameMgr.enemigos_muertos + 1
	
	--si el reto es el del altar se llama a la función del reto que calcula si ha matado al enemigo en rango.
	if RetoMgr.retoLoaded == "RetoAltar.lua" then
		Reto.EnemyDie(pos)
	end
	
	-- sacamos de la lista de enemigos vivos a este
	SpawnerMgr.EnemyIsDead(id)

	--sumamos al tamaño libre de la mochila el del tier del enemigo
	RetoMgr.ActualizarTamLibreMochila(tier)
	local nivelAux = (GameMgr.nivel_enemigos + level) *0.5
	if nivelAux < GameMgr.nivel_enemigos then
		nivelAux = GameMgr.nivel_enemigos
	end

	GameMgr.nivel_enemigos = nivelAux

---DEBUG
	-- DebugHUD.updateNivel(math.floor(nivelAux))
	-- DebugHUD.updateMochila(RetoMgr.tam_total_mochila - RetoMgr.tam_libre_mochila, RetoMgr.tam_total_mochila)
	-- DebugHUD.updatePromedioMuertes(GameMgr.velocidad_muertes)

	--Actualizamos la puntuacion
	FuncionesSistemaPuntuacion.puntosTier(tier, level)

end

function GameMgr.FinSala( )

	if GameMgr.salas_totales <= 0 then 
		GUIGameDeactivate()
		GUIGameActivate()
	end
	-- Solo reducimos el tiempo cuando hemos pasado de una sala de batalla a otra,
	-- por lo que omitimos la primera sala del juego
	if GameMgr.salas_totales > 0 then
		SpawnerMgr.FinSala()
	end

	GameMgr.salas_totales = GameMgr.salas_totales + 1
	
	FuncionesSistemaPuntuacion.puntosSalaSuperada(GameMgr.salas_totales)

	FuncionesSistemaPuntuacion.puntosTiempoSala(GameMgr.tiempo_en_sala)

	---DEBUG
	-- DebugHUD.updateSala(GameMgr.salas_totales)

	FuncionesEventoAvalancha.Release()

	FuncionesTrophyManager.nivelNuevo()

	FuncionesTrophyManager.aumentarValor("SuperarSala")
	FuncionesTrophyManager.aumentarValor("SuperarSalasMismaPartida")

	if GameMgr.salas_totales > 5 then
		FuncionesTrampas.updateTrampasSala5()
	elseif GameMgr.salas_totales > 10 then
		FuncionesTrampas.updateTrampasSala10()
	elseif GameMgr.salas_totales > 15 then
		FuncionesTrampas.updateTrampasSala15()
	end
	
end

function GameMgr.swapWeapon()
	Data_Player.swapWeapon()
end

function GameMgr.UpdateCollisionMatrix()
	local width = GameMgr.map:getMapTileWidth()
	local height = GameMgr.map:getMapTileHeight()

	GameMgr.mapCollisionMatrix = {}

	for i=0, width-1 do
		local tabla = {}
		for j=0,height-1 do
			tabla[j] = GameMgr.map:getTileColisionTP(i,j)
		end
		GameMgr.mapCollisionMatrix[i] = tabla
	end

end


--Esta funcion nos da el ID del tile correspondiente a la posicion mundo indicada
-- ****OJO se va a usar el tamaño de tile fijo!!
function GameMgr.getTile(posx, posy)
	-- body
	-- print(posx, posy)
	local x = math.floor(posx / 20)
	local y = math.floor(-posy /20)
	-- print("====",x, y)

	if x >= table.getn(GameMgr.mapCollisionMatrix) or x < 0 then
		return -1
	elseif y >= table.getn(GameMgr.mapCollisionMatrix[0]) or y < 0 then
		return -1
	else
		return GameMgr.mapCollisionMatrix[x][y]
	end
end

function GameMgr.positionToTilePosition(posx, posy)
	local x = math.floor(posx / 20)
	local y = math.floor(-posy / 20)

	return x, y
end

function UpdateLifeDoor(life, damage)
	return RetoMgr.UpdateLifeDoor(life, damage)
end
