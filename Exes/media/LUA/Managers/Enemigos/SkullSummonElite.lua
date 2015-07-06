
-------------------------------TABLA SkullSummonElite---------------------------
SkullSummonElite = {
	--variables del enemigo

	id = 1,
	tier = 5,
	Archetype = "SkullSummonElite",
	
	vida = function (x)
		return SkullSummon.vida(x) * 5
	end,

	ataque = function (x)
		return SkullSummon.ataque(x) * 1.5
	end,

	vel_mov = function (x)
		return x / 20
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,

	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = SkullSummon.maxVelApproach(x) * 1.12

		if speed > 0.2 then
			speed = 0.2
		end
		return speed
	end,

	maxMinionsSummon = function (x)
		return SkullSummon.maxMinionsSummon(x)
	end,

	maxDelayInvokeSummon = function (x)
		local delay = SkullSummon.maxDelayInvokeSummon(x)

		if delay < 1000 then
			delay = 1000
		end
		return delay
	end,

	maxDelaySendSummon = function (x)
		local delay = SkullSummon.maxDelaySendSummon(x)

		if delay < 500 then
			delay = 500
		end
		return delay
	end,

	GetNextID = function (  )
		local id = SkullSummonElite.id + 1
		SkullSummonElite.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

SkullSummonEliteContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 60}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 20},
		{name = "Habilidad1", peso = 10},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 10},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 1}, --1
		{name = "Arma3", peso = 2},
	},

	rangoPotenciadores = {min = 1, max = 5},
	rangoAlmas = {min = 40, max = 60}, --son porcentajes
	rangoDineroBase = {min = 150, max = 300},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + SkullSummonEliteContenedor.rangoDineroBase.min
		max = 2 * level + SkullSummonEliteContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[SkullSummonElite.Archetype] = SkullSummonEliteContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..SkullSummonElite.tier] == nil then
	SpawnerMgr.tiers["Tier"..SkullSummonElite.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..SkullSummonElite.tier].Enemigos, SkullSummonElite)