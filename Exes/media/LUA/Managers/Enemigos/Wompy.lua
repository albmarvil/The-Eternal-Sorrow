
-------------------------------TABLA Wompy---------------------------
Wompy = {
	--variables del enemigo

	id = 1,
	tier = 1,
	Archetype = "Wompy",
	
	vida = function (x)
		return 0.01*x*x + 0.75*x + 6
	end,

	ataque = function (x)
		return 0.5*x + 1
	end,

	vel_mov = function (x)
		return x / 20 --no se esta usando para moverse se usa el approach
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,
	
	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = 0.001 * x + 0.05

		if speed >= 0.15 then
			speed = 0.15
		end

		return speed 
	end,

	GetNextID = function (  )
		local id = Wompy.id + 1
		Wompy.id = id
		return id
	end,

}

-----------------DATOS DEL DROP---------------------

WompyContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 80}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 5},
		{name = "Habilidad1", peso = 3},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 10},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 0}, --1
		{name = "Arma3", peso = 0},
	},

	rangoPotenciadores = {min = 1, max = 1},
	rangoAlmas = {min = 1, max = 5}, --son porcentajes
	rangoDineroBase = {min = 10, max = 10},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + WompyContenedor.rangoDineroBase.min
		max = 2 * level + WompyContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[Wompy.Archetype] = WompyContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..Wompy.tier] == nil then
	SpawnerMgr.tiers["Tier"..Wompy.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..Wompy.tier].Enemigos, Wompy)