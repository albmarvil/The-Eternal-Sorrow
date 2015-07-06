-- author: Cesar Manuel Paz Guzman

CartelesOcupados = 
{
	{numeroCartel = 0, nuevaWindow = "null", tiempoVida = 0,}, 
}

function CartelesOcupados.Deactivate()
	for key, value in pairs(CartelesOcupados) do
		if type(value) == "table" and value["nuevaWindow"] ~="null" then
			value["nuevaWindow"]:setVisible(false)
			value["nuevaWindow"] = "null"
			value["tiempoVida"] = 0
		end
	end
end

function CartelesOcupados.BuscarCartelesVacios()
	for key ,v in pairs(CartelesOcupados) do
  		if type(v) == "table" then
    		for key2, v2 in pairs(v) do
    			if key2 == "nuevaWindow" and v2 == "null" then
    				return v["numeroCartel"]
    			end
    		end
  		end
	end
	--Si no se ha encontrado ningun de los que hay en la tabla lo que hacemos es introducir uno nuevo
	table.insert(CartelesOcupados, {numeroCartel = table.getn(CartelesOcupados),})
	return table.getn(CartelesOcupados)-1
end

function CartelesOcupados.mostrarCartel(nombre, info, icono)
		local posicionCartel = CartelesOcupados.BuscarCartelesVacios()

		if posicionCartel <= 6 then
			local nuevaWindow = GUI.GUIContext:getRootWindow():getChild("Carteles"):getChildAtIdx(posicionCartel)

			nuevaWindow:setVisible(true)

			CartelesOcupados[posicionCartel+1].nuevaWindow = nuevaWindow
			CartelesOcupados[posicionCartel+1].tiempoViva = 2000

			nuevaWindow:getChild("Background/ImagenObjeto"):setProperty("Image", icono)
			nuevaWindow:getChild("Background/Nombre"):setProperty("Text", nombre) 
			nuevaWindow:getChild("Background/Explicacion"):setProperty("Text", info)
		end
end

function CartelesOcupados.updateCartelesHabilidad(msecs)
	for key, value in pairs(CartelesOcupados) do
		if type(value) == "table" and value["nuevaWindow"] ~= "null" then

			if value.tiempoViva <= 0 then
				value["nuevaWindow"]:setVisible(false)
				
				value["nuevaWindow"] = "null"
			else 
				value.tiempoViva = value.tiempoViva - msecs
			end
		end
	end
end