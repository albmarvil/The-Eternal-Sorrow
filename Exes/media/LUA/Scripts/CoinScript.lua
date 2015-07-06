--Saw hereda de CscritpEntity
class 'CoinScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function CoinScript:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)

	self.value = 0
end

function CoinScript:spawn(entityInfo)

	if entityInfo:hasAttribute("value") then
		self.value = entityInfo:getIntAttribute("value")
	end
	return true
end

function CoinScript:respawn(entityInfo)
	if entityInfo:hasAttribute("value") then
		self.value = entityInfo:getIntAttribute("value")
	end
	return true
end

function CoinScript:activate()

	return true
end

function CoinScript:deactivate()

end

function CoinScript:tick(msecs)

end

function CoinScript:touched( other )

	if other ~= nil then
		if other:getType() == "Player" then
			
			Data_Player.player_info.money = Data_Player.player_info.money + self.value
			GameMgr.estadisticasPartida.dineroConseguido = GameMgr.estadisticasPartida.dineroConseguido + self.value
			
			-- Log.Debug("Moneda de "..self.value.." recogida | Dinero actual: "..Data_Player.player_info.money)
			LifeHUD.ActualizarDinero(Data_Player.player_info.money)
			--borramos el objeto usable del alma
			entityfactory:deferredDeleteEntity(self:getOwner())

			FuncionesSistemaPuntuacion.puntosDinero(self.value)

			---lanzamos el sonido
			guiSounds:playSound(SoundEvents.RecogerMoneda)
		end
	end
			

end