Map = {

--#################################################################################################################################################
--#####################################################   ESCENARIO    ############################################################################
--#################################################################################################################################################

	ParticulaPuertaCerrada = {
		type = "ParticulaPuertaCerrada",
		sourceParticles = {"particulaPuertaCerrada"},
	},

	ParticulaAmbientacion = {
		type = "PARTICULAAMBIENTACION",
		--boneParticles = {"-"},
		sourceParticles = {"particulaLava"},
	},
	
	TriggerPuertaVerticalDerecha = {
		type = "DOOR_VERTICAL_DER_TRIGGER",
		position = {100, -100, 0},
		tag = "door",
		orientation = {0, 0, 0},
		orientation_angle = 0,
		scale = 1,
		life = 100,
		model = "puertaConHuesos.mesh", --modelo puerta vertical
		material = "props/puerta",
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {10,30,30}, --dimensiones físicas puerta vertical
		physic_trigger = false,
		physic_group = 11,
		physic_mass = 1,
		doortrigger_dimensions = {5, 30, 30}, --trigger puertas verticales
		doortrigger_group = 5,
		offset = {5, 0, 0}, --puerta derecha
		scriptsLUA = {"DoorScript",},
		sourceParticles = {"LlamaCalaverasRoja"},
		particulaPuertaCerrada = "ParticulaPuertaCerrada",
	},
	
	TriggerPuertaVerticalIzquierda = {
		type = "DOOR_VERTICAL_IZQ_TRIGGER",
		position = {100, -100, 0},
		tag = "door",
		orientation = {0, 1, 0},
		orientation_angle = 180,
		scale = 1,
		life = 100,
		model = "puertaConHuesos.mesh", --modelo puerta vertical
		material = "props/puerta",
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {10,30,30}, --dimensiones físicas puerta vertical
		physic_trigger = false,
		physic_group = 11,
		physic_mass = 1,
		doortrigger_dimensions = {5, 30, 30}, --trigger puertas verticales
		doortrigger_group = 5,
		offset = {5, 0, 0}, --puerta izquierda
		scriptsLUA = {"DoorScript",},
		sourceParticles = {"LlamaCalaverasRoja"},
		particulaPuertaCerrada = "ParticulaPuertaCerrada",
	},
	
	TriggerPuertaHorizontal = {
		type = "DOOR_HORIZONTAL_TRIGGER",
		tag = "door",
		position = {100, -100, 0},
		orientation = {0, 0, -1},
		orientation_angle = 90,
		scale = 1,
		life = 100,
		model = "puertaConHuesos.mesh", --modelo puerta horizontal
		material = "props/puerta",
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {10,30,30}, --dimensiones físicas puerta horizontal
		physic_trigger = false,
		physic_group = 11,
		physic_mass = 1,
		doortrigger_dimensions = {5, 30, 30}, --trigger puertas horizontales
		doortrigger_group = 5,
		offset = {0, 0, 0}, --puerta abajo
		scriptsLUA = {"DoorScript",},
		sourceParticles = {"LlamaCalaverasRoja"},
		particulaPuertaCerrada = "ParticulaPuertaCerradaAbajo",
	},
	
	Antorcha = 
	{
		type = "Antorcha",
		sourceParticles = {"Antorcha"},
		position = {0, 0, -0.4},
		diffuse = {1, 0.3, 0},
		specular = {0.5, 0.5, 0.5},
		attenuation_range = 150,
		attenuation_constant = 1,
		attenuation_linear = 1,
		attenuation_quadratic = 0.1,
		torch_flickering = 350,
		torch_min_range = 1,--0.85,
		torch_max_range = 1, --1.15,
		torch_min_linear = 0.95,
		torch_max_linear = 1.05,
		torch_min_quadratic = 0.15,
		torch_max_quadratic = 1.85,
	},


--#################################################################################################################################################
--#####################################################   TILEADO    ################################################################################
--#################################################################################################################################################

	

	TileFondo1 = {
		type = "TILE_FONDO1",
		model = "Tile.mesh", --"Tile60.mesh",
		material = "Tiles/Lava/Fondo1",
		scale = {1, 1, 1},
		static = true,
	},

	TileFondo2 = {
		type = "TILE_FONDO2",
		model = "Tile.mesh",
		material = "Tiles/Lava/Fondo2",
		scale = {1, 1, 1},
		static = true,
	},

	TileFondo3 = {
		type = "TILE_FONDO3",
		model = "Tile.mesh",
		material = "Tiles/Lava/Fondo3",
		scale = {1, 1, 1},
		static = true,
	},

	TileFondo4 = {
		type = "TILE_FONDO4",
		model = "Tile.mesh",
		material = "Tiles/Lava/Fondo4",
		scale = {1, 1, 1},
		static = true,
	},

	TileCapa1 = {
		type = "TILE_DEFAULT",
		model = "Tile.mesh",  --"Tile.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 10, 10, 30}, --{ 10, 10, 25},
		physic_group = 1,
	},

	TileHielo = {
		type = "TILE_ICE",
		model = "Tile.mesh",  --"Tile.mesh",
		material = "Tiles/Lava/Hielo",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 10, 10, 30}, --{ 10, 10, 25},
		physic_group = 1,
	},

	TilePegajoso = {
		type = "TILE_GUM",
		model = "Tile.mesh",  --"Tile.mesh",
		material = "Tiles/Lava/Pegajoso",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 10, 10, 30}, --{ 10, 10, 25},
		physic_group = 1,
	},

	TILE_FRAGILE = {
		type = "TILE_FRAGILE",
		model = "Tile.mesh",  --"Tile.mesh",
		material = "Tiles/Lava/FragilRotura1",
		orientation = {1, 0, 0},
		orientation_angle = 180, --90, para el marine -- -90 para sai02
		scale = {1, 1, 1},
		static = true,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 10,10,30}, --{ 10, 10, 25},
		--physic_offset = { 0, 0, 0},
		physic_trigger = false,
		physic_group = 1,
		physic_mass = 1,
		tiletrigger_dimensions = {9, 5, 30}, --trigger puertas verticales
		offsetTrigger = {0, -10, 0}, --puerta derecha
		tiletrigger_group = 5,
		scriptsLUA = {"TileFragileScript",},
		-- Atributo para CTileFragile--
		numTouchsToBreak = 3,
		matBreak1 = "Tiles/Lava/FragilRotura1",
		matBreak2 = "Tiles/Lava/FragilRotura2",
		matBreak3 = "Tiles/Lava/FragilRotura3",

	},


	TILE_FRAGILE_BORDE = {
		type = "TILE_FRAGILE_BORDE",
		model = "Tile.mesh",  --"Tile.mesh",
		material = "Tiles/Lava/FragilRotura1",
		orientation = {1, 0, 0},
		orientation_angle = 180, --90, para el marine -- -90 para sai02
		scale = {1, 1, 1},
		static = true,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 10,10,30}, --{ 10, 10, 25},
		--physic_offset = { 0, 0, 0},
		physic_trigger = false,
		physic_group = 1,
		physic_mass = 1,
		tiletrigger_dimensions = {9, 5, 30}, --trigger puertas verticales
		offsetTrigger = {0, -10, 0}, --puerta derecha
		tiletrigger_group = 5,
		scriptsLUA = {"TileFragileScript",},
		-- Atributo para CTileFragile--
		numTouchsToBreak = 3,
		matBreak1 = "Tiles/Lava/FragilRotura1", ----FALTA MATERIAL CON BORDE
		matBreak2 = "Tiles/Lava/FragilRotura2",
		matBreak3 = "Tiles/Lava/FragilRotura3",
	},

	TileSlide1 = {
		type = "TILE_SLIDE_UR",
		model = "TileSlide01.mesh",      --"TileSlide0160.mesh",	---"TileSlide01.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSlide60.RepX",--"media/models/physics/TileSlide.RepX",
		physic_orientation = {0, 1, 0},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	TileSlide2 = {
		type = "TILE_SLIDE_UL",
		model = "TileSlide02.mesh", 
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSlide60.RepX",
		physic_orientation = {0, 0, 0},
		physic_orientation_angle = 0,
		physic_group = 1,
	},
	TileSlide3 = {
		type = "TILE_SLIDE_DR",
		model = "TileSlide03.mesh", 
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSlide60.RepX",
		physic_orientation = {0, 0, 1},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	TileSlide4 = {
		type = "TILE_SLIDE_DL",
		model = "TileSlide04.mesh", 
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSlide60.RepX",
		physic_orientation = {0, 0, 1},
		physic_orientation_angle = -90,
		physic_group = 1,
	},

	TileSlide5 = {
		type = "TILE_SEMISLIDE_UR1",
		model = "TileSemiSlide01.mesh",            --"TileSemiSlide0160.mesh",	---"TileSemiSlide01.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0160.RepX",---"media/models/physics/TileSemiSlide01.RepX",
		physic_orientation = {0, 0, 1},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	TileSlide6 = {
		type = "TILE_SEMISLIDE_UR2",
		model = "TileBigSemiSlide01.mesh",        --"TileSemiSlide0260.mesh",	---"TileSemiSlide02.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0260.RepX",	---"media/models/physics/TileSemiSlide02.RepX",
		physic_orientation = {1, 0, 0},
		physic_orientation_angle = 180,
		physic_group = 1,
	},
	TileSlide7 = {
		type = "TILE_SEMISLIDE_UL2",
		model = "TileBigSemiSlide02.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0260.RepX",
		physic_orientation = {0, 0, 1},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	TileSlide8 = {
		type = "TILE_SEMISLIDE_UL1",
		model = "TileSemiSlide02.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0160.RepX",
		physic_orientation = {1, 0, 0},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	TileSlide9 = {
		type = "TILE_SEMISLIDE_DR1",
		model = "TileSemiSlide03.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0160.RepX",
		physic_orientation = {0, 1, 0},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	TileSlide10 = {
		type = "TILE_SEMISLIDE_DR2",
		model = "TileBigSemiSlide03.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0260.RepX",
		physic_orientation = {0, 0, 0},
		physic_orientation_angle = 0,
		physic_group = 1,
	},

	TileSlide11 = {
		type = "TILE_SEMISLIDE_DL2",
		model = "TileBigSemiSlide04.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0260.RepX",
		physic_orientation = {0, 1, 0},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	TileSlide12 = {
		type = "TILE_SEMISLIDE_DL1",
		model = "TileSemiSlide04.mesh",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0160.RepX",
		physic_orientation = {0, 0, 0},
		physic_orientation_angle = 0,
		physic_group = 1,
	},


	---TILES CON BORDE

	--Falta material con borde
	TileCapa1Borde = {
		type = "TILE_DEFAULT_BORDE",
		model = "Tile.mesh",  --"Tile.mesh",
		-- material = "Tiles/Lava/FrontalBorde",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 10, 10, 30}, --{ 10, 10, 25},
		physic_group = 1,
	},

	--Falta material con borde
	TileSlide1Borde = {
		type = "TILE_SLIDE_UR_BORDE",
		model = "TileSlide01.mesh",      --"TileSlide0160.mesh",	---"TileSlide01.mesh",
		-- material = "Tiles/Lava/OblicuoBorde01",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSlide60.RepX",--"media/models/physics/TileSlide.RepX",
		physic_orientation = {0, 1, 0},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	---falta material con borde
	TileSlide2Borde = {
		type = "TILE_SLIDE_UL_BORDE",
		model = "TileSlide02.mesh", 
		-- material = "Tiles/Lava/OblicuoBorde02",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSlide60.RepX",
		physic_orientation = {0, 0, 0},
		physic_orientation_angle = 0,
		physic_group = 1,
	},

	--falta material con borde
	TileSlide5Borde = {
		type = "TILE_SEMISLIDE_UR1_BORDE",
		model = "TileSemiSlide01.mesh",            --"TileSemiSlide0160.mesh",	---"TileSemiSlide01.mesh",
		-- material = "Tiles/Lava/OblicuoBordeB01",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0160.RepX",---"media/models/physics/TileSemiSlide01.RepX",
		physic_orientation = {0, 0, 1},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	---falta material con borde
	TileSlide6Borde = {
		type = "TILE_SEMISLIDE_UR2_BORDE",
		model = "TileBigSemiSlide01.mesh",        --"TileSemiSlide0260.mesh",	---"TileSemiSlide02.mesh",
		-- material = "Tiles/Lava/OblicuoBordeA01",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0260.RepX",	---"media/models/physics/TileSemiSlide02.RepX",
		physic_orientation = {1, 0, 0},
		physic_orientation_angle = 180,
		physic_group = 1,
	},


	--falta material con borde
	TileSlide7Borde = {
		type = "TILE_SEMISLIDE_UL2_BORDE",
		model = "TileBigSemiSlide02.mesh",
		-- material = "Tiles/Lava/OblicuoBordeA02",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0260.RepX",
		physic_orientation = {0, 0, 1},
		physic_orientation_angle = 180,
		physic_group = 1,
	},

	--falta material con borde
	TileSlide8Borde = {
		type = "TILE_SEMISLIDE_UL1_BORDE",
		model = "TileSemiSlide02.mesh",
		-- material = "Tiles/Lava/OblicuoBordeB02",
		material = "Tiles/Lava/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0160.RepX",
		physic_orientation = {1, 0, 0},
		physic_orientation_angle = 180,
		physic_group = 1,
	},
	
	Puerta = {
		type = "TILE_DOOR",
		model = "Tile2.mesh",
		material = "door",
		scale = {1,1,1},
		static = true,
	},

	

}