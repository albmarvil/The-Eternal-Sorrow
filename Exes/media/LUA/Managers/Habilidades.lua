--@author Alberto Martínez Villarán
--@date Marzo, 2015

--Tabla donde estan registradas todas las habilidades del juego con un peso incorporado. Las habilidades están divididas en 3 niveles de poder o Tiers diferentes

Habilidades = {
	
	Tier1 = {

		--Ejemplo

		--{
		--  name = Nombre de la habilidad
		-- 	peso = Peso para tiradas de probabilidad. Solo s etienen en cuenta para el Tier en el que esta la habilidad
		-- 	type = tipo del arquetipo del objeto recogible

		--},

		{
			name = "AumentaSalto",
			peso = 20, 
			type = "UsableObjectSalto",

		},

		{
			name = "BarraVidaEnemigo",
			peso = 20, 
			type = "UsableObjectBarraVidaEnemigo",

		},

		{
			name = "ArrowEnemy",
			peso = 20, 
			type = "UsableObjectArrowEnemy",

		},

		{
			name = "ReduceTimeChest",
			peso = 20, 
			type = "UsableObjectReduceTimeChest",

		},

		{
			name = "AumentarTiempoReto",
			peso = 20, 
			type = "UsableObjectAumentarTiempoReto",

		},

	},

	Tier2 = {

		{
			name = "DamageEnemigoMelee",
			peso = 20, 
			type = "UsableObjectDamageMeleeEnemy",

		},

		{
			name = "RegeneracionVida",
			peso = 30, 
			type = "UsableObjectRegeneracion",

		},
	
	},

	Tier3 = {

		{
			name = "AumentoVidaMaxima",
			peso = 20, 
			type = "UsableObjectAumentoVidaMaxima",

		},

		{
			name = "AumentoPorcentajeAlmas",
			peso = 20, 
			type = "UsableObjectAumentoPorcentajeAlmas",

		},

	},


	--Esta función inicializa los vectores de acumulación de pesos para dejarlos listos para usarse

	Init = function ( )
		-- body
		local totalWeight = 0
		for k,v in pairs(Habilidades.Tier1) do
			if type(v) == "table" then
				totalWeight = totalWeight + v.peso
				v.pesoAcum = totalWeight
				-- print(v.peso, v.type, v.pesoAcum)
			end
		end
		Habilidades.Tier1.totalWeight = totalWeight
		-- print "TIER1"
		-- print ("Peso Total: "..Habilidades.Tier1.totalWeight)

		totalWeight = 0
		for k,v in pairs(Habilidades.Tier2) do
			if type(v) == "table" then
				totalWeight = totalWeight + v.peso
				v.pesoAcum = totalWeight
				-- print(v.peso, v.type, v.pesoAcum)
			end
		end
		Habilidades.Tier2.totalWeight = totalWeight
		-- print "TIER2"
		-- print ("Peso Total: "..Habilidades.Tier2.totalWeight)

		totalWeight = 0
		for k,v in pairs(Habilidades.Tier3) do
			if type(v) == "table" then
				totalWeight = totalWeight + v.peso
				v.pesoAcum = totalWeight
				-- print(v.peso, v.type, v.pesoAcum)
			end
		end
		Habilidades.Tier3.totalWeight = totalWeight
		-- print "TIER3"
		-- print ("Peso Total: "..Habilidades.Tier3.totalWeight)


	end,

	HabilidadID = 0,

	GetNextID = function (  )
		local id = Habilidades.HabilidadID + 1
		Habilidades.HabilidadID = id
		return id
	end,

	CreateHabilidad = function (tier, posicion)
		-- body
		local tablaTier = nil 

		if tier == 1 then
			tablaTier = Habilidades.Tier1
		elseif tier == 2 then
			tablaTier = Habilidades.Tier2
		elseif tier == 3 then
			tablaTier = Habilidades.Tier3
		end

		if tablaTier.totalWeight == 0 then
			Log.Warning("No hay habilidades registradas del Tier"..tier)
			return nil
		else

			-- local rnd = math.random(tablaTier.totalWeight)
			local rnd = Random(tablaTier.totalWeight)
			-- Log.Debug("GetHabilidad RND: "..rnd)

			for k,v in pairs(tablaTier) do
				if type(v) == "table" then
					if v.pesoAcum >= rnd then
						Log.Debug("La habilidad elegida para salir es: "..v.type)

						local entity = MapEntity(v.name..Habilidades.GetNextID())

						entity:SetType(v.type)
						--HACER algo guay con la pos, por ahor ala ponemos un poco arriba
						local y = posicion.y + 35
						entity:SetAttrib("position", toString({posicion.x, y, posicion.z}))

						NewEntity(entity, GameMgr.map)
						break
					end
				end
			end

		end

	end,

}


