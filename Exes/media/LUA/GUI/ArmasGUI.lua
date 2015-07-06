--author: Cesar Manuel Paz Guzman

ArmasHUD = 
{
	Left = {
		entidad = "null",	
		areaInicial = HUD.hudWindow:getChild("Armas/ArmaIzqFondo"):getProperty("Area"),
		areaTransparenciaInicial = HUD.hudWindow:getChild("Armas/ArmaIzqFondo2/transparenciaPadre"):getProperty("Area"),
		transparencia = HUD.hudWindow:getChild("Armas/ArmaIzqFondo2/transparenciaPadre"),
	},
	Right = {
		entidad = "null",
		areaInicial = HUD.hudWindow:getChild("Armas/ArmaDerFondo"):getProperty("Area"),
		areaTransparenciaInicial = HUD.hudWindow:getChild("Armas/ArmaDerFondo2/transparenciaPadre"):getProperty("Area"),
		transparencia = HUD.hudWindow:getChild("Armas/ArmaDerFondo2/transparenciaPadre"),
	},
}

function ArmasHUD.Activate()
	HUD.hudWindow:getChild("Armas/ArmaIzqFondo"):setProperty("Area", ArmasHUD.Left.areaInicial)
	HUD.hudWindow:getChild("Armas/ArmaDerFondo"):setProperty("Area", ArmasHUD.Right.areaInicial)

	ArmasHUD.Left.area = HUD.hudWindow:getChild("Armas/ArmaIzq")
	ArmasHUD.Right.area = HUD.hudWindow:getChild("Armas/ArmaDer")
	ArmasHUD.Right.area:setProperty("Image","")
	ArmasHUD.Left.area:setProperty("Image","")

	ArmasHUD.Left.areaFondo = HUD.hudWindow:getChild("Armas/ArmaIzqFondo")
	ArmasHUD.Right.areaFondo = HUD.hudWindow:getChild("Armas/ArmaDerFondo")
	ArmasHUD.Left.areaFondo:setProperty("Image", "NewHUD/arma1")
	ArmasHUD.Right.areaFondo:setProperty("Image", "NewHUD/arma1") 
	ArmasHUD.Left.transparencia:setVisible(false)
	ArmasHUD.Right.transparencia:setVisible(false)
end

function ArmasHUD.Deactivate()
	ArmasHUD.setearTransparencia("Right")
	ArmasHUD.setearTransparencia("Left")
	ArmasHUD.Left.entidad = "null"
	ArmasHUD.Right.entidad = "null"
end

function ArmasHUD.updateArms(brazo, degradacion)
	-- local scale = getScale("Armas/ArmaIzq/Transparencia")
	-- local offset = getOffset("Armas/ArmaIzq/Transparencia")
	-- Log.Debug("Scale: "..toString(scale).." offset: "..toString(offset))
	
	ArmasHUD[brazo].transparencia:setProperty("Area", "{{0.0550562,0},{"..degradacion..",0},{0.97,0},{0.95,0}}")

	local x = degradacion- 0.072
	local y = -x 
	ArmasHUD[brazo].transparencia:getChild("Transparencia"):setProperty(
		"Area", "{{0,0},{"..toString(y)..",0},{1,0},{1.05,0}}")
end

function ArmasHUD.swapArms()

	local entidadDer = ArmasHUD.Right.entidad
	ArmasHUD.Right.entidad = ArmasHUD.Left.entidad
	ArmasHUD.Left.entidad = entidadDer

	local imageDer = ArmasHUD.Right.area:getProperty("Image")
	ArmasHUD.Right.area:setProperty("Image", ArmasHUD.Left.area:getProperty("Image"))
	ArmasHUD.Left.area:setProperty("Image", imageDer)

	local transparenciaDer = ArmasHUD.Right.transparencia:getProperty("Area")
	ArmasHUD.Right.transparencia:setProperty("Area", ArmasHUD.Left.transparencia:getProperty("Area"))
	ArmasHUD.Left.transparencia:setProperty("Area", transparenciaDer)

	if ArmasHUD.Right.entidad ~= "null" then
		ArmasHUD.Right.entidad.brazo = "Right"
	elseif ArmasHUD.Right.entidad == "null" and ArmasHUD.Left.entidad ~= "null" then
		ArmasHUD.Right.transparencia:setVisible(false)
		ArmasHUD.Left.transparencia:setVisible(true)
	end

	if ArmasHUD.Left.entidad ~= "null" then
		ArmasHUD.Left.entidad.brazo = "Left"
	elseif ArmasHUD.Left.entidad == "null" and ArmasHUD.Right.entidad ~= "null" then
		ArmasHUD.Left.transparencia:setVisible(false)
		ArmasHUD.Right.transparencia:setVisible(true)
	end

	HUDPause.swapArmas()
end

function ArmasHUD.setearTransparencia(brazo)

	--ArmasHUD[brazo].areaFondo:getChild("Transparencia"):setProperty("Area", ArmasHUD[brazo].areaTransparenciaInicial)

	ArmasHUD[brazo].transparencia:setProperty("Area", ArmasHUD[brazo].areaTransparenciaInicial)
end