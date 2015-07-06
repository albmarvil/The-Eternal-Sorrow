---------------NOTA ENORME. Los bancos de sonidos y los eventos no pueden tener espacios en los nombres

SoundBanks = {
	"Master Bank",
	"Master Bank.strings",
	"Musica",
	"Varios",
	"Escenario",
	"Player",
	"Armas",
	-- "Surround_Ambience",
	-- "Weapons",
	-- "Effects",
	-- "TES_Efectos",
	-- "TES_Pantalla",
	-- "TES_Menu",
}

SoundEvents = {
	-- original = "/Ambience/original",
	-- suspense = "/Ambience/suspense",
	-- spirits = "/Ambience/spirits",
	-- katana = "/Weapons/katana",
	-- gun = "/Weapons/gun",
	-- gunSpecial = "/Weapons/gunSpecial",
	-- fire = "/Effects/Fire",
	-- enemySpawn = "/Effects/enemySpawn",
	-- enemyDeath = "/Effects/enemyDeath",
	-- mewtwoDeath = "/Effects/mewtwoDeath",
	-- heartbeat = "/Effects/heartbeat",
	-- grayNoise = "/Effects/GrayNoise",
	-- mainMenu = "/TES/Menu/MainMenu",
	-- drumBip = "/TES/Efectos/Drum bip",

	MainTheme = "/Musica/MainTheme3",
	MenuTheme = "/Musica/MenuTheme",
	InicioPartida = "/Varios/IniciarPartida",
	
	Muerte1 = "/Player/Muerte1",
	Muerte2 = "/Player/Muerte2",
	Muerte3 = "/Player/Muerte3",
	
	RecogerObjeto1 = "/Escenario/RecogerObjeto1",
	RecogerMoneda = "/Escenario/RecogerMoneda",
	Explosion1 = "/Escenario/Explosion1",
	Explosion2 = "/Escenario/Explosion2",

	ReboteEscudo = "/Armas/Escudo/ReboteEscudo",

	GreenSword = "/Varios/GreenSword",
	GreenSwordSpecial = "/Varios/GreenSwordSpecial",
	GunDart = "/Varios/Cerbatana1",
	VaporPresion = "/Varios/VaporPresion",
	Escudo1 = "/Varios/Escudo1",
	LaserBeam = "/Varios/LaserBeam",
	EnergyExplosion = "/Varios/EnergyExplosion",
}

function loadSoundBanks()
	-- body
	for k,v in pairs(SoundBanks) do
		soundServer:AddSoundBank(v)
	end
	--inicializamos los managers secundarios una vez se han cargado los bancos de sonidos
	soundServer:InitManagers()
end

---Al cargar este script ejecutamos la carga de los bancos de sonido
loadSoundBanks()