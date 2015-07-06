SistemaPuntuacion = 
{
	--MATAR ENEMIGOS
	MultiplicadorTier = 5,
	--PASAR SALA
	MultiplicadorSala = 200,

	--TIEMPO EN LA SALA
	MultiplicadorTiempoSala = 1,

	--COGER DINERO
	MultiplicadorDinero = 1,

	--COGER POTENCIADORES
	MultiplicadorObjetoRecogible = 10, --Potenciadores/habilidades

	--COFRES ABIERTOS (SEGUN EL NIVEL DEL COFRE)
	MultiplicadorCofreAbierto = 50,
	
}

FuncionesSistemaPuntuacion = {}

function FuncionesSistemaPuntuacion.Init()

end

function FuncionesSistemaPuntuacion.Release()

end

function FuncionesSistemaPuntuacion.puntosTier(tier, nivel)
	GameMgr.estadisticasPartida.puntuacionTotal = GameMgr.estadisticasPartida.puntuacionTotal + tier * tier * SistemaPuntuacion.MultiplicadorTier + tier * nivel
	FuncionesTrophyManager.aumentarPuntuacionPartida(tier * tier * SistemaPuntuacion.MultiplicadorTier + tier * nivel)
end

function FuncionesSistemaPuntuacion.puntosSalaSuperada(numeroSala)
	if GameMgr.salas_totales > 1 then
		GameMgr.estadisticasPartida.puntuacionTotal = GameMgr.estadisticasPartida.puntuacionTotal + numeroSala * SistemaPuntuacion.MultiplicadorSala + SistemaPuntuacion.MultiplicadorSala * 9
		FuncionesTrophyManager.aumentarPuntuacionPartida(numeroSala * SistemaPuntuacion.MultiplicadorSala + SistemaPuntuacion.MultiplicadorSala * 9)
	end
end

function FuncionesSistemaPuntuacion.puntosTiempoSala(tiempo)
	if GameMgr.salas_totales > 1 then
		GameMgr.estadisticasPartida.puntuacionTotal = GameMgr.estadisticasPartida.puntuacionTotal + 0.001 * tiempo * SistemaPuntuacion.MultiplicadorTiempoSala --Cada 10 segundos aumentamos 10 caramelos
		FuncionesTrophyManager.aumentarPuntuacionPartida(0.001 * tiempo * SistemaPuntuacion.MultiplicadorTiempoSala)
	end
end

function FuncionesSistemaPuntuacion.puntosDinero(valor)
	GameMgr.estadisticasPartida.puntuacionTotal = GameMgr.estadisticasPartida.puntuacionTotal + valor * SistemaPuntuacion.MultiplicadorDinero
	FuncionesTrophyManager.aumentarPuntuacionPartida(valor * SistemaPuntuacion.MultiplicadorDinero)
end	

function FuncionesSistemaPuntuacion.puntosObjetoRecogible()
	GameMgr.estadisticasPartida.puntuacionTotal = GameMgr.estadisticasPartida.puntuacionTotal + SistemaPuntuacion.MultiplicadorObjetoRecogible
	FuncionesTrophyManager.aumentarPuntuacionPartida(SistemaPuntuacion.MultiplicadorObjetoRecogible)
end

function FuncionesSistemaPuntuacion.puntosCofreAbierto(nivelCofre)
	GameMgr.estadisticasPartida.puntuacionTotal = GameMgr.estadisticasPartida.puntuacionTotal + SistemaPuntuacion.MultiplicadorCofreAbierto * nivelCofre
	FuncionesTrophyManager.aumentarPuntuacionPartida(SistemaPuntuacion.MultiplicadorCofreAbierto * nivelCofre)
end
