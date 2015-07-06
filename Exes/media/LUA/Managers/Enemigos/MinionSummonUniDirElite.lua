
-------------------------------TABLA MinionSummonUniDirElite---------------------------
MinionSummonUniDirElite = {
	--variables del enemigo

	id = 1,
	tier = 5,
	Archetype = "MinionSummonUniDirElite",
	
	vida = function (x)
		return 0.01*x*x + 0.75*x + 4
	end,

	ataque = function (x)
		return 0.5*x + 3
	end,

	vel_mov = function (x)
		return x / 20 --no se esta usando para moverse se usa el approach
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,
	
	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = 0.01 * x + 0.1

		if speed >= 0.15 then
			speed = 0.15
		end

		return speed 
	end,
	
	GetNextID = function (  )
		local id = MinionSummonUniDirElite.id + 1
		MinionSummonUniDirElite.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

MinionSummonUniDirEliteContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 10}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 2},
		{name = "Habilidad1", peso = 1},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 2},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 0}, --1
		{name = "Arma3", peso = 0},
	},

	rangoPotenciadores = {min = 1, max = 1},
	rangoAlmas = {min = 1, max = 5}, --son porcentajes
	rangoDineroBase = {min = 50, max = 100},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + MinionSummonUniDirEliteContenedor.rangoDineroBase.min
		max = 2 * level + MinionSummonUniDirEliteContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[MinionSummonUniDirElite.Archetype] = MinionSummonUniDirEliteContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.minions["Tier"..MinionSummonUniDirElite.tier] == nil then
	SpawnerMgr.minions["Tier"..MinionSummonUniDirElite.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.minions["Tier"..MinionSummonUniDirElite.tier].Enemigos, MinionSummonUniDirElite)