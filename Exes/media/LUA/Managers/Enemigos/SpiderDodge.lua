
-------------------------------TABLA SpiderDodge---------------------------
SpiderDodge = {
	--variables del enemigo

	id = 1,
	tier = 2,
	Archetype = "SpiderDodge",
	
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

		if speed >= 0.15 then
			speed = 0.15
		end

		return speed 
	end,

	maxDelayDodge = function (x)
		local delay  = -10*x + 1010

		if delay <= 500 then
			delay = 500
		end
		return delay
	end,

	rangeDetectDodge = function (x)
		return x * 3 + 100 
	end,

	sucessPercentDodge = function (x)
		return 0.5 * x + 18
	end,

	GetNextID = function (  )
		local id = SpiderDodge.id + 1
		SpiderDodge.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

SpiderDodgeContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 60}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 9},
		{name = "Habilidad1", peso = 10},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 10},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 0}, --1
		{name = "Arma3", peso = 0},
	},

	rangoPotenciadores = {min = 1, max = 2},
	rangoAlmas = {min = 1, max = 25}, --son porcentajes
	rangoDineroBase = {min = 20, max = 50},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + SpiderDodgeContenedor.rangoDineroBase.min
		max = 2 * level + SpiderDodgeContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[SpiderDodge.Archetype] = SpiderDodgeContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..SpiderDodge.tier] == nil then
	SpawnerMgr.tiers["Tier"..SpiderDodge.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..SpiderDodge.tier].Enemigos, SpiderDodge)