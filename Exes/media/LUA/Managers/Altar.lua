Altar = 
{

	Init = function ()

	end,

	--INFO Y FUNCIONES RELATIVAS A LOS PUNTOS DE INTERES
	puntosInteres = {},

	numPuntosInteres = 0,
	
	radio = 0,
	
	posAltar = nil,


	CreateObjects = function()
		--al entrar en una nueva sala actualizamos los puntos de interés

		--limpiamos valores de los puntos de interes de la sala anterior
		Altar.numPuntosInteres = 0
		Altar.puntosInteres = {}

		local puntos = getAltarPoints()

		--guardamos los puntos en lua para no acceder a C++
		if puntos ~= nil then
			for i = 0, puntos:size()-1, 1 do
				local aux_tabla = {peso = puntos:at(i):weight(), pos = puntos:at(i):pos(), radio = puntos:at(i):radio(), pesoAcum = 0}
				Altar.puntosInteres[i+1] = aux_tabla
				Altar.numPuntosInteres = Altar.numPuntosInteres + 1
			end
		end
		
		
		local totalWeight = 0
		for k,v in pairs(Altar.puntosInteres) do
			totalWeight = totalWeight + v.peso
			v.pesoAcum = totalWeight
		end
		
		--elegimos un punto donde colocar el altar
		local rnd = Random(totalWeight)
		
		local punto = nil

		for key,value in pairs(Altar.puntosInteres) do
			if value.pesoAcum >= rnd then
				punto = key
				
				
				Altar.posAltar = value.pos
				
				value.pos.z = -20
				
				Altar.radio = value.radio * value.radio
				
				--creamos el altar en la posición del punto calculado
				entityfactory:createEntityByType("AltarSacrificio", value.pos, GameMgr.map)
				break;
			end
		end
	end,
}