#pragma once

enum character
{
	CHAR_BOWSER       = 0x00,
	CHAR_DAISY        = 0x01,
	CHAR_DONKEY_KONG  = 0x02,
	CHAR_HONEY_QUEEN  = 0x03,
	CHAR_KOOPA_TROOPA = 0x04,
	CHAR_LAKITU       = 0x05,
	CHAR_LUIGI        = 0x06,
	CHAR_MARIO        = 0x07,
	CHAR_METAL_MARIO  = 0x08,
	CHAR_MII_MALE     = 0x09,
	CHAR_MII_FEMALE   = 0x0a,
	CHAR_PEACH        = 0x0b,
	CHAR_ROSALINA     = 0x0c,
	CHAR_SHY_GUY      = 0x0d,
	CHAR_TOAD         = 0x0e,
	CHAR_WARIO        = 0x0f,
	CHAR_WIGGLER      = 0x10,
	CHAR_YOSHI        = 0x11
};

enum kart
{
	KART_STANDARD      = 0x00,
	KART_BOLT_BUGGY    = 0x01,
	KART_BIRTHDAY_GIRL = 0x02,
	KART_EGG_1         = 0x03,
	KART_BDASHER       = 0x04,
	KART_ZUCCHINI      = 0x05,
	KART_KOOPA_CLOWN   = 0x06,
	KART_TINY_TUG      = 0x07,
	KART_BUMBLE_V      = 0x08,
	KART_CACT_X        = 0x09,
	KART_BRUISER       = 0x0a,
	KART_PIPE_FRAME    = 0x0b,
	KART_BARREL_TRAIN  = 0x0c,
	KART_CLOUD_9       = 0x0d,
	KART_BLUE_SEVEN    = 0x0e,
	KART_SODA_JET      = 0x0f,
	KART_GOLD_STANDARD = 0x10,
};

enum tire
{
	TIRE_STANDARD    = 0x00,
	TIRE_MONSTER     = 0x01,
	TIRE_ROLLER      = 0x02,
	TIRE_SLICK       = 0x03,
	TIRE_SLIM        = 0x04,
	TIRE_SPONGE      = 0x05,
	TIRE_GOLD_TIRES  = 0x06,
	TIRE_WOOD        = 0x07,
	TIRE_RED_MONSTER = 0x08,
	TIRE_MUSHROOM    = 0x09,
};

enum glider
{
	WING_SUPER_GLIDER  = 0x00,
	WING_PARAGLIDER    = 0x01,
	WING_PEACH_PARASOL = 0x02,
	WING_FLOWER_GLIDER = 0x03,
	WING_SWOOPER       = 0x04,
	WING_BEAST_GLIDER  = 0x05,
	WING_GOLD_GLIDER   = 0x06,
};

struct ghost
{
	uint32_t file_offset = 0; //< where ghost's data starts in spotpass file (if applicable)
	uint32_t ghost_id    = 0; //< where ghost's data starts in spotpass file (if applicable)

	std::wstring player_name = L"Player";

	bool fp_flag; //< true if ghost used first person 80% or more

	uint16_t finished_ms;  //< finished ghost time milliseconds
	uint16_t finished_sec; //< finished ghost time seconds
	uint16_t finished_min; //< finished ghost time minutes

	uint8_t course_id;

	uint8_t character_id;
	uint8_t kart_id;
	uint8_t tire_id;
	uint8_t glider_id;
};