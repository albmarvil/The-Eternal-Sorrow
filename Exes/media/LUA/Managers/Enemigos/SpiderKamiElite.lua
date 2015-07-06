
-------------------------------TABLA SpiderKamiElite---------------------------
SpiderKamiElite = {
	--variables del enemigo

	id = 1,
	tier = 5,
	Archetype = "SpiderKamiElite",
	
	vida = function (x)
		return SpiderKami.vida(x) * 5
	end,

	ataque = function (x)
		return SpiderKami.ataque(x) * 1.5
	end,

	vel_mov = function (x)
		return x / 20
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,

	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = SpiderKami.maxVelApproach(x) * 1.5

		if speed >= 0.2 then
			speed = 0.2
		end

		return speed 
	end,

	rangeKamikaze = function (x)
		local range = SpiderKami.rangeKamikaze(x)
		
		return range
	end,

	maxTimeKamikaze = function (x)
		local time = SpiderKami.maxTimeKamikaze(x)
		return time
	end,

	damageKamikaze = function (x)
		return SpiderKami.damageKamikaze(x) * 1.5
	end,
	
	GetNextID = function (  )
		local id = SpiderKamiElite.id + 1
		SpiderKamiElite.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

SpiderKamiEliteContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 60}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 12},
		{name = "Habilidad1", peso = 10},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 10},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 1}, --1
		{name = "Arma3", peso = 0},
	},

	rangoPotenciadores = {min = 1, max = 1},
	rangoAlmas = {min = 5, max = 20}, --son porcentajes
	rangoDineroBase = {min = 150, max = 200},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + SpiderKamiEliteContenedor.rangoDineroBase.min
		max = 2 * level + SpiderKamiEliteContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[SpiderKamiElite.Archetype] = SpiderKamiEliteContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..SpiderKamiElite.tier] == nil then
	SpawnerMgr.tiers["Tier"..SpiderKamiElite.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..SpiderKamiElite.tier].Enemigos, SpiderKamiElite)