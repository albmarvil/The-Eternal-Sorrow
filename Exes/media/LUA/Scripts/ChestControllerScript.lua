--Saw hereda de CscritpEntity
class 'ChestControllerScript' (CScriptEntity)

--Atributo privado __init. Se trata del constructor de esa clase
function ChestControllerScript:__init()
	--super(self, "hoa")
	CScriptEntity.__init(self)

	self.price = 0
	self.chestLevel = 0
	self.opened = false
end

function ChestControllerScript:spawn(entityInfo)

	if entityInfo:hasAttribute("price") then
		self.price = entityInfo:getFloatAttribute("price")
	end

	if entityInfo:hasAttribute("chest_level") then
		self.chestLevel = entityInfo:getIntAttribute("chest_level")
	end

	if entityInfo:hasAttribute("timeOpen") then
		self.timeOpen = entityInfo:getIntAttribute("timeOpen")
	end

	if entityInfo:hasAttribute("porcentToReduce") then
		self.porcentToReduce = entityInfo:getIntAttribute("porcentToReduce")
	end

	return true
end

function ChestControllerScript:respawn(entityInfo)

	if entityInfo:hasAttribute("price") then
		self.price = entityInfo:getFloatAttribute("price")
	end

	if entityInfo:hasAttribute("chest_level") then
		self.chestLevel = entityInfo:getIntAttribute("chest_level")
	end

	if entityInfo:hasAttribute("timeOpen") then
		self.timeOpen = entityInfo:getIntAttribute("timeOpen")
	end

	if entityInfo:hasAttribute("porcentToReduce") then
		self.porcentToReduce = entityInfo:getIntAttribute("porcentToReduce")
	end

	return true
end

function ChestControllerScript:activate()
	self.opened = false
	self.timeStamp = 0
	self.beginOpen = false
	setVisibleBillboardCofre(self:getOwner(), false)

	self.timeOpenInitial = self.timeOpen

	self.timeOpen = (self.timeOpenInitial * (100 - self.porcentToReduce * HadHability("ReduceTimeChest"))) * 0.01
	return true
end

function ChestControllerScript:deactivate()
	HUD.hudWindow:getChild("openChest"):setProperty("Visible", "false")
	HUD.hudWindow:getChild("letraEChest"):setProperty("Visible", "false")
end

function ChestControllerScript:tick(msecs)

	if self.beginOpen == true then
		
		self.timeStamp = self.timeStamp + msecs

		updateBillboardChest(self:getOwner(), self.timeOpen - self.timeStamp, self.timeOpen)

		if self.timeStamp >= self.timeOpen then
			self.timeStamp = 0
			self.beginOpen = false
			setVisibleBillboardCofre(self:getOwner(), false)
			self:open()
		end

	end
end

function ChestControllerScript:openChest()
	self.timeStamp = 0

	self.timeOpen = (self.timeOpenInitial * (100 - self.porcentToReduce * HadHability("ReduceTimeChest"))) * 0.01
	Log.Debug("timeOpen: "..toString(self.timeOpen))

	if Data_Player.player_info.money >= self.price and not self.opened then
		self.beginOpen = true
		setVisibleBillboardCofre(self:getOwner(), true)
		updateBillboardChest(self:getOwner(), self.timeOpen - self.timeStamp, self.timeOpen)
	end
end

function ChestControllerScript:cancelOpen()
	self.beginOpen = false
	setVisibleBillboardCofre(self:getOwner(), false)
end

function ChestControllerScript:touched(other)
	-- body
	if (other ~= nil and other:getType() == "Player" and not self.opened) then
		self.colourText = "FFFFFFFF"
		if self.price > Data_Player.player_info.money then
			self.colourText = "FFFF0000"
		end

		local posChest = getScreenPosition(self:getOwner())

		HUD.hudWindow:getChild("openChest"):setProperty("Area", "{{"..toString(posChest.x)..",0},{"..toString(posChest.y)
			..",0},{"..toString(posChest.x+0.04)..",0},{"..toString(posChest.y + 0.04)..",0}}")

		HUD.hudWindow:getChild("openChest"):setProperty("Visible", "true")

		HUD.hudWindow:getChild("letraEChest"):setProperty("Area", "{{"..toString(posChest.x+0.01)..",0},{"..toString(posChest.y-0.05)
			..",0},{"..toString(posChest.x+0.03)..",0},{"..toString(posChest.y - 0.01)..",0}}")

		HUD.hudWindow:getChild("letraEChest"):setProperty("Visible", "true")

		--self:setVisibleText(self:getOwner(), toString(self.price), self.colourText, true)

		HUD.hudWindow:getChild("openChest/cantidad"):setProperty("Text",toString(self.price))
		HUD.hudWindow:getChild("openChest/cantidad"):setProperty("NormalTextColour",self.colourText)
		--Seteamos el texto

		--Y lo hacemos visible
	end
end

function ChestControllerScript:untouched( other )
	-- body
	if (other ~= nil and other:getType() == "Player" and not self.opened) then

		--Hacemos invislbe el staticText
		HUD.hudWindow:getChild("openChest"):setProperty("Visible", "false")
		self:setVisibleText(self:getOwner(), toString(self.price), self.colourText, false)
		self:cancelOpen()
		HUD.hudWindow:getChild("letraEChest"):setProperty("Visible", "false")
	end
end

function ChestControllerScript:open()
	if not self.opened then
		-- Log.Debug("Se abre el cofre!! jai!")
		--comprobar si tengo almas
		if Data_Player.player_info.money >= self.price then
			HUD.hudWindow:getChild("openChest"):setProperty("Visible", "false")
			HUD.hudWindow:getChild("letraEChest"):setProperty("Visible", "false")

			Data_Player.player_info.money = Data_Player.player_info.money - self.price
			LifeHUD.ActualizarDinero(Data_Player.player_info.money)
			Log.Debug("Abro cofre con coste: "..self.price.." - Current money: "..Data_Player.player_info.money)
			self.opened = true

			openChestAnimation(self:getOwner())
			
			self:setVisibleText(self:getOwner(), toString(self.price), self.colourText, false)
			
			--llamo al Drop pasandole mi nivel de cofre y mi posicion para que suelte objetos
			Drop.CofreAbierto(self.chestLevel, self:getOwner():getPosition())

			--Actualizar puntuacion apertura cofre
			FuncionesSistemaPuntuacion.puntosCofreAbierto(self.chestLevel)

			GameMgr.estadisticasPartida.numeroCofresAbiertos = GameMgr.estadisticasPartida.numeroCofresAbiertos +1
			GameMgr.estadisticasPartida.dineroGastado = GameMgr.estadisticasPartida.dineroGastado + self.price

			if self.chestLevel == 1 then
				FuncionesTrophyManager.aumentarValor("AbrirCofresBasicos")	
			elseif self.chestLevel == 2 then
				FuncionesTrophyManager.aumentarValor("AbrirCofresIntermedios")
			elseif self.chestLevel == 3 then
				FuncionesTrophyManager.aumentarValor("AbrirCofresAltos")
			end

		else
			Log.Debug("Dinero insuficiente!!! No puedes abrir el cofre...BUSCATE UN TRABAJO!")
		end
	end
end