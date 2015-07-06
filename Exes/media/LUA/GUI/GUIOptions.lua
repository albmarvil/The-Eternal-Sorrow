-- author: Cesar Manuel Paz Guzman

--Contiene las funciones relacionadas con el GUI del menu principal

--Manejador de la ventana del menu


option = {}

function GUIOptionInit()
	option.optionWindow = GUI.Windows:loadLayoutFromFile("Option.layout");

	local bMenu = option.optionWindow:getChildElement("Exit")
	local listResolution = option.optionWindow:getChildElement("TabControlOpc/TabVideo/ResListbox")
	local checkFullScreen = option.optionWindow:getChildElement("TabControlOpc/TabVideo/checkbox")
	local checkVsync = option.optionWindow:getChildElement("TabControlOpc/TabVideo/checkbox2")
	local bControl = option.optionWindow:getChildElement("Botones")
	local slVolumeMusic = option.optionWindow:getChildElement("TabControlOpc/TabAudio/Slider")
	local slVolumeEffects = option.optionWindow:getChildElement("TabControlOpc/TabAudio/Slider2")
	local listFSAA = option.optionWindow:getChildElement("TabControlOpc/TabVideo/comboboxFSAA")

	bMenu:subscribeEvent("Clicked", "GUIBackToMenu")
	listResolution:subscribeEvent("ListSelectionAccepted", "UpdateResolution")
	checkFullScreen:subscribeEvent("SelectStateChanged","ChangeFullMode")
	checkVsync:subscribeEvent("SelectStateChanged", "changeVsync")
	bControl:subscribeEvent("Clicked", "GUIControls")
	slVolumeMusic:subscribeEvent("ValueChanged","UpdateVolumeMusic")
	slVolumeEffects:subscribeEvent("ValueChanged","UpdateVolumeEffects")
	listFSAA:subscribeEvent("ListSelectionAccepted", "UpdateFSAA")

	option.tutorialCompletado = false
end

function GUIOptionRelease()

end

function GUIOptionActivate()

	GUI.GUIContext:setRootWindow(option.optionWindow);

	GUI.Mouse:show()

	option.optionWindow:setVisible(true)
	option.optionWindow:activate()

	if option.tutorialCompletado == true then
		tutorialComplete()
	end
end


function GUIOptionDeactivate()

	option.optionWindow:deactivate()
	option.optionWindow:setVisible(false)

	GUI.Mouse:hide()
end

function GUIBackToMenu()
	app:pop()
end

function UpdateResolution()
	Log.Debug("UPDATE RESOLUTION")
	configurationManager:updateResolution()
end

function ChangeFullMode()
	configurationManager:changeFullMode()
end

function changeVsync()
	configurationManager:vsyncActive()
end

function GUIControls()
	app:push("optionControls")
end

function UpdateVolumeMusic()
	configurationManager:updateVolumeMusic()
end

function UpdateVolumeEffects()
	configurationManager:updateVolumeEffects()
end

function UpdateFSAA()
	configurationManager:updateFSAA()
end

function GUIControlsInit()
	option.controls = GUI.Windows:loadLayoutFromFile("OptionControls.layout")
	
	local buttonVolver = option.controls:getChildElement("Volver")
	buttonVolver:subscribeEvent("Clicked", "GUIBackToMenu")
end

function GUIControlsActivate()
	GUI.GUIContext:setRootWindow(option.controls);

	GUI.Mouse:show()

	option.controls:setVisible(true)
	option.controls:activate()

	
end

function GUIControlsDeactivate()
	option.controls:deactivate()
	option.controls:setVisible(false)

	GUI.Mouse:hide()
end
