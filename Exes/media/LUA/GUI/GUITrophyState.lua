-- author: Cesar Manuel Paz Guzman


function GUItrophyInit()
	trophyWindow = GUI.Windows:loadLayoutFromFile("Trophy.layout");
	local bExit = trophyWindow:getChildElement("Root/menu")

	bExit:subscribeEvent("Clicked", "GUITrophyMenuPrincipal")

	GUITrophyAreas()
	GUITrophyTooltips()
	FuncionesTrophyManager.Init()
	
end

function GUItrophyRelease()

end

function GUItrophyActivate()

	GUI.GUIContext:setRootWindow(trophyWindow);

	GUI.Mouse:show()

	FuncionesTrophyManager.actualizarImagenesTrofeo()

	trophyWindow:setVisible(true)
	trophyWindow:activate()

end


function GUItrophyDeactivate()

	trophyWindow:deactivate()
	trophyWindow:setVisible(false)

	GUI.Mouse:hide()
end

function GUITrophyMenuPrincipal()
	app:pop()
end

function GUITrophyTooltips()
	local ruta = "Root/TabControl/TabGaleria/ScrollablePane/Trofeo"

	for i=1, 66, 1 do
		trophyWindow:getChildElement(ruta..i):setProperty("TooltipText", trofeos[i])
	end
end

function GUITrophyAreas()
	local pos = 1
	for i=0,16,1 do --17 filas
		for j=0,3,1 do --4 columnas
			trophyWindow:getChildElement("Root/TabControl/TabGaleria/ScrollablePane/Trofeo"..pos):setProperty("Area",
				"{{"..toString(0.05+0.23*j)..",0},{"..toString(0.10+0.25*i)..",0},{"..
				toString(0.25+0.23*j)..",0},{"..toString(0.32+0.25*i)..",0}}")
			pos = pos + 1
		end
	end
end