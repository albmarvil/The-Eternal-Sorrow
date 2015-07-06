
-------------------------------TABLA Chulu---------------------------
Chulu = {
	--variables del enemigo

	id = 1,
	tier = 3,
	Archetype = "Chulu",
	
	vida = function (x)
		return 0.01*x*x + 0.75*x + 25
	end,

	ataque = function (x)
		return 0.5*x + 3
	end,

	vel_mov = function (x)
		return x / 20
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,

	maxVelApproach = function (x)
		local speed = 0.002 * x + 0.05

		if speed >= 0.15 then
			speed = 0.15
		end

		return speed
	end,

	ammountStealLife = function (x)
		return 0.01*x*x + 0.5*x + 5
	end,

	maxDelayStealLife = function (x)
		local aux = -2*x*x + x + 3000
		if aux <= 500 then
			aux = 500
		end
		return aux
	end,

	minDistanceStealLife = function (x)
		return 2 * x + 40
	end,
	
	GetNextID = function (  )
		local id = Chulu.id + 1
		Chulu.id = id
		return id
	end,
}

-----------------DATOS DEL DROP---------------------

ChuluContenedor = { 
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
	rangoDineroBase = {min = 75, max = 150},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + ChuluContenedor.rangoDineroBase.min
		max = 2 * level + ChuluContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[Chulu.Archetype] = ChuluContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..Chulu.tier] == nil then
	SpawnerMgr.tiers["Tier"..Chulu.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..Chulu.tier].Enemigos, Chulu)