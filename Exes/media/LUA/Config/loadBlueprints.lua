--author: Cesar Manuel Paz Guzman
--date Febrero 2015

function loadBlueprints(fileB)

	--ejecutamos
	Blueprints = nil --Borramos posible blueprints anterior

	scriptMgr:loadScript(Paths.pathMaps..fileB)

	--Si despues de ejecutar el fichero no hay nada en el mapa
	if Blueprints == nil then
		Log.Error("Blueprints es nulo")
		return 0
	end

	for key, value in pairs(Blueprints) do 
		--Recorremos cada una de las tablas que hay dentro, lo que representa los componentes del tipo entidad key
		addEntities(key, value)
	end
	

	return 1
end

function addEntities(key, value)

	if type(value) == "table" then
		for keyTabla, miniTabla in pairs(value) do
			if type(miniTabla) == "table" then

				for key2, value2 in pairs(miniTabla) do 
					
					-- print("Tipo: "..key.." - "..key2)
					if type(value2) == "boolean" then
						addComponent(key, key2, value2)	--Añade al tipo de entidad key, el componente key con el estado value	
					end 
					if type(value2) == "string" then
						--Funcion recursiva. Si como componente es otro tipo de entidad, es decir, que hereda los componentes de esta entidad
						--llamo a la siguiente funcion, la cual comprobara tambien si tiene otras entidades de las cuales hereda
						addComponentsHerencia(key, key2)
					end
				end
			end
		end
	end
end

function addComponentsHerencia(entidad, entidadPadre)
	--Busca en el blueprints la entidad padre
	for key, value in pairs(Blueprints) do
		--Si la encuentro
		if key == entidadPadre then
			--Vuelvo a leer sus componentes, y se los asocio a la entidad
			addEntities(entidad, value)
		end
	end
end