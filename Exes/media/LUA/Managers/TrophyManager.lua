--author: César Manuel Paz Gumán

TrophyManager = 
{
	MatarEnemigosMismoNivel = 1, --Completo
	MatarEnemigosTotales = 2, --Completo
	SuperarSala = 3, --Completo
	MatarEnemigosLaser = 4,
	MatarEnemigosLanzadardos = 5,
	MatarEnemigosEspada = 6,
	MatarEnemigosEscudo = 7,
	MatarEnemigosLatigo = 8, 
	MatarEnemigosEnvenenados = 9, 
	MatarEnemigosConfusos = 10, 
	MatarEnemigosRalentizados = 11, 
	MatarEnemigosCongelado = 12, 
	MatarEnemigosMismaPartida = 13, 
	MatarEnemigosMismaPartidaAvalancha = 14, 
	SuperarSalasMismaPartida = 15, 
	AbrirCofresBasicos = 16, 
	AbrirCofresIntermedios = 17, 
	AbrirCofresAltos = 18, 
	MatarEnemigosAvalanchaTotal = 19, 
	DestruirTiles = 20, 
	PuntuacionPartida = 21,
	DestruirObjetos = 22,
}


FuncionesTrophyManager = {}
FuncionesTrophyManager.timeStampCartelLogro = 0
FuncionesTrophyManager.tiempoVidaCartelLogro = 2500
FuncionesTrophyManager.cartelLogroIniciado = false

--Esto solo lo creo para la pntuacion total, dado que no se aumenta el valor uno por uno como en el resto de los casos. Indico entonces, false false false para indicar
--que la puntuacion total ya se ha cumplido o no para bronce, plata, oro
FuncionesTrophyManager.puntuacionTotal = 
{
	false,false,false
}

function FuncionesTrophyManager.Init()
	FuncionesTrophyManager.leerFichero()
end

function FuncionesTrophyManager.Release()
	FuncionesTrophyManager.juegoNuevo()
	FuncionesTrophyManager.nivelNuevo()
	FuncionesTrophyManager.escribirFichero()
end

function FuncionesTrophyManager.leerFichero()
	scriptMgr:loadScript(Paths.luaManagersPath.."Puntuaciones.lua")

	--Para cada uno de los tres tipos de trofeo: oro, plata, bronce
	for i = 1, 3, 1 do

		if(ObjetivosPuntuacionesTrofeo[TrophyManager["PuntuacionPartida"]][i][1] >= ObjetivosPuntuacionesTrofeo[TrophyManager["PuntuacionPartida"]][i][2]) then
			FuncionesTrophyManager.puntuacionTotal[i] = true
		end

	end
	
end

function FuncionesTrophyManager.escribirFichero()
	local archivo = io.open(Paths.luaManagersPath.."Puntuaciones.lua","w")

	archivo:write(	"ObjetivosPuntuacionesTrofeo = \n{\n")

	for key, value in pairs(ObjetivosPuntuacionesTrofeo) do

		archivo:write("	  {")

		for tipoTrofeo, tablaValores in pairs(value) do

			archivo:write("{"..ObjetivosPuntuacionesTrofeo[key][tipoTrofeo][1]..","..ObjetivosPuntuacionesTrofeo[key][tipoTrofeo][2].."},")

		end

		archivo:write("},\n")
	end

	archivo:write("}")

	archivo:close()
end

function FuncionesTrophyManager.actualizarImagenesTrofeo()
	local image = "" 
	local posImage = 1

	for key, value in pairs(ObjetivosPuntuacionesTrofeo) do

		for tipoTrofeo, tablaValores in pairs(value) do

			image = ""

			--Si se ha cumplido el objetivo
			if tablaValores[1] >= tablaValores[2] then
				image = FuncionesTrophyManager.obtenerImagenTrofeo(tipoTrofeo)
				trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..posImage.."/Titulo"):setProperty("Visible","false")
			else
				trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..posImage.."/Titulo"):setProperty("Text",toString(tablaValores[1]))
			end

			trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..posImage.."/imageTrofeo"):
			setProperty("Image", image)

			if tipoTrofeo == 1 then
				image =  "NewHUD/iconoTrofeoBloqueadoBronce"
			elseif tipoTrofeo == 2 then
				image = "NewHUD/iconoTrofeoBloqueadoPlata"
			elseif tipoTrofeo == 3 then
				image = "NewHUD/iconoTrofeoBloqueadoOro"
			end

			trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..posImage):
			setProperty("Image", image)

			posImage = posImage + 1
		end
	end

end

function FuncionesTrophyManager.obtenerImagenTrofeo(id)
	if id == 1 then
		return "NewHUD/iconoTrofeoDesBloqueadoBronce"
	elseif id == 2 then
		return "NewHUD/iconoTrofeoDesBloqueadoPlata"
	elseif id == 3 then
		return "NewHUD/iconoTrofeoDesBloqueadoOro"
	end
end

function FuncionesTrophyManager.aumentarValor(objetivo)

	--Para cada uno de los tres tipos de trofeo: oro, plata, bronce
	for i = 1, 3, 1 do
		ObjetivosPuntuacionesTrofeo[TrophyManager[objetivo]][i][1] = ObjetivosPuntuacionesTrofeo[TrophyManager[objetivo]][i][1] + 1

		if(ObjetivosPuntuacionesTrofeo[TrophyManager[objetivo]][i][1] == ObjetivosPuntuacionesTrofeo[TrophyManager[objetivo]][i][2] and FuncionesTrophyManager.cartelLogroIniciado == false) then
			--MOSTRAMOS CARTEL DE SE HA CUMPLIDO EL RETO
			FuncionesTrophyManager.mostrarCartel(objetivo, i)	
		end

	end

end

function FuncionesTrophyManager.aumentarPuntuacionPartida(valor)

	--Para cada uno de los tres tipos de trofeo: oro, plata, bronce
	for i = 1, 3, 1 do
		ObjetivosPuntuacionesTrofeo[TrophyManager["PuntuacionPartida"]][i][1] = ObjetivosPuntuacionesTrofeo[TrophyManager["PuntuacionPartida"]][i][1] + valor

		if(ObjetivosPuntuacionesTrofeo[TrophyManager["PuntuacionPartida"]][i][1] >= ObjetivosPuntuacionesTrofeo[TrophyManager["PuntuacionPartida"]][i][2] and
		 FuncionesTrophyManager.cartelLogroIniciado == false and FuncionesTrophyManager.puntuacionTotal[i] == false ) then
			--MOSTRAMOS CARTEL DE SE HA CUMPLIDO EL RETO
			FuncionesTrophyManager.mostrarCartel("PuntuacionPartida", i)
			FuncionesTrophyManager.puntuacionTotal[i] = true
		end

	end

end

function FuncionesTrophyManager.mostrarCartel(objetivo, tipoTrofeo)
	HUD.hudWindow:getChildElement("Logro"):setProperty("Visible", "true")
	FuncionesTrophyManager.timeStampCartelLogro = 0

	local posTexto = 3 * (TrophyManager[objetivo] - 1) + tipoTrofeo

	HUD.hudWindow:getChildElement("Logro/Explicacion"):setProperty("Text", trofeos[posTexto])

	FuncionesTrophyManager.cartelLogroIniciado = true

	local image = ""

	if tipoTrofeo == 1 then
		image = "NewHUD/backgroundBronce"
	elseif tipoTrofeo == 2 then
		image = "NewHUD/backgroundPlata"
	elseif tipoTrofeo == 3 then
		image = "NewHUD/backgroundOro"
	end

	HUD.hudWindow:getChildElement("Logro"):setProperty("Image", image)
end

function FuncionesTrophyManager.tick(msecs)

	if FuncionesTrophyManager.cartelLogroIniciado == true then
		FuncionesTrophyManager.timeStampCartelLogro = FuncionesTrophyManager.timeStampCartelLogro + msecs

		if FuncionesTrophyManager.timeStampCartelLogro >= FuncionesTrophyManager.tiempoVidaCartelLogro then
			FuncionesTrophyManager.cartelLogroIniciado = false
			HUD.hudWindow:getChildElement("Logro"):setProperty("Visible", "false")
		end
	end

end

function FuncionesTrophyManager.juegoNuevo()

	local pos = 0
	--Aqui reiniciaremos todas las puntuaciones que tengan que ver con la partida
	for i = 1, 3, 1 do

		if ObjetivosPuntuacionesTrofeo[TrophyManager["MatarEnemigosMismaPartida"]][i][1] < ObjetivosPuntuacionesTrofeo[TrophyManager["MatarEnemigosMismaPartida"]][i][2] then
			ObjetivosPuntuacionesTrofeo[TrophyManager["MatarEnemigosMismaPartida"]][i][1] = 0

			pos = 3 * (TrophyManager["MatarEnemigosMismaPartida"] - 1) + i

			trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..toString(pos).."/Titulo"):
			setProperty("Text","0")
		end


		if ObjetivosPuntuacionesTrofeo[TrophyManager["MatarEnemigosMismaPartidaAvalancha"]][i][1] < ObjetivosPuntuacionesTrofeo[TrophyManager["MatarEnemigosMismaPartidaAvalancha"]][i][2] then
			ObjetivosPuntuacionesTrofeo[TrophyManager["MatarEnemigosMismaPartidaAvalancha"]][i][1] = 0

			pos = 3 * (TrophyManager["MatarEnemigosMismaPartidaAvalancha"] - 1) + i

			trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..toString(pos).."/Titulo"):
			setProperty("Text","0")
		end


		if ObjetivosPuntuacionesTrofeo[TrophyManager["SuperarSalasMismaPartida"]][i][1] < ObjetivosPuntuacionesTrofeo[TrophyManager["SuperarSalasMismaPartida"]][i][2] then
			ObjetivosPuntuacionesTrofeo[TrophyManager["SuperarSalasMismaPartida"]][i][1] = 0

			pos = 3 * (TrophyManager["SuperarSalasMismaPartida"] - 1) + i

			trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..toString(pos).."/Titulo"):
			setProperty("Text","0")
		end

		if ObjetivosPuntuacionesTrofeo[TrophyManager["PuntuacionPartida"]][i][1] < ObjetivosPuntuacionesTrofeo[TrophyManager["PuntuacionPartida"]][i][2] then
			ObjetivosPuntuacionesTrofeo[TrophyManager["PuntuacionPartida"]][i][1] = 0

			pos = 3 * (TrophyManager["PuntuacionPartida"] - 1) + i

			trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..toString(pos).."/Titulo"):
			setProperty("Text","0")
		end
	end
	--Siempre que se empieza una partida, tambien se sale del nivel
	FuncionesTrophyManager.nivelNuevo()

end

function FuncionesTrophyManager.nivelNuevo()

	local pos = 0
	--Aqui reiniciaremos todas las puntuaciones que tengan que ver con el nivel
	for i = 1, 3, 1 do

		if ObjetivosPuntuacionesTrofeo[TrophyManager["MatarEnemigosMismoNivel"]][i][1] < ObjetivosPuntuacionesTrofeo[TrophyManager["MatarEnemigosMismoNivel"]][i][2] then
			ObjetivosPuntuacionesTrofeo[TrophyManager["MatarEnemigosMismoNivel"]][i][1] = 0

			pos = 3 * (TrophyManager["MatarEnemigosMismoNivel"] - 1) + i

			trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..toString(1).."/Titulo"):
			setProperty("Text","0")
		end
	end

end

