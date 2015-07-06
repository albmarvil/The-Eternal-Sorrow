-- author: Cesar Manuel Paz Guzman

--Contiene las funciones relacionadas con el GUI del menu principal

--Manejador de la ventana del menu
menuWindow = nil

function GUIMenuInit()
	menuWindow = GUI.Windows:loadLayoutFromFile("Menu.layout");

	--En layout estan definidos todos los elementos por nombre, entonces le pedimos a la ventana el componente que se llama Start
	--Le pedi al menuWindow el hijo start, y me lo quedo. Y me subscribo al evento onclick. 
	local bStart = menuWindow:getChildElement("Start")
	local bExit = menuWindow:getChildElement("Exit")
	local bOptions = menuWindow:getChildElement("Option")
	local bTrophy = menuWindow:getChildElement("Trophy")
	local bcreditos = menuWindow:getChildElement("Creditos")

	bStart:subscribeEvent("Clicked", "GUIMenuOnStartPressed")
	bExit:subscribeEvent("Clicked", "GUIMenuOnExitPressed")
	bOptions:subscribeEvent("Clicked", "GUIOptions")
	bTrophy:subscribeEvent("Clicked", "GUITrophy")
	bcreditos:subscribeEvent("Clicked", "GUICreditos")
end

function GUIMenuRelease()

end

function GUIMenuResume()
	GUI.GUIContext:setRootWindow(menuWindow);

	GUI.Mouse:show()

	menuWindow:setVisible(true)
	menuWindow:activate()
end

function GUIMenuPause()
	menuWindow:deactivate()
	menuWindow:setVisible(false)

	GUI.Mouse:hide()
end

function GUIMenuActivate()

	GUI.GUIContext:setRootWindow(menuWindow);

	GUI.Mouse:show()

	menuWindow:setVisible(true)
	menuWindow:activate()

end

function GUIMenuDeactivate()

	menuWindow:deactivate()
	menuWindow:setVisible(false)

	GUI.Mouse:hide()
end

-- GESTION DE EVENTOS

function GUIMenuOnStartPressed()
	--Esto me devuelve un bool, si es ok tambien me duvuelve state
	musicManager:stopAllSounds()
	guiSounds:playSound("/Varios/IniciarPartida")
	local ok = app:ChangeToState("game")
	if ok then
		Log.Debug("Cambiado con exito al estado game")
	else
		Log.Error("Imposible activar el estado de juego")
	end

end

function GUIMenuOnExitPressed()

	FuncionesTrophyManager.Release()

	configurationManager:saveConfig()
	
	app:exitRequest()
end

function GUIOptions()
	--Push de option
	app:push("option")
end

function GUIMenuAddButton()
	local bMenu = gameOverWindow:getChildElement("Return")
	bMenu:subscribeEvent("Clicked", "GUIGameOverOnReturnPressed")
end

function GUITrophy()
	app:push("trophy")
end

function GUICreditos()
	app:push("creditos")
end