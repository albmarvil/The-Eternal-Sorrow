--Script que controla el paso por la puerta, herede de CScritpEntity
class 'BulletDartCanibalSpecialScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function BulletDartCanibalSpecialScript:__init()
	CScriptEntity.__init(self)
	self.rangeCanibal = 0
	self.range2 = 0
end

function BulletDartCanibalSpecialScript:spawn(entityInfo)
	
	if entityInfo:hasAttribute("rangeCanibal") then
		self.rangeCanibal = entityInfo:getFloatAttribute("rangeCanibal")
		self.range2 = self.rangeCanibal * self.rangeCanibal
	end
	return true
end

function BulletDartCanibalSpecialScript:activate()
	 
	return true
end

function BulletDartCanibalSpecialScript:deactivate()
	
end

function BulletDartCanibalSpecialScript:touched(other)
	
	local posImpact = self:getOwner():getPosition()

	--Log.Debug("posimpact:" .. posImpact.x .."," ..posImpact.y)
	--Log.Debug("RangeCanibal: " ..self.rangeCanibal.x .. ", " ..self.rangeCanibal.y)

	entityfactory:createEntityByType("HumoVioleta", posImpact, GameMgr.map)
	
	for id, ent in pairs(SpawnerMgr.enemVivos) do
		local dirX = ent:getPosition().x - posImpact.x
		local dirY = ent:getPosition().y - posImpact.y
		local dist = dirX*dirX + dirY*dirY
		
		-- Log.Debug("Dist:" .. dist .. ". Range2: " ..self.range2)

		if dist < self.range2 then
			SendAltStateMessage("Canibal", ent)
		end  
	end

end

function BulletDartCanibalSpecialScript:respawn(entityInfo)
	if entityInfo:hasAttribute("rangeCanibal") then
		self.rangeCanibal = entityInfo:getFloatAttribute("rangeCanibal")
		self.range2 = self.rangeCanibal * self.rangeCanibal
	end
	return true
end


function BulletDartCanibalSpecialScript:tick(msecs)
end