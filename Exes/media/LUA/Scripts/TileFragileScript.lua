--Saw hereda de CscritpEntity
class 'TileFragileScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function TileFragileScript:__init()
	CScriptEntity.__init(self)
end

function TileFragileScript:spawn(entityInfo)
	
	return true
end

function TileFragileScript:respawn(entityInfo)

	return true
end

function TileFragileScript:activate()

	return true
end

function TileFragileScript:deactivate()

end

function TileFragileScript:tick(msecs)

end

function TileFragileScript:broken()
	-- print("tile fragil roto")
	Drop.ObjetoRompible(self:getOwner():getPosition())
end