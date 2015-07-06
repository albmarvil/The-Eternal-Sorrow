--Esta tabla almacena un par "NombreComponente"  -  "NombreHabilidad" necesario a la hora de enviar el mensaje
Nombre_Habilidad = {
	CAumentoUnSalto = "AumentarSalto",
	CVerVidaEnemigos = "BarraVidaEnemigo",
	CRegeneracionVida = "RegeneracionVida",
	CAumentoVidaMaxima = "AumentoVidaMaxima",
	CPorcentajeAlmas = "AumentoPorcentajeAlmas",
	CDamageMeleeEnemy = "DamageEnemigoMelee",
	CArrowEnemy = "ArrowEnemy",
	CReduceTimeChest = "ReduceTimeChest",
	CAumentarTiempoReto = "AumentarTiempoReto",
}

Data_Player = {
	habilidades = {}, --habilidades que obtiene el jugador, será una tabla que almacene una tabla de habilidad con un contador
	
	--estadísticas del personaje
	player_stats = {
		vida = 0,
		ataque = 0,
		velocidad_ataque = 0,
		velocidad_ataque_normal = 0,
		velocidad_ataque_especial = 0,
		velocidad_movimiento = 0,
	},
	
	--información del personaje
	player_info = {
		vidaActual = 0,
		money = 0,
		max_velocidad_ataque = 0,
		max_velocidad_ataque_especial = 0,
		max_velocidad_ataque_normal = 0,
		-- multiplicador_velocidad_ataque_derecha = 0,
		-- multiplicador_velocidad_ataque_izquierda = 0,
		type_arma_derecha = "",
		type_arma_izquierda = "",
		usable_type_arma_derecha = "",
		usable_type_arma_izquierda = "",
		distanceToAument = 0,
		minLifeBarDistance = 0,
	},
	
	changeStatR = false, --booleano que se activa al recoger un potenciador, lo emplea el arma derecha en el HUD
	changeStatL = false, --booleano que se activa al recoger un potenciador, lo emplea el arma izquierda en el HUD
}

-----------------------------METODOS------------------------------
--Esta función añade una habilidad a la tabla de habilidades si no existe o suma uno al contador si ya la ha obtenido previamente
function AddHability(name_hability)
	if Data_Player.habilidades[Nombre_Habilidad[name_hability]] == nil then
		Data_Player.habilidades[Nombre_Habilidad[name_hability]] = {contador = 1,} --la habilidad existe, suma uno a su contador
	else
		--la habilidad no existe crea la tabla
		Data_Player.habilidades[Nombre_Habilidad[name_hability]].contador = Data_Player.habilidades[Nombre_Habilidad[name_hability]].contador + 1
	end
end

function HadHability(hability)
	if Data_Player.habilidades[hability] == nil then
		return 0
	else
		return Data_Player.habilidades[hability].contador
	end
end

function GetDistanceView()
	return Data_Player.player_info.distanceToAument
end

function GetMinDistanceView()
	-- body
	return Data_Player.player_info.minLifeBarDistance
end


--Devuelve el arma que tiene el player en la mano que se le pasa por argumento
function GetUsableWeapon(mano)
	if mano == "LeftArm" then
		return Data_Player.player_info.usable_type_arma_izquierda
	else
		return Data_Player.player_info.usable_type_arma_derecha
	end
end


function SetWeapon(mano, type_arma, usable_type_arma, multi, type_attack)
	if mano == "Left" then
		Data_Player.player_info.type_arma_izquierda = type_arma
		Data_Player.player_info.usable_type_arma_izquierda = usable_type_arma
		Data_Player.player_info.multiplicador_velocidad_ataque_izquierda = multi
	else
		Data_Player.player_info.type_arma_derecha = type_arma
		Data_Player.player_info.usable_type_arma_derecha = usable_type_arma
		Data_Player.player_info.multiplicador_velocidad_ataque_derecha = multi
	end
end

function GetWeapon(mano)
	if mano == "LeftArm" then
		return Data_Player.player_info.type_arma_izquierda
	else
		return Data_Player.player_info.type_arma_derecha
	end
end


--------------------------------------------------------------------------------------------------------------
-------------------------------------FUNCIONES DE Data_Player-------------------------------------------------
--------------------------------------------------------------------------------------------------------------
function Data_Player.DeletePjValues()
	--Borramos los datos de las estadísticas del personaje
	Data_Player.player_stats.vida = 0
	Data_Player.player_stats.ataque = 0
	Data_Player.player_stats.velocidad_ataque = 0
	Data_Player.player_stats.velocidad_ataque_normal = 0
	Data_Player.player_stats.velocidad_ataque_especial = 0
	Data_Player.player_stats.velocidad_movimiento = 0
	
	--Borramos los datos de la información referente al player
	Data_Player.player_info.money = 0
	Data_Player.player_info.multiplicador_velocidad_ataque_derecha = 0
	Data_Player.player_info.multiplicador_velocidad_ataque_izquierda = 0
	Data_Player.player_info.type_arma_derecha = ""
	Data_Player.player_info.type_arma_izquierda = ""
	Data_Player.player_info.usable_type_arma_derecha = ""
	Data_Player.player_info.usable_type_arma_izquierda = ""
	
	Data_Player.habilidades = {}
end

function Data_Player.InitPjValues()
	--Inicializamos las estadísticas del personaje
	Data_Player.player_stats.vida = 100
	Data_Player.player_stats.ataque = 10
	Data_Player.player_stats.velocidad_ataque = 20
	Data_Player.player_stats.velocidad_ataque_especial = Data_Player.player_stats.velocidad_ataque * 50 * 2 --milisegundos --el calculo sale 2000ms
	Data_Player.player_stats.velocidad_ataque_normal = Data_Player.player_stats.velocidad_ataque * 50 --milisegundos  --el calculo sale 1000ms
	Data_Player.player_stats.velocidad_movimiento = 200
	
	--Inicializamos la información referente al player
	Data_Player.player_info.money = 500
	Data_Player.player_info["vidaActual"] = Data_Player.player_stats.vida
	-- Data_Player.player_info.multiplicador_velocidad_ataque_derecha = 0
	-- Data_Player.player_info.multiplicador_velocidad_ataque_izquierda = 0
	Data_Player.player_info.max_velocidad_ataque = Data_Player.player_stats.velocidad_ataque
	Data_Player.player_info.max_velocidad_ataque_especial = Data_Player.player_stats.velocidad_ataque_especial
	Data_Player.player_info.max_velocidad_ataque_normal = Data_Player.player_stats.velocidad_ataque_normal
	Data_Player.player_info.minLifeBarDistance = 50
	Data_Player.player_info.distanceToAument = 15--GameMgr.player:getEntityInfo():getFloatAttribute("distanceToAument")

	Data_Player.habilidades = {}
end

function Data_Player.UpdateCValues()
	--Inicializamos las estadisticas de C++
	for key, value in pairs(Data_Player.player_stats) do
		if key ~= "velocidad_ataque_normal" and key ~= "velocidad_ataque_especial" then 
			Data_Player.ChangeCValues(key, value)
		end

		
		if key == "vida" then
			--actualizamos el hud de vida para mostrar los nuevos valores de la vida total y de la vida restante
			LifeHUD.setLife(Data_Player.player_stats[key])
			LifeHUD.setLife(Data_Player.player_info["vidaActual"])
		end
	end
end

function Data_Player.ChangeCValues(key, value)
	--cambiamos las estadísticas de C++ para que al atacar se haga todo correctamente
	if value ~= 0 then 
		if key == "velocidad_ataque" then
			-- Log.Debug("Normal: "..Data_Player.player_stats["velocidad_ataque_normal"])
			-- Log.Debug("Special: "..Data_Player.player_stats["velocidad_ataque_especial"])
			SendStatsMenssage("velocidad_ataque_normal", (Data_Player.player_stats["velocidad_ataque_normal"]), GameMgr.player)
			SendStatsMenssage("velocidad_ataque_especial", (Data_Player.player_stats["velocidad_ataque_especial"]), GameMgr.player)
		else
			if key == "velocidad_movimiento" then
				SendStatsMenssage(key, (0.001 * Data_Player.player_stats[key]), GameMgr.player)
			else
				if key == "vida" then
					SendStatsMenssage("vidaActual", Data_Player.player_info["vidaActual"], GameMgr.player)
				else
					SendStatsMenssage(key, Data_Player.player_stats[key], GameMgr.player)
				end
			end
		end
	end
end

function Data_Player.ChangeStatValue(key, value)
	-- Log.Debug("Cambio stat: "..key.." "..value)

	-- Log.Debug(key.." valor estat "..Data_Player.player_stats[key])
	if key == "velocidad_ataque" then
		--Si la stat es la velocidad de ataque modifica los valores de las velocidades normal y especial, son los valores base
		Data_Player.player_stats["velocidad_ataque_especial"] = Data_Player.player_stats["velocidad_ataque_especial"] - value * 20 --Cada pto equivale a 10ms
		Data_Player.player_stats["velocidad_ataque_normal"] = Data_Player.player_stats["velocidad_ataque_normal"] - value * 20
		
		--Modifica el valor de la velocidad de ataque que verá el player, que es el total de potenciadores recogidos
		Data_Player.player_stats[key] = Data_Player.player_stats[key] + value

		--limitamos la velocidad de ataque a un mínimo de 1
		if(Data_Player.player_stats[key] < 1) then
			Data_Player.player_stats[key] = 1
		end
		
		--Activa el changeStat para que el HUD del arma modifique su velocidad de carga
		Data_Player.changeStatR = true
		Data_Player.changeStatL = true
	else
		--En cualquier otro caso simplemente modifica el valor
		Data_Player.player_stats[key] = Data_Player.player_stats[key] + value
	end
	-- Log.Debug(key.." valor estat "..Data_Player.player_stats[key])
end

function Data_Player.CheckStatValues(key, value)
	--capamos los máximos y mínimos de las velocidades de movimiento y ataque
	if key == "velocidad_movimiento" then
		if Data_Player.player_stats[key] < 10 then
			Data_Player.player_stats[key] = 10
		elseif Data_Player.player_stats[key] > 1000 then
			Data_Player.player_stats[key] = 1000
		end
	end
	
	--Cogemos el máximo valor que pueden tomar las distintas velocidades de ataque (normal y especial)
	
	local max_velocidad_normal = Data_Player.player_info["max_velocidad_ataque_normal"] + Data_Player.player_info["max_velocidad_ataque"] * 20 
	-- print("max_velAttkNormal: "..max_velocidad_normal.." / "..Data_Player.player_info["max_velocidad_ataque_normal"])
	local max_velocidad_especial = Data_Player.player_info["max_velocidad_ataque_especial"] + Data_Player.player_info["max_velocidad_ataque"] * 20
	-- print("max_velAttkSpecial: "..max_velocidad_especial.." / "..Data_Player.player_info["max_velocidad_ataque_especial"])
	
	if key == "velocidad_ataque" then
		if Data_Player.player_stats["velocidad_ataque_normal"] < 0.5 * Data_Player.player_info["max_velocidad_ataque_normal"] then
			Data_Player.player_stats["velocidad_ataque_normal"] = 0.5 * Data_Player.player_info["max_velocidad_ataque_normal"]
		elseif Data_Player.player_stats["velocidad_ataque_normal"] > max_velocidad_normal then
			Data_Player.player_stats["velocidad_ataque_normal"] = max_velocidad_normal
		end
		
		if Data_Player.player_stats["velocidad_ataque_especial"] < 0.5 * Data_Player.player_info["max_velocidad_ataque_especial"] then
			Data_Player.player_stats["velocidad_ataque_especial"] = 0.5 * Data_Player.player_info["max_velocidad_ataque_especial"]
		elseif Data_Player.player_stats["velocidad_ataque_especial"] > max_velocidad_especial then
			Data_Player.player_stats["velocidad_ataque_especial"] = max_velocidad_especial
		end
	end
	
	
	--si es la vida no dejamos que baje menos de 5000
	if key == "vida" then
		if Data_Player.player_stats[key] < 50 then
			Data_Player.player_stats[key] = 50
		end
		
		--debemos comprobar que la total no es mayor que la actual
		--actualizamos si es necesario la vida actual
		if Data_Player.player_stats[key] < Data_Player.player_info["vidaActual"] then
			Data_Player.player_info["vidaActual"] = Data_Player.player_stats[key]
		end
		
		--actualizamos el hud de vida para mostrar los nuevos valores de la vida total y de la vida restante
		LifeHUD.setLife(Data_Player.player_stats[key])
		LifeHUD.setLife(Data_Player.player_info["vidaActual"])
	end
	
	--en caso del ataque debemos comprobar que no es negativo, puesto que en este caso en vez de restar vida al enemigo se la sumaria.
	--Además dejaremos como valor mínimo la mitad de la base, es decir, 5
	if key == "ataque" then
		if Data_Player.player_stats[key] < 5 then
			Data_Player.player_stats[key] = 5
		end
	end
end

--Actualiza las estadísticas del player cuando éste recoge un potenciador
function Data_Player.UpdateStats(tabla)
	for key, value in pairs(tabla) do
		Data_Player.ChangeStatValue(key, value)
		
		Data_Player.CheckStatValues(key, value)
		
		if key ~= "velocidad_ataque_normal" and key ~= "velocidad_ataque_especial" then 
			Data_Player.ChangeCValues(key, value)
		end
	end
end

function Data_Player.swapWeapon()
	-- Log.Debug("SWAP WEAPON")
	local tipoArmaIzq = Data_Player.player_info.type_arma_izquierda
	local tipoUsableArmaIzq = Data_Player.player_info.usable_type_arma_izquierda

	Data_Player.player_info.type_arma_izquierda = Data_Player.player_info.type_arma_derecha
	Data_Player.player_info.usable_type_arma_izquierda = Data_Player.player_info.usable_type_arma_derecha

	Data_Player.player_info.type_arma_derecha = tipoArmaIzq
	Data_Player.player_info.usable_type_arma_derecha = tipoUsableArmaIzq

	ArmasHUD.swapArms()
end

function Data_Player.getLifeMax()
	--Log.Debug("Vida: "..toString(Data_Player.player_stats.vida))

	return Data_Player.player_stats.vida
end

function Data_Player.GetWeapons()
	if Data_Player.player_info.type_arma_derecha ~= "" or Data_Player.player_info.type_arma_izquierda ~= "" then
		return 1
	end
	
	return 0
end