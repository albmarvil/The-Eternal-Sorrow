-- author: Alberto Martínez Villarán
-- date Junio de 2015

-- Manager de la banda sonora que controla los parametros de tension y los manda a FMOD. Encargado de calcular la tensión de juego a partir de las variables de partida




SoundtrackMgr = {
	
	Playing = false,
	PlayingMenu = false,
	Tension = 0,
	Horda = 0,

	--pesos ponderados de las variables de juego sobre la tension
	pesoMochila = 10,
	pesoObjetivoSala = 50,
	pesoVida = 35,
	pesoVelMuertes = 5,
}


function SoundtrackMgr.InitMgr()

	SoundtrackMgr.Tension = 0
	SoundtrackMgr.Horda = 0	
	SoundtrackMgr.Playing = false

end

function SoundtrackMgr.InitValues()
	--Probablemente haya que aprar los sonidos antes
	-- musicManager:stopAllSounds()
	Log.Debug("InitValues del mgr de musica")

	if SoundtrackMgr.PlayingMenu then --si tenemos que lanzar el menu

		musicManager:stopAllSounds()
		musicManager:playSound(SoundEvents.MenuTheme)
		SoundtrackMgr.PlayingMenu = false

	elseif not SoundtrackMgr.Playing or SoundtrackMgr.Tension > 100 or SoundtrackMgr.Horda > 0 or SoundtrackMgr.PlayingMenu then --si la principal esta parada...

		musicManager:stopAllSounds()
		musicManager:playSound(SoundEvents.MainTheme)
		SoundtrackMgr.Playing = true
		SoundtrackMgr.PlayingMenu = false

	end

	SoundtrackMgr.Tension = 0
	SoundtrackMgr.Horda = 0	

	musicManager:setSoundParameter(SoundEvents.MainTheme, "TENSION", SoundtrackMgr.Tension)
	musicManager:setSoundParameter(SoundEvents.MainTheme, "HORDA", SoundtrackMgr.Horda)
end

function SoundtrackMgr.Release()
	musicManager:stopAllSounds()
	SoundtrackMgr.Playing = false
end

function SoundtrackMgr.tick(msecs)
		if not EventoAvalancha.iniciado then
		--calculamos cuanto contribuye cada variable a la tensión
		local tamOcupadoMochila = (RetoMgr.tam_total_mochila - RetoMgr.tam_libre_mochila)
		-- Log.Debug("mochila: "..(RetoMgr.tam_total_mochila - RetoMgr.tam_libre_mochila).." / "..RetoMgr.tam_total_mochila)
		-- Log.Debug("tamOcupadoMochila: "..tamOcupadoMochila)
		local mochila = SoundtrackMgr.pesoMochila * (tamOcupadoMochila / RetoMgr.tam_total_mochila)

		-- Log.Warning("pesoMochila: "..mochila)

		-- Log.Debug("Vida: "..Data_Player.player_info.vidaActual.." / "..Data_Player.player_stats.vida)
		local damaged = Data_Player.player_stats.vida - Data_Player.player_info.vidaActual
		local porDamaged = damaged / Data_Player.player_stats.vida
		local vida = SoundtrackMgr.pesoVida * porDamaged
		-- Log.Warning("pesoVida: "..vida)

		local pObjectivoSala = Reto.PorcentajeObjetivo()
		-- Log.Debug("porcentaje objetivo: "..pObjectivoSala)
		local objetivo = pObjectivoSala * SoundtrackMgr.pesoObjetivoSala
		-- Log.Warning("pesoObjetivo: "..objetivo)

		-- Log.Debug("vel muertes: "..GameMgr.velocidad_muertes.." / 1.5")
		local vel = (GameMgr.velocidad_muertes * SoundtrackMgr.pesoVelMuertes) / 1.5

		-- Log.Warning("pesoMuertes: "..vel)


		local sumatorio = mochila + vida + objetivo + vel

		-- Log.Error("TENSION: "..sumatorio)

		SoundtrackMgr.Tension = math.max(SoundtrackMgr.Tension, math.min(sumatorio, 100))--No queremos que sobrepase de 100 en la dinamica normal ni que disminuya

		-- DebugHUD.updateAuxNumber(SoundtrackMgr.Tension)


		musicManager:setSoundParameter(SoundEvents.MainTheme, "TENSION", SoundtrackMgr.Tension)
		-- musicManager:setSoundParameter(SoundEvents.MainTheme, "HORDA", SoundtrackMgr.Horda)
	end
end


function SoundtrackMgr.ActivarSalida()

	SoundtrackMgr.Tension = 101
	SoundtrackMgr.Horda = 0
	musicManager:setSoundParameter(SoundEvents.MainTheme, "TENSION", SoundtrackMgr.Tension)
	musicManager:setSoundParameter(SoundEvents.MainTheme, "HORDA", SoundtrackMgr.Horda)
end

function SoundtrackMgr.ActivarHorda()
	-- SoundtrackMgr.Tension = 100
	SoundtrackMgr.Horda = 1
	-- musicManager:setSoundParameter(SoundEvents.MainTheme, "TENSION", SoundtrackMgr.Tension)
	musicManager:setSoundParameter(SoundEvents.MainTheme, "HORDA", SoundtrackMgr.Horda)
end


function SoundtrackMgr.ActivarHabTesoro()
	Log.Debug("Activo musica guachis de esto")
	-- musicManager:stopAllSounds()
	-- musicManager:playSound(SoundEvents.MenuTheme)
	SoundtrackMgr.Playing = false
	SoundtrackMgr.PlayingMenu = true
end