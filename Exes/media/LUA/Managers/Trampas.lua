--author: Cesar Manuel Paz Guzman

Trampas = 
{
	{nombre = "Heiser", porcentajedanho = 6, sonido = SoundEvents.VaporPresion, }, --Nombre de la trampa, con el porcentaje de daño que quita y el sonido que emite
	-- {nombre = "TrampaRayo", porcentajedanho = 7, sonido = "NADA", },
	{nombre = "HumoVenenoso", porcentajedanho = 4, sonido = "NADA", },
	-- {nombre = "Pinchos", porcentajedanho = 5, sonido = "NADA", },
}

FuncionesTrampas = {}

FuncionesTrampas.trampasPuntosInteres = {
}

FuncionesTrampas.TrampaID = 0

--Las tres funciones siguientes es para que en las primeras 5 salas no apareca ninguna trampas y vayan aparenciedo cada vez mas
--a medida que avanzamos de nivel
function FuncionesTrampas.updateTrampasSala5()
	FuncionesTrampas.trampasPuntosInteres = {
		10,
		50,
		math.huge, ---este último limita el máx ptos de Interes
	}
end

function FuncionesTrampas.updateTrampasSala10()
	FuncionesTrampas.trampasPuntosInteres = {
		10,
		30,
		60,
		math.huge, ---este último limita el máx ptos de Interes
	}
end

function FuncionesTrampas.updateTrampasSala15()
	FuncionesTrampas.trampasPuntosInteres = {
		10,
		25,
		40,
		math.huge, ---este último limita el máx ptos de Interes
	}
end

function FuncionesTrampas.nextTrampaID( )
	-- body
	FuncionesTrampas.TrampaID = FuncionesTrampas.TrampaID + 1
	return FuncionesTrampas.TrampaID
end

function FuncionesTrampas.escogerTrampa()
	local rnd = RandomElement(Trampas)

	return Trampas[rnd].nombre
end

function FuncionesTrampas.escogerSonido(tipoTrampa)
	for k,v in pairs(Trampas) do
		if v.nombre == tipoTrampa then
			return v.sonido
		end
	end
	return "-"
end

function FuncionesTrampas.obtenerDanho(nombre)
	for key,value in pairs(Trampas) do 
		if value.nombre == nombre then
			return value.porcentajedanho
		end
	end
end

function FuncionesTrampas.obtenerNumeroTrampas(numPuntosInteres)

	for key,value in pairs(FuncionesTrampas.trampasPuntosInteres) do
		if numPuntosInteres <= value then
			return key
		end
	end

	return 0

end

function FuncionesTrampas.crearTrampas()

	local numeroTrampasAInstancia = FuncionesTrampas.obtenerNumeroTrampas(Drop.numPuntosInteres)

	for i=1,numeroTrampasAInstancia do

		--acumulamos los ptos de interes
		local totalWeight = 0

		for key,value in pairs(Drop.puntosInteres) do
			totalWeight = totalWeight + value.peso
			value.pesoAcum = totalWeight
		end


		--elegimos en qué punto colocar el cofre
		local rnd = Random(totalWeight)
		local punto = nil

		for key,value in pairs(Drop.puntosInteres) do
			if value.pesoAcum >= rnd then
				punto = key

				--este es el pto de interes elegido para el cofre
				local entity = MapEntity("Trampilla"..FuncionesTrampas.nextTrampaID())

				entity:SetType("TILE_TRAMPILLA")

				--Colocamos los cofres en una pos retrasada
				entity:SetAttrib("position", value.pos.x..","..value.pos.y..", 0")

				AddEntity(entity)
				break
			end
		end

		-- --borramos este punto de interés de la tabla
		Drop.puntosInteres[punto] = nil
		Drop.numPuntosInteres = Drop.numPuntosInteres - 1

	end
end