-- @Author Alberto Martínez Villarán
-- @date Marzo, 2015

-- Este fichero se encarga de cargar todos los archivos del directorio LUA/Config

--Configuración de la Application
scriptMgr:loadScript(Paths.luaConfigPath.."config.lua")

--Configuración del motor de sonidos y los bancos de sonido
scriptMgr:loadScript(Paths.luaConfigPath.."SoundsConfig.lua")

--Configuración de las animaciones
scriptMgr:loadScript(Paths.luaConfigPath.."AnimationConfig.lua")

--Parseador de mapas
scriptMgr:loadScript(Paths.luaConfigPath.."loadBlueprints.lua")
scriptMgr:loadScript(Paths.luaConfigPath.."MapParser.lua")

