--Saw hereda de CscritpEntity
class 'nameClass' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function nameClass:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)
end

function nameClass:spawn(entityInfo)

	return true
end

function nameClass:respawn(entityInfo)

	return true
end

function nameClass:activate()

	return true
end

function nameClass:deactivate()

end

function nameClass:tick(msecs)

end