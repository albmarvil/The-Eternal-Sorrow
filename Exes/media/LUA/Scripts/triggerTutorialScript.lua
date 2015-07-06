-- author: Cesar Manuel Paz Guzman

class 'triggerTutorialScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function triggerTutorialScript:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)
end

function triggerTutorialScript:spawn(entityInfo)

	return true
end

function triggerTutorialScript:activate()

	return true
end

function triggerTutorialScript:deactivate()
	HUD.hudWindow:getChild("tutorialBillboard"):setProperty("Visible", "false")
end

function triggerTutorialScript:respawn(entityInfo)

	return true
end


function triggerTutorialScript:tick(msecs)

end

function triggerTutorialScript:touched(other)

	if (other ~= nil and other:getType() == "Player") then

		HUD.hudWindow:getChild("tutorialBillboard"):setProperty("Visible", "true")

	end
end

function triggerTutorialScript:untouched(other)

	if (other ~= nil and other:getType() == "Player") then

		--Aqui haremos que desaparezca
		HUD.hudWindow:getChild("tutorialBillboard"):setProperty("Visible", "false")
	end
end