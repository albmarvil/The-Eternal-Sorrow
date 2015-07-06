Blueprints = {
	Player = {

		--Primero los componentes que originan movimiento en la entidad (movimiento físico)
		{CAvatarController= true,},

		--Después su componente físico
		{CPhysicController= true,}, 

		--Por último el resto de componentes que puedan escuchar la posicion lógica de la entidad. Es interesante colocar el grafico primero
		{CAnimatedGraphics= true,},
		--{CPhysicsDebug = true}, 
		{CArms= true,}, 
		{CLifePlayer = true,}, 
		{CParticleController = true},
		{CAumentoUnSalto= false,}, 
		{CVerVidaEnemigos = false,},
		{CRegeneracionVida = false,},
		{CAumentoVidaMaxima = false,},
		{CPorcentajeAlmas = false,},
		{CDamageMeleeEnemy = false,},
		{CArrowEnemy = false,},
		{CReduceTimeChest = false,},
		{CAumentarTiempoReto = false,},
		{CScriptComponent = true,},
		{SoundListener = true,}, 
		-- {SoundSystem = true,}, 
		{CRibbonTrailComponent = true,},
		{CRibbonTrailComponentBis = true,},
		{CWaitAnimations = true,},
		{CAltStatesController = true},

	},

	CameraPause = 
	{
		{CCameraPause = true},
	},

	SaiPause = 
	{
		{CAnimatedGraphics= true,},
		{CArms= true,}, 
		{CParticleController = true},
	},

	-- ENEMIGOS FINALES --
	Wompy = {

		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false},

		{CPhysicController= true}, 
		--{CPhysicsDebug = true},
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true},
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},

	SpiderDodge = {

		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false}, 
		{CIADodge = false}, 

		{CPhysicController= true}, 
		-- {CPhysicsDebug = true},

		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},

	SpiderKami = {

		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false}, 
		{CIAKamikaze = false}, 

		{CPhysicController= true}, 
		-- {CPhysicsDebug = true},
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
		{CParticleController = true},
	},

	SkullShoot = {
		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIAShootRaycast = false}, 

		{CPhysicController= true},
		 
		{CAnimatedGraphics= true}, 
		--{CPhysicsDebug = true},
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},

	SkullSummon = {
		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIASummon = false}, 

		{CPhysicController= true}, 
		--{CPhysicsDebug = true},
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},

	Chulu = {
		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIAStealLife = false},

		{CPhysicController= true},
		--{CPhysicsDebug = true},
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},

	MinionSummonKami = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAApproachMultiDir = false}, 
		{CIAKamikaze = false}, 

		{CPhysicController= true}, 
		--{CPhysicsDebug = true},
		{CGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true},
		{CParticleController = true},
	},

	MinionSummonUniDir = {
		{CEntityController= true},
		{CIAApproachPosition = false},

		{CPhysicController= true}, 
		--{CPhysicsDebug = true},
		{CGraphics= true},
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CTimeToLive= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true},
		{CParticleController = true},
	},

	MinionSummonUniDirElite = {
		{CEntityController= true},
		{CIAApproachPosition = false},

		{CPhysicController= true}, 
		--{CPhysicsDebug = true},
		{CGraphics= true},
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CTimeToLive= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true},
		{CParticleController = true},
	},

	WompyElite = {

		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false},

		{CPhysicController= true}, 
		--{CPhysicsDebug = true},
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true},
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},

	SpiderDodgeElite = {

		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false}, 
		{CIADodge = false}, 

		{CPhysicController= true}, 
		-- {CPhysicsDebug = true},

		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},

	SpiderKamiElite = {

		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false}, 
		{CIAKamikaze = false}, 

		{CPhysicController= true}, 
		-- {CPhysicsDebug = true},
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
		{CParticleController = true},
	},

	SkullShootElite = {
		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIAShootRaycast = false}, 

		{CPhysicController= true},
		 
		{CAnimatedGraphics= true}, 
		--{CPhysicsDebug = true},
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},

	SkullSummonElite = {
		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIASummon = false}, 

		{CPhysicController= true}, 
		--{CPhysicsDebug = true},
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},

	ChuluElite = {
		{CEntityController= true},
		{CPushbackComponent = true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIAStealLife = false},

		{CPhysicController= true},
		--{CPhysicsDebug = true},
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
		{CAltStatesController = true},
	},
	---ENEMIGOS TEST

	Dragonite = {
		--primero las IAS y los que originan movimiento en la entidad
		{CEntityController= true}, 
		{CIAWander = false},

		{CPhysicController= true},
		
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemAppr = {

		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false},

		{CPhysicController= true}, 
		
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true},
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemTeleport = {

		{CEntityController= true},
		{CIATeleport = false},

		{CPhysicController= true},

		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemPong = {

		{CEntityController= true},
		{CIAPong = false}, 

		{CPhysicController= true},
		 
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprMDSummon = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIASummon = false}, 

		{CPhysicController= true}, 
		 
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprShootPl = {
		{CEntityController= true}, 
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false}, 
		{CIAShootPlayer = false}, 

		{CPhysicController= true},
		
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemShootRay = {

		{CEntityController= true}, 
		{CIAShootRaycast = false},

		{CPhysicController= true}, 
		
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemPongShootPl = {

		{CEntityController= true}, 
		{CIAPong = false}, 
		{CIAShootPlayer = false}, 

		{CPhysicController= true}, 
		
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemShootUni = {
		{CEntityController= true},
		{CIAShootUniDir = false},

		{CPhysicController= true},
		
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprWander = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false}, 
		{CIAWander = false},  

		{CPhysicController= true}, 
		
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprMDShootRay = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIAShootRaycast = false}, 

		{CPhysicController= true},
		 
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemSeekRayShootRay = {
		{CEntityController= true}, 
		{CIASeekRay = false}, 
		{CIAShootRaycast = false}, 

		{CPhysicController= true}, 
		 
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprKami = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false}, 
		{CIAKamikaze = false}, 

		{CPhysicController= true}, 
		
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprMDKami = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIAKamikaze = false}, 

		{CPhysicController= true}, 
		
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprMDStealLife = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIAStealLife = false},

		{CPhysicController= true},

		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	

	EnemApprDodge = {

		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false}, 
		{CIADodge = false}, 

		{CPhysicController= true}, 
		 
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprDefend = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproach = false}, 
		{CIADefend = false}, 
		{CPhysicController= true}, 
		 
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprMDDodge = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIADodge = false}, 

		{CPhysicController= true}, 
		 
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	EnemApprMDDefend = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIADefend = false},

		{CPhysicController= true}, 
		 
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{SoundSystem3D = true}, 
		{CBarraLifeController = true},
	},

	Mewtwo = {
		{CEntityController= true},
		{CAIIntermediate = true},
		{CIAAvoid = true},
		{CIAApproachMultiDir = false}, 
		{CIAShootRaycast= false},

		{CPhysicController= true}, 
		 
		{CAnimatedGraphics= true}, 
		{CScriptComponent = true}, 
		{CLifeButler = true}, 
		{CLifeEnemy= true}, 
		{CEnemyMeleeAttack= true}, 
		{CBarraLifeController = true},
	},

	Camera = {
		{CCamera= true}, 
		{CCameraEffects= true}, 
		{SoundSystem = true}, 
		{CShake = true},
	},
	
	Barril = {
		{CPhysicEntity= true},
		{CGraphics= true}, 
		 
		{CPhysicsDebug = true},
	},

	DebugRange = {
		{CPhysicEntity= true},
		{CGraphics= true}, 
		 
		{CPhysicsDebug = true},
	},
	
	PlanoColision = { {CPhysicEntity= true,} },
	
	-- -- Particulas = {CParticleController = true,},
	
	DOOR_VERTICAL_DER_TRIGGER = {
		{CPhysicEntity= true},
		{CPhysicDoor = true},

		{CGraphics= true}, 
		{CScriptComponent= true}, 
		{CLifeDoor = true},
		{CParticleController = true},
	},

	DOOR_VERTICAL_IZQ_TRIGGER = {
		{CPhysicEntity= true},
		{CPhysicDoor = true},

		{CGraphics= true}, 
		{CScriptComponent= true}, 
		{CLifeDoor = true},
		{CParticleController = true},
	},

	DOOR_HORIZONTAL_TRIGGER = {
		{CPhysicEntity= true},
		{CPhysicDoor = true},

		{CGraphics= true}, 
		{CScriptComponent= true}, 
		{CLifeDoor = true},
		{CParticleController = true},
	},
	
	Antorcha = {
		{CParticleController = true}, 
		{CPuntualLight= true}, 
		{CTorchBehaviour= true},
	},
	
	LuzPlayer = {
		{CPlayerLightController= true}, 
		{CPuntualLight= true},
	},
	
	LuzEscena = { {CDirectional = true}, },

	-- ESCENARIO --
	TILE_FRAGILE = { 
		{CPhysicEntity= true},
		{CPhysicTileFragile = true},	
		{CGraphics= true}, 
		{CTileFragile = true},
		{CScriptComponent = true,},
	},

	TILE_TRAMPILLA = { 
		{CPhysicEntity= true},	
		{CGraphics= true}, 
		{CScriptComponent = true},
		{SoundSystem3D = true}, 
		
		
		{CParticleController = true},
	},

	TILE_FRAGILE_BORDE = { 
		{CPhysicEntity= true},
		{CPhysicTileFragile = true},	
		{CGraphics= true}, 
		{CTileFragile = true},
		{CScriptComponent = true,},
	},

	TILE_STONES= { 
		{CPhysicEntity= true},
		{CGraphics= true}, 
		{CTileFragile = true},
		{CScriptComponent = true,},
	},

	TILE_PLATFORM= { 
		{CPhysicEntity= true},	
		--{CPhysicTileFragile = true},
		{CGraphics= true}, 
		--{CPhysicsDebug = true}, 
		{CScriptComponent = true,},
	},


	--Ejemplos luces
	Puntual = { {CPuntualLight= true}, },
	
	Directional = { {CDirectional= true}, },
	
	Spotlight = { {CSpotLight= true}, },
	----


	GreenSwordSlash = {
		{CSwordSlashController = true},
		{CPhysicEntity = true},
		-- {CGraphics = true},
		{CTimeToLive = true},
		{CParticleController = true},
		-- {CPhysicsDebug = true},
	},

	GreenSwordSlash2 = {
		{GreenSwordSlash = "-"},
	},

	GreenSwordSlash3 = {
		{GreenSwordSlash = "-"},
	},

	GreenSwordSlash4 = {
		{GreenSwordSlash = "-"},
	},

	GreenSwordSlash5 = {
		{GreenSwordSlash = "-"},
	},

	LaserBeam = {
		{CLaserBeamController= true},

		{CRibbonTrailComponent = true},
	},

	LaserBall = {
		{CLaserBallController= true},

		{CPhysicEntity= true},
		{CPhysicCoin = true},

		{CGraphics= true}, 
		--{CPhysicsDebug = true}, 
		{CParticleController = true},
		{CScriptComponent = true},
	},

	BulletDart = {
		{CBulletController= true},

		{CPhysicEntity= true},
		--{CPhysicsDebug = true}, 
		{CGraphics= true}, 
		{CRibbonTrailComponent = true},
		{CScriptComponent = true},	},

	BulletDartCanibal = {
		{CBulletController= true},

		{CPhysicEntity= true},
		--{CPhysicsDebug = true},
		{CGraphics= true},
		{CRibbonTrailComponent = true}, 
		{CScriptComponent = true},
	},

	BulletDartCanibalSpecial = {
		{CBulletController= true},

		{CPhysicEntity= true},
		-- {CPhysicsDebug = true}, 
		{CGraphics= true}, 
		{CTimeToLive = true},
		{CScriptComponent = true},
		--[[{CParticleController = true},]]
	},

	TongueBall = {
		{CTongueBallController= true},

		{CPhysicEntity= true},
		{CPhysicCoin = true},
		{CGraphics= true}, 
		
		--{CTimeToLive = true},
		{CScriptComponent = true},
		--{CPhysicsDebug = true}, 
		{CParticleController = true},
	},

	NormalBullet = {
		{CBulletController= true},

		{CPhysicEntity= true},

		{CGraphics= true}, 
		{CTimeToLive = true},
		--[[{CParticleController = true},]]
	},

	EnemyBullet = {
		{CBulletController= true},

		{CPhysicEntity= true},
		
		{CGraphics= true}, 
		--{CPhysicsDebug = true}, 
		{CTimeToLive = true},
		{CParticleController = true},
	},
	
	Explosion = {
		{CParticleController = true}, 
		{CTimeToLive= true},
	},

	Heiser = {
		{CParticleController = true},
	},

	TrampaRayo = {
		{CParticleController = true}, 
	},

	HumoVenenoso = {
		{CParticleController = true}, 
	},

	Pinchos = 
	{
		{CGraphics= true}, 
		{CPinchosController = true},
	},

	SpawnPoint = { {CScriptComponent = true}, },
	
	Nieve = { {CParticleController = true}, },

	Niebla = { {CParticleController = true}, },

	SpawnEnem = {
		{CParticleController = true}, 
		{CTimeToLive = true},
	},

	Chispa = {
		{CParticleController = true}, 
		{CTimeToLive = true},
	},

	ChispaDanhoPlayer = {
		{CParticleController = true}, 
		{CTimeToLive = true},
	},

	ChispaDanhoEnemy = {
		{CParticleController = true}, 
		{CTimeToLive = true},
	},
	
	LaserBeamParticle =  {
		{CParticleController = true},
		{CTimeToLive = true},
	},

	AlmaAzulFeedback = {
		{CParticleController = true},
		{CSoulFeedbackController = true,},
		{CTimeToLive = true},
	},

	AlmaVerdeFeedback = {
		{CParticleController = true},
		{CSoulFeedbackController = true,},
		{CTimeToLive = true},
	},

	AlmaRojaFeedback = {
		{CParticleController = true},
		{CSoulFeedbackController = true,},
		{CTimeToLive = true},
	},
	
	AltarFeedback = {
		{CAltarFeedbackController = true},
		{CParticleController = true},
	},

	Trampa = {CGraphics= true,},

	-- ---USableObjects

	UsableObjectKatana = {
		{CEntityController= true},
		{CPhysicController= true},
		{CAnimatedGraphics= true}, 
		{CUsableObjectTrigger= true},
		{CTriggerUsableWeapon = true},
		{CParticleController = true},
		-- {CPhysicsDebug = true}, 
	},
	
	UsableObjectAK47 = {
		{UsableObject = '-'}, 
		-- {CPhysicsDebug = true},
	},

	UsableObjectDartGun = {
		{CEntityController= true},
		{CPhysicController= true},
		{CAnimatedGraphics= true}, 
		{CUsableObjectTrigger= true},
		{CTriggerUsableWeapon = true},
		{CParticleController = true},
		--{CPhysicsDebug = true}, 
	},

	UsableObjectTongue = {
		{CEntityController= true},
		{CPhysicController= true},
		{CAnimatedGraphics= true}, 
		{CUsableObjectTrigger= true},
		{CTriggerUsableWeapon = true},
		{CParticleController = true},
		--{CPhysicsDebug = true}, 
	},

	UsableObjectBroadSword = {
		{CEntityController= true},
		{CPhysicController= true},
		{CAnimatedGraphics= true}, 
		{CUsableObjectTrigger= true},
		{CTriggerUsableWeapon = true},
		{CParticleController = true},
	--	{CPhysicsDebug = true}, 
	},

	UsableObjectGreenSword = {
		{CEntityController= true},
		{CPhysicController= true},
		{CAnimatedGraphics= true}, 
		{CUsableObjectTrigger= true},
		{CTriggerUsableWeapon = true},
		{CParticleController = true},
		--{CPhysicsDebug = true}, 
	},

	UsableObjectShield = {
		{CEntityController= true},
		{CPhysicController= true},
		{CAnimatedGraphics= true}, 
		{CUsableObjectTrigger= true},
		{CTriggerUsableWeapon = true},
		{CParticleController = true},
		--{CPhysicsDebug = true}, 
	},

	UsableObjectLaser= {
		{CEntityController= true},
		{CPhysicController= true},
		{CAnimatedGraphics= true}, 
		{CUsableObjectTrigger= true},
		{CTriggerUsableWeapon = true},
		{CParticleController = true},
	--	{CPhysicsDebug = true}, 
	},

	Habilidades = {
		{CPhysicEntity= true},
		-- {CBillBoard= true}, 
		{CGraphics= true}, 
		{CFloatingObject= true}, 
		{CUsableObjectTrigger= true},
		{CScriptComponent = true},
	},
	UsableObjectSalto = {
		{Habilidades = '-'}, 
	},

	UsableObjectBarraVidaEnemigo = {
		{Habilidades = '-'}, 
	},

	UsableObjectRegeneracion = {
		{Habilidades = '-'}, 
	},

	UsableObjectAumentoVidaMaxima = {
		{Habilidades = '-'}, 
	},

	UsableObjectAumentoPorcentajeAlmas = {
		{Habilidades = '-'}, 
	},

	UsableObjectDamageMeleeEnemy = {
		{Habilidades = '-'}, 
	},

	UsableObjectArrowEnemy = {
		{Habilidades = '-'}, 
	},

	UsableObjectReduceTimeChest = {
		{Habilidades = '-'}, 
	},

	UsableObjectAumentarTiempoReto = {
		{Habilidades = '-'}, 
	},

	Potenciador = { 
		{UsableObject = '-'}, 
		{CScriptComponent = true}, 
		{CParticleController = true}, 
	},
	
	UsableObject = { 
		 
		{CPhysicEntity= true},
		{CGraphics= true}, 
		{CFloatingObject= true}, 
		{CUsableObjectTrigger= true},
	},

	Soul = {
		{CPhysicEntity= true}, 

		{CUsableObjectTrigger= true}, 
		{CScriptComponent = true}, 
		{CParticleController = true},
	},

	Soul2 = {
		{Soul= "-"}, 
	},

	Soul3 = {
		{Soul= "-"}, 
	},

	Coin = {
		{CCoinTorqueController = true},
	
		{CPhysicEntity= true},
		{CPhysicCoin = true},

		{CGraphics= true}, 
		{CScriptComponent = true}, 
		{CTimeToDisappear = true},
	},

	Coin2 = {
		{Coin= "-"}, 
	},

	Coin3 = {
		{Coin= "-"}, 
	},

	MonedaHabitacionTesoro = {
		{Coin= "-"}, 
	},
	
	Stone1 = {
		{CCoinTorqueController = true},
		{CPhysicEntity= true},

		{CGraphics= true}, 
		{CScriptComponent = true}, 
		{CTimeToDisappear = true},
		--{CPhysicsDebug = true},
	},

	Stone2 = {
		{CCoinTorqueController = true},
		{CPhysicEntity= true},

		{CGraphics= true}, 
		{CScriptComponent = true}, 
		{CTimeToDisappear = true},
		--{CPhysicsDebug = true},
	},

	MovableText = {
		{CMovableText2 = true},
	},

	MovableTextDamage = { {CMovableText2 = true}, },

	-- -----------
	-- -------ARMAS
	Katana = { --eS UN ARMA MELEE SUS COMPONENTES SE EJECUTAN DE FORMA ESPECIAL
		
		{CWeaponController= true},
		{CPhysicWeapon = true},
		{CGraphics= true},
		-- {CPhysicsDebug = true},
		-- {CRibbonTrailComponent = true},

		{CKatana = true},
		{SoundSystem = true}, 
		{CScriptComponent = true}, 
		
	},
	BroadSword = {  
		{CWeaponController= true},

		{CPhysicWeapon = true},

		{CGraphics= true},
		-- {CPhysicsDebug = true},
		-- {CRibbonTrailComponent = true},

		{CKatana = true},
		{SoundSystem = true}, 
		{CScriptComponent = true}, 
		
	},

	GreenSword = {  
		{CWeaponController= true},

		{CPhysicWeapon = true},

		{CGraphics= true},
		-- {CPhysicsDebug = true},
		-- {CRibbonTrailComponent = true},
		{CGreenSword = true},
		{SoundSystem = true}, 
		{CScriptComponent = true}, 
		
	},

	Shield = {  
		{CWeaponController= true},
		{CPhysicWeapon = true},
		{CGraphics= true},
		
		{CMeleeShield = true},
		{SoundSystem = true}, 
		{CScriptComponent = true}, 
		-- {CPhysicsDebug = true},
	},

	ShieldSpecial = { 
		{CShieldSpecialController = true}, 
		{CPhysicController = true},
		{CPhysicCoin = true},
		{CAnimatedGraphics= true},
		{CRibbonTrailComponent = true},
		{SoundSystem3D = true},
		-- {CPhysicsDebug = true},
	},
	
	Tongue = {
		{CWeaponController= true},
		{CPhysicWeapon = true},

		{CAnimatedGraphics= true}, 
		
		{CMeleeTongue = true}, 
		{CScriptComponent = true},
		--{CPhysicsDebug = true},
	},


	DartGun = {
		
		{CWeaponController= true},
		{CGraphics= true},  
		{CGunDart = true }, 
		{SoundSystem = true}, 
		{CScriptComponent = true},
	},

	Laser = {
		
		{CWeaponController= true},
		{CGraphics= true},  
		{CGunLaser = true }, 
		{SoundSystem = true}, 
		{CScriptComponent = true},
	},
	
	AK47 = {
		
		{CWeaponController= true},
		{CGraphics= true},  
		{CGunAK47 = true }, 
		{SoundSystem = true}, 
		{CScriptComponent = true},
	},

	-- ----------
	-- -------SMART OBJECTS DE SALTO
	SmartObjectJump = {
		{CPhysicEntity= true}, 
		{CSmartObjectJump= true},
	},

	JumpLeft = { {SmartObjectJump = '-'}, },

	JumpRight = { {SmartObjectJump = '-'}, },
	-- -------------------------------------

	Chest = {
		{CPhysicEntity= true}, 
		{CAnimatedGraphics= true}, 
		{CBillBoard = true},
		{CScriptComponent = true}, 
		{CStaticText = true},
		
		{CParticleController = true},
		-- {CPhysicsDebug = true},
	},
	
	Chest2 = {
		{CPhysicEntity= true}, 
		{CAnimatedGraphics= true}, 
		{CBillBoard = true},
		{CScriptComponent = true}, 
		{CStaticText = true},

		{CParticleController = true},
		-- {CPhysicsDebug = true},
	},

	Chest3 = {
		{CPhysicEntity= true}, 
		{CAnimatedGraphics= true}, 
		{CBillBoard = true},
		{CScriptComponent = true}, 
		{CStaticText = true},

		{CParticleController = true},
		-- {CPhysicsDebug = true},
	},

	-- --POTENCIADORES
	Potenciador1 = { {Potenciador = "-"}, },
	Potenciador2 = { {Potenciador = "-"}, },
	Potenciador3 = { {Potenciador = "-"}, },
	Potenciador4 = { {Potenciador = "-"}, },
	Potenciador5 = { {Potenciador = "-"}, },
	Potenciador6 = { {Potenciador = "-"}, },
	Potenciador7 = { {Potenciador = "-"}, },
	Potenciador8 = { {Potenciador = "-"}, },
	Potenciador9 = { {Potenciador = "-"}, },
	Potenciador10 = { {Potenciador = "-"}, },
	Potenciador11 = { {Potenciador = "-"}, },
	Potenciador12 = { {Potenciador = "-"}, },
	Potenciador13 = { {Potenciador = "-"}, },
	Potenciador14 = { {Potenciador = "-"}, },
	Potenciador15 = { {Potenciador = "-"}, },
	Potenciador16 = { {Potenciador = "-"}, },
	Potenciador17 = { {Potenciador = "-"}, },
	Potenciador18 = { {Potenciador = "-"}, },
	Potenciador19 = { {Potenciador = "-"}, },
	Potenciador20 = { {Potenciador = "-"}, },
	Potenciador21 = { {Potenciador = "-"}, },
	Potenciador22 = { {Potenciador = "-"}, },
	Potenciador23 = { {Potenciador = "-"}, },
	Potenciador24 = { {Potenciador = "-"}, },


	---Elementos de menu

	CameraMenu = {
		{CCameraMenu= true}, 
		{SoundSystem = true}, 
	},

	SoulMenu = {
		{CSoulController = true},

		{CParticleController = true},
		{CSoulLightEffect = true},
		-- {CTorchBehaviour = true},
		{CPuntualLight = true},
		{CTimeToLive = true},
	},

	SoulMenuGenerator = {
		{CSoulMenuGenerator = true},
	},

	
	-------props escenario-------
	PropTapiz = {
		{CPhysicEntity= true}, 	
		{CAnimatedGraphics= true},
		{CBannerAnimationController= true},
		--{CPhysicsDebug = true},
	},
	
	Cobweb1 = {
		{CGraphics= true},
	},
	
	Cobweb2 = {
		{CGraphics= true},
	},
	
	Cobweb3 = {
		{CGraphics= true},
	},
	
	Cobweb4 = {
		{CGraphics= true},
	},
	
	Cobweb5 = {
		{CGraphics= true},
	},

	AltarSacrificio = {
		{CGraphics= true},
		
		
		{CParticleController = true},
	},

	-- triggers tutorial -----

	puntoTutorial = {
		{CParticleController = true},
	},

	triggerTutorial1 = {
		{CPhysicEntity= true},
		{CScriptComponent= true}, 
	},

	HumoMuerteEnemigo = {
		{CParticleController = true},
		{CTimeToLive = true},
	},

	HumoVioleta = {
	---Componente de debug
		-- {CGraphics = true},
		
		{CParticleController = true},
		{CTimeToLive = true},
	},

	HumoSangre = {
		{CParticleController = true},
		{CTimeToLive = true},
		{CScriptComponent= true}, 
	},
	
	PARTICULAAMBIENTACION = {
		{CParticleController = true},
	},

	ExplosionPuerta = 
	{
		{CParticleController = true},
		{CTimeToLive = true},
	},

	ExplosionAranha = 
	{
		{CParticleController = true},
		{CTimeToLive = true},
	},

	ExplosionLaser = {
		{CParticleController = true},
		{CTimeToLive = true},		{SoundSystem3D = true},	},
	
	SaltoAire = {
		{CParticleController = true},
		{CTimeToLive = true},
	},

	ParticulaPuertaCerrada = 
	{
		{CParticleController = true},
	},

	ParticulaPuertaCerradaAbajo = 
	{
		{CParticleController = true},
	},
	
	ParticulaPuertaAbierta =
	{
		{CParticleController = true},
	},

	ParticulaChupaAlmas =
	{
		{CParticleController = true},
		{CParticulaChupaAlmasController = true},
		{CTimeToLive = true},
	},

	---------------------------------------------------------------------------------------------------
--------------------------------------OBJETOS ROMPIBLES--------------------------------------------
---------------------------------------------------------------------------------------------------

	objetosRotos = 
	{
		{CCoinTorqueController = true},
		{CPhysicEntity= true},

		{CGraphics= true}, 
		{CScriptComponent = true}, 
		{CTimeToDisappear = true},
		--{CPhysicsDebug = true},
	},

	cajaEntera = 
	{
		{CGraphics= true},
		{CPhysicEntity= true},
		{CLifeBrokenObjects = true},
		--{CPhysicsDebug = true},
		--{CScriptComponent= true},
	},

	cajaParte1 = { {objetosRotos = "-"}, },

	cajaParte2 = { {objetosRotos = "-"}, },

	cajaParte3 = { {objetosRotos = "-"}, },

	cajaParte4 = { {objetosRotos = "-"}, },

	cajaParte5 = { {objetosRotos = "-"}, },

	barrilEntero = 
	{
		{CGraphics= true},
		{CPhysicEntity= true},
		{CLifeBrokenObjects = true},
		--{CPhysicsDebug = true},
		--{CScriptComponent= true},
	},

	barrilParte1 = { {objetosRotos = "-"}, },

	barrilParte2 = { {objetosRotos = "-"}, },

	barrilParte3 = { {objetosRotos = "-"}, },

	barrilParte4 = { {objetosRotos = "-"}, },

	barrilParte5 = { {objetosRotos = "-"}, },

	calavera = 
	{
		{CGraphics= true},
		{CPhysicEntity= true},
		{CLifeBrokenObjects = true},
		--{CPhysicsDebug = true},
		--{CScriptComponent= true},
	},

	pilaCalaveras = 
	{
		{CGraphics= true},
		{CPhysicEntity= true},
		{CLifeBrokenObjects = true},
		--{CPhysicsDebug = true},
		--{CScriptComponent= true},
	},

	calaveraParte1 = { {objetosRotos = "-"}, },

	calaveraParte2 = { {objetosRotos = "-"}, },

	calaveraParte3 = { {objetosRotos = "-"}, },
}
