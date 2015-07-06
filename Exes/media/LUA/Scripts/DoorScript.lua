--Script que controla el paso por la puerta, herede de CScritpEntity
class 'DoorScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function DoorScript:__init()
	CScriptEntity.__init(self)
end

function DoorScript:spawn(entityInfo)

	return true
end

function DoorScript:activate()
	GameMgr.exit_door = self:getOwner()
	return true
end

function DoorScript:deactivate()
	
end

function DoorScript:touched(other)
	if (other ~= nil and other:getType() == "Player") then
		if not RetoMgr.is_reto then
			GameMgr.FinSala()
			
			Ambientaciones.nextMap()
			--carga new mapa
			logicMapMgr = LogicMapManager.getInstance()
			logicMapMgr:nextMap()

		end
	end
end

function DoorScript:respawn(entityInfo)

	return true
end


function DoorScript:tick(msecs)
end