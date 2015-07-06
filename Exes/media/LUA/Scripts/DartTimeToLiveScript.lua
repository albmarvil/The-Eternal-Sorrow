--Saw hereda de CscritpEntity
class 'DartTimeToLiveScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function DartTimeToLiveScript:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)
	self.timeToLive = 0
	self.acum = 0
end

function DartTimeToLiveScript:spawn(entityInfo)

	if entityInfo:hasAttribute("time_to_live") then
		self.timeToLive = entityInfo:getFloatAttribute("time_to_live")* 1000
	end
	return true
end

function DartTimeToLiveScript:respawn(entityInfo)
	if entityInfo:hasAttribute("timeToLive") then
		self.timeToLive = entityInfo:getFloatAttribute("time_to_live")* 1000
	end
	return true
end

function DartTimeToLiveScript:activate()
	self.acum = 0
	return true
end

function DartTimeToLiveScript:deactivate()

end

function DartTimeToLiveScript:tick(msecs)

	self.acum = self.acum + msecs

	if self.acum > self.timeToLive then
		entityfactory:deferredDeleteEntity(self:getOwner())

		entityfactory:createEntityByType("Chispa", self:getOwner():getPosition(), GameMgr.map)
	end

end