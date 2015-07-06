
class 'LaserBallScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function LaserBallScript:__init()
	CScriptEntity.__init(self)

	self.maxTimeUntilExplode = 0
	self.multiplRange = 0
	self.multiplDmg = 0
	self.baseRange = 0
	self.baseDmg = 0
	self.timeUntilExplode = 0
	self.touches = 0
end

function LaserBallScript:spawn(entityInfo)
	if entityInfo:hasAttribute("timeUntilExplode") then
		self.maxTimeUntilExplode = entityInfo:getIntAttribute("timeUntilExplode")
	end

	if entityInfo:hasAttribute("baseRange") then
		self.baseRange = entityInfo:getFloatAttribute("baseRange")
	end

	if entityInfo:hasAttribute("baseDmg") then
		self.baseDmg = entityInfo:getFloatAttribute("baseDmg")
	end

	if entityInfo:hasAttribute("multiplRange") then
		self.multiplRange = entityInfo:getFloatAttribute("multiplRange")
	end

	if entityInfo:hasAttribute("multiplDmg") then
		self.multiplDmg = entityInfo:getFloatAttribute("multiplDmg")
	end

	return true
end

function LaserBallScript:respawn(entityInfo)
	if entityInfo:hasAttribute("timeUntilExplode") then
		self.maxTimeUntilExplode = entityInfo:getIntAttribute("timeUntilExplode")
	end

	if entityInfo:hasAttribute("baseRange") then
		self.baseRange = entityInfo:getFloatAttribute("baseRange")
	end

	if entityInfo:hasAttribute("baseDmg") then
		self.baseDmg = entityInfo:getFloatAttribute("baseDmg")
	end

	if entityInfo:hasAttribute("multiplRange") then
		self.multiplRange = entityInfo:getFloatAttribute("multiplRange")
	end

	if entityInfo:hasAttribute("multiplDmg") then
		self.multiplDmg = entityInfo:getFloatAttribute("multiplDmg")
	end

	return true
end

function LaserBallScript:activate()
	self.timeUntilExplode = 0
	self.touches = 0

	--entityfactory:createEntityByType("LaserBallParticle", self, GameMgr.map)

	return true
end

function LaserBallScript:deactivate()
end

function LaserBallScript:laserToLaserBall()
	self.touches = self.touches + 1
end

function LaserBallScript:touched(other)
	local dmgEnt = self.baseDmg + self.touches * self.multiplDmg
	dmgEnt = dmgEnt / 2

	SendDamagedMessage(other, dmgEnt)

	local dir = Vector3(other:getPosition().x - self:getOwner():getPosition().x,other:getPosition().y - self:getOwner():getPosition().y ,0)
	sendPushMessage(other, 0.3, dir)
	return true
end

function LaserBallScript:tick(msecs)

	self.timeUntilExplode = self.timeUntilExplode + msecs

	if self.timeUntilExplode >= self.maxTimeUntilExplode then
		self:explode()

	end
end

function LaserBallScript:explode()
	local posImpact = self:getOwner():getPosition()

	local range = self.baseRange + self.touches * self.multiplRange
	local dmgEnt = self.baseDmg + self.touches * self.multiplDmg

	local range2 = range * range

	-- ENTIDAD DEBUG VISUAL DEL RANGO
	-- local entity = MapEntity("DebugRange")
	-- entity:SetType("DebugRange")

	-- entity:SetAttrib("physic_radius", toString(range))
	-- entity:SetAttrib("position", toString({posImpact.x, posImpact.y, posImpact.z}))
	-- NewEntity(entity, GameMgr.map)
	-----------------------------------

	--Log.Debug("RANGE: " .. self.baseRange .. " + " .. self.touches .. " * " .. self.multiplRange .. " = " .. range)

	shakeCamera(self:getOwner(), 15, 2)

	local nScale = 0.4 + 0.13 * self.touches

	local part = entityfactory:createEntityByType("ExplosionLaser", posImpact, GameMgr.map)
	scaleParticle(part, nScale, "ExplosionLaser")

	for id, ent in pairs(SpawnerMgr.enemVivos) do
		local dirX = ent:getPosition().x - posImpact.x
		local dirY = ent:getPosition().y - posImpact.y
		local dist = dirX*dirX + dirY*dirY
	
		if dist < range2 then
			SendDamagedMessage(ent, dmgEnt)

			local dir = Vector3(dirX, dirY ,0)
			sendPushMessage(ent, 0.8, dir)

			local rnd = Random(100)

			if rnd >= 80 then
				SendAltStateMessage("Frozen", ent)
			end
		end  
	end

	entityfactory:deferredDeleteEntity(self:getOwner())
end
