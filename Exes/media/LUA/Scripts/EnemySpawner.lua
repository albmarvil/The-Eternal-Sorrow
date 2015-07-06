--Saw hereda de CscritpEntity
class 'EnemySpawner' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function EnemySpawner:__init()
	CScriptEntity.__init(self)
end

function EnemySpawner:spawn(entityInfo)
	if entityInfo:hasAttribute("position") then
		--registramos el punto de spawn en el SpawnerMgr, que almacena una tabla con ellos
		SpawnerMgr.puntos_spawn[SpawnerMgr.total_puntos] = entityInfo:getVector3Attribute("position")
		SpawnerMgr.total_puntos = SpawnerMgr.total_puntos + 1
	end
	return true
end

function EnemySpawner:respawn(entityInfo)
	if entityInfo:hasAttribute("position") then
		--registramos el punto de spawn en el SpawnerMgr, que almacena una tabla con ellos
		SpawnerMgr.puntos_spawn[SpawnerMgr.total_puntos] = entityInfo:getVector3Attribute("position")
		SpawnerMgr.total_puntos = SpawnerMgr.total_puntos + 1
	end
	return true
end

function EnemySpawner:activate()
	return true
end

function EnemySpawner:deactivate()
end