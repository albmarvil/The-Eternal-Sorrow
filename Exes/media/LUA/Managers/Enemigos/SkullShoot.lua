
-------------------------------TABLA SkullShoot---------------------------
SkullShoot = {
	--variables del enemigo
	
	id = 1,
	tier = 1,
	Archetype = "SkullShoot",
	
	vida = function (x)
		return 0.01*x*x + 0.75*x + 10	--5 * x + 70
	end,

	ataque = function (x)
		return 0.5*x + 3	--x + 6
	end,

	vel_mov = function (x)
		return x / 20   ----no se usa
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,
	
	
	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = 0.002 * x + 0.05

		if speed >= 0.15 then
			speed = 0.15
		end

		return speed  
	end,

	maxDelayShoot = function (x)
		local aux = -2*x*x + x + 3000
		if aux <= 500 then
			aux = 500
		end
		return aux
	end,

	-- rangeShoot = function (x)
	-- 	return 20 * x + 800
	-- end,

	bulletDmgShoot = function (x)
		return 0.01*x*x + 0.5*x + 3	--x * 2 + 8 
	end,

	GetNextID = function (  )
		local id = SkullShoot.id + 1
		SkullShoot.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

SkullShootContenedor = { 
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
	rangoAlmas = {min = 5, max = 10}, --son porcentajes
	rangoDineroBase = {min = 10, max = 20},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + SkullShootContenedor.rangoDineroBase.min
		max = 2 * level + SkullShootContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[SkullShoot.Archetype] = SkullShootContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..SkullShoot.tier] == nil then
	SpawnerMgr.tiers["Tier"..SkullShoot.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..SkullShoot.tier].Enemigos, SkullShoot)