
namespace Map
{

	/*
	CONSTANTES DE DIMENSIONES DE UN TILE
	*/

	enum
	{
		ANCHO_TILE = 20,
		ALTO_TILE = 20,
		PROFUNDO_TILE = 50,
	};

	enum TipoCapa 
	{
		Fondo = 0,
		Colision = 1,
		Logic = 2,
		PuntosInteres = 3,
		PuntosAltares = 4,
	};

	/*
	Este enum tiene el tipo de tile, el cual coincide con un arquetipo y es representado por un ID en el mapa
	Ejemplo:

	TILE_FONDO - ID: 1 y su arquetipo es "TILE_FONDO"
	*/
	enum TipoTile
	{
		TILE_DEFAULT_BORDE = 1,
		TILE_DEFAULT = 2,
		TILE_ICE = 3,
		TILE_GUM = 4,
		TILE_FRAGILE = 5,
		TILE_FRAGILE_BORDE = 6,
		TILE_STONES = 18,
		TILE_PLATFORM = 17,
		TILE_SLIDE_UR = 7,
		TILE_SLIDE_UL = 8,
		TILE_SLIDE_DR = 19,
		TILE_SLIDE_DL = 20,
		TILE_SEMISLIDE_UR1 = 9,
		TILE_SEMISLIDE_UR2 = 10,
		TILE_SEMISLIDE_UL2 = 11,
		TILE_SEMISLIDE_UL1 = 12,
		TILE_SEMISLIDE_DR1 = 21,
		TILE_SEMISLIDE_DR2 = 22,
		TILE_SEMISLIDE_DL2 = 23,
		TILE_SEMISLIDE_DL1 = 24,
		TILE_DOOR = 13,
		DOOR_VERTICAL_DER_TRIGGER = 14,
		DOOR_HORIZONTAL_TRIGGER = 15,
		DOOR_VERTICAL_IZQ_TRIGGER = 16,//aqui se meten las puertas y abajo de se cambia el enumerado
		SMART_OBJECT_JUMP_RIGHT = 28,
		SMART_OBJECT_JUMP_LEFT = 27,
		ANTORCHA = 26,
		SPAWNPOINT = 25,
		TAPIZ = 29,
		COBWEB1 = 31,
		COBWEB2 = 45,
		COBWEB3 = 44,
		COBWEB4 = 32,
		COBWEB5 = 33,
		PARTICULAAMBIENTACION = 42,
		ALTAR50 = 51,
		ALTAR80 = 52,
		ALTAR100 = 53,
		PLANOCOLISION = 48,
		TILE_FONDO1 = 49,
		TILE_FONDO2 = 50,
		TILE_FONDO3 = 61,
		TILE_FONDO4 = 62,
		TILE_SLIDE_UR_BORDE = 55,
		TILE_SLIDE_UL_BORDE = 56,
		TILE_SEMISLIDE_UR1_BORDE = 57,
		TILE_SEMISLIDE_UR2_BORDE = 58,
		TILE_SEMISLIDE_UL2_BORDE = 59,
		TILE_SEMISLIDE_UL1_BORDE = 60,
		MonedaHabitacionTesoro = 46,
	};


	class TileArchetype
	{


	public:
		static std::string TipoTileToArchetype(TipoTile tipo)
		{
			std::string res = "TILE_DEFAULT";
			switch (tipo)
			{
			case Map::TILE_DEFAULT_BORDE:
				res = "TILE_DEFAULT_BORDE";
				break;
			case Map::TILE_DEFAULT:
				res = "TILE_DEFAULT";
				break;
			case Map::TILE_ICE:
				res = "TILE_ICE";
				break;
			case Map::TILE_GUM:
				res = "TILE_GUM";
				break;
			case Map::TILE_FRAGILE:
				res = "TILE_FRAGILE";
				break;
			case Map::TILE_FRAGILE_BORDE:
				res = "TILE_FRAGILE_BORDE";
				break;
			case Map::TILE_STONES:
				res = "TILE_STONES";
				break;
			case Map::TILE_PLATFORM:
				res = "TILE_PLATFORM";
				break;
			case Map::TILE_SLIDE_UR:
				res = "TILE_SLIDE_UR";
				break;
			case Map::TILE_SLIDE_UL:
				res = "TILE_SLIDE_UL";
				break;
			case Map::TILE_SLIDE_UR_BORDE:
				res = "TILE_SLIDE_UR_BORDE";
				break;
			case Map::TILE_SLIDE_UL_BORDE:
				res = "TILE_SLIDE_UL_BORDE";
				break;
			case Map::TILE_SLIDE_DR:
				res = "TILE_SLIDE_DR";
				break;
			case Map::TILE_SLIDE_DL:
				res = "TILE_SLIDE_DL";
				break;
			case Map::TILE_SEMISLIDE_UR1:
				res = "TILE_SEMISLIDE_UR1";
				break;
			case Map::TILE_SEMISLIDE_UR2:
				res = "TILE_SEMISLIDE_UR2";
				break;
			case Map::TILE_SEMISLIDE_UL1:
				res = "TILE_SEMISLIDE_UL1";
				break;
			case Map::TILE_SEMISLIDE_UL2:
				res = "TILE_SEMISLIDE_UL2";
				break;
			case Map::TILE_SEMISLIDE_UR1_BORDE:
				res = "TILE_SEMISLIDE_UR1_BORDE";
				break;
			case Map::TILE_SEMISLIDE_UR2_BORDE:
				res = "TILE_SEMISLIDE_UR2_BORDE";
				break;
			case Map::TILE_SEMISLIDE_UL1_BORDE:
				res = "TILE_SEMISLIDE_UL1_BORDE";
				break;
			case Map::TILE_SEMISLIDE_UL2_BORDE:
				res = "TILE_SEMISLIDE_UL2_BORDE";
				break;
			case Map::TILE_SEMISLIDE_DR1:
				res = "TILE_SEMISLIDE_DR1";
				break;
			case Map::TILE_SEMISLIDE_DR2:
				res = "TILE_SEMISLIDE_DR2";
				break;
			case Map::TILE_SEMISLIDE_DL1:
				res = "TILE_SEMISLIDE_DL1";
				break;
			case Map::TILE_SEMISLIDE_DL2:
				res = "TILE_SEMISLIDE_DL2";
				break;
			case Map::TILE_DOOR:
				res = "TILE_DOOR";
				break;
			case Map::DOOR_VERTICAL_DER_TRIGGER:
				res = "DOOR_VERTICAL_DER_TRIGGER";
				break;
			case Map::DOOR_HORIZONTAL_TRIGGER:
				res = "DOOR_HORIZONTAL_TRIGGER";
				break;
			case Map::DOOR_VERTICAL_IZQ_TRIGGER:
				res = "DOOR_VERTICAL_IZQ_TRIGGER";
				break;
			case Map::SMART_OBJECT_JUMP_LEFT:
				res = "JumpLeft";
				break;
			case Map::SMART_OBJECT_JUMP_RIGHT:
				res = "JumpRight";
				break;
			case Map::ANTORCHA:
				res = "Antorcha";
				break;
			case Map::PLANOCOLISION:
				res = "PlanoColision";
				break;
			case Map::SPAWNPOINT:
				res = "SpawnPoint";
				break;
			case Map::TAPIZ:
				res = "PropTapiz";
				break;
			case Map::COBWEB1:
				res = "Cobweb1";
				break;
			case Map::COBWEB2:
				res = "Cobweb2";
				break;
			case Map::COBWEB3:
				res = "Cobweb3";
				break;
			case Map::COBWEB4:
				res = "Cobweb4";
				break;
			case Map::COBWEB5:
				res = "Cobweb5";
				break;
			case Map::ALTAR100:
				res = "AltarSacrificio";
				break;
			case Map::ALTAR50:
				res = "AltarSacrificio";
				break;
			case Map::ALTAR80:
				res = "AltarSacrificio";
				break;
			case Map::TILE_FONDO1:
				res = "TILE_FONDO1";
				break;
			case Map::TILE_FONDO2:
				res = "TILE_FONDO2";
				break;
			case Map::TILE_FONDO3:
				res = "TILE_FONDO3";
				break;
			case Map::TILE_FONDO4:
				res = "TILE_FONDO4";
				break;
			case Map::PARTICULAAMBIENTACION:
				res = "PARTICULAAMBIENTACION";
				break;
			case Map::MonedaHabitacionTesoro:
				res = "MonedaHabitacionTesoro";
				break;
			default:
				res = "TILE_DEFAULT";
				break;
			}

			return res;
		};
	};
}
	