Map = {
	
	Scene = {
		type ="SceneConfiguration",
		ambient_light = { 0.1, 0.1, 0.1},
		-- ambient_light = { 0.9, 0.9, 0.9},
	},
	
	CameraMenu = {
		type = "CameraMenu",
		position = {120, -140, 80},
		targetPosition = {140, -140, -40},
		fov = 60,
		far_clip_distance = 400,
		near_clip_distance = 1,
	},

	-- LuzEscena = {
	-- 	type = "LuzEscena",
	-- 	diffuse = {0.325, 0.325, 0.325},
	-- 	specular = {0.3, 0.3, 0.3},
	-- },


	SoulMenuGenerator = {
		type = "SoulMenuGenerator",
		position = {200, -260, 0},
		generator_time = 40,
		generator_offset = {30, 0, 0},
	},

}
