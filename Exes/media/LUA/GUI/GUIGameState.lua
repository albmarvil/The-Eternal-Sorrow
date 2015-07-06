-- author: Cesar Manuel Paz Guzman

HUD = {
	isActivate = false,
}


function HUD.tick(msecs)

	if isActivate == true then

		CartelesOcupados.updateCartelesHabilidad(msecs)

		if CartelCentralHUD.isFinished == false then --Si necesito hacer su tick lo hago
			CartelCentralHUD.udpateCartelCentral(msecs)
		end

		FuncionesTrophyManager.tick(msecs)
	end

end

function GUIGameRelease()

end

function GUIGameInit()

	--Log.Debug("GUIGAMEINIT")
	HUD.hudWindow = GUI.Windows:loadLayoutFromFile("Time.layout")

	scriptMgr:loadScript(Paths.luaGUIPath.."VidaDineroGUI.lua")

	scriptMgr:loadScript(Paths.luaGUIPath.."CartelesInformacionObjetos.lua")
	
	scriptMgr:loadScript(Paths.luaGUIPath.."CartelCentralGUI.lua")

end

function GUIGameResume()

	GUI.GUIContext:setRootWindow(HUD.hudWindow);

	GUI.Mouse:setDefaultImage("cursorImages/mirilla")
	GUI.Mouse:setImage("cursorImages/mirilla")

	GUI.Mouse:setExplicitRenderSize(CEGUI.Sizef(20, 20))
	GUI.Mouse:show()

	HUD.hudWindow:setVisible(true)
	HUD.hudWindow:activate()

	isActivate = true

end

function GUIGamePause()

	HUD.hudWindow:deactivate()
	HUD.hudWindow:setVisible(false)

	GUI.Mouse:setDefaultImage("OgreTrayImages/MouseArrow");
	GUI.Mouse:setImage("OgreTrayImages/MouseArrow");
	
	isActivate = false

end

function GUIGameActivate()

	--Log.Debug("GUIGAMEACTIVATE")

	GUI.GUIContext:setRootWindow(HUD.hudWindow);

	GUI.Mouse:setDefaultImage("cursorImages/mirilla")
	GUI.Mouse:setImage("cursorImages/mirilla")

	GUI.Mouse:setExplicitRenderSize(CEGUI.Sizef(20, 20))
	GUI.Mouse:show()

	HUD.hudWindow:setVisible(true)
	HUD.hudWindow:activate()

	isActivate = true
	
	scriptMgr:loadScript(Paths.luaGUIPath.."ArmasGUI.lua")

	HUDPause.nuevoJuego()

	ArmasHUD.Activate()

	LifeHUD.Activate()

	LifeHUD.setLife(100)
end


function GUIGameDeactivate()

	--Log.Debug("GUIGAMEDEACTIVATE")

	HUD.hudWindow:deactivate()
	HUD.hudWindow:setVisible(false)


	GUI.Mouse:hide()
	GUI.Mouse:setDefaultImage("OgreTrayImages/MouseArrow");
	GUI.Mouse:setImage("OgreTrayImages/MouseArrow");
	
	isActivate = false

	ArmasHUD.Deactivate()

	LifeHUD.Deactivate()

	CartelesOcupados.Deactivate()

	CartelCentralHUD.Deactivate()

end

function HUD.setVisible(boolean)
	HUD.hudWindow:setVisible(boolean)

	if boolean == false then
		CartelCentralHUD.Deactivate()
		GUI.Mouse:hide()
	else
		GUI.Mouse:show()
	end
end
