--Script que controla el paso por la puerta, herede de CScritpEntity
class 'HumoSangreScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function HumoSangreScript:__init()
	CScriptEntity.__init(self)
end

function HumoSangreScript:spawn(entityInfo)
	
	if entityInfo:hasAttribute("rangeBlood") then
		self.rangeBlood = entityInfo:getFloatAttribute("rangeBlood")
		self.range2 = self.rangeBlood * self.rangeBlood
	end
	return true
end

function HumoSangreScript:activate()
	 
	self.tickInterval = 100
	self.tickTime = 0
	return true
end

function HumoSangreScript:deactivate()
	
end

function HumoSangreScript:touched(other)
end

function HumoSangreScript:respawn(entityInfo)
	
	if entityInfo:hasAttribute("rangeBlood") then
		self.rangeBlood = entityInfo:getFloatAttribute("rangeBlood")
		self.range2 = self.rangeBlood * self.rangeBlood
	end
	return true
end


function HumoSangreScript:tick(msecs)

	self.tickTime = self.tickTime + msecs

	if self.tickTime >= self.tickInterval then

		self.tickTime = 0

		local posImpact = self:getOwner():getPosition()

		--ENTIDAD DEBUG VISUAL DEL RANGO
		-- local entity = MapEntity("DebugRange")
		-- entity:SetType("DebugRange")

		-- entity:SetAttrib("physic_radius", toString(self.rangeBlood))
		-- entity:SetAttrib("position", toString({posImpact.x, posImpact.y, posImpact.z}))
		-- Log.Debug("2")
		-- NewEntity(entity, GameMgr.map)
		
		for id, ent in pairs(SpawnerMgr.enemVivos) do
			local dirX = ent:getPosition().x - posImpact.x
			local dirY = ent:getPosition().y - posImpact.y
			local dist = dirX*dirX + dirY*dirY
			
			-- Log.Debug("Dist:" .. dist .. ". Range2: " ..self.range2)

			if dist < self.range2 then
				SendAltStateMessage("Poison", ent)
				SendAltStateMessage("Slowdown", ent)
			end  
		end
	end
	
end