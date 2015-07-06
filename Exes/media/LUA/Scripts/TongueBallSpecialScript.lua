--Script que controla el paso por la puerta, herede de CScritpEntity
class 'TongueBallSpecialScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function TongueBallSpecialScript:__init()
	CScriptEntity.__init(self)
end

function TongueBallSpecialScript:spawn(entityInfo)
	
	return true
end

function TongueBallSpecialScript:activate()

	return true
end

function TongueBallSpecialScript:deactivate()
	
end

function TongueBallSpecialScript:touched(other)
	
	local posImpact = self:getOwner():getPosition()
	
	entityfactory:createEntityByType("HumoSangre", posImpact, GameMgr.map)
	entityfactory:deferredDeleteEntity(self:getOwner())
end

function TongueBallSpecialScript:respawn(entityInfo)
	
	return true
end


function TongueBallSpecialScript:tick(msecs)
end