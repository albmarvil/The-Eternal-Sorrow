--author: Cesar Manuel Paz Guzman

class 'heiserScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function heiserScript:__init()
	CScriptEntity.__init(self)

end

function heiserScript:spawn(entityInfo)

	self.delayDamage = 1500

	return true
end

function heiserScript:respawn(entityInfo)
	return true
end

function heiserScript:activate()

	self.trampa = nil
	self.trampa2 = nil
	self.trampa3 = nil

	self.tipo = FuncionesTrampas.escogerTrampa()
	self.sonido = FuncionesTrampas.escogerSonido(self.tipo)
	self.danho = FuncionesTrampas.obtenerDanho(self.tipo)
	self.touched = false
	self.timeStamp = 0

	return true
end

function heiserScript:deactivate()

	if self.trampa ~= nil then

		entityfactory:deferredDeleteEntity(self.trampa)

		if self.tipo ~= "Pinchos" then
			entityfactory:deferredDeleteEntity(self.trampa2)
			entityfactory:deferredDeleteEntity(self.trampa3)
		end
	end

end

function heiserScript:tick(msecs)

	if self.touched == true then
		self.timeStamp = self.timeStamp + msecs
		if self.timeStamp >= self.delayDamage then
			self.timeStamp = 0

			--Aplicamos daño
			self:applyDamage()
		end
	end

end

function heiserScript:touched(other)

	if (other ~= nil and other:getType() == "Player") then

		--Si todavia no nos hemos creado la trampa, nos creamos el feedback de la trampa escogida (particulas, pinchos)
		if self.trampa == nil then

			if self.tipo == "Pinchos" then
				self.trampa = entityfactory:createEntityByType(self.tipo, self:getOwner():getPosition(), GameMgr.map)
			else
				--Si es el player nos creamos la particula de heiser
				self.trampa = entityfactory:createEntityByType(self.tipo, self:getOwner():getPosition(), GameMgr.map)

				self.trampa2 = entityfactory:createEntityByType(self.tipo, Vector3(self:getOwner():getPosition().x,
					self:getOwner():getPosition().y, self:getOwner():getPosition().z + 20), GameMgr.map)

				self.trampa3 = entityfactory:createEntityByType(self.tipo, Vector3(self:getOwner():getPosition().x,
					self:getOwner():getPosition().y, self:getOwner():getPosition().z - 20), GameMgr.map)
			end

		--Si son las particulas, la accion es que empiecen a ejecutarse
		elseif self.tipo ~= "Pinchos" then
			startParticle(self.trampa)
			startParticle(self.trampa2)
			startParticle(self.trampa3)
		end

		if self.tipo == "Pinchos" then
			--Aqui ejecutamos animacion de subida 
			upPinchos(self.trampa)
		end

		--lanzamos el sonido
		startSound(self.sonido, self:getOwner():getName())

		--Aplicamos el primer daño
		self:applyDamage()

		self.touched = true

	end
end

function heiserScript:untouched(other)
	if (other ~= nil and other:getType() == "Player" and self.trampa ~= nil) then

		if self.tipo == "Pinchos" then
			--Aqui ejecutamos animacion de bajada 
			downPinchos(self.trampa)
		else
			stopParticle(self.trampa)
			stopParticle(self.trampa2)
			stopParticle(self.trampa3)
		end

		--paramos el sonido
		stopSound(self.sonido, self:getOwner():getName())

		self.touched = false
		self.timeStamp = 0

	end
end

function heiserScript:applyDamage()

	--Mandamos mensaje de daño
	SendDamagedMessage(GameMgr.player, Data_Player.player_stats.vida * self.danho * 0.01)

	if self.tipo == "Heiser" then
		SendAltStateMessage("Poison", GameMgr.player)
	elseif self.tipo == "HumoVenenoso" then
		SendAltStateMessage("Slowdown", GameMgr.player)
	end
	
	--Aqui poner una chispa de daño

	--Y algun sonido de daño quizas
end