-- author: Cesar Manuel Paz Guzman

LifeHUD = 
{
	areaLifeInicial = HUD.hudWindow:getChildElement("Life/LifeRestante"):getProperty("Area"),

}

function LifeHUD.Activate()
	Log.Debug("Activate VIDA")
	HUD.hudWindow:getChildElement("Life/LifeRestante"):setProperty("Area",LifeHUD.areaLifeInicial)
	HUD.hudWindow:getChildElement("Life/textoVida"):setProperty("Text",toString(Data_Player.player_info.vidaActual).."/"..toString(Data_Player.player_stats.vida))
end

function LifeHUD.Deactivate()
	LifeHUD.ActualizarDinero(0)
end

function LifeHUD.ActualizarDinero(guita)
	HUD.hudWindow:getChild("Money/cantidad"):setProperty("Text", "x"..toString(guita))
end


function LifeHUD.setLife(n)

--	Log.Debug("Vida Maxima2: "..toString(Data_Player.player_stats.vida))
	
	HUD.hudWindow:getChildElement("Life/textoVida"):setProperty("Text",toString(math.floor(Data_Player.player_info.vidaActual)).."/"..toString(Data_Player.player_stats.vida))
	
	ancho = toString((Data_Player.player_info.vidaActual / Data_Player.player_stats.vida)*0.88 + 0.1) 

	HUD.hudWindow:getChildElement("Life/LifeRestante"):setProperty("Area","{{0.1,0},{0.173016,0},{"..ancho..",0},{0.537302,0}")

	--Log.Debug("ancho: "..toString(ancho).." VIDA Actual: "..toString(Data_Player.player_info.vidaActual))

end



-------DEBUG
DebugHUD = 
{
	

}
function DebugHUD.updateNivel(n)

	
	HUD.hudWindow:getChildElement("DEBUG/nivelMedio"):setProperty("Text", "Lvl: "..toString(n))

end

function DebugHUD.updateSala(n)

	
	HUD.hudWindow:getChildElement("DEBUG/sala"):setProperty("Text", "Nº Sala: "..toString(n))

end

function DebugHUD.updateMochila(actual, total)
	-- body
	HUD.hudWindow:getChildElement("DEBUG/actualMochila"):setProperty("Text", toString(actual))

	HUD.hudWindow:getChildElement("DEBUG/totalMochila"):setProperty("Text", toString(total))
end

function DebugHUD.updateTimepoSpawner(actual, total)
	HUD.hudWindow:getChildElement("DEBUG/tiempoTotalSpawner"):setProperty("Text", toString(math.floor(total)))
	HUD.hudWindow:getChildElement("DEBUG/tiempoActualSpawner"):setProperty("Text", toString(actual))
end

function DebugHUD.updatePromedioMuertes(promedio)

	 local mult = 10^(3 or 0)
  	local aux =  math.floor(promedio * mult + 0.5) / mult
	HUD.hudWindow:getChildElement("DEBUG/promedio"):setProperty("Text", toString(aux))
end


function DebugHUD.updateAuxNumber(auxNumber)

	local mult = 10^(3 or 0)
  	local aux =  math.floor(auxNumber * mult + 0.5) / mult
	HUD.hudWindow:getChildElement("DEBUG/aux"):setProperty("Text", toString(aux))
end