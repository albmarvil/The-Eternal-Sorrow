--author: Cesar Manuel Paz Guzman

--Contiene las funciones relacionadas con el GUI del GameOver

--Manejador de la ventana del game Over
gameOverWindow = { enabled = false,}
function GUIGameOverInit()
	
end

function GUIGameOverRelease()

end

function GUIGameOverActivate()

	gameOverWindow.enabled = true

	GUIPauseActivate()

	gameOverWindow.enabled = false
end

function GUIGameOverDeactivate()

	GUIPauseDeactivate()

	--GUI.GUIContext:setRootWindow(menuWindow);
end
