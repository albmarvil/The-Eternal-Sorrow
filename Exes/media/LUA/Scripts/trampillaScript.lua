--Saw hereda de CscritpEntity
class 'heiserScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function heiserScript:__init()
	CScriptEntity.__init(self)

end

function heiserScript:spawn(entityInfo)
	return true
end

function heiserScript:respawn(entityInfo)
	return true
end

function heiserScript:activate()

	return true
end

function heiserScript:deactivate()

end

function heiserScript:tick(msecs)

end

function heiserScript:touched(other)
	if (other ~= nil and other:getType() == "Player") then
		--Si es el player nos creamos la particula de heiser
	end
end

function heiserScript:untouched(other)
	if (other ~= nil and other:getType() == "Player") then
		--Si es el player nos creamos la particula de heiser
	end
end
