-- author: Cesar Manuel Paz Guzman

class 'ArmaHUDScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function ArmaHUDScript:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)

	self.nombre = "-"
end

function ArmaHUDScript:spawn(entityInfo)

	if self:getOwner():getEntityInfo():hasAttribute("icono") then
		self.icono = self:getOwner():getEntityInfo():getStringAttribute("icono") 
	end

	if self:getOwner():getEntityInfo():hasAttribute("descripcionArma") then
		self.descripcion = self:getOwner():getEntityInfo():getStringAttribute("descripcionArma") 
	end

	--[[if self:getOwner():getEntityInfo():hasAttribute("maxDelaySpecialAtk") then
		self.maxSpecialDelay = self:getOwner():getEntityInfo():getIntAttribute("maxDelaySpecialAtk") 
	end--]]
	self.brazo = "null"	
	self.degradacion = 0.072

	if entityInfo:hasAttribute("nombreArma") then
		self.nombre = entityInfo:getStringAttribute("nombreArma")
	end

	Log.Debug("SPAWN")
	return true
end

function ArmaHUDScript:activate()
	self.brazo = "null"	
	self.degradacion = 0.072
	return true
end

function ArmaHUDScript:deactivate()

	self.brazo = "null"
end

function ArmaHUDScript:respawn(entityInfo)
	if self:getOwner():getEntityInfo():hasAttribute("icono") then
		self.icono = self:getOwner():getEntityInfo():getStringAttribute("icono") 
	end
	if self:getOwner():getEntityInfo():hasAttribute("descripcionArma") then
		self.descripcion = self:getOwner():getEntityInfo():getStringAttribute("descripcionArma") 
	end
	--[[if self:getOwner():getEntityInfo():hasAttribute("maxDelaySpecialAtk") then
		self.maxSpecialDelay = self:getOwner():getEntityInfo():getIntAttribute("maxDelaySpecialAtk") 
	end	--]]

	self.degradacion = 0.072
	if entityInfo:hasAttribute("nombreArma") then
		self.nombre = entityInfo:getStringAttribute("nombreArma")
	end

	return true
end

function ArmaHUDScript:tick(msecs)
	if self.brazo ~= "null" and self.degradacion < 0.95 then
		--si ha variado la habilidad recalculamos los datos
		if Data_Player.changeStatR and self.brazo == "Right" then
			--calculamos el delay máximo
			self.degradacion = 0.072
			
			self.maxSpecialDelay = Data_Player.player_stats.velocidad_ataque_especial * Data_Player.player_info.multiplicador_velocidad_ataque_derecha
			Data_Player.changeStatR = false
			
			self.degradacion2 = (0.95 - self.degradacion)/self.maxSpecialDelay
		elseif self.brazo == "Left" and Data_Player.changeStatL then
			--calculamos el delay máximo
			self.degradacion = 0.072
			
			self.maxSpecialDelay = Data_Player.player_stats.velocidad_ataque_especial * Data_Player.player_info.multiplicador_velocidad_ataque_izquierda
			Data_Player.changeStatL = false
			
			self.degradacion2 = (0.95 - self.degradacion)/self.maxSpecialDelay
		end
	end
	
		self.degradacion = self.degradacion + self.degradacion2*msecs
		ArmasHUD.updateArms(self.brazo, self.degradacion)
end

function ArmaHUDScript:arm(brazo)

	Log.Debug("MOSTRAR CARTEL")

	self:setArm(brazo)

	CartelesOcupados.mostrarCartel(self.nombre, self.descripcion , self.icono)

end

---FUNCION QUE SE LLAMA AL PASAR DE NIVEL. Se llama esta y no la anterior, para no mostrar el cartel. 
function ArmaHUDScript:setArm(brazo)
	Log.Debug("NO MUESTRA CARTEL")
	self.brazo = brazo

	self.brazo = brazo
	self.degradacion = 0.072
	ArmasHUD.setearTransparencia(self.brazo)

	-- self.nombre = self:getOwner():getType()
	
	
	--calculamos el delay máximo
	if self.brazo == "Right" then
		self.maxSpecialDelay = Data_Player.player_stats.velocidad_ataque_especial * Data_Player.player_info.multiplicador_velocidad_ataque_derecha
	else
		self.maxSpecialDelay = Data_Player.player_stats.velocidad_ataque_especial * Data_Player.player_info.multiplicador_velocidad_ataque_izquierda
	end
	
	self.degradacion2 = (0.95 - self.degradacion)/self.maxSpecialDelay

	ArmasHUD[self.brazo].area:setProperty("Image", self.icono)

	HUDPause.agregarArma(self.brazo, self.icono, self.descripcion)
	
	ArmasHUD[self.brazo].transparencia:setVisible(true)

	ArmasHUD[self.brazo].entidad = self

end

function ArmaHUDScript:specialAtkLaunched()
	ArmasHUD.setearTransparencia(self.brazo)
	self.degradacion = 0.072
end


function ArmaHUDScript:calculateAreaTransparecia()

end