-- author: Cesar Manuel Paz Guzman

creditoTable = 
{
}

creditosWindow = nil
 
function GUICreditosInit()
	creditosWindow = GUI.Windows:loadLayoutFromFile("Creditos.layout")

	--Ponemos los textos en cada una de las ventanas correspondiente al texto

	creditosWindow:getChild("texto"):setProperty("Text",Creditos[1])
	creditoTable.areaInicial = creditosWindow:getChild("texto"):getProperty("Area")
	creditoTable.areaInicialSai = creditosWindow:getChild("sai"):getProperty("Area")

	--Esta está separada, dado que es la imagen final
	--creditoTable[table.getn(Creditos)+1] = creditosWindow:getChild("Creditos2"):getChildAtIdx(table.getn(Creditos)):getProperty("Area")
end

function GUICreditosRelease()

end

function GUICreditosActivate()

	GUI.GUIContext:setRootWindow(creditosWindow);

	GUI.Mouse:show()

	creditosWindow:setVisible(true)
	creditosWindow:activate()

	--Inicializamos el area de cada texto, para que vuelva a empezar desde 0
	creditosWindow:getChild("texto"):setProperty("Area",creditoTable.areaInicial)	
	creditosWindow:getChild("sai"):setProperty("Area",creditoTable.areaInicialSai)	
end

function GUICreditosDeactivate()

	creditosWindow:deactivate()
	creditosWindow:setVisible(false)

	GUI.Mouse:hide()
end
