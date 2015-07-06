-- hereda de CScriptEntity
class 'EnemyStateMachineScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function EnemyStateMachineScript:__init()
	CScriptEntity.__init(self)
end

function EnemyStateMachineScript:spawn(entityInfo)

	self.currentState = StatesList[entityInfo:getStringAttribute("defaultIA")]
	
	
	return true
end

function EnemyStateMachineScript:respawn(entityInfo)

	self.currentState = StatesList[entityInfo:getStringAttribute("defaultIA")]

	return true
end

function EnemyStateMachineScript:tick(msecs)
	
	self.currentState.Tick(self)
end

function EnemyStateMachineScript:changeState(state)
	
	-- Log.Debug("CAMBIO: " .. StatesList[state].name)
	self.currentState.Sleep(self)
	self.currentState = StatesList[state]
	self.currentState.Awake(self)

end

function EnemyStateMachineScript:activate()
	self.currentState.Awake(self)
	return true
end

function EnemyStateMachineScript:deactivate()

end
