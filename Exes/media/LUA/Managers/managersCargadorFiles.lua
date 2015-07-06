-- @Author Alberto Martínez Villarán
-- @date Marzo, 2015

-- Este fichero se encarga de cargar todos los archivos del directorio LUA/Managers


scriptMgr:loadScript(Paths.luaManagersPath.."GameMgr.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Cordura.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."DataPlayer.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."RetoMgr.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Ambientaciones.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."MapList.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Puntuacion.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."EventoAvalancha.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."TrophyManager.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Puntuaciones.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."SoundtrackMgr.lua")


--Fichero relacionados con el drop
scriptMgr:loadScript(Paths.luaManagersPath.."DropManager.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Potenciadores.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Armas.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Habilidades.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Cofres.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Altar.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Almas.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Dinero.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."Trampas.lua")
scriptMgr:loadScript(Paths.luaManagersPath.."ObjetosRompibles.lua")

----El spawner se debe hacer despues de cargar todos los ficheros de drop
scriptMgr:loadScript(Paths.luaManagersPath.."SpawnerMgr.lua")