README!!!


Convenio de organización del directorio de LUA

Intentad ubicar los ficheros de forma ordenada en los directorios que más convenga!!


-Todos los directorios son cargados con un fichero xxxxCargadorFiles.lua

-En el directorio raíz LUA/ existe el CargadorFiles.lua general. Este fichero se encargará de cargar los cargadores de ficheros de cada directorio
    **En caso de querer añadir un directorio nuevo, es importante que se de alta, junto con su xxxxxCargadorFiles.lua



=====================================================================================

-Config -> directorio de configuraciones generales del juego (Config del motor de sonido, animaciones, parsers)

-Managers -> directorio para el núcleo duro de la lógica de juego (Drop, Retos ....)

-IA -> Espacio resevado para las utilidades de la lógica del comportamiento de los enemigos

-GUI  -> directorio reservado para la definición de menús e interfaces (y su configuración)

-Scripts -> Hereda de scriptEntity?? entonces ponlo aqui. Todo lo que sea un script de comportamiento va en este directorio


=====================================================================================


SCRIPTS

Para comodidad del convenio de nombres, todo lo que herede de ScriptEntity, es decir un script de comportamiento, debrá acabar su nombre con xxxxxScript.lua




========================================= BACKUP =========================================

En la raíz del proyecto se va a dejar una carpeta con los datos necesarios para revertir el commit. Es altamente probable que el proyecto de fallo en la raiz del directorio. Os recomiendo que integreis con cabeza, pero en caso de que todo vaya mal, los Src y la carpeta LUA está intacta en este directorio