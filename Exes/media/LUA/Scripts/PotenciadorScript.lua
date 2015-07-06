-- author: Cesar Manuel Paz Guzman

class 'PotenciadorScript' (CScriptEntity)


--Atributo privado __init. Se trata del constructor de esa clase
function PotenciadorScript:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)
	self.tipoGeneral = nil
	self.tipoEspecifico = nil
end

function PotenciadorScript:spawn(entityInfo)

	if entityInfo:hasAttribute("tipoGeneral") then
		self.tipoGeneral = entityInfo:getStringAttribute("tipoGeneral") 
	end

	if entityInfo:hasAttribute("tipoEspecifico") then
		self.tipoEspecifico = entityInfo:getStringAttribute("tipoEspecifico")
	end

	if entityInfo:hasAttribute("iconoPause") then
		self.iconoPause = entityInfo:getStringAttribute("iconoPause")
	end
	--self.stats = 

	return true
end

function PotenciadorScript:actualizarMovableText(_self)

	local textoPos = ""
	local textoNeg = ""

	-- Log.Debug("self.tipoGeneral: ".._self.tipoGeneral.." self.tipoEspecifico: ".._self.tipoEspecifico)
	for key, value in pairs(Potenciadores[_self.tipoGeneral].lista[_self.tipoEspecifico].estadisticas) do 
		if value > 0 then
			textoPos = textoPos.."+"..value
			if key == "vida" then
				textoPos = textoPos.."H "
			elseif key == "ataque" then
				textoPos = textoPos.."A "
			elseif key == "velocidad_movimiento" then
				textoPos = textoPos.."S "
			elseif key == "velocidad_ataque" then
				textoPos = textoPos.."AS "
			end
		elseif value < 0 then
			textoNeg = textoNeg..value
			if key == "vida" then
				textoNeg = textoNeg.."H "
			elseif key == "ataque" then
				textoNeg = textoNeg.."A "
			elseif key == "velocidad_movimiento" then
				textoNeg = textoNeg.."S "
			elseif key == "velocidad_ataque" then
				textoNeg = textoNeg.."AS "
			end
		end

	end

	--CREAMOS DOS NUEVAS ENTIDAD CON NEWENTITY, UNA PARA EL TEXTO POSITIVO Y OTRA PARA EL NEGATIVO en caso de tener valores

	if textoPos ~= "" and textoNeg ~= "" then

		local posNueva = Vector3(_self:getOwner():getPosition().x, _self:getOwner():getPosition().y, 40)
		local entityPos = entityfactory:createEntityByType("MovableText", posNueva, GameMgr.map)
		_self:sendMessageMovableText(textoPos, entityPos, true)
		_self:sendMessageSetColorMovableText("green", entityPos, true)

		posNueva = Vector3(_self:getOwner():getPosition().x, _self:getOwner():getPosition().y - 10, 40)
		local entityNeg = entityfactory:createEntityByType("MovableText", posNueva, GameMgr.map)
		_self:sendMessageMovableText(textoNeg, entityNeg, true)
		_self:sendMessageSetColorMovableText("red", entityNeg, true)

	else
		local posNueva = Vector3(_self:getOwner():getPosition().x, _self:getOwner():getPosition().y, 40)
		local entity = entityfactory:createEntityByType("MovableText", posNueva, GameMgr.map)

		if textoPos ~= "" then
			_self:sendMessageMovableText(textoPos, entity, true)
			_self:sendMessageSetColorMovableText("green", entity, true)
		elseif textoNeg ~= "" then
			_self:sendMessageMovableText(textoNeg, entity,true)
			_self:sendMessageSetColorMovableText("red", entity, true)
		end
	end	


end

function PotenciadorScript:respawn(entityInfo)
	if entityInfo:hasAttribute("tipoGeneral") then
		self.tipoGeneral = entityInfo:getStringAttribute("tipoGeneral") 
	end

	if entityInfo:hasAttribute("tipoEspecifico") then
		self.tipoEspecifico = entityInfo:getStringAttribute("tipoEspecifico")
	end

	if entityInfo:hasAttribute("iconoPause") then
		self.iconoPause = entityInfo:getStringAttribute("iconoPause")
	end

	self.stats = Potenciadores[self.tipoGeneral].lista[self.tipoEspecifico].estadisticas

	return true
end


function PotenciadorScript:activate()
	
	return true
end

function PotenciadorScript:deactivate()

end



function PotenciadorScript:tick(msecs)

end

function PotenciadorScript:touched(other)
	if (other ~= nil and other:getType() == "Player") then

		PotenciadorScript:actualizarMovableText(self)
		FuncionesPotenciadores.ActualizarStats(Potenciadores[self.tipoGeneral].lista[self.tipoEspecifico].estadisticas)
		
		HUDPause.agregarPotenciador(self.tipoEspecifico, self.iconoPause)
		HUDPause.agregarTooltipPotenciadores(self.tipoGeneral, self.tipoEspecifico)

		-- FuncionesPotenciadores.DebugEstadisticasPersonaje()
		entityfactory:deferredDeleteEntity(self:getOwner())

		GameMgr.estadisticasPartida.numeroPotenciadoresCogidos = GameMgr.estadisticasPartida.numeroPotenciadoresCogidos +1

		FuncionesSistemaPuntuacion.puntosObjetoRecogible()
	end
end