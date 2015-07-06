-- Cargador de mapas. Llama a las funciones correspondientes
--del motor. No debe llamarser desde lua para cargar un nuevo mapa,
--pues el motor no estara preparado para recibir las solicitudes de 
--creacion de entidades nuevas!!!!!

function toString(value)
	if type(value) == "boolean" then
			if value then return "true"
			else return "false" end
	end
	if type(value) == "table" then
		local resultado = ""
		for key, value2 in pairs(value) do
			resultado = resultado..value2..","
		end
		return resultado
	end
	return ""..value
end

function loadMap(mapFile)
	Map = nil -- Borramos posible mapa anterior
	
	scriptMgr:loadScript(Paths.pathMaps..mapFile)

	if Map == nil then
	   -- El fichero no define un mapa...
	   Log.Error("El fichero no define un mapa")
	   return 0
	end

	-- Y procesamos
	for key, value in pairs(Map) do
		local entity = MapEntity(key)
		entity:SetType(value.type)
		for k,v in pairs(value) do
			if k ~= "type" then
				entity:SetAttrib(k, toString(v))
			end
		end
		AddEntity(entity)
	end
	
	return 1
end
