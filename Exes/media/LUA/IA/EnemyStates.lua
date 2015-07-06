None = {

	name = "None",

	Awake = function (enemyStateMachine)
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		
	end,
}

Idle = {

	name = "Idle",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAIdle", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAIdle", true)  
	end,
}

Approach = {

	name = "Approach",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", true)  
	end,
}

ApproachMultiDir = {

	name = "ApproachMultiDir",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", true)  
	end,
}

ApproachPosition = {

	name = "ApproachPosition",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIApproachPosition", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIApproachPosition", true)  
	end,
}

ShootPlayer = {

	name = "ShootPlayer",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAShootPlayer", false)  
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAShootPlayer", true)  
	end,
}

ShootUniDir = {

	name = "ShootUniDir",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAShootUniDir", false)  
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAShootUniDir", true)  
	end,
}

ShootRaycast = {

	name = "ShootRaycast",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAShootRaycast", false)  
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAShootRaycast", true)  
	end,
}

Kamikaze = {

	name = "Kamikaze",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAKamikaze", false)  
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAKamikaze", true)  
	end,
}

Dodge = {

	name = "Dodge",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIADodge", false)  
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIADodge", true)  
	end,
}

Teleport = {

	name = "Teleport",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIATeleport", false)  
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIATeleport", true)  
	end,
}

Defend = {

	name = "Defend",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIADefend", false)  
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIADefend", true)  
	end,
}

Invincible = {

	name = "Invincible",
	active = false,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAInvincible", false)  
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAInvincible", true)  
	end,
}

Pong = {

	name = "Pong",
	active = false,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAPong", false)  
		
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAPong", true)  
	end,
}

Wander = {

	name = "Wander",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAWander", false)  
		
	end,

	Tick = function (enemyStateMachine)
		
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAWander", true)  
	end,
}

Summon = {

	name = "Summon",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIASummon", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIASummon", true)  
	end,
}

SeekRay = {

	name = "SeekRay",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIASeekRay", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIASeekRay", true)  
	end,
}

SeekRay = {

	name = "StealLife",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAStealLife", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAStealLife", true)  
	end,
}

------------------------- COMPUESTAS -----------------------------------

ApproachMultiDir_ShootPlayer = {

	name = "ApproachMultiDir_ShootPlayer",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", false)  
		enemyStateMachine:setSleep("CIAShootPlayer", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", true)  
		enemyStateMachine:setSleep("CIAShootPlayer", true)  
	end,
}

ApproachMultiDir_Summon = {

	name = "ApproachMultiDir_Summon",

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", false)  
		enemyStateMachine:setSleep("CIASummon", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", true)  
		enemyStateMachine:setSleep("CIASummon", true)  
	end,
}

Approach_Dodge = {

	name = "Approach_Dodge",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", false)  
		enemyStateMachine:setSleep("CIADodge", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", true)  
		enemyStateMachine:setSleep("CIADodge", true)  
	end,
}

Approach_Defend = {

	name = "Approach_Defend",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", false)  
		enemyStateMachine:setSleep("CIADefend", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", true)  
		enemyStateMachine:setSleep("CIADefend", true)  
	end,
}

Approach_Kamikaze = {

	name = "Approach_Kamikaze",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", false)  
		enemyStateMachine:setSleep("CIAKamikaze", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", true)  
		enemyStateMachine:setSleep("CIAKamikaze", true)  
	end,
}

ApproachMultiDir_Dodge = {

	name = "ApproachMultiDir_Dodge",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", false)  
		enemyStateMachine:setSleep("CIADodge", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", true)  
		enemyStateMachine:setSleep("CIADodge", true)  
	end,
}

ApproachMultiDir_Defend = {

	name = "ApproachMultiDir_Defend",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", false)  
		enemyStateMachine:setSleep("CIADefend", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", true)  
		enemyStateMachine:setSleep("CIADefend", true)  
	end,
}

ApproachMultiDir_ShootRaycast = {

	name = "ApproachMultiDir_ShootRaycast",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", false)  
		enemyStateMachine:setSleep("CIAShootRaycast", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", true)  
		enemyStateMachine:setSleep("CIAShootRaycast", true)  
	end,
}

ApproachMultiDir_Kamikaze = {

	name = "ApproachMultiDir_Kamikaze",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", false)  
		enemyStateMachine:setSleep("CIAKamikaze", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", true)  
		enemyStateMachine:setSleep("CIAKamikaze", true)  
	end,
}

ApproachMultiDir_StealLife = {

	name = "ApproachMultiDir_StealLife",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", false)  
		enemyStateMachine:setSleep("CIAStealLife", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproachMultiDir", true)  
		enemyStateMachine:setSleep("CIAStealLife", true)  
	end,
}

ApproachToWander = {

	name = "ApproachToWander",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", false) 
		enemyStateMachine:setSleep("CAIAVOID", false)
	end,

	Tick = function (enemyStateMachine)
		
		-- Si sale del rango de deteccion despertaremos a Wander
		if enemyStateMachine:getOwner():getPosition().x > GameMgr.player:getPosition().x + ApproachToWander.rangeDetectionX or
	  	   enemyStateMachine:getOwner():getPosition().x < GameMgr.player:getPosition().x - ApproachToWander.rangeDetectionX then
			enemyStateMachine:changeState("WanderToApproach")
		end
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAApproach", true)  
		enemyStateMachine:setSleep("CAIAVOID", true)  
	end,
}

WanderToApproach = {

	name = "WanderToApproach",
	rangeDetectionX = 100,

	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAWander", false)  
	end,

	Tick = function (enemyStateMachine)
		-- Si entra del rango de deteccion despertaremos a Approach
		if enemyStateMachine:getOwner():getPosition().x > GameMgr.player:getPosition().x - WanderToApproach.rangeDetectionX and
	   	   enemyStateMachine:getOwner():getPosition().x < GameMgr.player:getPosition().x + WanderToApproach.rangeDetectionX then
			enemyStateMachine:changeState("ApproachToWander")
		end
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAWander", true)  
	end,
}

Pong_ShootPlayer = {

	name = "Pong_ShootPlayer",
	
	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAPong", false)  
		enemyStateMachine:setSleep("CIAShootPlayer", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIAPong", true)  
		enemyStateMachine:setSleep("CIAShootPlayer", true)
	end,
}

SeekRay_ShootRaycast = {

	name = "SeekRay_ShootRaycast",
	
	Awake = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIASeekRay", false)  
		enemyStateMachine:setSleep("CIAShootRaycast", false)  
	end,

	Tick = function (enemyStateMachine)
	
	end,

	Sleep = function (enemyStateMachine)
		enemyStateMachine:setSleep("CIASeekRay", true)  
		enemyStateMachine:setSleep("CIAShootRaycast", true)
	end,
}