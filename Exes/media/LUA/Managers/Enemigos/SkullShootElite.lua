
-------------------------------TABLA SkullShootElite---------------------------
SkullShootElite = {
	--variables del enemigo
	
	id = 1,
	tier = 4,
	Archetype = "SkullShootElite",
	
	vida = function (x)
		return SkullShoot.vida(x) * 5
	end,

	ataque = function (x)
		return SkullShoot.ataque(x) * 1.5
	end,

	vel_mov = function (x)
		return x / 20   ----no se usa
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,
	
	
	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = SkullShoot.maxVelApproach(x) * 1.2

		if speed >= 0.2 then
			speed = 0.2
		end

		return speed  
	end,

	maxDelayShoot = function (x)
		local aux = -2*x*x + x + 1500
		if aux <= 500 then
			aux = 500
		end
		return aux
	end,

	-- rangeShoot = function (x)
	-- 	return 20 * x + 800
	-- end,

	bulletDmgShoot = function (x)
		return SkullShoot.bulletDmgShoot(x) * 1.5
	end,

	GetNextID = function (  )
		local id = SkullShootElite.id + 1
		SkullShootElite.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

SkullShootEliteContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 60}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 7},
		{name = "Habilidad1", peso = 5},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 10},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 0}, --1
		{name = "Arma3", peso = 0},
	},

	rangoPotenciadores = {min = 1, max = 1},
	rangoAlmas = {min = 10, max = 20}, --son porcentajes
	rangoDineroBase = {min = 50, max = 80},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + SkullShootEliteContenedor.rangoDineroBase.min
		max = 2 * level + SkullShootEliteContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[SkullShootElite.Archetype] = SkullShootEliteContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..SkullShootElite.tier] == nil then
	SpawnerMgr.tiers["Tier"..SkullShootElite.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..SkullShootElite.tier].Enemigos, SkullShootElite)