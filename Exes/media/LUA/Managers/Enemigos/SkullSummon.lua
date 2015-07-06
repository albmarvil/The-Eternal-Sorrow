
-------------------------------TABLA SkullSummon---------------------------
SkullSummon = {
	--variables del enemigo

	id = 1,
	tier = 3,
	Archetype = "SkullSummon",
	
	vida = function (x)
		return 0.01*x*x + 0.75*x + 60
	end,

	ataque = function (x)
		return 0.01*x*x + 0.5*x +9
	end,

	vel_mov = function (x)
		return x / 20
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,

	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = 0.0035 * x + 0.05

		if speed > 0.15 then
			speed = 0.15
		end
		return speed
	end,

	maxMinionsSummon = function (x)
		return 0.2 * x + 2
	end,

	maxDelayInvokeSummon = function (x)
		local delay = 4000 - (x*100)

		if delay < 1000 then
			delay = 1000
		end
		return delay
	end,

	maxDelaySendSummon = function (x)
		local delay = 1500 - (x*20)

		if delay < 500 then
			delay = 500
		end
		return delay
	end,

	GetNextID = function (  )
		local id = SkullSummon.id + 1
		SkullSummon.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

SkullSummonContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 60}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 15},
		{name = "Habilidad1", peso = 10},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 10},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 1}, --1
		{name = "Arma3", peso = 0},
	},

	rangoPotenciadores = {min = 1, max = 5},
	rangoAlmas = {min = 25, max = 50}, --son porcentajes
	rangoDineroBase = {min = 80, max = 200},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + SkullSummonContenedor.rangoDineroBase.min
		max = 2 * level + SkullSummonContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[SkullSummon.Archetype] = SkullSummonContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..SkullSummon.tier] == nil then
	SpawnerMgr.tiers["Tier"..SkullSummon.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..SkullSummon.tier].Enemigos, SkullSummon)