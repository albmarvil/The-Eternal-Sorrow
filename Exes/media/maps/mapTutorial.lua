Map = {
	
	Scene = {
		type ="SceneConfiguration",
		ambient_light = { 0.2, 0.2, 0.2},
		-- ambient_light = { 0.9, 0.9, 0.9},
	},
	
	David = {
		type = "Player",
		position = {340, -260, 0},
	},
	Camera = {
		type = "Camera",
	},

	LuzPlayer = {
		type = "LuzPlayer",
	},

	SaiPause = {
		type = "SaiPause",
		position = {-340, 2000, -218},
	},

	CameraPause = {
		type = "CameraPause",
		position = {-340, 2017, -170},
	},

	LuzEscena = {
		type = "LuzEscena",
	},

	puntoTutorial1 = {
		type = "puntoTutorial",
		position = {440,-420,0},
	},

	triggerTutorial =
	{
		type = "triggerTutorial1",
		position = {440,-420,0},
	},

	LanzaDardosRecogible = {
		type = "UsableObjectDartGun",
		position = {540, -420, 0},
	},

	puntoTutorial2 = {
		type = "puntoTutorial",
		position = {720,-520,0},
	},

	triggerTutorial2 =
	{
		type = "triggerTutorial2",
		position = {720,-520,0},
	},

	Coin1 = {
		type = "Coin",
		value = 1,
		start_impulse = 20,
		position = {820, -520, 0},
		time_to_start_to_disappear = 50000,
	},

	Coin2 = {
		type = "Coin2",
		value = 10,
		start_impulse = 20,
		position = {860, -520, 0},
		time_to_start_to_disappear = 50000,
	},
	Coin3 = {
		type = "Coin3",
		value = 100,
		start_impulse = 20,
		position = {900, -520, 0},
		time_to_start_to_disappear = 50000,
	},

	puntoTutorial3 = {
		type = "puntoTutorial",
		position = {1140,-620,0},
	},

	triggerTutorial3 =
	{
		type = "triggerTutorial3",
		position = {1140,-620,0},
	},

	Chest = {	
		type = "Chest",
		position = {1240, -620, -20},
		price = 1000,
	},

	puntoTutorial4 = {
		type = "puntoTutorial",
		position = {1460,-720,0},
	},

	triggerTutorial4 =
	{
		type = "triggerTutorial4",
		position = {1460,-720,0},
	},

	Soul = {
		type = "Soul",
		position = {1540,-720,0},
		souls = 5,
		sourceParticles = {"AlmaAzul"},
	},

	Soul2 = {
		type = "Soul2",
		position = {1570,-720,0},
		souls = 10,
		sourceParticles = {"AlmaVerde"},
	},

	Soul3 = {
		type = "Soul3",
		position = {1600,-720,0},
		souls = 25,
		sourceParticles = {"AlmaRoja"},
	},

	puntoTutorial5 = {
		type = "puntoTutorial",
		position = {1780,-810,0},
	},

	triggerTutorial5 =
	{
		type = "triggerTutorial5",
		position = {1780,-810,0},
	},

	Potenciador9 = {
		type = "Potenciador9",
		position = {1820,-810,0},
		tipoGeneral = "Positivos",
		tipoEspecifico = "VelMovimientoPos5",
	},

	Potenciador14 = {
		type = "Potenciador14",
		position = {1860,-810,0},
		tipoGeneral = "Neutros",
		tipoEspecifico = "Asesino",
	},

	Potenciador23 = {
		type = "Potenciador23",
		position = {1900,-810,0},
		tipoGeneral = "Neutros",
		tipoEspecifico = "Berseker",
	},

	puntoTutorial6 = {
		type = "puntoTutorial",
		position = {2100,-920,0},
	},

	triggerTutorial6 =
	{
		type = "triggerTutorial6",
		position = {2100,-920,0},
	},

	PowerUpSalto = {
		type = "UsableObjectSalto",
		position = {2160,-920,0},
	},

	puntoTutorial7 = {
		type = "puntoTutorial",
		position = {2400,-560,0},
	},

	triggerTutorial7 =
	{
		type = "triggerTutorial7",
		position = {2400,-560,0},
	},

	puntoTutorial8 = {
		type = "puntoTutorial",
		position = {2360,-1100,0},
	},

	triggerTutorial8 =
	{
		type = "triggerTutorial8",
		position = {2360,-1100,0},
	},

	puntoTutorial9 = {
		type = "puntoTutorial",
		position = {2520,-1420,0},
	},

	triggerTutorial9 =
	{
		type = "triggerTutorial9",
		position = {2520,-1420,0},
	},

	puntoTutorial10 = {
		type = "puntoTutorial",
		position = {2060,-1100,0},
	},

	triggerTutorial10 =
	{
		type = "triggerTutorial10",
		position = {2060,-1100,0},
	},

	puntoTutorial11 = {
		type = "puntoTutorial",
		position = {1400,-1200,0},
	},

	triggerTutorial11 =
	{
		type = "triggerTutorial11",
		position = {1400,-1200,0},
	},

	puntoTutorial12 = {
		type = "puntoTutorial",
		position = {840,-780,0},
	},

	triggerTutorial12 =
	{
		type = "triggerTutorial12",
		position = {840,-780,0},
	},

	Trampilla1 = {
		type = "TILE_TRAMPILLA",
		position = {780,-780,0},
	},

	Trampilla2 = {
		type = "TILE_TRAMPILLA",
		position = {680,-940,0},
	},

	Trampilla3 = {
		type = "TILE_TRAMPILLA",
		position = {820,-940,0},
	},

	Trampilla4 = {
		type = "TILE_TRAMPILLA",
		position = {800,-1120,0},
	},

	puntoTutorial13 = {
		type = "puntoTutorial",
		position = {720,-1420,0},
	},

	triggerTutorial13 =
	{
		type = "triggerTutorial13",
		position = {720,-1420,0},
	},

	puntoTutorial14 = {
		type = "puntoTutorial",
		position = {1000,-1420,0},
	},

	triggerTutorial14 =
	{
		type = "triggerTutorial14",
		position = {1000,-1420,0},
	},

	puntoTutorial15 = {
		type = "puntoTutorial",
		position = {1340,-1420,0},
	},

	triggerTutorial15 =
	{
		type = "triggerTutorial15",
		position = {1340,-1420,0},
	},

	puntoTutorial16 = {
		type = "puntoTutorial",
		position = {2220,-700,0},
	},

	triggerTutorial16 =
	{
		type = "triggerTutorial16",
		position = {2220,-700,0},
	},
}
