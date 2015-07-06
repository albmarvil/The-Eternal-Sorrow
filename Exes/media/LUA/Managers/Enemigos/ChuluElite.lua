
-------------------------------TABLA ChuluElite---------------------------
ChuluElite = {
	--variables del enemigo

	id = 1,
	tier = 5,
	Archetype = "ChuluElite",
	
	vida = function (x)
		return Chulu.vida(x) * 5
	end,

	ataque = function (x)
		return Chulu.ataque(x) * 1.5
	end,

	vel_mov = function (x)
		return x / 20
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,

	maxVelApproach = function (x)
		local speed = Chulu.maxVelApproach(x) * 1.1

		if speed >= 0.2 then
			speed = 0.2
		end

		return speed
	end,

	ammountStealLife = function (x)
		return Chulu.ammountStealLife(x) * 1.5
	end,

	maxDelayStealLife = function (x)
		local aux = Chulu.maxDelayStealLife(x)
		if aux <= 500 then
			aux = 500
		end
		return aux
	end,

	minDistanceStealLife = function (x)
		return Chulu.minDistanceStealLife(x)
	end,
	
	GetNextID = function (  )
		local id = ChuluElite.id + 1
		ChuluElite.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

ChuluEliteContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 60}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 15},
		{name = "Habilidad1", peso = 10},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 10},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 0}, --1
		{name = "Arma3", peso = 0},
	},

	rangoPotenciadores = {min = 1, max = 2},
	rangoAlmas = {min = 15, max = 25}, --son porcentajes
	rangoDineroBase = {min = 100, max = 200},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + ChuluEliteContenedor.rangoDineroBase.min
		max = 2 * level + ChuluEliteContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[ChuluElite.Archetype] = ChuluEliteContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..ChuluElite.tier] == nil then
	SpawnerMgr.tiers["Tier"..ChuluElite.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..ChuluElite.tier].Enemigos, ChuluElite)