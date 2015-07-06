Map = {

--#################################################################################################################################################
--#####################################################   ESCENARIO    ############################################################################
--#################################################################################################################################################

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
		sourceParticles = {"LlamaCalaverasVerde"},
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
		sourceParticles = {"LlamaCalaverasVerde"},
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
		sourceParticles = {"LlamaCalaverasVerde"},
		particulaPuertaCerrada = "ParticulaPuertaCerradaAbajo",
	},
	
	PuntoInteresCofre = {
		type = "PUNTO_INTERES_COFRE",
		position = {0, 0, 0},
	},

	PuntoInteresTrampa = {
		type = "PUNTO_INTERES_TRAMPA",
		position = {0, 0, 0},
	},

	Trampa = {
		type = "Trampa",
		position = {0, 0, 0},
		model = "barril.mesh",
		scale = 2, 
	},

	PlanoColision = {
		type = "PlanoColision",
		position = {0, 0, 0},
		static = true,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 10000, 10000, 1},
		physic_group = 0,
	},


	JumpRight = {
		type = "JumpRight",
		jump_direction = 1,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {10, 10, 25},
		physic_group = 5,
		physic_trigger = true,
	},
	JumpLeft = {
		type = "JumpLeft",
		jump_direction = -1,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {10, 10, 25},
		physic_group = 5,
		physic_trigger = true,
	},

	Chest = {
		type = "Chest",
		tag = "chest",
		orientation = {0, 1, 0},
		orientation_angle = 180,
		chest_level = 1,
		model = "cofrePeq.mesh",
		material = "Props/CofrePeq",
		defaultAnimation = "Idle",
		position = {0,0,0},
		scale = {0.2, 0.2, 0.2},
		static = false,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 6, 6, 40},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 5,
		scaleStaticText = {50,50,50},
		offsetStaticText = {5,20,40},
		scriptsLUA = {"ChestControllerScript",},
		timeOpen = 1000,
		materialBillboard = "lifeBar",
		dimensionBillboard = {18,2,0},
		offsetBillboard = {0,40,0},
		lifeBar = true,
		porcentToReduce = 5, --Cada vez que cogemos una habilidad de reducir tiempo para abrir cofre, reducimos un 3% 
		
		sourceParticles = {"CofrePeq"},
		--boneParticles = {"Bone.001_end",},
	},
	
	Chest2 = {
		type = "Chest2",
		tag = "chest",
		chest_level = 1,
		model = "cofreMed.mesh",
		material = "Props/CofreMed",
		defaultAnimation = "Idle",
		position = {0,0,0},
		scale = {1, 1, 1},
		static = false,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 6, 6, 40},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 5,
		scaleStaticText = {0,0,0},
		offsetStaticText = {5,20,40},
		scriptsLUA = {"ChestControllerScript",},
		timeOpen = 2000,
		materialBillboard = "lifeBar",
		dimensionBillboard = {18,2,0},
		offsetBillboard = {0,40,0},
		lifeBar = true,
		porcentToReduce = 4, --Cada vez que cogemos una habilidad de reducir tiempo para abrir cofre, reducimos un 4% 
		
		
		sourceParticles = {"CofreMed"},
		boneParticles = {"joint6",},
	},
	
	Chest3 = {
		type = "Chest3",
		tag = "chest",
		chest_level = 1,
		model = "cofreGra.mesh",
		material = "Props/CofreGra",
		defaultAnimation = "Idle",
		position = {0,0,0},
		scale = {0.11, 0.11, 0.11},
		static = false,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 6, 6, 40},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 5,
		scaleStaticText = {0,0,0},
		offsetStaticText = {5,20,40},
		scriptsLUA = {"ChestControllerScript",},
		timeOpen = 3000,
		materialBillboard = "lifeBar",
		dimensionBillboard = {18,2,0},
		offsetBillboard = {0,40,0},
		lifeBar = true,
		porcentToReduce = 3, --Cada vez que cogemos una habilidad de reducir tiempo para abrir cofre, reducimos un 5% 
		
		sourceParticles = {"CofreGra"},
	},

	LuzEscena = {
		type = "LuzEscena",
		position = {200, -200, 150},
		diffuse = { 0.8, 0.8, 0.8},
		specular = {0.7, 0.7, 0.8},
		direction = {0, -1, -1},
	},

--#################################################################################################################################################
--#####################################################   CAMARA Y PLAYER    ######################################################################
--#################################################################################################################################################

	Camera = {
		type = "Camera",
		targetDistance = 275,		
		targetHeight = 20,
		orientation = {0, 0, 0},
		orientation_angle = 0,
		relative_movement = 1,
		fov = 60,
		far_clip_distance = 400,
		near_clip_distance = 2,
	},

	CameraPause = {
		type = "CameraPause",
		offsetZ = -50,
		angle = 149.25,
	},

	SaiPause = 
	{
		type = "SaiPause",
		position = {200, -100, -300},
		entity_offset = {0, 15, 0},
		scale = 1,			---2.5 para el marine
		model = "sai01.mesh",                 -- Test 10/3/2015: 19
		material = "MaterialSaiPause",
		defaultAnimation = "Ihgh", --"IdleKatana", para el marineS
		weaponR = "None",
		weaponL = "None",
		animation_config = "SaiAnimations",
		sourceParticles = {"particulaCabezaSai",},
		boneParticles = {"Bone.004",},
	},

	Sai = {
		type = "Player",
		tag = "player",
		position = {200, -100, 0},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = -90, --90, para el marine -- -90 para sai02
		scale = 1,			---2.5 para el marine
		material = "MaterialSai",
		materialPoison = "MaterialSaiVeneno",
		materialFrozen = "MaterialSaiCongelado",
		-- life = 100.0,
		proporcionShake = 0.7,
		isPlayer = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {10,0.5,0},
		model = "sai01.mesh",
		-- base_damage = 10,
		-- maxHorizontalSpeed = 200,          -- Test 9/3/2015: 0.2
		horizontalAcceleration = 0.0011,   -- Test 9/3/2015: 0.0011
		numJumpMax = 2,
		jumpHeight = 19,                   -- Test 10/3/2015: 19
		initialJumpSpeed = 0.28,           -- Test 10/3/2015: 0.28
		gravityAcceleration = 0.00075,     -- Test 10/3/2015: 0.00075
		minJumpSpeed = 0.01,
		maxFallingSpeed = 0.5,
		defaultAnimation = "Ihgh", --"IdleKatana", para el marine
		physic_shape = "capsule",
		physic_radius = 5,
		physic_height = 22,
		physic_group = 4,
		lookingDirection = 1,
		weaponR = "None",
		weaponL = "None",
		maxDelayGrabWeapon = 1000,
		shoot = false,
		scriptsLUA = {"ChestOpenerScript",},
		sourceParticles = {"particulaCabezaSai",},
		boneParticles = {"Bone.004",},
		--boneParticles = {"CATRigLArmPalm"},

		--SONIDOS
		-- sounds = {SoundEvents.heartbeat,},
		-- started_sounds = {SoundEvents.heartbeat,},
		deathSound = SoundEvents.Muerte3, --sonido de muerte

		animation_config = "SaiAnimations",
		componente_vida = "CLifePlayer",
		-- maxDelayMainAtk = 300,
		-- maxDelaySpecialAtk = 600,
		maxWaitTime = 3000,
		--atributos trail
		trail_center = true,
		trail_material = "Trails/Jump",
		trail_color_change = {8, 8, 8},
		trail_width = 5,
		trail_width_change = 0,
		trail_length = 60,
		trail_visible = false,

		trail2_material = "Trails/Jump",
		trail2_color_change = {8, 8, 8},
		trail2_width = 5,
		trail2_width_change = 0,
		trail2_length = 60,
		trail2_visible = false,
		trail2_offset = {0, 16, 0},

		-- distanceToAument = 20, --Distancia a aumentar en la vision del PJ cada vez que se coge una 
		lifeMaxToAument = 1,
		
		maxDistance = 20,
	},

	LuzPlayer = {
		type = "LuzPlayer",
		diffuse = {1, 1, 1},
		specular = {1, 1, 1},
		attenuation_range = 60,
		attenuation_constant = 1,
		attenuation_linear = 0.5,
		attenuation_quadratic = 0.5,
		player_offset = {0, 15 , -20},
	},

	MovableText = {
		type = "MovableText",
		time_to_live = 3.5,
		position = {200, -200, 0},
		scaleMovableText = {10, 10, 1},
		speed = 0.04,
	},

	MovableTextDamage = {
		type = "MovableTextDamage",
		time_to_live = 0.5,
		position = {200, -200, 0},
		scaleMovableText = {5,5,1},
		speed = 0.02,
	},

--#################################################################################################################################################
--#####################################################   ENEMIGOS    #############################################################################
--#################################################################################################################################################
	
	-- ENEMIGOS FINALES --
	Wompy = {
		type = "Wompy",
		tag = "enemy",
		position = {400, -150, 1},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		entity_offset = {0, 7.5, 0},
		scale = 1,
		model = "enemigo_basico.mesh",
		material = "Enemigos/Basico",
		materialCanibal = "Enemigos/BasicoCanibal",
		materialPoison = "Enemigos/BasicoVeneno",
		materialFrozen = "Enemigos/BasicoCongelacion",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 7.5,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		vida = 10,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach",
		accelMovApproach = 0.0001,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
	},

	SpiderDodge = {
		type = "SpiderDodge",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "spider.mesh",
		material = "Enemigos/Spider",
		materialCanibal = "Enemigos/SpiderCanibal",
		materialPoison = "Enemigos/SpiderVeneno",
		materialFrozen = "Enemigos/SpiderCongelacion",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 7.5,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		vida = 15,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach_Dodge",
		accelMovApproach = 0.0001,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		maxDelayDodge = 100,          --- Dodge
		rangeDetectDodge = 100,       --- Dodge
		sucessPercentDodge = 90,      --- Dodge
		accelDodge = 0.0005,          --- Dodge
	},

	SpiderKami = {
		type = "SpiderKami",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "spider.mesh",
		material = "Enemigos/SpiderBomb",
		materialCanibal = "Enemigos/SpiderBombCanibal",
		materialPoison = "Enemigos/SpiderBombVeneno",
		materialFrozen = "Enemigos/SpiderBombCongelacion",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 7.5,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		vida = 10,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,
		sourceParticles = {"ChispaSpiderKami"},
		startVisibleParticles = false,
		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach_Kamikaze",
		accelMovApproach = 0.0001,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		damageKamikaze = 80,          --- Kamikaze
		rangeKamikaze = 50,           --- Kamikaze
		maxTimeKamikaze = 1000,       --- Kamikaze
	},

	SkullSummon = {
		type = "SkullSummon",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1.5,
		model = "fantasma.mesh",
		material = "Enemigos/Fantasma",
		materialCanibal = "Enemigos/FantasmaCanibal",
		materialPoison = "Enemigos/FantasmaVeneno",
		materialFrozen = "Enemigos/FantasmaCongelacion",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "VolarX",
		static = false,
		physic_shape = "capsule",
		physic_radius = 9,
		physic_height = 10,
		physic_group = 13,
		physic_offset = { 0, -10, 0},
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 0,
		rangoDestino = 300,
		vida = 15,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_Summon",
		accelMovApproach = 0.0001,     --- Approach
		maxVelApproach = 0.08,         --- Approach
		minDistSlowdownApproach = 100, --- Approach
		typeMinionSummon = "MinionSummonUniDir",   --- Summon
		distMinionSummon = 20,         --- Summon
		maxMinionsSummon = 6,          --- Summon
		velRotMinionSummon = 0.1,      --- Summon
		maxDelaySendSummon = 2000,     --- Summon
		maxDelayInvokeSummon = 3000,   --- Summon
	},

	SkullShoot = {
		type = "SkullShoot",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 7, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		materialCanibal = "Enemigos/VoladorCanibal",
		materialPoison = "Enemigos/VoladorVeneno",
		materialFrozen = "Enemigos/VoladorCongelacion",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 7.5,
		physic_offset = {0, -7.5, 0},
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 0,
		rangoDestino = 300,
		vida = 20,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_ShootRaycast",
		accelMovApproach = 0.0011,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		approachMDwithRange = true,   --- ApproachMultiDir
		minDistSlowdownApproach = 5,  --- Approach
		typeBullet = "EnemyBullet",   --- ShootPlayer/Raycast
		maxDelayShoot = 750,          --- ShootPlayer/Raycast
		rangeShoot = 4000,            --- ShootPlayer/Raycast
		bulletDmgShoot = 10,          --- ShootPlayer/Raycast
	},

	Chulu = {
		type = "Chulu",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 8, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "Cthulhu.mesh",
		material = "Enemigos/Chulu",
		materialCanibal = "Enemigos/ChuluCanibal",
		materialPoison = "Enemigos/ChuluVeneno",
		materialFrozen = "Enemigos/ChuluCongelacion",
		animation_config = "CthulhuAnimations",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Idle",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 8,
		physic_group = 13,
		physic_offset = {0, -7, 0},
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 0,
		rangoDestino = 300,
		vida = 20,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_StealLife",
		accelMovApproach = 0.0011,    --- Approach
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 40, --- Approach
		maxDelayStealLife = 1000,     --- StealLife
		ammountStealLife = 10,        --- StealLife
		minDistanceStealLife = 50,    --- StealLife
	},

	MinionSummonKami = {
		type = "MinionSummonKami",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 5, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemBullet.mesh",
		material = "bala/EnemBalaRoja",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 5,
		physic_height = 3,
		physic_group = 3,
		physic_offset = {0, -3, 0},
		speed = 0.2,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 0,
		rangoDestino = 300,
		life = 70,
		damage = 0,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,
		-- Particula
		sourceParticles = {"MinionParticle"},

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "None",
		accelMovApproach = 0.0011,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 1,  --- Approach
		damageKamikaze = 50,          --- Kamikaze
		rangeKamikaze = 10,           --- Kamikaze
		maxTimeKamikaze = 100,        --- Kamikaze
	},

	MinionSummonUniDir = {
		type = "MinionSummonUniDir",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 6, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemBullet.mesh",
		material = "bala/EnemBalaRoja",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 5,
		physic_height = 5,
		physic_group = 6,
		physic_offset = {0, -8, 0},
		speed = 0.2,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 0,
		rangoDestino = 300,
		life = 70,
		damage = 0,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,
		-- Particula
		sourceParticles = {"MinionParticle"},

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "None",
		accelMovApproach = 0.0011,    --- Approach/MultiDir
		maxVelApproach = 0.14,        --- Approach
		minDistSlowdownApproach = 1,  --- Approach
		time_to_live = 2,
	},

	------------- ELITES --------------

	WompyElite = {
		type = "WompyElite",
		tag = "enemy",
		position = {400, -150, 1},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		entity_offset = {0, 7.5, 0},
		scale = 1.7,
		model = "enemigo_basico.mesh",
		material = "Enemigos/BasicoElite",
		materialCanibal = "Enemigos/BasicoEliteCanibal",
		materialPoison = "Enemigos/BasicoEliteVeneno",
		materialFrozen = "Enemigos/BasicoEliteCongelacion",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 17,
		physic_height = 17,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach",
		accelMovApproach = 0.0002,    --- Approach/MultiDir
		maxVelApproach = 0.12,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
	},

	SpiderDodgeElite = {
		type = "SpiderDodgeElite",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1.7,
		model = "spider.mesh",
		material = "Enemigos/SpiderElite",
		materialCanibal = "Enemigos/SpiderEliteCanibal",
		materialPoison = "Enemigos/SpiderEliteVeneno",
		materialFrozen = "Enemigos/SpiderEliteCongelacion",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 12.75,
		physic_height = 12.75,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach_Dodge",
		accelMovApproach = 0.0001,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		maxDelayDodge = 100,          --- Dodge
		rangeDetectDodge = 100,       --- Dodge
		sucessPercentDodge = 90,      --- Dodge
		accelDodge = 0.0005,          --- Dodge
	},

	SpiderKamiElite = {
		type = "SpiderKamiElite",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1.7,
		model = "spider.mesh",
		material = "Enemigos/SpiderBombElite",
		materialCanibal = "Enemigos/SpiderBombEliteCanibal",
		materialPoison = "Enemigos/SpiderBombEliteVeneno",
		materialFrozen = "Enemigos/SpiderBombEliteCongelacion",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 12.75,
		physic_height = 12.75,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,
		sourceParticles = {"ChispaSpiderKami"},
		startVisibleParticles = false,
		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach_Kamikaze",
		accelMovApproach = 0.0001,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		damageKamikaze = 80,          --- Kamikaze
		rangeKamikaze = 50,           --- Kamikaze
		maxTimeKamikaze = 1000,       --- Kamikaze
	},

	SkullSummonElite = {
		type = "SkullSummonElite",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 2,
		model = "fantasma.mesh",
		material = "Enemigos/FantasmaElite",
		materialCanibal = "Enemigos/FantasmaEliteCanibal",
		materialPoison = "Enemigos/FantasmaEliteVeneno",
		materialFrozen = "Enemigos/FantasmaEliteCongelacion",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "VolarX",
		static = false,
		physic_shape = "capsule",
		physic_radius = 18,
		physic_height = 20,
		physic_group = 13,
		physic_offset = { 0, -20, 0},
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 0,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_Summon",
		accelMovApproach = 0.0001,     --- Approach
		maxVelApproach = 0.08,         --- Approach
		minDistSlowdownApproach = 100, --- Approach
		typeMinionSummon = "MinionSummonUniDirElite",   --- Summon
		distMinionSummon = 20,         --- Summon
		maxMinionsSummon = 6,          --- Summon
		velRotMinionSummon = 0.1,      --- Summon
		maxDelaySendSummon = 2000,     --- Summon
		maxDelayInvokeSummon = 3000,   --- Summon
	},

	SkullShootElite = {
		type = "SkullShootElite",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 7, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1.7,
		model = "volador.mesh",
		material = "Enemigos/VoladorElite",
		materialCanibal = "Enemigos/VoladorEliteCanibal",
		materialPoison = "Enemigos/VoladorEliteVeneno",
		materialFrozen = "Enemigos/VoladorEliteCongelacion",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 12.75,
		physic_height = 12.75,
		physic_offset = {0, -12.75, 0},
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 0,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_ShootRaycast",
		accelMovApproach = 0.0011,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		approachMDwithRange = true,   --- ApproachMultiDir
		minDistSlowdownApproach = 5,  --- Approach
		typeBullet = "EnemyBullet",   --- ShootPlayer/Raycast
		maxDelayShoot = 750,          --- ShootPlayer/Raycast
		rangeShoot = 4000,            --- ShootPlayer/Raycast
		bulletDmgShoot = 10,          --- ShootPlayer/Raycast
	},

	ChuluElite = {
		type = "ChuluElite",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 8, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1.5,
		model = "Cthulhu.mesh",
		material = "Enemigos/ChuluElite",
		materialCanibal = "Enemigos/ChuluEliteCanibal",
		materialPoison = "Enemigos/ChuluEliteVeneno",
		materialFrozen = "Enemigos/ChuluEliteCongelacion",
		animation_config = "CthulhuAnimations",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Idle",
		static = false,
		physic_shape = "capsule",
		physic_radius = 11.25,
		physic_height = 12,
		physic_group = 13,
		physic_offset = {0, -10.5, 0},
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 0,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_StealLife",
		accelMovApproach = 0.0011,    --- Approach
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 40, --- Approach
		maxDelayStealLife = 1000,     --- StealLife
		ammountStealLife = 10,        --- StealLife
		minDistanceStealLife = 50,    --- StealLife
	},


	MinionSummonUniDirElite = {
		type = "MinionSummonUniDirElite",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 6, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemBullet.mesh",
		material = "bala/EnemBalaRoja",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 5,
		physic_height = 5,
		physic_group = 6,
		physic_offset = {0, -8, 0},
		speed = 0.2,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 0,
		rangoDestino = 300,
		life = 70,
		damage = 0,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,
		-- Particula
		sourceParticles = {"MinionParticle"},

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "None",
		accelMovApproach = 0.0011,    --- Approach/MultiDir
		maxVelApproach = 0.14,        --- Approach
		minDistSlowdownApproach = 1,  --- Approach
		time_to_live = 2,
	},

	-- ENEMIGOS DE PRUEBA --
	Mewtwo = {
		type = "Mewtwo",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		numJumpMax = 0,
		minJumpSpeed = 0.01,
		rangoDestino = 300,
		componente_vida = "CLifeEnemy",
		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_ShootRaycast",
		accelMovApproach = 0.0011,    --- Approach
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		typeBullet = "EnemyBullet",   --- ShootPlayer/Raycast/UniDir
		maxDelayShoot = 1000,         --- ShootPlayer/Raycast/UniDir
		rangeShoot = 200,             --- ShootPlayer/Raycast/UniDir
	},

	Dragonite = {
		type = "Dragonite",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemigo_basico.mesh",
		material = "Enemigos/Basico",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 3,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		componente_vida = "CLifeEnemy",

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Wander",
		movementTolerance = 0.01,     -- Steering
		slowdownMovement = 0.96,      -- Steering
		accelMovApproach = 0.0011,    --- Approach
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		damageKamikaze = 90,          --- Kamikaze
		rangeKamikaze = 100,          --- Kamikaze
		maxTimeKamikaze = 3000,       --- Kamikaze
		maxDelayDodge = 1000,         --- Dodge
		rangeDetectDodge = 100,       --- Dodge
		sucessPercentDodge = 70,      --- Dodge
		accelDodge = 0.0005,          --- Dodge
		maxTimeDefend = 1000,         --- Defend
		rangeDefend = 100,            --- Defend
		maxVelPong = 0.08,            --- Pong
		maxTimeWander = 3000,         --- Wander
		maxTimeWait = 2000,           --- Wander
		maxVelWander = 0.03,          --- Wander
		typeBullet = "EnemyBullet",   --- ShootPlayer/Raycast/UniDir
		maxDelayShoot = 1000,         --- ShootPlayer/Raycast/UniDir
		rangeShoot = 200,             --- ShootPlayer/Raycast/UniDir
		dirShoot = "Down",            --- ShootUniDir
		distMinionSummon = 20,        --- Summon
		velRotMinionSummon = 0.1,     --- Summon
	},

	EnemAppr = {
		type = "EnemAppr",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemigo_basico.mesh",
		material = "Enemigos/Basico",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach",
		accelMovApproach = 0.0001,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
	},

	EnemPong = {
		type = "EnemPong",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemigo_basico.mesh",
		material = "Enemigos/Basico",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Pong",
		movementTolerance = 0.01, -- Steering
		slowdownMovement = 0.96,  -- Steering
		maxVelPong = 0.08,        --- Pong
	},

	EnemApprShootPl = {
		type = "EnemApprShootPl",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach_ShootPlayer",
		accelMovApproach = 0.0011,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		typeBullet = "EnemyBullet",   --- ShootPlayer/Raycast
		maxDelayShoot = 1000,         --- ShootPlayer/Raycast
		rangeShoot = 200,             --- ShootPlayer/Raycast
	},

	EnemShootRay = {
		type = "EnemShootRay",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ShootRaycast",
		typeBullet = "EnemyBullet", --- ShootPlayer/Raycast
		maxDelayShoot = 1000,       --- ShootPlayer/Raycast
		rangeShoot = 200,           --- ShootPlayer/Raycast/UniDir
	},

	EnemPongShootPl = {
		type = "EnemPongShootPl",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Pong_ShootPlayer",
		movementTolerance = 0.01, -- Steering
		slowdownMovement = 0.96,  -- Steering
		maxVelPong = 0.08,        --- Pong
		typeBullet = "EnemyBullet", --- ShootPlayer/Raycast/UniDir
		maxDelayShoot = 1000,       --- ShootPlayer/Raycast/UniDir
		rangeShoot = 200,           --- ShootPlayer/Raycast/UniDir
	},

	EnemShootUni = {
		type = "EnemShootUni",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, -1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ShootUniDir",
		movementTolerance = 0.01,   -- Steering
		slowdownMovement = 0.96,    -- Steering
		typeBullet = "EnemyBullet", --- ShootPlayer/Raycast/UniDir
		maxDelayShoot = 500,       --- ShootPlayer/Raycast/UniDir
		rangeShoot = 700,           --- ShootPlayer/Raycast/UniDir
		dirShoot = "Left",         --- ShootUniDir
	},

	EnemApprWander = {
		type = "EnemApprWander",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemigo_basico.mesh",
		material = "Enemigos/Basico",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachToWander",
		accelMovApproach = 0.0001,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		maxTimeWander = 2000,         --- Wander
		maxTimeWait = 1000,           --- Wander
		maxVelWander = 0.03,          --- Wander
	},

	EnemApprMDShootRay = {
		type = "EnemApprMDShootRay",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_ShootRaycast",
		accelMovApproach = 0.0011,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		approachMDwithRange = true,   --- ApproachMultiDir
		minDistSlowdownApproach = 5, --- Approach
		typeBullet = "EnemyBullet",   --- ShootPlayer/Raycast
		maxDelayShoot = 750,         --- ShootPlayer/Raycast
		rangeShoot = 400,             --- ShootPlayer/Raycast
	},

	EnemSeekRayShootRay = {
		type = "EnemSeekRayShootRay",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "SeekRay_ShootRaycast",
		velRotSeekRay = 1,          --- SeekRay
		maxDistSeekRay = 1000,         --- SeekRay
		maxDelayTurnSeekRay = 2000,   --- SeekRay
		minAngleSeekRay = 0,          --- SeekRay
		maxAngleSeekRay = 1000,          --- SeekRay
		initialDirSeekRay = 1,        --- SeekRay
		typeBullet = "EnemyBullet",   --- ShootPlayer/Raycast
		maxDelayShoot = 750,          --- ShootPlayer/Raycast
		rangeShoot = 400,             --- ShootPlayer/Raycast
	},

	EnemApprKami = {
		type = "EnemApprKami",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemigo_basico.mesh",
		material = "Enemigos/Basico",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach_Kamikaze",
		accelMovApproach = 0.0001,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		damageKamikaze = 80,          --- Kamikaze
		rangeKamikaze = 50,           --- Kamikaze
		maxTimeKamikaze = 1000,       --- Kamikaze
	},

	EnemApprMDKami = {
		type = "EnemApprMDKami",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_Kamikaze",
		accelMovApproach = 0.0011,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		damageKamikaze = 90,          --- Kamikaze
		rangeKamikaze = 100,          --- Kamikaze
		maxTimeKamikaze = 3000,       --- Kamikaze
	},

	

	EnemApprDodge = {
		type = "EnemApprDodge",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemigo_basico.mesh",
		material = "Enemigos/Basico",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach_Dodge",
		accelMovApproach = 0.0001,    --- Approach/MultiDir
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		maxDelayDodge = 100,          --- Dodge
		rangeDetectDodge = 100,       --- Dodge
		sucessPercentDodge = 100,      --- Dodge
		accelDodge = 0.0005,          --- Dodge
	},

	EnemApprMDSummon = {
		type = "EnemApprMDSummon",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "fantasma.mesh",
		material = "Enemigos/Fantasma",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "VolarX",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_Summon",
		accelMovApproach = 0.0001,     --- Approach
		maxVelApproach = 0.02,         --- Approach
		minDistSlowdownApproach = 100, --- Approach
		typeMinionSummon = "MinionSummonUniDir",   --- Summon
		distMinionSummon = 20,         --- Summon
		maxMinionsSummon = 6,          --- Summon
		velRotMinionSummon = 0.1,      --- Summon
		maxDelaySendSummon = 2000,     --- Summon
		maxDelayInvokeSummon = 3000,   --- Summon
	},

	

	EnemApprMDDodge = {
		type = "EnemApprMDDodge",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_Dodge",
		accelMovApproach = 0.0001,    --- Approach
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		maxDelayDodge = 1000,         --- Dodge
		rangeDetectDodge = 100,       --- Dodge
		sucessPercentDodge = 10,      --- Dodge
		accelDodge = 0.0005,          --- Dodge
	},

	EnemTeleport = {
		type = "EnemTeleport",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Teleport",
		maxDelayTeleporting = 250,   --- Teleport
		maxCooldownTeleport = 250,   --- Teleport
	},

	EnemApprDefend = {
		type = "EnemApprDefend",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 90,
		scale = 1,
		model = "enemigo_basico.mesh",
		material = "Enemigos/Basico",
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Walk",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 3,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0.0005,
		maxFallingSpeed = 0.3,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "Approach_Defend",
		accelMovApproach = 0.0011,    --- Approach
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		maxTimeDefend = 1000,         --- Defend
		rangeDefend = 100,            --- Defend
		sucessPercentDefend = 50,     --- Defend
	},

	EnemApprMDDefend = {
		type = "EnemApprMDDefend",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "volador.mesh",
		material = "Enemigos/Volador",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Aleteo",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_Defend",
		accelMovApproach = 0.0011,    --- Approach
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 10, --- Approach
		maxTimeDefend = 1000,         --- Defend
		rangeDefend = 100,            --- Defend
		sucessPercentDefend = 50,     --- Defend
	},

	EnemApprMDStealLife = {
		type = "EnemApprMDStealLife",
		tag = "enemy",
		position = {400, -150, 1},
		entity_offset = {0, 15, 0},
		orientation = {0, 1, 0},
		orientation_angle = 60,
		scale = 1,
		model = "Cthulhu.mesh",
		material = "Enemigos/Cthulhu",
		animation_config = "CthulhuAnimations",
		volador = true,
		materialLifeBar = "lifeBar",
		dimensionLifeBar = {15,2,0},
		defaultAnimation = "Idle",
		static = false,
		physic_shape = "capsule",
		physic_radius = 7.5,
		physic_height = 15,
		physic_group = 13,
		speed = 0.08,
		angularSpeed = 0.003,
		accel = 0.001,
		angularAccel = 0.00001,
		lookingDirection = 1,
		gravityAcceleration = 0,
		maxFallingSpeed = 0,
		minJumpSpeed = 0.01,
		initialJumpSpeed = 0.2,
		jumpHeight = 30,
		numJumpMax = 1,
		rangoDestino = 300,
		life = 70,
		damage = 5,
		attack_time = 750,
		componente_vida = "CLifeEnemy",
		tier = 1,

		-- Variables para IA
		scriptsLUA = {"EnemyStateMachineScript",},
		defaultIA = "ApproachMultiDir_StealLife",
		accelMovApproach = 0.0011,    --- Approach
		maxVelApproach = 0.08,        --- Approach
		minDistSlowdownApproach = 40, --- Approach
		maxDelayStealLife = 1000,     --- StealLife
		ammountStealLife = 10,        --- StealLife
		minDistanceStealLife = 60,    --- StealLife
	},

	Spawner = {
		type = "SpawnPoint",
		position = {0, 0, 0},
		spawn_time = 5,
		scriptsLUA = {"EnemySpawner",},
	},



--#################################################################################################################################################
--#####################################################   BALAS    ################################################################################
--#################################################################################################################################################

	GreenSwordSlash = {
		type = "GreenSwordSlash",
		basic_shape = "Prefab_Plane",
		material = "SwordSlash",
		scale = {0.13,0.13,0.13},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {10, 10, 60},
		physic_offset = {0,0, -30},
		physic_trigger = true,
		physic_group = 8, --No colisiona ni con tiles ni con player
		physic_mass = 1,
		time_to_live = 3,
		speed = 0.18,
		sourceParticles = {"slash",},
	},

	GreenSwordSlash2 = {
		type = "GreenSwordSlash2",
		basic_shape = "Prefab_Plane",
		material = "SwordSlash",
		scale = {0.195,0.195,0.195},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {15, 15, 60},
		physic_offset = {0,0, -30},
		physic_trigger = true,
		physic_group = 8,
		physic_mass = 1,
		time_to_live = 3,
		speed = 0.17,
		sourceParticles = {"slash2",},
	},

	GreenSwordSlash3 = {
		type = "GreenSwordSlash3",
		basic_shape = "Prefab_Plane",
		material = "SwordSlash",
		scale = {0.26,0.26,0.26},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {20, 20, 60},
		physic_offset = {0,0, -30},
		physic_trigger = true,
		physic_group = 8,
		physic_mass = 1,
		time_to_live = 3,
		speed = 0.16,
		sourceParticles = {"slash3",},
	},

	GreenSwordSlash4 = {
		type = "GreenSwordSlash4",
		basic_shape = "Prefab_Plane",
		material = "SwordSlash",
		scale = {0.39,0.39,0.39},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {30, 30, 60},
		physic_offset = {0,0, -30},
		physic_trigger = true,
		physic_group = 8,
		physic_mass = 1,
		time_to_live = 3,
		speed = 0.12,
		sourceParticles = {"slash4",},
	},

	GreenSwordSlash5 = {
		type = "GreenSwordSlash5",
		basic_shape = "Prefab_Plane",
		material = "SwordSlash",
		scale = {0.65,0.65,0.65},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {50, 50, 60},
		physic_offset = {0,0, -30},
		physic_trigger = true,
		physic_group = 8,
		physic_mass = 1,
		time_to_live = 3,
		speed = 0.1,
		sourceParticles = {"slash5",},
	},


	LaserBeam = {
		type = "LaserBeam",
		trail_center = true,
		trail_visible = false,
		trail_material = "Trails/Laser",
		trail_width = 10,
		trail_width_change = 30,
		trail_length = 400,
		trail_color_change = {8, 8, 8},
		particlesBeamOffset = 25,
	},

	LaserBall = {
		type = "LaserBall",
		tag = "laserBall",
		position = {0, 0, 0},
		speed = 0.3,
		rotateBullet = false,
		distance = 0,
		model = "laserBall.mesh",
		material = "bala/LaserBall",
		scale = {1, 1, 1},
		static = false,
		collideTile = "true",
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_group = 16,
		physic_mass = 1,
		physic_trigger = true,
		scriptsLUA = {"LaserBallScript",},
		sourceParticles = {"LaserBallParticle",},
		cointrigger_dimensions = {10, 10, 10},
		cointrigger_group = 8,
		time_to_activate_trigger = 0,
		-- Atributos del controller
		velMov = 0.08,
		timeUntilExplode = 2000,
		baseRange = 80,		
		baseDmg = 20,
		multiplRange = 10,
		multiplDmg = 1.5,
	},

	BulletDart = {
		type = "BulletDart",
		position = {0, 0, 0},
		speed = 0.3,
		rotateBullet = true,
		distance = 0,
		model = "bulletDart.mesh",
		material = "bala/Dart",
		scale = {0.3, 0.3, 0.3},
		static = false,
		collideTile = "true",
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {2, 6, 10},
		physic_trigger = true,
		physic_group = 9,
		physic_mass = 1,
		time_to_live = 0.5, --tiempo mÃ¡ximo de la bala en el aire, en segundos
		scriptsLUA = {"DartTimeToLiveScript"},

		trail_center = false,
		trail_material = "Trails/GreenDart",
		trail_color_change = {8, 8, 8},
		trail_width = 3.5,
		trail_width_change = 10,
		trail_length = 70,
		trail_visible = true,	},

	BulletDartCanibal = {
		type = "BulletDartCanibal",
		position = {0, 0, 0},
		speed = 0.3,
		rotateBullet = true,
		distance = 0,
		model = "bulletDart.mesh",
		material = "bala/DartCanibal",
		scale = {0.4, 0.4, 0.4},
		static = false,
		collideTile = "true",
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {2, 7, 10},
		physic_trigger = true,
		physic_group = 9,
		physic_mass = 1,
		time_to_live = 0.5, --tiempo mÃ¡ximo de la bala en el aire, en segundos
		scriptsLUA = {"DartTimeToLiveScript"},
		

		trail_center = false,
		trail_material = "Trails/CanibalDart",
		trail_color_change = {8, 8, 8},
		trail_width = 3.5,
		trail_width_change = 10,
		trail_length = 70,
		trail_visible = true,
	},

	BulletDartCanibalSpecial = {
		type = "BulletDartCanibalSpecial",
		position = {0, 0, 0},
		speed = 0.3,
		rotateBullet = true,
		distance = 0,
		model = "bulletDart.mesh",
		material = "bala/DartCanibal",
		scale = {0.6, 0.6, 0.6},
		static = false,
		collideTile = "true",
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {3, 8, 10},
		physic_trigger = true,
		physic_group = 9,
		physic_mass = 1,
		time_to_live = 2, --tiempo máximo de la bala en el aire, en segundos
		-- sourceParticles = {"torchFireRed"},
		scriptsLUA = {"BulletDartCanibalSpecialScript",},
		rangeCanibal = 80,
	},

	TongueBall = {
		type = "TongueBall",
		tag = "tongueBall",
		static = false,
		scale = {1.7, 1.7, 1.7},
		model = "laserBall.mesh",
		material ="bala/EnemBalaRoja",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_radius = 6.5,
		physic_trigger = false,
		physic_group = 9,
		physic_mass = 1,
		physic_material = "coin", --material físico
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		cointrigger_dimensions = {8, 8, 8},
		cointrigger_group = 9,
		time_to_activate_trigger = 0,
		-- Atributos del controller
		start_impulse = 350, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 0, -- angulo de dispersión de la moneda al salir
		-- Atributos de sangre
		scriptsLUA = {"TongueBallSpecialScript",},
		-- Particula
		sourceParticles = {"MinionParticle"},
	},

	NormalBullet = {
		type = "NormalBullet",
		position = {0, 0, 0},
		speed = 0.25,
		rotateBullet = false,
		distance = 0,
		model = "bala1.mesh",
		scale = {0.3, 0.3, 0.3},
		static = false,
		collideTile = "true",
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {5, 5, 10},
		physic_trigger = true,
		physic_group = 9,
		physic_mass = 1,
		time_to_live = 2, --tiempo máximo de la bala en el aire, en segundos
		-- sourceParticles = {"torchFireRed"},
	},

	EnemyBullet = {
		type = "EnemyBullet",
		position = {0, 0, 0},
		speed = 0.12,
		rotateBullet = false,
		entity_offset = {0, 5, 0},
		distance = 0,
		model = "enemBullet.mesh",
		material = "bala/EnemBalaVerde";
		scale = {1.8, 1.8, 1.8},
		static = false,
		collideTile = "true",
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {5, 5, 10},
		physic_trigger = true,
		physic_group = 6,
		physic_mass = 1,
		time_to_live = 5, --tiempo máximo de la bala en el aire, en segundos
		sourceParticles = {"EnemBulletParticle"},
	},

	Explosion = {
		type = "Explosion",
		source = "explosion",
		position = {0, 0, 1},
		material = "explosion",
		scale = 0.5,
		quota = 100,
		time_to_live = 5,
	},


--#################################################################################################################################################
--#####################################################   LUCES    ################################################################################
--#################################################################################################################################################

	Puntual = {
		type = "Puntual",
		position = {400, -100, 0},
		diffuse = {0, 1, 0},
		specular = {0, 1, 0},
	},

	Directional = {
		type = "Directional",
		position = {0, 150, 0},
		diffuse = { 1, 0, 0},
		specular = {1, 0, 0},
		direction = {0, 1, 0},
	},

	SpotL = {
		type = "Spotlight",
		position = {400, -100, 0},
		diffuse = {0, 0, 1},
		specular = {0, 0, 1},
		direction = {-1, -1, 0},
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
--#####################################################   OBJETOS USABLES    ######################################################################
--#################################################################################################################################################

	PowerUpSalto = {
		type = "UsableObjectSalto",
		tag = "powerup",
		skill_name = "AumentarSalto",
		skill_name_desc = "Extra Jump",
		skill_description = "You get one extra jump",
		basic_shape = "Prefab_Plane",
		material = "Habilidades/UpSalto",
		scale = {0.13,0.13,0.13},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"HabilidadesScript",},
		icono = "NewHUD/iconoJumpUp",
	},

	BarraVidaEnemigo = {
		type = "UsableObjectBarraVidaEnemigo",
		tag = "powerup",
		skill_name = "BarraVidaEnemigo",
		skill_name_desc = "Over9000 scouter",
		skill_description = "You can see the enemys's life",
		basic_shape = "Prefab_Plane",
		material = "Habilidades/BarraVidaEnemigo",
		scale = {0.13,0.13,0.13},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"HabilidadesScript",},
		icono = "NewHUD/iconoVerBarraEnemigo",
	},

	RegeneracionVida = {
		type = "UsableObjectRegeneracion",
		tag = "powerup",
		skill_name = "RegeneracionVida",
		skill_name_desc = "Life Regeneration",
		skill_description = "Life regenerates with time",
		basic_shape = "Prefab_Plane",
		material = "Habilidades/RegeneracionVida",
		scale = {0.13,0.13,0.13},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"HabilidadesScript",},
		icono = "NewHUD/iconoRegeneracionVida",
	},

	AumentoVidaMaxima = {
		type = "UsableObjectAumentoVidaMaxima",
		tag = "powerup",
		skill_name = "AumentoVidaMaxima",
		skill_name_desc = "+ Maximum Health",
		skill_description = "+1 to maximum health per dungeon",
		basic_shape = "Prefab_Plane",
		material = "Habilidades/AumentoVidaMaxima",
		scale = {0.13,0.13,0.13},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"HabilidadesScript",},
		icono = "NewHUD/iconoAumentoVidaMax",
	},

	AumentoPorcentajeAlmas = 
	{
		type = "UsableObjectAumentoPorcentajeAlmas",
		tag = "powerup",
		skill_name = "AumentoPorcentajeAlmas",
		skill_name_desc = "+ Soul Recovery",
		skill_description = "Increases soul recovery",
		basic_shape = "Prefab_Plane",
		material = "Habilidades/AumentoPorcentajeAlmas",
		scale = {0.13,0.13,0.13},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"HabilidadesScript",},
		icono = "NewHUD/iconoPorcentajeAlmas",
	},

	DamageEnemigoMelee = 
	{
		type = "UsableObjectDamageMeleeEnemy",
		tag = "powerup",
		skill_name = "DamageEnemigoMelee",
		skill_name_desc = "Spiked Breastplate",
		skill_description = "Returns % of melee damage",
		material = "Habilidades/DamageEnemigoMelee",
		basic_shape = "Prefab_Plane",
		scale = {0.13,0.13,0.13},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"HabilidadesScript",},
		icono = "NewHUD/iconoDamanageEnemy",
	}, 

	ReduceTimeChest = 
	{
		type = "UsableObjectReduceTimeChest",
		tag = "powerup",
		skill_name = "ReduceTimeChest",
		skill_name_desc = "Fast Opening",
		skill_description = "Reduces opening time on chests",
		basic_shape = "Prefab_Plane",
		material = "Habilidades/ReduceTimeChest",
		scale = {0.13,0.13,0.13},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"HabilidadesScript",},
		icono = "NewHUD/iconoReduceTimeChest",
	},

	ArrowEnemy = 
	{
		type = "UsableObjectArrowEnemy",
		tag = "powerup",
		skill_name = "ArrowEnemy",
		skill_name_desc = "Enemy Locator",
		skill_description = "Locates enemies off screen",
		material = "Habilidades/ArrowEnemy",
		basic_shape = "Prefab_Plane",
		scale = {0.11,0.11,0.11},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"HabilidadesScript",},
		icono = "NewHUD/iconoArrowEnemy",
	},

	AumentarTiempoReto = 
	{
		type = "UsableObjectAumentarTiempoReto",
		tag = "powerup",
		skill_name = "AumentarTiempoReto",
		skill_name_desc = "Extra Time",
		skill_description = "You get extra time\nto complete challenges",
		material = "Habilidades/AumentarTiempoReto",
		basic_shape = "Prefab_Plane",
		scale = {0.11,0.11,0.11},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 10,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"HabilidadesScript",},
		icono = "NewHUD/iconoAumentarTiempoReto",
	},

	ShieldRecogible = {
		type = "UsableObjectShield",
		tag = "weapon",
		weaponEntityType = "Shield",
		model = "ShieldRecogible.mesh",
		material = "arma/Shield",
		scale = {1.5, 1.5, 1.5},
		static = false,
		defaultAnimation = "Move",
		defaultSpeedAnimation = 0.5,
		numJumpMax = 0,
		jumpHeight = 0,
		initialJumpSpeed = 0,
		gravityAcceleration = 0.00075,
		minJumpSpeed = 0,
		horizontalAcceleration  = 0,
		numJumpMax = 0,
		jumpHeight = 0,
		initialJumpSpeed = 0,
		gravityAcceleration = 0.00075,
		minJumpSpeed = 0,
		physic_shape = "capsule",
		physic_radius = 15,
		physic_height = 3,
		physic_group = 17,
		cointrigger_dimensions = {20, 20, 5},
		cointrigger_group = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		physic_mass = 1,
		
		sourceParticles = {"ShieldRecogible"},
		boneParticles = {"Bone002"},
	},

	KatanaRecogible = {
		type = "UsableObjectKatana",
		tag = "weapon",
		weaponEntityType = "Katana",
		model = "KatanaRecogible.mesh",
		
		material = "arma/espadaLuis",
		scale = {1.5, 1.5, 1.5},
		static = false,
		
		defaultAnimation = "Yaw",
		defaultSpeedAnimation = 0.5,
		
		horizontalAcceleration  = 0,
		numJumpMax = 0,
		jumpHeight = 0,
		initialJumpSpeed = 0,
		gravityAcceleration = 0.00075,
		minJumpSpeed = 0,
		physic_shape = "capsule",
		physic_radius = 5,
		physic_height = 20,
		physic_group = 17,
		
		cointrigger_dimensions = {5, 30, 5},
		cointrigger_group = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		physic_mass = 1,
		
		sourceParticles = {"KatanaRecogible"},
		boneParticles = {"Bone001"},
	},

	BroadSwordRecogible = {
		type = "UsableObjectBroadSword",
		tag = "weapon",
		weaponEntityType = "BroadSword",
		model = "BroadSwordRecogible.mesh",
		
		material = "arma/BroadSword",
		scale = {1.5, 1.5, 1.5},
		static = false,
		
		defaultAnimation = "I",
		defaultSpeedAnimation = 0.5,
		
		horizontalAcceleration  = 0,
		numJumpMax = 0,
		jumpHeight = 0,
		initialJumpSpeed = 0,
		gravityAcceleration = 0.00075,
		minJumpSpeed = 0,
		physic_shape = "capsule",
		physic_radius = 5,
		physic_height = 15,
		physic_group = 17,
		
		cointrigger_dimensions = {5, 30, 5},
		cointrigger_group = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		physic_mass = 1,
		
		sourceParticles = {"BroadSwordRecogible"},
		boneParticles = {"Bone003"},
	},

	GreenSwordRecogible = {
		type = "UsableObjectGreenSword",
		tag = "weapon",
		weaponEntityType = "GreenSword",
		model = "GreenSwordRecogible.mesh",
		
		material = "arma/EspadaVerde",
		scale = {1.5, 1.5, 1.5},
		static = false,
		
		defaultAnimation = "Yaw",
		defaultSpeedAnimation = 0.5,
		
		horizontalAcceleration  = 0,
		numJumpMax = 0,
		jumpHeight = 0,
		initialJumpSpeed = 0,
		gravityAcceleration = 0.00075,
		minJumpSpeed = 0,
		physic_shape = "capsule",
		physic_radius = 5,
		physic_height = 20,
		physic_group = 17,
		
		cointrigger_dimensions = {5, 30, 5},
		cointrigger_group = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		physic_mass = 1,
		
		sourceParticles = {"GreenSwordRecogible"},
		boneParticles = {"Bone001"},
	},

	AK47Recogible = {
		type = "UsableObjectAK47",
		tag = "weapon",
		weaponEntityType = "AK47",
		model = "AK47.mesh",
		material = "rad",
		scale = {1.5, 1.5, 1.5},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 5,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
	},

	TongueRecogible = {
		type = "UsableObjectTongue",
		tag = "weapon",
		weaponEntityType = "Tongue",
		model = "LatigoRecogible.mesh",
		material = "arma/Latigo",
		scale = {2, 2, 2},
		static = false,
		
		defaultAnimation = "Move",
		defaultSpeedAnimation = 0.5,
		
		horizontalAcceleration  = 0,
		numJumpMax = 0,
		jumpHeight = 0,
		initialJumpSpeed = 0,
		gravityAcceleration = 0.00075,
		minJumpSpeed = 0,
		physic_shape = "capsule",
		physic_radius = 18,
		physic_height = 3,
		physic_group = 17,
		
		cointrigger_dimensions = {10, 20, 5},
		cointrigger_group = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		physic_mass = 1,
		
		sourceParticles = {"LatigoRecogible"},
		boneParticles = {"Bone002"},
	},

	DartGunRecogible = {
		type = "UsableObjectDartGun",
		tag = "weapon",
		weaponEntityType = "DartGun",
		model = "DartGunRecogible.mesh",
		
		material = "arma/DartGun",
		scale = {2, 2, 2},
		static = false,
		
		defaultAnimation = "Move",
		defaultSpeedAnimation = 0.5,
		
		horizontalAcceleration  = 0,
		numJumpMax = 0,
		jumpHeight = 0,
		initialJumpSpeed = 0,
		gravityAcceleration = 0.00075,
		minJumpSpeed = 0,
		physic_shape = "capsule",
		physic_radius = 18,
		physic_height = 3,
		physic_group = 17,
		
		cointrigger_dimensions = {10, 20, 5},
		cointrigger_group = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		physic_mass = 1,
		
		sourceParticles = {"DardoRecogible"},
		boneParticles = {"Bone002"},
	},

	LaserRecogible = {
		type = "UsableObjectLaser",
		tag = "weapon",
		weaponEntityType = "Laser",
		model = "LaserRecogible.mesh",
		
		material = "arma/laser",
		scale = {2.3, 2.3, 2.3},
		static = false,
		
		defaultAnimation = "Move",
		defaultSpeedAnimation = 0.5,
		
		horizontalAcceleration  = 0,
		numJumpMax = 0,
		jumpHeight = 0,
		initialJumpSpeed = 0,
		gravityAcceleration = 0.00075,
		minJumpSpeed = 0,
		physic_shape = "capsule",
		physic_radius = 18,
		physic_height = 3,
		physic_group = 17,
		
		cointrigger_dimensions = {10, 20, 5},
		cointrigger_group = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		physic_mass = 1,
		
		sourceParticles = {"LaserRecogible"},
		boneParticles = {"Bone002"},
	},

	Soul = {
		type = "Soul",
		tag = "soul",
		-- souls = 1,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 5,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		souls = 5,
		feedbackParticle = "AlmaAzulFeedback",
		sourceParticles = {"AlmaAzul"},
		scriptsLUA = {"SoulScript",}
	},

	Soul2 = {
		type = "Soul2",
		tag = "soul",
		-- souls = 1,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 6,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		souls = 10,
		feedbackParticle = "AlmaVerdeFeedback",
		sourceParticles = {"AlmaVerde"},
		scriptsLUA = {"SoulScript",}
	},

	Soul3 = {
		type = "Soul3",
		tag = "soul",
		-- souls = 1,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 7,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		souls = 25,
		feedbackParticle = "AlmaRojaFeedback",
		sourceParticles = {"AlmaRoja"},
		scriptsLUA = {"SoulScript",}
	},


	Coin = {
		type = "Coin",
		tag = "coin",
		static = false,
		scale = {1, 1, 1},
		model = "Moneda.mesh",
		material ="coin2",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_radius = 5,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 1,
		physic_material = "coin", --material físico
		cointrigger_dimensions = {5, 5, 3},
		cointrigger_group = 5,
		time_to_activate_trigger = 500, ---Tiempo en el cual podemos recoger la moneda
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		torque_speed = 4000,
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		scriptsLUA = {"CoinScript",},
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 1,
		proporcion = 1.3,
		timeInvisible = 0.1,		
	},


	Coin2 = {
		type = "Coin2",
		tag = "coin",
		static = false,
		scale = {1.2, 1.2, 1.2},
		model = "Moneda.mesh",
		material ="coin",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 1,
		physic_material = "coin", --material físico
		cointrigger_dimensions = {6, 6, 3.6},
		cointrigger_group = 5,
		time_to_activate_trigger = 500, ---Tiempo en el cual podemos recoger la moneda
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		torque_speed = 4000,
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		scriptsLUA = {"CoinScript",},
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 1,
		proporcion = 1.3,
		timeInvisible = 0.1,		
	},


	Coin3 = {
		type = "Coin3",
		tag = "coin",
		static = false,
		scale = {1.8, 1.8, 1.8},
		model = "Moneda.mesh",
		material ="coin3",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_radius = 9,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 1,
		physic_material = "coin", --material físico
		cointrigger_dimensions = {9, 9, 5.4},
		cointrigger_group = 5,
		time_to_activate_trigger = 500, ---Tiempo en el cual podemos recoger la moneda
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		torque_speed = 4000,
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		scriptsLUA = {"CoinScript",},
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 1,
		proporcion = 1.3,
		timeInvisible = 0.1,		
	},

	MonedaHabitacionTesoro = 
	{
		type = "MonedaHabitacionTesoro",
		tag = "coin",
		static = false,
		scale = {1.2, 1.2, 1.2},
		model = "Moneda.mesh",
		material ="coin",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 1,
		physic_material = "coin", --material físico
		cointrigger_dimensions = {6, 6, 3.6},
		cointrigger_group = 5,
		time_to_activate_trigger = 500, ---Tiempo en el cual podemos recoger la moneda
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = true, --activa la restricción de verticalidad
		torque_speed = 4000,
		start_impulse = 20, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 5, -- angulo de dispersión de la moneda al salir
		value = 10,
		scriptsLUA = {"CoinScript",},
		time_to_start_to_disappear = 15,
		time_between_parpadeo = 1,
		proporcion = 1.3,
		timeInvisible = 0.1,		
	},

--#################################################################################################################################################
--#####################################################   ARMAS    ################################################################################
--#################################################################################################################################################

	-- ARMAS FINALES --
	Laser = {
		type = "Laser",
		tag = "weapon",
		attackType = "Ranged",
		model = "laser.mesh",
		material = "arma/laser",
		scale = {1, 1, 1},
		needNode = false,
		nombreArma = "Laser",
		descripcionArma = "Long range laser that freezes the enemy",
		--Atributos del componente de ataque del arma
		mainDamage = 1.2,--multiplicador del daño principal
		specialDamage = 1.1, --multiplicador del daño especial
		MultiVelMainAttack = 1,--multiplicador de la velocidad del ataque principal
		MultiVelSpecialAttack = 2.5, --multiplicador de la velocidad del ataque especial
		maxDurationSpecialAttack = 3000,
		numHitsSpecialAttack = 10, --impactos de daño por segundo del ataque especial
		icono = "NewHUD/Laser",
		scriptsLUA = {"ArmaHUDScript",},

		sounds = {SoundEvents.LaserBeam,},

	},

	BroadSword = {
		type = "BroadSword",
		tag = "weapon",
		attackType = "Melee",
		model = "broadSword.mesh",
		material = "arma/BroadSword",
		scale = {1, 1, 1},
		needNode = false,
		static = false,
		nombreArma = "BroadSword",
		descripcionArma = "Espada de largo alcance",
		--fisica del arma
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {5,20,5},
		physic_trigger = true,
		physic_mass = 5,
		physic_group = 5,
		--offset lógico. Sirve para fijar el pivote lógico del arma con respecto al origen.
		offset = {0, 20, 0},
		--Atributos del componente de ataque del arma
		mainAttackSpeedAnimation = 1.5,
		mainDamage = 1.8, --multiplicador del daño principal
		specialDamage = 2, --multiplicador del daño especial
		MultiVelMainAttack = 1.5, --multiplicador de la velocidad del ataque principal
		MultiVelSpecialAttack = 1,--multiplicador de la velocidad del ataque especial
		maxDurationSpecialAttack = 3000,

		--Icono del arma para el hud
		icono = "NewHUD/Espada2",
		scriptsLUA = {"ArmaHUDScript",},
		sourceParticles = {"mp_explosion_flare_large"},
		--parametros del trail
		trail_visible = false,
		trail_material = "debug_draw",
		trail_width = 18,
		trail_width_change = 100,
		trail_length = 25,
	},

	GreenSword = {
		type = "GreenSword",
		tag = "weapon",
		attackType = "Melee",
		model = "espadaVerde.mesh",
		material = "arma/EspadaVerdeEspecial1",
		scale = {1, 1, 1},
		needNode = false,
		nombreArma = "Green Sword",
		descripcionArma = "A powerful magic sword",
		static = false,
		--fisica del arma
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {20,18,5},
		physic_trigger = true,
		physic_mass = 5,
		physic_group = 5,
		--offset lógico. Sirve para fijar el pivote lógico del arma con respecto al origen.
		offset = {0, 20, -10},
		--Atributos del componente de ataque del arma
		mainAttackSpeedAnimation = 1.25,
		mainDamage = 1.9, --multiplicador del daño principal
		specialDamage = 2, --multiplicador del daño especial
		MultiVelMainAttack = 0.7, --multiplicador de la velocidad del ataque principal
		MultiVelSpecialAttack = 1,--multiplicador de la velocidad del ataque especial
		maxDurationSpecialAttack = 3000,

		--Icono del arma para el hud
		icono = "NewHUD/EspadaJade",
		scriptsLUA = {"ArmaHUDScript",},
		--parametros del trail
		trail_visible = false,
		trail_material = "debug_draw",
		trail_width = 18,
		trail_width_change = 100,
		trail_length = 25,
		----Datos logica de arma
		materialEspadaVerde1 = "arma/EspadaVerdeEspecial1",
		materialEspadaVerde2 = "arma/EspadaVerdeEspecial2",
		materialEspadaVerde3 = "arma/EspadaVerdeEspecial2",
		materialEspadaVerde4 = "arma/EspadaVerdeEspecial4",
		materialEspadaVerde5 = "arma/EspadaVerdeEspecial5",
		Slash1 = "GreenSwordSlash",
		Slash2 = "GreenSwordSlash2",
		Slash3 = "GreenSwordSlash3",
		Slash4 = "GreenSwordSlash4",
		Slash5 = "GreenSwordSlash5",

		sounds = {SoundEvents.GreenSword, SoundEvents.GreenSwordSpecial,},
	},

	Shield = {
		type = "Shield",
		tag = "weapon",
		attackType = "Melee",
		model = "shield.mesh",
		material = "arma/Shield",
		defaultAnimation = "Yaw",
		scale = {1, 1, 1},
		needNode = false,
		nombreArma = "Spiked Shield",
		descripcionArma = "Enemies back off",
		static = false,
		--fisica del arma
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {15,11,11},
		physic_trigger = true,
		physic_mass = 5,
		physic_group = 5,
		--offset lógico. Sirve para fijar el pivote lógico del arma con respecto al origen.
		-- offset = {10, 0, 0},
		--Atributos del componente de ataque del arma
		mainAttackSpeedAnimation = 1.2,
		mainDamage = 0.5, --multiplicador del daño principal
		specialDamage = 2, --multiplicador del daño especial
		MultiVelMainAttack = 0.75, --multiplicador de la velocidad del ataque principal
		MultiVelSpecialAttack = 1.5,--multiplicador de la velocidad del ataque especial
		maxDurationSpecialAttack = 3000,
		--Icono del arma para el hud
		icono = "NewHUD/Escudo",
		scriptsLUA = {"ArmaHUDScript",},

		materialShieldSangre = "arma/ShieldSangre",
		cleanUpTime = 5000, ---timepo en limpiar la sangre del escudo
	},

	ShieldSpecial= {
		type = "ShieldSpecial",
		model = "shield.mesh",
		material = "arma/Shield",
		position = {0, 0, 0},
		scale = {2, 2, 2},
		static = false,
		defaultAnimation = "Roll",
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "capsule",
		physic_radius = 15,
		physic_height = 1,
		physic_offset = { 0, -15.5, 0},
		physic_trigger = false,
		physic_mass = 5,
		physic_group = 9,
		cointrigger_dimensions = {10, 10, 30},
		cointrigger_group = 9,
		coin_offset = {-15.5,-15.5,0},
		time_to_activate_trigger = 100, ---Tiempo en el cual podemos recoger la moneda
		-- Variables para IA
		maxVelPong = 0.25,         --- Pong
		maxHitsPong = 5,          --- Pong
		maxTimePong = 6000,       --- Pong
		maxVelApproach = 0.5,     --- Approach
		minDistStopAppr = 20,     --- Approach

		---Atributos del trail
		trail_center = false,
		trail_material = "Trails/Shield",
		trail_color_change = {4, 4, 4},
		trail_width = 12,
		trail_width_change = 2,
		trail_length = 120,
		trail_visible = true,

		materialShieldSangre = "arma/ShieldSangre",
		cleanUpTime = 5000, ---timepo en limpiar la sangre del escudo

		sounds3D = {SoundEvents.ReboteEscudo,},
	},

	Tongue = {
		type = "Tongue",
		tag = "weapon",
		attackType = "Ranged",
		model = "latigo.mesh",
		material = "arma/Latigo",
		scale = {1, 1, 1},
		needNode = false,
		nombreArma = "Tongue",
		descripcionArma = "Poison that can slow down the enemy",
		static = false,
		defaultAnimation = "Idle",
		--fisica del arma
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {4,18,5},
		physic_trigger = true,
		physic_mass = 5,
		physic_group = 5,
		--offset lógico. Sirve para fijar el pivote lógico del arma con respecto al origen.
		--offset = {0, 20, 0},
		--Atributos del componente de ataque del arma
		mainAttackSpeedAnimation = 2,
		MultiVelMainAttack = 0.5, --multiplicador de la velocidad del ataque principal
		MultiVelSpecialAttack = 1,--multiplicador de la velocidad del ataque especial
		mainDamage = 2, --multiplicador del daño principal
		specialDamage = 4, --multiplicador del daño especial
		maxDurationSpecialAttack = 3000,

		--Icono del arma para el hud
		icono = "NewHUD/Latigo",
		scriptsLUA = {"ArmaHUDScript",},
	},

	DartGun = {
		type = "DartGun",
		tag = "weapon",
		attackType = "Ranged",
		model = "dartGun.mesh",
		material = "arma/DartGun",
		scale = {1, 1, 1},
		needNode = false,
		nombreArma = "Dart Gun",
		descripcionArma = "Enemies will eat each other!",
		--Atributos del componente de ataque del arma
		mainDamage = 0.3,--multiplicador del daño principal
		specialDamage = 1, --multiplicador del daño especial
		MultiVelMainAttack = 0.15,--multiplicador de la velocidad del ataque principal
		MultiVelSpecialAttack = 2.5, --multiplicador de la velocidad del ataque especial

		icono = "NewHUD/Arco",
		scriptsLUA = {"ArmaHUDScript",},

		sounds = {SoundEvents.GunDart,},
	},

	-- ARMAS DE PRUEBA --

	
	Katana = {
		type = "Katana",
		tag = "weapon",
		attackType = "Melee",
		model = "espadaLuis.mesh",
		material = "arma/espadaLuis",
		scale = {1, 1, 1},
		needNode = false,
		static = false,
		nombreArma = "Katana",
		descripcionArma = "Espada de corto alcance",
		--fisica del arma
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {4,13,4},
		physic_trigger = true,
		physic_mass = 5,
		physic_group = 5,
		--offset lógico. Sirve para fijar el pivote lógico del arma con respecto al origen.
		offset = {0, 14, 0},
		--Atributos del componente de ataque del arma
		mainAttackSpeedAnimation = 1.5,
		mainDamage = 1.8, --multiplicador del daño principal
		specialDamage = 2, --multiplicador del daño especial
		MultiVelMainAttack = 1, --multiplicador de la velocidad del ataque principal
		MultiVelSpecialAttack = 1,--multiplicador de la velocidad del ataque especial
		maxDurationSpecialAttack = 3000,

		--Icono del arma para el hud
		icono = "NewHUD/Espada1",
		scriptsLUA = {"ArmaHUDScript",},
		--parametros del trail
		trail_visible = false,
		trail_material = "debug_draw",
		trail_width = 18,
		trail_width_change = 100,
		trail_length = 25,
	},

	AK47 = {
		type = "AK47",
		tag = "weapon",
		attackType = "Ranged",
		model = "AK47.mesh",
		material = "rad",
		scale = {1, 1, 1},
		needNode = false,
		--Atributos del componente de ataque del arma
		mainDamage = 1.8,--multiplicador del daño principal
		specialDamage = 2, --multiplicador del daño especial
		MultiVelMainAttack = 0.3,--multiplicador de la velocidad del ataque principal
		MultiVelSpecialAttack = 5, --multiplicador de la velocidad del ataque especial
		maxDurationSpecialAttack = 3000,

		icono = "NewHUD/Arco",
		scriptsLUA = {"ArmaHUDScript",},
	},

	


--#################################################################################################################################################
--#####################################################   POTENCIADORES    ########################################################################
--#################################################################################################################################################



	Potenciador = {
		type = "Potenciador",
		model = "Runa.mesh",
		material = "Runa1",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "capsule",
		physic_radius = 7,
		physic_height = 12,
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorRojo"},
	},
	Potenciador1 = {
		type = "Potenciador1",
		model = "Runa.mesh",
		material = "Runa1",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAzul"},
		iconoPause = "Runas/Potenciador12",
		tipoAnimation = 0,
	},
	Potenciador2 = {
		type = "Potenciador2",
		model = "Runa.mesh",
		material = "Runa2",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAmarillo"},
		iconoPause = "Runas/Potenciador11",
		tipoAnimation = 0,
	},
	Potenciador3 = {
		type = "Potenciador3",
		model = "Runa.mesh",
		material = "Runa3",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorVerde"},
		iconoPause = "Runas/Potenciador9",
		tipoAnimation = 0,
	},
	Potenciador4 = {
		type = "Potenciador4",
		model = "Runa.mesh",
		material = "Runa4",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorRojo"},
		iconoPause = "Runas/Potenciador10",
		tipoAnimation = 0,
	},
	Potenciador5 = {
		type = "Potenciador5",
		model = "Runa.mesh",
		material = "Runa5",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAzul"},
		iconoPause = "Runas/Potenciador4",
		tipoAnimation = 0,
	},
	Potenciador6 = {
		type = "Potenciador6",
		model = "Runa.mesh",
		material = "Runa6",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAmarillo"},
		iconoPause = "Runas/Potenciador3",
		tipoAnimation = 0,
	},
	Potenciador7 = {
		type = "Potenciador7",
		model = "Runa.mesh",
		material = "Runa7",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorVerde"},
		iconoPause = "Runas/Potenciador1",
		tipoAnimation = 0,
	},
	Potenciador8 = {
		type = "Potenciador8",
		model = "Runa.mesh",
		material = "Runa8",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorRojo"},
		iconoPause = "Runas/Potenciador2",
		tipoAnimation = 0,
	},
	Potenciador9 = {
		type = "Potenciador9",
		model = "Runa.mesh",
		material = "Runa9",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAzul"},
		iconoPause = "Runas/Potenciador24",
		tipoAnimation = 0,	
	},
	Potenciador10 = {
		type = "Potenciador10",
		model = "Runa.mesh",
		material = "Runa10",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAmarillo"},
		iconoPause = "Runas/Potenciador23",
		tipoAnimation = 0,
	},
	Potenciador11 = {
		type = "Potenciador11",
		model = "Runa.mesh",
		material = "Runa11",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorVerde"},
		iconoPause = "Runas/Potenciador21",
		tipoAnimation = 0,
	},
	Potenciador12 = {
		type = "Potenciador12",
		model = "Runa.mesh",
		material = "Runa12",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorRojo"},
		iconoPause = "Runas/Potenciador22",
		tipoAnimation = 0,
	},
	Potenciador13 = {
		type = "Potenciador13",
		model = "Runa.mesh",
		material = "Runa13",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAzul"},
		iconoPause = "Runas/Potenciador16",
		tipoAnimation = 0,
	},
	Potenciador14 = {
		type = "Potenciador14",
		model = "Runa.mesh",
		material = "Runa14",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAmarillo"},
		iconoPause = "Runas/Potenciador15",
		tipoAnimation = 0,
	},
	Potenciador15 = {
		type = "Potenciador15",
		model = "Runa.mesh",
		material = "Runa15",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorVerde"},
		iconoPause = "Runas/Potenciador13",
		tipoAnimation = 0,
	},
	Potenciador16 = {
		type = "Potenciador16",
		model = "Runa.mesh",
		material = "Runa16",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorRojo"},
		iconoPause = "Runas/Potenciador14",
		tipoAnimation = 0,
	},
	Potenciador17 = {
		type = "Potenciador17",
		model = "Runa.mesh",
		material = "Runa17",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAzul"},
		iconoPause = "Runas/Potenciador8",
		tipoAnimation = 0,
	},
	Potenciador18 = {
		type = "Potenciador18",
		model = "Runa.mesh",
		material = "Runa18",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAmarillo"},
		iconoPause = "Runas/Potenciador7",
		tipoAnimation = 0,
	},
	Potenciador19 = {
		type = "Potenciador19",
		model = "Runa.mesh",
		material = "Runa19",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorVerde"},
		iconoPause = "Runas/Potenciador5",
		tipoAnimation = 0,
	},
	Potenciador20 = {
		type = "Potenciador20",
		model = "Runa.mesh",
		material = "Runa20",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorRojo"},
		iconoPause = "Runas/Potenciador6",
		tipoAnimation = 0,
	},
	Potenciador21 = {
		type = "Potenciador21",
		model = "Runa.mesh",
		material = "Runa21",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAzul"},
		iconoPause = "Runas/Potenciador20",
		tipoAnimation = 0,
	},
	Potenciador22 = {
		type = "Potenciador22",
		model = "Runa.mesh",
		material = "Runa22",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorAmarillo"},
		iconoPause = "Runas/Potenciador19",
		tipoAnimation = 0,
	},
	Potenciador23 = {
		type = "Potenciador23",
		model = "Runa.mesh",
		material = "Runa23",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorVerde"},
		iconoPause = "Runas/Potenciador17",
		tipoAnimation = 0,
	},
	Potenciador24 = {
		type = "Potenciador24",
		model = "Runa.mesh",
		material = "Runa24",
		tag = "Potenciador",
		scale = {2, 2, 2},
		static = false,
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "box",
		physic_dimensions = {6, 10, 5},
		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,
		scriptsLUA = {"PotenciadorScript",},
		sourceParticles = {"PotenciadorRojo"},
		iconoPause = "Runas/Potenciador18",
		tipoAnimation = 0,
	},

--#################################################################################################################################################
--#####################################################   TILEADO    ################################################################################
--#################################################################################################################################################


	TileFondo1 = {
		type = "TILE_FONDO1",
		model = "Tile.mesh", --"Tile60.mesh",
		material = "Tiles/Castillo/Fondo1",
		scale = {1, 1, 1},
		static = true,
	},

	TileFondo2 = {
		type = "TILE_FONDO2",
		model = "Tile.mesh",
		material = "Tiles/Castillo/Fondo2",
		scale = {1, 1, 1},
		static = true,
	},

	TileFondo3 = {
		type = "TILE_FONDO3",
		model = "Tile.mesh",
		material = "Tiles/Castillo/Fondo3",
		scale = {1, 1, 1},
		static = true,
	},

	TileFondo4 = {
		type = "TILE_FONDO4",
		model = "Tile.mesh",
		material = "Tiles/Castillo/Fondo4",
		scale = {1, 1, 1},
		static = true,
	},

	TileCapa1 = {
		type = "TILE_DEFAULT",
		model = "Tile.mesh",  --"Tile.mesh",
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Hielo",
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
		material = "Tiles/Castillo/Pegajoso",
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
		material = "Tiles/Castillo/FragilRotura1",
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
		matBreak1 = "Tiles/Castillo/FragilRotura1",
		matBreak2 = "Tiles/Castillo/FragilRotura2",
		matBreak3 = "Tiles/Castillo/FragilRotura3",

	},


	TILE_FRAGILE_BORDE = {
		type = "TILE_FRAGILE_BORDE",
		model = "Tile.mesh",  --"Tile.mesh",
		material = "Tiles/Castillo/FragilRotura1",
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
		matBreak1 = "Tiles/Castillo/FragilRotura1", ----FALTA MATERIAL CON BORDE
		matBreak2 = "Tiles/Castillo/FragilRotura2",
		matBreak3 = "Tiles/Castillo/FragilRotura3",
	},


	TILE_STONES = {
		type = "TILE_STONES",
		model = "TilePiedras.mesh",  --"Tile.mesh",
		material = "Tiles/Castillo/Piedras",
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
		tiletrigger_dimensions = {9, 5, 30}, 
		offsetTrigger = {0, 10, 0}, 
		tiletrigger_group = 5,
		scriptsLUA = {"TileFragileScript",},
		-- Atributo para CTileFragile--
		numHitsToBreak = 3,
	},

	TILE_PLATFORM = {
		type = "TILE_PLATFORM",
		model = "TilePlat.mesh",  --"Tile.mesh",
		material = "Tiles/Castillo/Plataforma",
		orientation = {1, 0, 0},
		orientation_angle = 180, --90, para el marine -- -90 para sai02
		scale = {1, 1, 1},
		static = true,
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 10,5,30}, --{ 10, 10, 30},
		physic_offset = { 0, -5, 0},
		physic_trigger = false,
		physic_group = 14,
		physic_mass = 1,
		-- tiletrigger_dimensions = {50,30,1}, --trigger puertas verticales
		-- offsetTrigger = {-15, -25, 0}, 
		-- tiletrigger_group = 5,
		scriptsLUA = {"TileAtravesableScript",},
	},

	TileSlide1 = {
		type = "TILE_SLIDE_UR",
		model = "TileSlide01.mesh",      --"TileSlide0160.mesh",	---"TileSlide01.mesh",
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		material = "Tiles/Castillo/Frontal",
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
		-- material = "Tiles/Castillo/FrontalBorde",
		material = "Tiles/Castillo/Frontal",
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
		-- material = "Tiles/Castillo/OblicuoBorde01",
		material = "Tiles/Castillo/Frontal",
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
		-- material = "Tiles/Castillo/OblicuoBorde02",
		material = "Tiles/Castillo/Frontal",
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
		-- material = "Tiles/Castillo/OblicuoBordeB01",
		material = "Tiles/Castillo/Frontal",
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
		-- material = "Tiles/Castillo/OblicuoBordeA01",
		material = "Tiles/Castillo/Frontal",
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
		-- material = "Tiles/Castillo/OblicuoBordeA02",
		material = "Tiles/Castillo/Frontal",
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
		-- material = "Tiles/Castillo/OblicuoBordeB02",
		material = "Tiles/Castillo/Frontal",
		scale = {1, 1, 1},
		static = true,
		physic_entity = "fromFile",
		physic_file = "media/models/physics/TileSemiSlide0160.RepX",
		physic_orientation = {1, 0, 0},
		physic_orientation_angle = 180,
		physic_group = 1,
	},


	Trampilla = {
		type = "TILE_TRAMPILLA",
		scale = {1, 1, 1},
		static = true,
		model = "trampilla.mesh",  --"Tile.mesh",
		material = "Props/trampilla",
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = { 10,10,30}, 

		physic_trigger = true,
		physic_group = 5,
		physic_mass = 1,

		--Aqui van los sonidos de todas las trampas
		sounds3D = {SoundEvents.VaporPresion,},

		scriptsLUA = {"heiserScript",},
		
		sourceParticles = {"Trampilla"},
	},
	
	Puerta = {
		type = "TILE_DOOR",
		model = "Tile2.mesh",
		material = "door",
		scale = {1,1,1},
		static = true,
	},

	Pinchos = 
	{
		type = "Pinchos",
		scale = 1,
		model = "pinchos.mesh",
		material = "trampa/Pinchos",
		static = false,
	},


--#################################################################################################################################################
--#####################################################   MENU    ################################################################################
--#################################################################################################################################################


	SoulMenu = {
		type = "SoulMenu",
		sourceParticles = {"AlmaAzul"},
		position = {0, 0, 0},
		max_speed = 0.005,
		time_cicle = 5,
		accel = 0.02,
		light_time_cicle = 2.5,
		light_effect_wait = 5,
		diffuse = {0.1, 0.1, 0.9},
		specular = {0.2, 0.3, 0.9},
		attenuation_range = 100,
		attenuation_constant = 1,
		attenuation_linear = 0.9,
		attenuation_quadratic = 0.05,
		torch_flickering = 350,
		torch_min_range = 0.95,
		torch_max_range = 1.05,
		torch_min_linear = 0.95,
		torch_max_linear = 1.05,
		torch_min_quadratic = 0.35,
		torch_max_quadratic = 1.65,
		time_to_live = 120,
	},

	SoulMenuGenerator = {
		type = "SoulMenuGenerator",
		position = {0, 0, 0},
		generator_time = 50,
		generator_offset = {60, 0, 0},
	},


--#################################################################################################################################################
--#####################################################   PARTÍCULAS    ###########################################################################
--#################################################################################################################################################

	AlmaAzulFeedback = 
	{
		type = "AlmaAzulFeedback",
		sourceParticles = {"AlmaAzulFeedback2"},
		time_to_live = 2,
	},
	AlmaVerdeFeedback = 
	{
		type = "AlmaVerdeFeedback",
		sourceParticles = {"AlmaVerdeFeedback2"},
		time_to_live = 2,
	},
	AlmaRojaFeedback = 
	{
		type = "AlmaRojaFeedback",
		sourceParticles = {"AlmaRojaFeedback2"},
		time_to_live = 2,
	},
	
	AltarFeedback =
	{
		type = "AltarFeedback",
		sourceParticles = {"AlmaAltarFeedback"},
		Velocidad = 0.05,
		RadioParada = 10,
	},

	LaserBeamParticle = 
	{
		type = "LaserBeamParticle",
		sourceParticles = {"LaserBeamParticle"},
		time_to_live = 2,
	},

	SaltoAire = {
		type = "SaltoAire",
		sourceParticles = {"SaltoAire"},
		time_to_live = 1,
	},

	HumoMuerteEnemigo = {
		type = "HumoMuerteEnemigo",
		sourceParticles = {"explosionMuerte2"}, --smoke
		time_to_live = 2,
	},

	HumoVioleta = {
		type = "HumoVioleta",
		sourceParticles = {"humoVioleta"},
		time_to_live = 2,
		------ATRIBUTOS DE DEBUG GRAFICO ---> encajan con los atributos del ataque especial del DartGun
		basic_shape = "Prefab_Sphere",
		material = "debug_draw",
		scale = {0.8, 0.8, 0.8},   --->Con escala 1 mide 100, con 0.5 mide 50
		static = false,
	},

	HumoSangre = {
		type = "HumoSangre",
		sourceParticles = {"humoSangre"},
		time_to_live = 4,
		------ATRIBUTOS DE DEBUG GRAFICO ---> encajan con los atributos del ataque especial del DartGun
		basic_shape = "Prefab_Sphere",
		material = "debug_draw",
		scale = {0.8, 0.8, 0.8},   --->Con escala 1 mide 100, con 0.5 mide 50
		static = false,
		-- Atibutos del script
		scriptsLUA = {"HumoSangreScript",},
		rangeBlood = 40,
	},

	ParticulaAmbientacion = {
		type = "PARTICULAAMBIENTACION",
		sourceParticles = {"-"},
	},

	ExplosionPuerta = {
		type = "ExplosionPuerta",
		sourceParticles = {"explosionPuerta",},
		time_to_live = 4,
	},

	ExplosionLaser = {
		type = "ExplosionLaser",
		sourceParticles = {"ExplosionLaser",},
		time_to_live = 6,		sounds3D = {SoundEvents.EnergyExplosion,},
		started_sounds =  {SoundEvents.EnergyExplosion,},	},

	ExplosionAranha = {
		type = "ExplosionAranha",
		sourceParticles = {"ExplosionAranha",},
		time_to_live = 4,
	},

	Heiser = {
		type = "Heiser",
		sourceParticles = {"Smoke04",},
		time_to_live = 4,
	},

	HumoVenenoso = {
		type = "HumoVenenoso",
		sourceParticles = {"Smoke05",},
		time_to_live = 4,
	},

	TrampaRayo = {
		type = "TrampaRayo",
		sourceParticles = {"Flare02",},
		time_to_live = 4,
	},

	ParticulaPuertaCerrada = {
		type = "ParticulaPuertaCerrada",
		sourceParticles = {"particulaPuertaCerrada"},
	},

	ParticulaPuertaCerradaAbajo = {
		type = "ParticulaPuertaCerradaAbajo",
		sourceParticles = {"particulaPuertaCerradaAbajo"},
	},

	ParticulaPuertaAbierta = {
		type = "ParticulaPuertaAbierta",
		sourceParticles = {"ParticulaPuertaAbierta"},
	},

	ParticulaChupaAlmas = {
		type = "ParticulaChupaAlmas",
		sourceParticles = {"particulaChupaAlmas"},
		speed = 0.1,
		distanceToDisappear = 5,
		time_to_live = 2,
	},
	
	Niebla = 
	{
		type = "Niebla",
		sourceParticles = {"CopyOfNiebla"},
		position = {200,-325,0},
	},

	Chispa = 
	{
		type = "Chispa",
		sourceParticles = {"CopyOfDamage1"},  --CopyOfDamage1
		time_to_live = 1,
	},

	ChispaDanhoPlayer = 
	{
		type = "ChispaDanhoPlayer",
		sourceParticles = {"particulaChispaDanhoPlayer"},
		time_to_live = 1,
	},

	ChispaDanhoEnemy = 
	{
		type = "ChispaDanhoEnemy",
		sourceParticles = {"particulaChispaDanhoEnemigo"},
		time_to_live = 1,
	},

	SpawnEnem = 
	{
		type = "SpawnEnem",
		sourceParticles = {"SpawnEnem"},  --CopyOfDamage1
		time_to_live = 1,
	},

	-------------------------------------------------------------------------
	-----------------------------PROPS ESCENARIO-----------------------------
	-------------------------------------------------------------------------
	PropTapiz = {
		type = "PropTapiz",
		model = "tapiz.mesh",
		material = "Props/Estandarte",
		scale = 1,
		static = false,
		defaultAnimation = "Idle",
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {5, 20, 30}, --dimensiones físicas puerta horizontal
		physic_trigger = true,
		physic_group = 5,
	},

	Stone1 = {
		type = "Stone1",
		tag = "stone1",
		static = false,
		scale = {1, 1, 1},
		model = "piedra1.mesh",
		material = "props/Piedra1",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_radius = 5,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 2,
		physic_material = "stone", --material físico
		cointrigger_dimensions = {5, 5, 3},
		cointrigger_group = 5,
		time_to_activate_trigger = 500, ---Tiempo en el cual podemos recoger la moneda
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = false, --activa la restricción de verticalidad
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		scriptsLUA = {"CoinScript",},
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		proporcion = 1.3,
		timeInvisible = 0.1,	
	},

	Stone2 = {
		type = "Stone2",
		tag = "stone2",
		static = false,
		scale = {2, 2, 2},
		model = "piedra2.mesh",
		material ="props/Piedra2",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_radius = 5,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 1,
		physic_material = "stone", --material físico
		cointrigger_dimensions = {5, 5, 3},
		cointrigger_group = 5,
		time_to_activate_trigger = 500, ---Tiempo en el cual podemos recoger la moneda
		z0_constraint = true, --activa la restricción para Z = 0
		vertical_constraint = false, --activa la restricción de verticalidad
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,	
	},


	Cobweb1 = {
		type = "Cobweb1",
		model = "Cobweb.mesh",
		material = "Props/Cobweb1",
		scale = 1,
		static = true,
	},
	
	Cobweb2 = {
		type = "Cobweb2",
		model = "Cobweb.mesh",
		material = "Props/Cobweb2",
		scale = 1,
		static = true,
	},
	
	Cobweb3 = {
		type = "Cobweb3",
		model = "Cobweb.mesh",
		material = "Props/Cobweb3",
		scale = 1,
		static = true,
	},
	
	Cobweb4 = {
		type = "Cobweb4",
		model = "Cobweb.mesh",
		material = "Props/Cobweb4",
		scale = 1,
		static = true,
	},
	
	Cobweb5 = {
		type = "Cobweb5",
		model = "Cobweb.mesh",
		material = "Props/Cobweb5",
		scale = 1,
		static = true,
	},
	
	AltarSacrificio = {
		type = "AltarSacrificio",
		model = "altarSacrificio.mesh",
		material = "Props/AltarSacrificio",
		scale = 1,
		static = true,
		
		sourceParticles = {"AltarSacrificio"},
		
	},

--#################################################################################################################################################
--#####################################################   BARTULOS    ################################################################################
--#################################################################################################################################################


	Barril = {
		type = "Barril",
		position = {550, -100, 5},
		orientation = {0, 0, 0},
		orientation_angle = 0,
		scale = 3,
		model = "barril.mesh",
	},

	DebugRange = {
		type = "DebugRange",
		position = {0, 0, 0},
		scale = {0.01, 0.01, 0.01},
		model = "laserBall.mesh", 
		static = false,
		physic_dimensions = {1, 1, 1},
		collideTile = "true",
		physic_entity = "rigid",
		physic_type = "kinematic",
		physic_shape = "sphere",
		physic_radius = 1,
		physic_group = 16,
		physic_mass = 1,
		physic_trigger = true,
	},

--#################################################################################################################################################
--#####################################################   TUTORIAL   ##############################################################################
--#################################################################################################################################################

	puntoTutorial =
	{
		type = "puntoTutorial",
		sourceParticles = {"teleport"},
		position = {0, 0, 0},
	},

	triggerTutorial1 =
	{
		type = "triggerTutorial1",
		position = {0,0,0},
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {10, 50, 5},
		physic_group = 5,
		physic_trigger = true,
		scriptsLUA = {"triggerTutorialScript",},
	},



---------------------------------------------------------------------------------------------------
--------------------------------------OBJETOS ROMPIBLES--------------------------------------------
---------------------------------------------------------------------------------------------------

	cajaEntera = 
	{
		type = "cajaEntera",
		static = false,
		tag = "objetoRompible",
		scale = {1, 1, 1},
		entity_offset = {0, 10, 0},
		model = "caja.mesh",
		material ="objetosRompibles/Caja",
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {8.5, 6, 10},
		physic_trigger = false,
		physic_group = 18,
		physic_mass = 1,
		objetosRompibles = {"cajaParte1", "cajaParte2", "cajaParte3", "cajaParte4", "cajaParte5",},
		life = 25,
	},

	cajaParte1 = 
	{
		type = "cajaParte1",
		static = false,
		scale = {1, 1, 1},
		model = "cajaParte1.mesh",
		material ="objetosRompibles/Caja",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "coin",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,	
	},

	cajaParte2 = 
	{
		type = "cajaParte2",
		static = false,
		scale = {1, 1, 1},
		model = "cajaParte2.mesh",
		material ="objetosRompibles/Caja",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "coin",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	cajaParte3 = 
	{
		type = "cajaParte3",
		static = false,
		scale = {1, 1, 1},
		model = "cajaParte3.mesh",
		material ="objetosRompibles/Caja",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "coin",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	cajaParte4 = 
	{
		type = "cajaParte4",
		static = false,
		scale = {1, 1, 1},
		model = "cajaParte4.mesh",
		material ="objetosRompibles/Caja",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "coin",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	cajaParte5 = 
	{
		type = "cajaParte5",
		static = false,
		scale = {1, 1, 1},
		model = "cajaParte5.mesh",
		material ="objetosRompibles/Caja",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "coin",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	barrilEntero = 
	{
		type = "barrilEntero",
		static = false,
		tag = "objetoRompible",
		scale = {1, 1, 1},
		entity_offset = {0, 10, 0},
		model = "barrilEntero.mesh",
		material ="objetosRompibles/Barril",
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {7, 8, 10},
		physic_trigger = false,
		physic_group = 18,
		physic_mass = 1,
		objetosRompibles = {"barrilParte1", "barrilParte2", "barrilParte3", "barrilParte4", "barrilParte5",},
		life = 25,
	},

	barrilParte1 = 
	{
		type = "barrilParte1",
		static = false,
		scale = {1, 1, 1},
		model = "barrilParte1.mesh",
		material ="objetosRompibles/Barril",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "coin",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	barrilParte2 = 
	{
		type = "barrilParte2",
		static = false,
		scale = {1, 1, 1},
		model = "barrilParte2.mesh",
		material ="objetosRompibles/Barril",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "coin",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	barrilParte3 = 
	{
		type = "barrilParte3",
		static = false,
		scale = {1, 1, 1},
		model = "barrilParte3.mesh",
		material ="objetosRompibles/Barril",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "coin",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	barrilParte4 = 
	{
		type = "barrilParte4",
		static = false,
		scale = {1, 1, 1},
		model = "barrilParte4.mesh",
		material ="objetosRompibles/Barril",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_material = "coin",
		physic_shape = "sphere",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	barrilParte5 = 
	{
		type = "barrilParte5",
		static = false,
		scale = {1, 1, 1},
		model = "barrilParte5.mesh",
		material ="objetosRompibles/Barril",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_material = "coin",
		physic_shape = "sphere",
		physic_radius = 6,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 5,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 200, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	calavera = 
	{
		type = "calavera",
		tag = "objetoRompible",
		orientation = {0,1,0},
		orientation_angle = 180,
		static = false,
		scale = {1, 1, 1},
		entity_offset = {0, 10, 0},
		model = "calavera.mesh",
		material ="objetosRompibles/Calavera",
		physic_entity = "rigid",
		physic_material = "stone",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {5, 2.5, 10},
		physic_trigger = false,
		physic_group = 18,
		physic_mass = 1,
		objetosRompibles = {"calaveraParte1", "calaveraParte2", "calaveraParte3",},
		life = 25,
	},

	pilaCalaveras = 
	{
		type = "pilaCalaveras",
		static = false,
		tag = "objetoRompible",
		scale = {1, 1, 1},
		entity_offset = {0, 10, 0},
		model = "pilaCalaveras.mesh",
		material ="objetosRompibles/Calavera",
		physic_entity = "rigid",
		physic_type = "static",
		physic_shape = "box",
		physic_dimensions = {8, 6, 10},
		physic_trigger = false,
		physic_group = 18,
		physic_mass = 1,
		objetosRompibles = {"calaveraParte1", "calaveraParte2", "calaveraParte3", "calaveraParte1", "calaveraParte2", "calaveraParte3",},
		life = 50,
	},

	calaveraParte1 = 
	{
		type = "calaveraParte1",
		static = false,
		scale = {1, 1, 1},
		model = "calaveraParte1.mesh",
		material ="objetosRompibles/Calavera",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "stone",
		physic_radius = 3,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 1,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 100, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	calaveraParte2 = 
	{
		type = "calaveraParte2",
		static = false,
		scale = {1, 1, 1},
		model = "calaveraParte2.mesh",
		material ="objetosRompibles/Calavera",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "stone",
		physic_radius = 3,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 1,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 100, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},

	calaveraParte3 = 
	{
		type = "calaveraParte3",
		static = false,
		scale = {1, 1, 1},
		model = "calaveraParte3.mesh",
		material ="objetosRompibles/Calavera",
		physic_entity = "rigid",
		physic_type = "dynamic",
		physic_shape = "sphere",
		physic_material = "stone",
		physic_radius = 3,
		physic_trigger = false,
		physic_group = 10,
		physic_mass = 1,
		z0_constraint = true, --activa la restricción para Z = 0
		start_impulse = 100, ---impulso máximo de inicio
		impulse_deviation = 0.25, --desviación del impulso (valor entre 0 y 1)
		dispersion_angle = 20, -- angulo de dispersión de la moneda al salir
		time_to_start_to_disappear = 5,
		time_between_parpadeo = 0.1,
		scriptsLUA = {"CoinScript",},
		proporcion = 1.3,
		timeInvisible = 0.1,
	},
}