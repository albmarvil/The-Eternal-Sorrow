--author: César Manuel Paz Gumán

EventoAvalancha = 
{
	--Tamaños de la mochila
	tam_total_mochila = 0,
	tam_decimal_mochila = 0,
	tam_libre_mochila = 0,

	nivel_enemigos = 1,

	--Tiempo de calma 
	tiempoCalma = 18000, --8 segundos de calma

	--Tamaño de la mochila al empezar el tiempo de avalancha
	--Multiplicador del tam de mochila sobre el actual
	mult_mochila_avalancha = 5,
	mult_nivel_enemigos_avalancha = 2,

	--factor reductor de la mochila en el momento de calma
	mult_reductor_mochila = 0.5,

	--Se invoca cada 0.2 segundos en el tiempo de avalancha
	tiempoInvocacionEnSalaAvalancha = 200, -- 0.2 segundos un enemigo
	tiempoInvocacionEnSala = 0,
	
	--Variabls timeStamp para el tick
	timeStamp = 0,

	--Tag que indica si se ha empezado el evento o no
	iniciado = false,
	hordaInvocada = false,

	--flag que permite funcionar a este manager
	enabled = true,
}

FuncionesEventoAvalancha = {}

--esta funcion se llama en cada sala para reiniciar valores
function FuncionesEventoAvalancha.InitValues()
	FuncionesEventoAvalancha.Release()
	EventoAvalancha.timeStamp = 0
	EventoAvalancha.iniciado = false
	EventoAvalancha.hordaInvocada = false

	EventoAvalancha.enabled = true


end

function FuncionesEventoAvalancha.Init()

	-- if GameMgr.salas_totales ~= EventoAvalancha.sala_evento then

		EventoAvalancha.tam_total_mochila = RetoMgr.tam_total_mochila
		EventoAvalancha.tam_decimal_mochila = RetoMgr.tam_decimal_mochila
		EventoAvalancha.tam_libre_mochila = RetoMgr.tam_libre_mochila

		RetoMgr.tam_total_mochila = RetoMgr.tam_total_mochila * EventoAvalancha.mult_reductor_mochila
		RetoMgr.tam_decimal_mochila = RetoMgr.tam_decimal_mochila * EventoAvalancha.mult_reductor_mochila
		-- RetoMgr.tam_libre_mochila = 0

		EventoAvalancha.nivel_enemigos = GameMgr.nivel_enemigos

		EventoAvalancha.tiempoInvocacionEnSala = SpawnerMgr.tiempoInvocacionEnSala


		EventoAvalancha.iniciado = true

		-- Log.Debug("La calma antes e la tormenta...")

		----LANZAMOS EL EVENTO DE SONIDO
		SoundtrackMgr.ActivarSalida()

		-- EventoAvalancha.sala_evento = GameMgr.salas_totales


	-- end
end

function FuncionesEventoAvalancha.Release()
	if EventoAvalancha.iniciado == true then
		RetoMgr.tam_total_mochila = EventoAvalancha.tam_total_mochila
		RetoMgr.tam_decimal_mochila = EventoAvalancha.tam_decimal_mochila
		RetoMgr.tam_libre_mochila = RetoMgr.tam_total_mochila --mochila vacía

		GameMgr.nivel_enemigos = EventoAvalancha.nivel_enemigos

		
		EventoAvalancha.iniciado = false
		EventoAvalancha.hordaInvocada = false

		EventoAvalancha.timeStamp = 0
	end
	EventoAvalancha.enabled = false
end

function FuncionesEventoAvalancha.InvocarHorda()
	if not EventoAvalancha.hordaInvocada then
		EventoAvalancha.hordaInvocada = true

		-- Log.Debug("Invoco a la horda de los SORROWITOS!!!!")
		RetoMgr.tam_total_mochila = EventoAvalancha.tam_total_mochila * EventoAvalancha.mult_mochila_avalancha
		RetoMgr.tam_decimal_mochila = EventoAvalancha.tam_decimal_mochila * EventoAvalancha.mult_mochila_avalancha
		RetoMgr.tam_libre_mochila = RetoMgr.tam_total_mochila --mochila vacia para la horda

		SpawnerMgr.tiempoInvocacionEnSala = EventoAvalancha.tiempoInvocacionEnSalaAvalancha
		GameMgr.nivel_enemigos = EventoAvalancha.nivel_enemigos * EventoAvalancha.mult_nivel_enemigos_avalancha

		----LANZAMOS EVENTO DE SONIDO
		SoundtrackMgr.ActivarHorda()
	end
end

function FuncionesEventoAvalancha.tick(msecs)

	--solo si el numero de salas totales es mayor que 1 (ni en el tutorial ni en la sala inicial)
	if GameMgr.salas_totales >= 1 then

		--iniciamos el evento cuando nos lo indique el reto
		if Reto.ActivarEventoAvalancha() and not EventoAvalancha.iniciado and EventoAvalancha.enabled then
			FuncionesEventoAvalancha.Init()
		end

		if EventoAvalancha.timeStamp < EventoAvalancha.tiempoCalma and EventoAvalancha.iniciado then
			EventoAvalancha.timeStamp = EventoAvalancha.timeStamp + msecs

			-- Log.Debug("RELAX")
			-- Log.Debug(EventoAvalancha.timeStamp.." / "..EventoAvalancha.tiempoCalma)

			if EventoAvalancha.timeStamp >= EventoAvalancha.tiempoCalma then
				FuncionesEventoAvalancha.InvocarHorda()
			end

		end

	end
end