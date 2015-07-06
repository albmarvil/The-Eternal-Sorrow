--Saw hereda de CscritpEntity
class 'SoulScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function SoulScript:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)

	self.souls = 0
	self.feedback = "-"
end

function SoulScript:spawn(entityInfo)

	if entityInfo:hasAttribute("souls") then
		self.souls = entityInfo:getIntAttribute("souls")
	end

	if entityInfo:hasAttribute("feedbackParticle") then
		self.feedback = entityInfo:getStringAttribute("feedbackParticle")
	end
	return true
end

function SoulScript:respawn(entityInfo)
	if entityInfo:hasAttribute("souls") then
		self.souls = entityInfo:getIntAttribute("souls")
	end

	if entityInfo:hasAttribute("feedbackParticle") then
		self.feedback = entityInfo:getStringAttribute("feedbackParticle")
	end
	return true
end

function SoulScript:activate()

	return true
end

function SoulScript:deactivate()

end

function SoulScript:tick(msecs)

end

function SoulScript:touched( other )

	if other ~= nil then
		if other:getType() == "Player" then
			-- Log.Warning("La mecanica de recogida de almas no esta finalizada del todo")

			local vidaActual = Data_Player.player_info.vidaActual
			
			local contador = HadHability(Nombre_Habilidad["CPorcentajeAlmas"])
			-- print("el contador de almas es "..contador)
			local curacion = Data_Player.player_stats.vida * (self.souls + contador) * 0.01
			
			
			-- OJO! que estamos curando por encima de un limite máximo que no hay
			-- OJO! la curacion es un valor float, lo truncamos para que sea un valor entero
			if math.floor(curacion) < 1 then
				vidaActual = vidaActual + math.ceil(curacion)
			else
				vidaActual = vidaActual + math.floor(curacion)
			end

			if vidaActual >= Data_Player.player_stats.vida then
				vidaActual = Data_Player.player_stats.vida
			end

			-- Log.Debug("Vida actual tras una curacion de "..curacion.." - "..vidaActual)

			Data_Player.player_info.vidaActual = vidaActual
			-- LifeHUD.setLife(vidaActual)
			SendStatsMenssage("vidaActual", vidaActual, GameMgr.player)

			-- if self.souls == 5 then
			-- 	entityfactory:createEntityByType("AlmaAzulFeedback", self:getOwner():getPosition(), GameMgr.map)
			-- elseif self.souls == 10 then
			-- 	entityfactory:createEntityByType("AlmaVerdeFeedback", self:getOwner():getPosition(), GameMgr.map)
			-- elseif self.souls == 25 then
			-- 	entityfactory:createEntityByType("AlmaRojaFeedback", self:getOwner():getPosition(), GameMgr.map)
			-- end

			entityfactory:createEntityByType(self.feedback, self:getOwner():getPosition(), GameMgr.map)
			--borramos el objeto usable del alma
			entityfactory:deferredDeleteEntity(self:getOwner())



		end
	end
			

end