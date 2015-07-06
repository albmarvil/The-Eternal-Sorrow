Paths = {
	pathLUA = "./media/LUA/",
	pathMaps = "./media/maps/",
	pathLOG = "./logs/TES.log",
	pathSounds = "./media/sounds/",

	
}


Paths.luaScriptsPath = Paths.pathLUA.."Scripts/"
Paths.luaConfigPath = Paths.pathLUA.."Config/"
Paths.luaManagersPath = Paths.pathLUA.."Managers/"
Paths.luaIAPath = Paths.pathLUA.."IA/"
Paths.luaGUIPath = Paths.pathLUA.."GUI/"
Paths.luaEnemyPath = Paths.pathLUA.."Managers/Enemigos/"


function cargarFicheros()

	scriptMgr:loadScript(Paths.luaConfigPath.."configCargadorFiles.lua")

	scriptMgr:loadScript(Paths.luaGUIPath.."guiCargadorFiles.lua")

	scriptMgr:loadScript(Paths.luaManagersPath.."managersCargadorFiles.lua")

	scriptMgr:loadScript(Paths.luaIAPath.."iaCargadorFiles.lua")

	scriptMgr:loadScript(Paths.pathLUA.."RandomUtils.lua")

	scriptMgr:loadScript(Paths.luaManagersPath.."RellenadoPoolInicial.lua")

end
