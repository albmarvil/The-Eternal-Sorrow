
-------------------------------TABLA WompyElite---------------------------
WompyElite = {
	--variables del enemigo

	id = 1,
	tier = 4,
	Archetype = "WompyElite",
	
	vida = function (x)
		return Wompy.vida(x) * 5
	end,

	ataque = function (x)
		return Wompy.ataque(x) * 1.2
	end,

	vel_mov = function (x)
		return x / 20 --no se esta usando para moverse se usa el approach
	end,

	vel_ataque = function (x)
		return -2 * x + 1002
	end,
	
	-- Atributos de IA--
	maxVelApproach = function (x)
		local speed = Wompy.maxVelApproach(x) * 1.5

		if speed >= 0.2 then
			speed = 0.2
		end

		return speed 
	end,

	GetNextID = function (  )
		local id = WompyElite.id + 1
		WompyElite.id = id
		return id
	end,

}

-----------------DATOS DEL DROP---------------------

WompyEliteContenedor = { 
			---Se usan tablas sin claves para que se iteren siempre de la misma forma
	pesos = {
		{name = "Nada", peso = 60}, --Esto sirve para indicar que no debe caer nada
		{name = "Almas", peso = 8},
		{name = "Habilidad1", peso = 10},
		{name = "Habilidad2", peso = 0}, --2
		{name = "Habilidad3", peso = 0}, --1
		{name = "Potenciador", peso = 10},
		{name = "Arma1", peso = 1},
		{name = "Arma2", peso = 0}, --1
		{name = "Arma3", peso = 0},
	},

	rangoPotenciadores = {min = 1, max = 1},
	rangoAlmas = {min = 5, max = 15}, --son porcentajes
	rangoDineroBase = {min = 30, max = 50},

	--esta funcion devuelve la cantidad de dinero, tanto el limite superior como inferior
	CrecimientoDinero = function (level)
		local min, max = 0

		min = 2 * level + WompyEliteContenedor.rangoDineroBase.min
		max = 2 * level + WompyEliteContenedor.rangoDineroBase.max

		return min, max
	end,
}
----------------------Damos de alta el contenedor en el drop manager -------------------

Drop.Contenedores[WompyElite.Archetype] = WompyEliteContenedor

-------------------------APUNTADO EN LA TABLA-----------------------

if SpawnerMgr.tiers["Tier"..WompyElite.tier] == nil then
	SpawnerMgr.tiers["Tier"..WompyElite.tier] = {Enemigos = {},}
end

table.insert(SpawnerMgr.tiers["Tier"..WompyElite.tier].Enemigos, WompyElite)