--Saw hereda de CscritpEntity
class 'ChestOpenerScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function ChestOpenerScript:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)
	self.touchedChest = nil
end

function ChestOpenerScript:spawn(entityInfo)
	
	return true
end

function ChestOpenerScript:respawn(entityInfo)

	return true
end

function ChestOpenerScript:activate()

	return true
end

function ChestOpenerScript:deactivate()

end

function ChestOpenerScript:tick(msecs)

end

function ChestOpenerScript:touched(other)
	-- body
	if other ~= nil then
		if other:getTag() == "chest" then
			-- Log.Debug("He tocado un cofre!!! "..other:getName())
			self.touchedChest = other
		end
	end
end

function ChestOpenerScript:untouched( other )
	-- body
	if other ~= nil then
		if other:getTag() == "chest" then
			-- Log.Debug("He dejado de tocar un cofre!!! "..other:getName())
			self.touchedChest = nil
		end
	end
end

function ChestOpenerScript:use(pressed)
	-- body

	if self.touchedChest ~= nil and pressed == true then
		-- Log.Debug("Abro el cofre: "..self.touchedChest:getName())
		sendOpenChestMessage(self.touchedChest)
	elseif self.touchedChest ~= nil and pressed == false then
		sendCancelOpenChestMessage(self.touchedChest)
	end

end
