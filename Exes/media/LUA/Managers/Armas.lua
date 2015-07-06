--@author Alberto Martínez Villarán
--@date Marzo, 2015

--Tabla donde estan registradas todas las armas del juego con un peso incorporado. Las armas están divididas en 3 niveles de poder o Tiers diferentes

Armas = {
	
	Tier1 = {

		--Ejemplo

		--{
		--  name = Nombre del arma
		-- 	peso = Peso para tiradas de probabilidad. Solo s etienen en cuenta para el Tier en el que esta el arma
		-- 	type = tipo del arquetipo del objeto recogible

		--},

		-- {
		-- 	name = "Katana",
		-- 	peso = 50, 
		-- 	type = "UsableObjectKatana",

		-- },

		{
			name = "DartGun",
			peso = 50,
			type = "UsableObjectDartGun",

		},

		{
			name = "Shield",
			peso = 50,
			type = "UsableObjectShield",

		},

		

	},

	Tier2 = {

		{
			name = "Laser",
			peso = 50,
			type = "UsableObjectLaser",

		},

		{
			name = "GreenSword",
			peso = 50,
			type = "UsableObjectGreenSword",

		},
	},

	Tier3 = {

		{
			name = "Tongue",
			peso = 60,
			type = "UsableObjectTongue",

		},
	},


	--Esta función inicializa los vectores de acumulación de pesos para dejarlos listos para usarse

	Init = function ( )
		-- body
		local totalWeight = 0
		for k,v in pairs(Armas.Tier1) do
			if type(v) == "table" then
				totalWeight = totalWeight + v.peso
				v.pesoAcum = totalWeight
				-- print(v.peso, v.type, v.pesoAcum)
			end
		end
		Armas.Tier1.totalWeight = totalWeight
		-- print "TIER1"
		-- print ("Peso Total: "..Armas.Tier1.totalWeight)

		totalWeight = 0
		for k,v in pairs(Armas.Tier2) do
			if type(v) == "table" then
				totalWeight = totalWeight + v.peso
				v.pesoAcum = totalWeight
				-- print(v.peso, v.type, v.pesoAcum)
			end
		end
		Armas.Tier2.totalWeight = totalWeight
		-- print "TIER2"
		-- print ("Peso Total: "..Armas.Tier2.totalWeight)

		totalWeight = 0
		for k,v in pairs(Armas.Tier3) do
			if type(v) == "table" then
				totalWeight = totalWeight + v.peso
				v.pesoAcum = totalWeight
				-- print(v.peso, v.type, v.pesoAcum)
			end
		end
		Armas.Tier3.totalWeight = totalWeight
		-- print "TIER3"
		-- print ("Peso Total: "..Armas.Tier3.totalWeight)


	end,

	ArmaID = 0,

	GetNextID = function (  )
		local id = Armas.ArmaID + 1
		Armas.ArmaID = id
		return id
	end,

	CreateArma = function (tier, posicion)
		-- body
		local tablaTier = nil 

		if tier == 1 then
			tablaTier = Armas.Tier1
		elseif tier == 2 then
			tablaTier = Armas.Tier2
		elseif tier == 3 then
			tablaTier = Armas.Tier3
		end

		if tablaTier.totalWeight == 0 then
			Log.Warning("No hay armas registradas del Tier"..tier)
			return nil
		else

			-- local rnd = math.random(tablaTier.totalWeight)
			local rnd = Random(tablaTier.totalWeight)
			-- Log.Debug("GetArma RND: "..rnd)
			for k,v in pairs(tablaTier) do
				if type(v) == "table" then
					if v.pesoAcum >= rnd then
						Log.Debug("El arma elegida para salir es: "..v.type)

						local entity = MapEntity(v.name..Armas.GetNextID())

						entity:SetType(v.type)
						--HACER algo guay con la pos, por ahor ala ponemos un poco arriba
						entity:SetAttrib("position", toString({posicion.x, (posicion.y+15), posicion.z}))

						NewEntity(entity, GameMgr.map)
						break
					end
				end
			end

		end

	end,

}


