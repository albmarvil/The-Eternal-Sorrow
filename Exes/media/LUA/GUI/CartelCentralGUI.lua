--author: Cesar Manuel Paz Guzman

CartelCentralHUD = 
{
	Centro = {
		velocidad = 1.25, --Valocidad a la que se va a ir el cartel hacia arriba
		area = HUD.hudWindow:getChild("Centro"):getProperty("Area"),
		posXDestino = 0.18,
		posXDestino2 = 0.33,
		posXDestino3 = 1,
		posXActual = -0.47,
	},
	Arriba = {
		posYDestino = 0,
		posYActual = -0.23, --HARDCODEADO
		alturaActual = 0, 
		alturaDestino = 0.23, --HARDCODEADO
		numeroCartelActivado = 0,
		nombresCarteles = {"CartelArriba1", "CartelArriba2", "CartelArriba3",},
		areaCartel1 = HUD.hudWindow:getChild("CartelArriba1"):getProperty("Area"),
		areaCartel2 = HUD.hudWindow:getChild("CartelArriba2"):getProperty("Area"),
		areaCartel3 = HUD.hudWindow:getChild("CartelArriba3"):getProperty("Area"),
	},

	isFinished = false,
}

--Numero de objetivos es por ejemplo, matar a 10 enemigos es un objetivo, en 50 segundos, ya son dos objetivos 
--y asi saber que cartel sacar
function CartelCentralHUD.Activate(numero, informacionCentral) 

	HUD.hudWindow:getChild("Centro/Texto"):setProperty("Text", informacionCentral)
	CartelCentralHUD.Arriba.numeroCartelActivado = numero

	--Activamos el cartel que corresponda, segun el numero de objetivos a cumplir a mostrar para pasar el nivel
	if numero> 0 and numero < 4 then
		HUD.hudWindow:getChild(CartelCentralHUD.Arriba.nombresCarteles[numero]):setVisible(true)
	end

end

function CartelCentralHUD.Deactivate()

	for key, value in pairs(CartelCentralHUD.Arriba.nombresCarteles) do
		HUD.hudWindow:getChild(value):setVisible(false)
	end

	--volvemos a indicarle a las areas que cojan el area inicial
	HUD.hudWindow:getChild("Centro"):setProperty("Area", CartelCentralHUD.Centro.area)
	HUD.hudWindow:getChild("CartelArriba1"):setProperty("Area",CartelCentralHUD.Arriba.areaCartel1)
	HUD.hudWindow:getChild("CartelArriba2"):setProperty("Area",CartelCentralHUD.Arriba.areaCartel2)
	HUD.hudWindow:getChild("CartelArriba3"):setProperty("Area",CartelCentralHUD.Arriba.areaCartel3)

end

function CartelCentralHUD.udpateCartelCentral(msecs)

		--Primera frase, movemos el cartel rapidamente hacia el centro
		if CartelCentralHUD.Centro.posXActual < CartelCentralHUD.Centro.posXDestino then
			CartelCentralHUD.desplazamientoCarteCentralIzquierda(CartelCentralHUD.Centro.velocidad, msecs)
			--Log.Debug("Derecha de todo")
		--Segunda fase, lentamente vamos hacia la posicion de destino2, cuando llega a ese destino, vuelve a moverse rapido hasta deaparecer
		elseif CartelCentralHUD.Centro.posXActual < CartelCentralHUD.Centro.posXDestino2 then
			CartelCentralHUD.desplazamientoCarteCentralIzquierda(CartelCentralHUD.Centro.velocidad*0.015, msecs)
			--Log.Debug("Centro")
		elseif CartelCentralHUD.Centro.posXActual > CartelCentralHUD.Centro.posXDestino2 and 
			CartelCentralHUD.Centro.posXActual < CartelCentralHUD.Centro.posXDestino3 then
			--Log.Debug("Izquierda")
			CartelCentralHUD.desplazamientoCarteCentralIzquierda(CartelCentralHUD.Centro.velocidad, msecs)

		elseif CartelCentralHUD.Centro.posXActual > CartelCentralHUD.Centro.posXDestino3 
			and CartelCentralHUD.Arriba.numeroCartelActivado > 0 then 
			--Log.Debug("Izquierda de todo")
			CartelCentralHUD.desplazamientoCartelArriba(msecs) 	
		end
end

function CartelCentralHUD.desplazamientoCarteCentralIzquierda(velocidad, msecs)
	--Log.Debug("posXActual: "..toString(CartelCentralHUD.Centro.posXActual))

	CartelCentralHUD.Centro.posXActual = CartelCentralHUD.Centro.posXActual + 30*0.002*velocidad
	local auxPosX = CartelCentralHUD.Centro.posXActual + 0.45
	HUD.hudWindow:getChild("Centro"):setProperty("Area", "{{"..CartelCentralHUD.Centro.posXActual..",0},{0,0},{"..
		auxPosX..",0},{0.25,0}}")

	--Log.Debug("posXActual2: "..toString(CartelCentralHUD.Centro.posXActual).." msecs: "..toString(msecs))
end

function CartelCentralHUD.desplazamientoCartelArriba(msecs) 

	CartelCentralHUD.Arriba.posYActual = CartelCentralHUD.Arriba.posYActual + msecs/1000
	CartelCentralHUD.Arriba.alturaActual = CartelCentralHUD.Arriba.alturaActual + msecs/1000

	if CartelCentralHUD.Arriba.alturaActual >= CartelCentralHUD.Arriba.alturaDestino then
		CartelCentralHUD.isFinished = true
		CartelCentralHUD.Arriba.posYActual = 0
		CartelCentralHUD.Arriba.alturaActual = CartelCentralHUD.Arriba.alturaDestino
	end

	--HARDCODEO A TOPE, SOLO LOS NUMEROS, QUE SON COPIADOS DE LAS AREAS DE LOS CARTELES, POR QUE NO SE COMO SACARLAS A LUA
	if CartelCentralHUD.Arriba.numeroCartelActivado == 1 then
		HUD.hudWindow:getChild("CartelArriba1"):setProperty("Area", "{{0.38,0},{"..CartelCentralHUD.Arriba.posYActual
			..",0},{0.57,0},{"..CartelCentralHUD.Arriba.alturaActual..",0}}")
	elseif CartelCentralHUD.Arriba.numeroCartelActivado == 2 then
		HUD.hudWindow:getChild("CartelArriba2"):setProperty("Area", "{{0.38,0},{"..CartelCentralHUD.Arriba.posYActual
			..",0},{0.62,0},{"..CartelCentralHUD.Arriba.alturaActual..",0}}")
	elseif CartelCentralHUD.Arriba.numeroCartelActivado == 3 then
		HUD.hudWindow:getChild("CartelArriba3"):setProperty("Area", "{{0.38,0},{"..CartelCentralHUD.Arriba.posYActual
			..",0},{0.61,0},{"..CartelCentralHUD.Arriba.alturaActual..",0}}")
	end

	
end

function CartelCentralHUD.insertarTexto(posicionTexto, nombre, valor)

	local windowEscribir = HUD.hudWindow:getChild(CartelCentralHUD.Arriba.nombresCarteles[CartelCentralHUD.Arriba.numeroCartelActivado]):getChildAtIdx(posicionTexto)
	windowEscribir:getChild("Nombre"):setProperty("Text", nombre)
	windowEscribir:getChild("Cantidad"):setProperty("Text", valor)
end

function CartelCentralHUD.cambiarColorTexto(color)
	local windowColor = HUD.hudWindow:getChild(CartelCentralHUD.Arriba.nombresCarteles[CartelCentralHUD.Arriba.numeroCartelActivado])
	local colorFinal = nil
	if color == "verde" then
		colorFinal = "FF00FF00"
	elseif color == "rojo" then
		colorFinal = "FFFFFFFF"
	end

	local value = 2
	while  value <= windowColor:getChildCount() do
		windowColor:getChildAtIdx(value-1):getChild("Cantidad"):setProperty("NormalTextColour",colorFinal)
		value = value +1 
	end
end

function CartelCentralHUD.transformarHora(segundos)
	local minutos = math.floor(segundos / 60)
	local segundos = math.floor(segundos % 60)
	local segundos2 = toString(segundos)
	if segundos < 10 then 
		segundos2 = "0"..segundos2
	end
	return minutos..":"..segundos2
end