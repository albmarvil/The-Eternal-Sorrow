--Este fichero contiene la información de configuración de las animaciones.
--Este fichero debería contener dentro de la tabla Animations TODAS las configuraciones necesarias para
--TODAS las entidades animadas del juego. Se puede tomar como ejemplo el player, que es quizás la más compleja.
Animations = {
	
	PlayerAnimations = {
	
		Idle = {

			Transitions = {
				Walk = 0.2,
				WalkBack = 0.2,
				IdleKatana = 0.2,
				Run = 0.2,
			},

		},

		Walk = {

			Transitions = {
				Idle = 0.2,
				WalkBack = 0.2,
				IdleKatana = 0.2,
				Run = 0.2,
			},

		},

		Walkback = {

			Transitions = {
				Idle = 0.2,
				Walk = 0.2,
				IdleKatana = 0.2,
				Run = 0.2,
			},

		},

		Run = {

			Transitions = {
				Idle = 0.2,
				Walk = 0.8,
				IdleKatana = 0.2,
				Walkback = 0.2,
			}

		},

		IdleKatana = {

			Transitions = {
				Idle = 0.2,
				Walk = 0.2,
				Walkback = 0.2,
				Run = 0.2,
			}

		},

		FireKatana = {

			Transitions = {
			},
		},


		-----Configuración de las máscaras
		LeftArm = {

			Mask = {
				{32, 1.0},
				{33, 1.0},
				{34, 1.0},
				{35, 1.0},
				{36, 1.0},
				{37, 1.0},
				{38, 1.0},
				{39, 1.0},
				{40, 1.0},
				{41, 1.0},
				{42, 1.0},
				{43, 1.0},
			},
		},

		RightArm = {

			Mask = {
				{44, 1.0},
				{45, 1.0},
				{46, 1.0},
				{47, 1.0},
				{48, 1.0},
				{49, 1.0},
				{50, 1.0},
				{51, 1.0},
				{52, 1.0},
				{53, 1.0},
				{54, 1.0},
				{55, 1.0},
			},
		},

		default = {

			Mask = {
				{0, 1.0},
				{1, 1.0},
				{2, 1.0},
				{3, 1.0},
				{4, 1.0},
				{5, 1.0},
				{6, 1.0},
				{7, 1.0},
				{8, 1.0},
				{9, 1.0},
				{10, 1.0},
				{11, 1.0},
				{12, 1.0},
				{13, 1.0},
				{14, 1.0},
				{15, 1.0},
				{16, 1.0},
				{17, 1.0},
				{18, 1.0},
				{19, 1.0},
				{20, 1.0},
				{21, 1.0},
				{22, 1.0},
				{23, 1.0},
				{24, 1.0},
				{25, 1.0},
				{26, 1.0},
				{27, 1.0},
				{28, 1.0},
				{29, 1.0},
				{30, 1.0},
				{31, 1.0},
				{56, 1.0},
				{57, 1.0},
				{58, 1.0},
				{59, 1.0},
				{60, 1.0},
				{61, 1.0},
				{62, 1.0},
				{63, 1.0},
				{64, 1.0},
				{65, 1.0},
				{66, 1.0},
				{67, 1.0},
				{68, 1.0},
				{69, 1.0},
				{70, 1.0},
				{71, 1.0},
				{72, 1.0},
				{73, 1.0},
				{74, 1.0},
				{75, 1.0},
				{76, 1.0},
				{77, 1.0},
				{78, 1.0},
				{79, 1.0},
				{80, 1.0},
				{81, 1.0},
				{82, 1.0},
				{83, 1.0},
				{84, 1.0},
				{85, 1.0},
				{86, 1.0},
				{87, 1.0},
				{88, 1.0},
				{89, 1.0},
				{90, 1.0},
				{91, 1.0},
				{92, 1.0},
				{93, 1.0},
				{94, 1.0},
				{95, 1.0},
				{96, 1.0},
				{97, 1.0},
				{98, 1.0},
			},
		},

		LeftArmBone = "Bip01 L UpperArm",
		RightArmBone = "Bip01 R UpperArm",

		LeftHandBone = "Bip01 L Hand",
		RightHandBone = "Bip01 R Hand",

	},


	EspermiAnimations = {
	
		Idle = {

			Transitions = {
				Walk = 0.2,
				Walkback = 0.2,
				Jump = 0.2,
				Death = 0.2,
			},

		},

		Walk = {

			Transitions = {
				Idle = 0.2,
				Walkback = 0.2,
				Jump = 0.2,
				Death = 0.2,
			},

		},

		Walkback = {

			Transitions = {
				Walk = 0.2,
				Idle = 0.2,
				Jump = 0.2,
				Death = 0.2,
			},

		},

		Jump = {

			Transitions = {
				Falling = 0.2,
				Air = 0.2,
				Jump = 0.2,
				Death = 0.2,
			},

		},

		Air = {

			Transitions = {
				Falling = 0.2,
			},

		},

		Falling = {

			Transitions = {
				Walk = 0.2,
				Walkback = 0.2,
				Idle = 0.2,
				Death = 0.2,
			},

		},

		Death = {

			Transitions = {
			},

		},

		ShootRight = {

			Transitions = {
			},

		},

		ShootLeft = {

			Transitions = {
			},

		},

		MeleeRight = {

			Transitions = {
			},

		},

		MeleeLeft = {

			Transitions = {
			},

		},



		-----Configuración de las máscaras
		LeftArm = {

			Mask = {
				{5, 1.0},
				{6, 1.0},
				{7, 1.0},
				{8, 1.0},
				{9, 1.0},
				{10, 1.0},
			},
		},

		RightArm = {

			Mask = {
				{16, 1.0},
				{17, 1.0},
				{18, 1.0},
				{19, 1.0},
				{20, 1.0},
				{21, 1.0},
			},
		},

		default = {

			Mask = {

				{0, 1.0},
				{1, 1.0},
				{2, 1.0},
				{3, 1.0},
				{4, 1.0},
				{11, 1.0},
				{12, 1.0},
				{13, 1.0},
				{14, 1.0},
				{15, 1.0},
				{22, 1.0},
				{23, 1.0},
				{24, 1.0},
				{25, 1.0},
				{26, 1.0},
				{27, 1.0},

			},
		},

		LeftArmBone = "CATRigLArm01",
		RightArmBone = "CATRigRArm01",

		LeftHandBone = "CATRigLArmPalm",
		RightHandBone = "CATRigRArmPalm",

	},
	
	SaiAnimations = {
		--Transiciones
		Idle = {

			Transitions = {
				Walk = 0.2,
				Walkback = 0.2,
				Jump = 0.1,
				Death = 0.2,
				IdleWait1 = 0.2,
				IdleWait2 = 0.2,
				IdleWait3 = 0.2,
			},
		},
		
		IdleWait1 = {

			Transitions = {
				Walk = 0.2,
				Walkback = 0.2,
				Jump = 0.1,
				Death = 0.2,
				Idle = 0.2,
				IdleWait2 = 0.2,
				IdleWait3 = 0.2,
			},
		},
		
		IdleWait2 = {

			Transitions = {
				Walk = 0.2,
				Walkback = 0.2,
				Jump = 0.1,
				Death = 0.2,
				IdleWait1 = 0.2,
				Idle = 0.2,
				IdleWait3 = 0.2,
			},
		},
		
		IdleWait3 = {

			Transitions = {
				Walk = 0.2,
				Walkback = 0.2,
				Jump = 0.1,
				Death = 0.2,
				IdleWait1 = 0.2,
				IdleWait2 = 0.2,
				Idle = 0.2,
			},
		},

		Walk = {

			Transitions = {
				Idle = 0.2,
				Walkback = 0.2,
				Jump = 0.1,
				Death = 0.2,
			},
		},

		Walkback = {

			Transitions = {
				Walk = 0.2,
				Idle = 0.2,
				Jump = 0.1,
				Death = 0.2,
			},
		},

		Jump = {

			Transitions = {
				Fall = 0.2,
				Jump = 0.2,
				Death = 0.2,
			},
		},

		Fall = {

			Transitions = {
				Walk = 0.2,
				Walkback = 0.2,
				Idle = 0.2,
				Death = 0.2,
			},
		},
		
		IdleSword1R = {

			Transitions = {
				IdleGunR = 0.2,
				AttackSword1R = 0.2,
				AttackSword1FR = 0.2,
				AttackShieldR = 0.2,

				IdleSword1R = 0.2,
			},
		},
		
		IdleSword1L = {

			Transitions = {
				IdleGunL = 0.2,
				AttackSword1L = 0.2,
				AttackSword1FL = 0.2,
				AttackShieldL = 0.2,
				IdleSword1L = 0.2,
			},
		},


		IdleSword2R = {

			Transitions = {
				IdleGunR = 0.2,
				AttackSword2R = 0.2,
				AttackSword2FR = 0.2,
				AttackShieldR = 0.2,

				IdleSword2R = 0.2,
			},
		},
		
		IdleSword2L = {

			Transitions = {
				IdleGunL = 0.2,
				AttackSword2L = 0.2,
				AttackSword2FL = 0.2,
				AttackShieldL = 0.2,
				IdleSword2L = 0.2,
			},
		},
		
		IdleGunR = {

			Transitions = {
				IdleSword1R = 0.2,
				AttackGunR = 0.2,
				IdleGunR = 0.2,
			},
		},
		
		IdleGunL = {

			Transitions = {
				IdleSword1L = 0.2,
				AttackGunL = 0.2,
				IdleGunL = 0.2,
			},
		},
		
		AttackSword1R = {

			Transitions = {
				IdleSword1R = 0.2,
			},
		},
		
		AttackSword1L = {

			Transitions = {
				IdleSword1L = 0.2,
			},
		},


		AttackSword1FR = {

			Transitions = {
				IdleSword1R = 0.2,
			},
		},
		
		AttackSword1FL = {

			Transitions = {
				IdleSword1L = 0.2,
			},
		},

		AttackSword2L = {

			Transitions = {
				IdleSword1L = 0.2,
			},
		},
		
		AttackSword2R = {

			Transitions = {
				IdleSword1R = 0.2,
			},
		},

		AttackSword2FL = {

			Transitions = {
				IdleSword1L = 0.2,
			},
		},
		
		AttackSword2FR = {

			Transitions = {
				IdleSword1R = 0.2,
			},
		},

		AttackGunR = {

			Transitions = {
				IdleGunR = 0.2,
			},
		},
		
		AttackGunL = {

			Transitions = {
				IdleGunL = 0.2,
			},
		},
		
		-----Configuración de las máscaras
		LeftArm = {

			Mask = {
				{5, 1.0},
				{6, 1.0},
				{7, 1.0},
				{8, 1.0},
				{9, 1.0},
			},
		},

		RightArm = {

			Mask = {
				{11, 1.0},
				{12, 1.0},
				{13, 1.0},
				{14, 1.0},
				{15, 1.0},
			},
		},

		default = {

			Mask = {

				{0, 1.0},
				{1, 1.0},
				{2, 1.0},
				{3, 1.0},
				{4, 1.0},
				{10, 1.0},
				{16, 1.0},
				{17, 1.0},
				{18, 1.0},
				{19, 1.0},
				{20, 1.0},
				{21, 1.0},
				{22, 1.0},
				{23, 1.0},
			},
		},
		
		body = {

			Mask = {

				{0, 1.0},
				{1, 1.0},
				{2, 1.0},
				{3, 1.0},
				{4, 1.0},
				{5, 1.0},
				{6, 1.0},
				{7, 1.0},
				{8, 1.0},
				{9, 1.0},
				{10, 1.0},
				{11, 1.0},
				{12, 1.0},
				{13, 1.0},
				{14, 1.0},
				{15, 1.0},
				{16, 1.0},
				{17, 1.0},
				{18, 1.0},
				{19, 1.0},
				{20, 1.0},
				{21, 1.0},
				{22, 1.0},
				{23, 1.0},
			},
		},
			
		

		LeftArmBone = "Bone.007",
		RightArmBone = "Bone.010",

		LeftHandBone = "Bone.024",
		RightHandBone = "Bone.023",

	},

	CthulhuAnimations = {
		--Transiciones
		Idle = {

			Transitions = {
				Drenar = 0.1,
			},
		},

		Drenar = {

			Transitions = {
				Idle = 0.4,
			},
		},
	}
}



--funcion que dada una configuracion de animacion, una animacion destino y una animacion origen, nos devuelve el tiempo de transicion
function getTransitionTime(table, from, to)
	-- Log.Debug("Entrada: "..table.." - "..from.." - "..to)
	--buscamos la tabla con el nombre table

	local animation = Animations[table]
	if animation == nil then
		 -- Log.Warning("No se ha encontrado la animacion origen")
		return 0
	end
	local fromAnimation = animation[from]
	if fromAnimation == nil then
		 -- Log.Warning("No se ha encontrado la animacion destino")
		return 0
	end
	local transitions = fromAnimation["Transitions"]
	if transitions == nil then
		 -- Log.Warning("No se ha encontrado datos sobre animaciones a transitar")
		return 0
	end
	local toAnimationTime = transitions[to]
	if toAnimationTime == nil then
		 -- Log.Warning("No se ha encontrado datos sobre el tiempo de transicion a otras")
		return 0
	end

	return toAnimationTime

end



--funcion que nos devuelve la máscara de animación de una animación concreta
function getAnimationMaskValue(animationConfig, maskName, index)
	-- body

	local animation = Animations[animationConfig]
	if animation == nil then
		-- Log.Warning("No se ha encontrado ninguna configuracion de animacion valida")
		return nil
	end
	local maskTable = animation[maskName]
	if maskTable == nil then
		-- Log.Warning("No se ha encontrado ninguna mascara valida")
		return nil
	end
	local mask = maskTable["Mask"]
	if mask == nil then
		-- Log.Warning("No se ha encontrado ninguna mascara valida")
		return nil
	end
	
	if type(mask) == "table" then
		return mask[index]
	end
	-- Log.Warning("No se ha encontrado ninguna mascara valida")
	return nil
end

function getMaskLength( animationConfig, maskName )
	-- body
	-- print("Entrada "..animationConfig.." - "..animationName)

	local animation = Animations[animationConfig]
	if animation == nil then
		-- Log.Warning("No se ha encontrado ninguna configuracion de animacion valida")
		return 0
	end
	local maskTable = animation[maskName]
	if maskTable == nil then
		-- Log.Warning("No se ha encontrado ninguna mascara valida")
		return 0
	end
	local mask = maskTable["Mask"]
	if mask == nil then
		-- Log.Warning("No se ha encontrado ninguna mascara valida")
		return 0
	end
	
	if type(mask) == "table" then
		return table.getn(mask)
	end
	-- Log.Warning("No se ha encontrado ninguna mascara valida")
	return 0



	
end



function getLeftArmBone( animationConfig )

	local animation = Animations[animationConfig]
	if animation == nil then
		log.Error("No se ha encontrado una configuracion de animaciones valida")
		return ""
	end
	local bone = animation["LeftArmBone"]
	if bone == nil then
		Log.Error("No se ha encontrado el hueso del brazo izquierdo. Revise la tabla")
		return ""
	end
	return bone
end

function getRightArmBone( animationConfig )
	local animation = Animations[animationConfig]
	if animation == nil then
		Log.Error("No se ha encontrado una configuracion de animaciones valida")
		return ""
	end
	local bone = animation["RightArmBone"]
	if bone == nil then
		Log.Error("No se ha encontrado el hueso del brazo derecho. Revise la tabla")
		return ""
	end
	return bone
end


function getLeftHandBone( animationConfig )
	local animation = Animations[animationConfig]
	if animation == nil then
		Log.Error("No se ha encontrado una configuracion de animaciones valida")
		return ""
	end
	local bone = animation["LeftHandBone"]
	if bone == nil then
		Log.Error("No se ha encontrado el hueso de la mano izquierda. Revise la tabla")
		return ""
	end
	return bone
end

function getRightHandBone( animationConfig )
	local animation = Animations[animationConfig]
	if animation == nil then
		Log.Error("No se ha encontrado una configuracion de animaciones valida")
		return ""
	end
	local bone = animation["RightHandBone"]
	if bone == nil then
		Log.Error("No se ha encontrado el hueso de la mano derecha. Revise la tabla")
		return ""
	end
	return bone
end