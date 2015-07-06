--author: Cesar Manuel Paz Guzman

ObjRompibles = 
{
	"pilaCalaveras",
	"calavera",
	"barrilEntero",
	"cajaEntera",
}

FuncionesObjRompibles = {}

FuncionesObjRompibles.objRompiblesPuntosInteres = {
	10,
	25,
	50,
	math.huge, ---este último limita el máx ptos de Interes
}

FuncionesObjRompibles.objRompibleID = 0

function FuncionesObjRompibles.nextObjetoRompibleID( )
	FuncionesObjRompibles.objRompibleID = FuncionesObjRompibles.objRompibleID + 1

	return FuncionesObjRompibles.objRompibleID
end

function FuncionesObjRompibles.escogeObjetoRompible()
	local rnd = RandomRange(1,4) --Aleatoriamente escogemos el tipo de objeto rompible a instanciar

	return ObjRompibles[rnd]
end

function FuncionesObjRompibles.obtenerNumeroObjetosRompibles(numPuntosInteres)

	for key,value in pairs(FuncionesObjRompibles.objRompiblesPuntosInteres) do
		if numPuntosInteres <= value then
			return key --Segun la tabla de puntos de interes, obtenemos cuantos objtos rompibles podria haber 
		end
	end

end

function FuncionesObjRompibles.crearObjetosRompibles()

	local numeroObjetosAInstanciar = FuncionesObjRompibles.obtenerNumeroObjetosRompibles(Drop.numPuntosInteres)

	for i=1,numeroObjetosAInstanciar do

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


				local tipo = FuncionesObjRompibles.escogeObjetoRompible()

				--este es el pto de interes elegido para el cofre
				local entity = MapEntity(tipo..FuncionesObjRompibles.nextObjetoRompibleID())

				entity:SetType(tipo)

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
