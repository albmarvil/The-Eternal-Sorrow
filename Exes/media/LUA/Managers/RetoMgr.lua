--author: Bárbara Domínguez de la Torre González
--date: Abril 2015


PathsRetos = Paths.luaManagersPath.."Retos/"

RetoMgr = {
	-------------------------------------------------------
	--------------VARIABLES DADAS POR EL RETO--------------
	-------------------------------------------------------
	
	--tamaño decimal de la mochila. El de abajo es el entero
	tam_decimal_mochila = 3,
	--tamaño total de la mochila
	tam_total_mochila = 3,
	--espacio en la mochila
	tam_libre_mochila = 0,
	--Contador de tiempo de la mochila
	mochila_time_count = 0,
	--variable para controlar si hay reto
	is_reto = false,
	retoLoaded = "",
	
	--tabla que almacena todos los retos posibles
	Retos = {
	--tendrá la forma
	--	RetoPuerta = { nombre = "nombre_archivo.lua", peso = valor, peso_total = valor_acumulado},
	},
}

----------------------------------------------------------------------------
----------------------------METODOS RETO MANAGER----------------------------
----------------------------------------------------------------------------

function RetoMgr.InitMgr()
	--inicializamos las variables necesarias del Reto Manager
	RetoMgr.mochila_time_count = 0
	RetoMgr.is_reto = false
	--scriptMgr:loadScript(PathsRetos.."RetoA.lua")
end

function RetoMgr.Release()
	tam_decimal_mochila = 1
	tam_total_mochila = 3
	tam_libre_mochila = 3
	mochila_time_count = 0
	is_reto = false
	retoLoaded = ""
	
end

function RetoMgr.LoadTutorial()

	Log.Debug("CARGANDO TUTORIAL ")

	RetoMgr.retoLoaded = "RetoTutorial.lua"

	scriptMgr:loadScript(PathsRetos..RetoMgr.retoLoaded)

	--activamos el HUD propio del reto
	Reto.activarHUD()

	RetoMgr.is_tutorial = true

end

function RetoMgr.LoadReto()

	--cogemos el reto
	archivo = RetoMgr.FindReto()
	RetoMgr.retoLoaded = archivo
	
	--de momento solo carga un reto xq no hay más
	scriptMgr:loadScript(PathsRetos..RetoMgr.retoLoaded)
	
	--activamos el HUD propio del reto
	Reto.activarHUD()
	
	--hay reto, asi que la variable is_reto es true
	RetoMgr.is_reto = true
	
	--Si es el primer reto que cargamos, el tamaño de la mochila debe ser incial
	-- if GameMgr.salas_totales == 1 then
	-- 	RetoMgr.tam_total_mochila = Reto.tam_inicial_mochila
	-- 	RetoMgr.tam_decimal_mochila = Reto.tam_inicial_mochila
	-- end
	RetoMgr.tam_libre_mochila = RetoMgr.tam_total_mochila
end

function RetoMgr.FindReto()
	--si es la primera sala sacamos el reto de la puerta

	-- Log.Debug("n salas: "..GameMgr.salas_totales)
	if GameMgr.salas_totales == 0 then
		
		return "RetoPuerta.lua"
	elseif GameMgr.salas_totales ~= -1 then
		--calculamos los pesos acumulados de los retos
		local peso_acumulado = 0

		for key,value in pairs(MapasPorReto[FuncionesMapList.currentMap]) do
			peso_acumulado = peso_acumulado + value.peso
			value.peso_acumulado = peso_acumulado
		end
		
		--sacamos un random de los pesos acumulados
		local rnd = Random(peso_acumulado)
		
		local peso = -1
		for key,value in pairs(MapasPorReto[FuncionesMapList.currentMap]) do
			if value.peso_acumulado > peso and rnd <= value.peso_acumulado then
				return value.nombre_archivo
			end
			
			peso = value.peso_acumulado
		end
	end
end

function RetoMgr.InitValues()
	RetoMgr.mochila_time_count = 0
end

function RetoMgr.tick(msecs)
	--se ejecutan las operaciones correspondientes al reto concreto, siempre y cuando haya un reto
	--si hay reto se ejecutan sus funciones
	if RetoMgr.is_reto then
		--llamamos al tick del reto que se ha cargado anteriormente
		Reto.tick(msecs)

		--Actualizacion de la velocidad de muertes, variable propia del GameMgr
		GameMgr.velocidad_muertes = GameMgr.enemigos_muertos / (GameMgr.tiempo_en_sala * 0.001) --en segundos

		--Actualización de la mochila
		RetoMgr.mochila_time_count = RetoMgr.mochila_time_count + msecs

		if RetoMgr.mochila_time_count >= Reto.refreshTime then
			RetoMgr.mochila_time_count = 0
			--Actualizamos el peso de la mochila
			RetoMgr.ActualizaTamMochila(Reto.parameters)
		end
	end

	if RetoMgr.is_tutorial then
		Reto.tick(msecs)
	end
end

function RetoMgr.ActualizaTamMochila(FuncParameters)
	local a, b, c = FuncParameters(GameMgr.salas_totales)
	local x = GameMgr.velocidad_muertes
	local res = c + b*x + a*x*x

	local incremento = math.floor(RetoMgr.tam_decimal_mochila)
	RetoMgr.tam_decimal_mochila = RetoMgr.tam_decimal_mochila + res

	--sacamos el incremento entero, sin la parte decimal
	incremento = math.floor(RetoMgr.tam_decimal_mochila) - incremento



	RetoMgr.tam_total_mochila = RetoMgr.tam_total_mochila + incremento
	RetoMgr.tam_libre_mochila = RetoMgr.tam_libre_mochila + incremento

	---DEBUG
	-- DebugHUD.updateMochila(RetoMgr.tam_total_mochila - RetoMgr.tam_libre_mochila, RetoMgr.tam_total_mochila)
	-- DebugHUD.updateAuxNumber(RetoMgr.tam_decimal_mochila)
end

function RetoMgr.ActualizarTamLibreMochila(value)
	RetoMgr.tam_libre_mochila = RetoMgr.tam_libre_mochila + value
	
	--Si por alguna razón el tamaño libre de la mochila es mayor que el total lo modificamos
	if RetoMgr.tam_libre_mochila > RetoMgr.tam_total_mochila then
		RetoMgr.tam_libre_mochila = RetoMgr.tam_total_mochila
	end

end

function RetoMgr.UpdateLifeDoor(life, damage)
	-- if RetoMgr.retoLoaded == "RetoPuerta.lua" or RetoMgr.retoLoaded == "RetoTutorial.lua" then
	if type(Reto.UpdateLife) == "function" then
		Reto.UpdateLife(life, damage)
		return true
	end
	return false
end




------------------------------------------------------------------------------------------
--------------------AÑADIMOS TODOS LOS ARCHIVOS DE RETOS A LA TABLA-----------------------
------------------------------------------------------------------------------------------
-- RetoMatarEnemigos = {nombre_archivo = "RetoA.lua", peso = 20, peso_total = 0}
-- table.insert(RetoMgr.Retos, RetoMatarEnemigos)

-- RetoPuerta = {nombre_archivo = "RetoPuerta.lua", peso = 80, peso_total = 0}
-- table.insert(RetoMgr.Retos, RetoPuerta)