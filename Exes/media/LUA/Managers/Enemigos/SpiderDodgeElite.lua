
-------------------------------TABLA SpiderDodgeElite---------------------------
SpiderDodgeElite = {
	--variables del enemigo

	id = 1,
	tier = 5,
	Archetype = "SpiderDodgeElite",
	
	vida = function (x)
		return SpiderDodge.vida(x) * 5 
	end,

	ataque = function (x)
		return SpiderDodge.ataque(x) * 1.5
	end,

	vel_mov = function (x)
		return x / 20
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,
	
	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = SpiderDodge.maxVelApproach(x) * 1.2

		if speed >= 0.2 then
			speed = 0.2
		end

		return speed 
	end,

	maxDelayDodge = function (x)
		local delay  = -10*x + 710

		if delay <= 500 then
			delay = 500
		end
		return delay
	end,

	rangeDetectDodge = function (x)
		return x * 3 + 300 
	end,

	sucessPercentDodge = function (x)
		return SpiderDodge.sucessPercentDodge(x) * 1.15
	end,

	GetNextID = function (  )
		local id = SpiderDodgeElite.id + 1
		SpiderDodgeElite.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

SpiderDodgeEliteContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 60}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 12},
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

		min = 2 * level + SpiderDodgeEliteContenedor.rangoDineroBase.min
		max = 2 * level + SpiderDodgeEliteContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[SpiderDodgeElite.Archetype] = SpiderDodgeEliteContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..SpiderDodgeElite.tier] == nil then
	SpawnerMgr.tiers["Tier"..SpiderDodgeElite.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..SpiderDodgeElite.tier].Enemigos, SpiderDodgeElite)