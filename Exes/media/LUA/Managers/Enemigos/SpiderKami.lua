
-------------------------------TABLA SpiderKami---------------------------
SpiderKami = {
	--variables del enemigo

	id = 1,
	tier = 2,
	Archetype = "SpiderKami",
	
	vida = function (x)
		return 0.01*x*x + 0.75*x + 20
	end,

	ataque = function (x)
		return 0.01*x*x + 0.5*x + 3
	end,

	vel_mov = function (x)
		return x / 20
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,

	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = 0.002 * x + 0.07

		if speed >= 0.18 then
			speed = 0.18
		end

		return speed 
	end,

	rangeKamikaze = function (x)
		local range = 3*x + 40
		if range > 120 then
			range = 120
		end
		return range
	end,

	maxTimeKamikaze = function (x)
		local time = 2000 - (x * 50)
		if time < 500 then
			time = 500
		end
		return time
	end,

	damageKamikaze = function (x)
		return SpiderKami.ataque(x) * 3.5
	end,
	
	GetNextID = function (  )
		local id = SpiderKami.id + 1
		SpiderKami.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

SpiderKamiContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 60}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 9},
		{name = "Habilidad1", peso = 10},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 10},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 1}, --1
		{name = "Arma3", peso = 0},
	},

	rangoPotenciadores = {min = 1, max = 1},
	rangoAlmas = {min = 1, max = 5}, --son porcentajes
	rangoDineroBase = {min = 100, max = 100},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + SpiderKamiContenedor.rangoDineroBase.min
		max = 2 * level + SpiderKamiContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[SpiderKami.Archetype] = SpiderKamiContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..SpiderKami.tier] == nil then
	SpawnerMgr.tiers["Tier"..SpiderKami.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..SpiderKami.tier].Enemigos, SpiderKami)