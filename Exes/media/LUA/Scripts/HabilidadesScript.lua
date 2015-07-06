-- author: Cesar Manuel Paz Guzman

class 'HabilidadesScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function HabilidadesScript:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)
end

function HabilidadesScript:spawn(entityInfo)

--	Log.Debug("PowerUpSalto spawneado")
	if self:getOwner():getEntityInfo():hasAttribute("skill_name") then
		self.skill_name = self:getOwner():getEntityInfo():getStringAttribute("skill_name") 
	end

	if self:getOwner():getEntityInfo():hasAttribute("skill_name_desc") then
		self.skill_name_desc = self:getOwner():getEntityInfo():getStringAttribute("skill_name_desc") 
	end

	if self:getOwner():getEntityInfo():hasAttribute("skill_description") then
		self.skill_description = self:getOwner():getEntityInfo():getStringAttribute("skill_description") 
	end
	if self:getOwner():getEntityInfo():hasAttribute("icono") then
		self.icono = self:getOwner():getEntityInfo():getStringAttribute("icono") 
	end
	return true
end

function HabilidadesScript:activate()

	return true
end

function HabilidadesScript:deactivate()

end

function HabilidadesScript:respawn(entityInfo)

	if self:getOwner():getEntityInfo():hasAttribute("skill_name") then
		self.skill_name = self:getOwner():getEntityInfo():getStringAttribute("skill_name") 
	end
	if self:getOwner():getEntityInfo():hasAttribute("skill_description") then
		self.skill_description = self:getOwner():getEntityInfo():getStringAttribute("skill_description") 
	end
	if self:getOwner():getEntityInfo():hasAttribute("icono") then
		self.icono = self:getOwner():getEntityInfo():getStringAttribute("icono") 
	end

	if self:getOwner():getEntityInfo():hasAttribute("skill_name_desc") then
		self.skill_name_desc = self:getOwner():getEntityInfo():getStringAttribute("skill_name_desc") 
	end
	return true
end


function HabilidadesScript:tick(msecs)

end

function HabilidadesScript:touched(other)

	if (other ~= nil and other:getType() == "Player") then
		
		CartelesOcupados.mostrarCartel(self.skill_name_desc, self.skill_description, self.icono)
		
		self:sendMessageSETHABILIDAD(self.skill_name, other)

		HUDPause.agregarHabilidad(self.skill_name, self.icono) 

		HUDPause.agregarTooltipHabilidad(self.skill_name, self.skill_description)
		
		entityfactory:deferredDeleteEntity(self:getOwner())

		GameMgr.estadisticasPartida.numeroHabilidadesCogidas = GameMgr.estadisticasPartida.numeroHabilidadesCogidas +1

		FuncionesSistemaPuntuacion.puntosObjetoRecogible()
	end
end