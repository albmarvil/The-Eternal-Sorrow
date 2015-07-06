-- author: Cesar Manuel Paz Guzman

--TABLA GUI global con variables que apuntan a los 
--distintos subsistemas del GUI

-- CEGUI se renderiza a través de directX y openGL. 

Log.Debug("initGUI cargado")
GUI = {
	System = CEGUI.System:getSingleton(), --El sistema de CEGUI se inicializa con esta llamada. 
	Schemes = CEGUI.SchemeManager:getSingleton(),
	Fonts = CEGUI.FontManager:getSingleton(),
	Windows = CEGUI.WindowManager:getSingleton(),
	GUIContext = CEGUI.System:getSingleton():getDefaultGUIContext(),
	UDim = CEGUI.UDim,
}

--Añado aqui, por que el GUIContext se inicializa dentro de la tabla. 
GUI.Mouse = GUI.GUIContext:getMouseCursor()

--Configuracion
GUI.Schemes:createFromFile("Propio.scheme")
GUI.Schemes:createFromFile("GlossySerpent.scheme")
GUI.Schemes:createFromFile("TaharezLook.scheme")
GUI.Schemes:createFromFile("OgreTray.scheme")
GUI.Schemes:createFromFile("Generic.scheme")
GUI.Schemes:createFromFile("VanillaSkin.scheme")
GUI.Schemes:createFromFile("WindowsLook.scheme")
GUI.Schemes:createFromFile("cursor.scheme")

local defaultFont = GUI.Fonts:createFromFile("Jura-13.font")
GUI.GUIContext:setDefaultFont(defaultFont)

GUI.Mouse:setDefaultImage("OgreTrayImages/MouseArrow");
GUI.Mouse:setImage("OgreTrayImages/MouseArrow");

GUI.GUIContext:setDefaultTooltipType("GlossySerpent/Tooltip")

--Cargamos el resto de funciones relacoinadas con ell GUI desde aqui
scriptMgr:loadScript(Paths.luaGUIPath.."GUIMenuPrincipal.lua")
scriptMgr:loadScript(Paths.luaGUIPath.."VidaDineroGUI.lua")
scriptMgr:loadScript(Paths.luaGUIPath.."GUIGameState.lua")
scriptMgr:loadScript(Paths.luaGUIPath.."GUIPauseState.lua")
scriptMgr:loadScript(Paths.luaGUIPath.."GUIGameOver.lua")
scriptMgr:loadScript(Paths.luaGUIPath.."GUIOptions.lua")
scriptMgr:loadScript(Paths.luaGUIPath.."textos.lua")
scriptMgr:loadScript(Paths.luaGUIPath.."GUITrophyState.lua")
scriptMgr:loadScript(Paths.luaGUIPath.."CreditosGUI.lua")
