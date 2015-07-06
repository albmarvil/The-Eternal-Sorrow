--author: Cesar Manuel Paz Guzman

class 'TileAtravesableScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function TileAtravesableScript:__init()
	CScriptEntity.__init(self)

end

function TileAtravesableScript:spawn(entityInfo)
	ChangeCollisionGroupTileAtravesable(self:getOwner(), true)
	return true
end

function TileAtravesableScript:respawn(entityInfo)
	ChangeCollisionGroupTileAtravesable(self:getOwner(), true)
	return true
end

function TileAtravesableScript:activate()

	return true
end

function TileAtravesableScript:deactivate()

end

function TileAtravesableScript:tick(msecs)
	if (GameMgr.player:getPosition().y < self:getOwner():getPosition().y+10) then
		ChangeCollisionGroupTileAtravesable(self:getOwner(), true)
	else
		ChangeCollisionGroupTileAtravesable(self:getOwner(), false)
	end
end
