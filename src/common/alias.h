#pragma once

// 1 - 4
// stores where each course is inside of cup file
static const uint8_t cup_course_index[] =
{
	
	2, // 0
	4, // 1
	3, // 2
	2, // 3

	1, // 4
	4, // 5
	3, // 6
	1, // 7

	1, // 8
	4, // 9
	2, // 10
	3, // 11

	1, // 12
	4, // 13
	2, // 14
	3, // 15

	3, // 16
	1, // 17
	4, // 18
	3, // 19

	4, // 20
	3, // 21
	2, // 22
	4, // 23

	2, // 24
	3, // 25
	1, // 26
	1, // 27

	1, // 28
	2, // 29
	2, // 30
	4, // 31

	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,

	1, // 42
	2, // 59 
	3, // 44
	4 // 45
};

// course id for every course in each cup
static const uint8_t cup_courses[32][4] =
{
	{  4,  3,  2,  5 },   // Mushroom Cup
	{  8,  0, 15,  1 },   // Flower Cup
	{ 12, 14,  6,  9 },   // Star Cup
	{  7, 10, 11, 13 },   // Special Cup
	{ 26, 29, 19, 20 },   // Shell Cup
	{ 28, 30, 16, 23 },   // Banana Cup
	{ 27, 22, 25, 18 },   // Leaf Cup
	{ 17, 24, 21, 31 },   // Lightning Cup
	{ 0xff, 0xff, 0xff, 0xff }, // Unused
	{ 42, 59, 44, 45 },   // Bell Cup  10
	{ }, // Acorn Cup  11
	{ }, // Cloud Cup  12
	{ }, // Boo Cup  13
	{ }, // Spring Cup  14
	{ }, // Egg Cup  15
	{ }, // Bullet Cup  16
	{ }, // Rainbow Cup  17
	{ }, // Blooper Cup  18
	{ }, // Feather Cup  19
	{ }, // Fireball Cup  20
	{ }, // Bob-omb Cup  21
	{ }, // Cherry Cup  22
	{ }, // Coin Cup  23
	{ }, // Pickaxe Cup  24
	{ }, // Mega Cup  25
	{ }, // Propeller Cup  26
	{ }, // POW Cup  27
	{ }, // Rock Cup  28
	{ }, // Moon Cup  29
	{ }, // Hammer Cup  30
	{ }, // Wonder Cup  31
};

static const char* course_name[] = 
{
	// Original Courses
	"Mario Circuit",        // 0
	"Rock Rock Mountain",   // 1
	"Cheep Cheep Lagoon",   // 2
	"Daisy Hills",          // 3
	"Toad Circuit",         // 4
	"Shy Guy Bazaar",       // 5
	"Neo Bowser City",      // 6
	"DK Jungle",            // 7
	"Wuhu Loop",            // 8
	"Maka Wuhu",            // 9
	"Rosalina's Ice World", // 10
	"Bowser's Castle",      // 11
	"Piranha Plant Slide",  // 12 
	"Rainbow Road",         // 13
	"Wario Shipyard",       // 14 
	"Music Park",           // 15
	"Wii Coconut Mall",     // 16
	"Wii Koopa Cape",       // 17
	"Wii Maple Treeway",    // 18
	"Wii Mushroom Gorge",   // 19
	"DS Luigi's Mansion",   // 20
	"DS Airship Fortress",  // 21
	"DS DK Pass",           // 22
	"DS Waluigi Pinball",   // 23
	"GCN Dino Dino Jungle", // 24
	"GCN Daisy Cruiser",    // 25
	"N64 Luigi Raceway",    // 26
	"N64 Kalamari Desert",  // 27
	"N64 Koopa Beach",      // 28
	"GBA Bowser Castle 1",  // 29
	"SNES Mario Circuit 2", // 30
	"SNES Rainbow Road",    // 31

	// Battle
	"Wuhu Town",            // 32
	"Honeybee Hive",        // 33
	"Sherbet Rink",         // 34
	"DS Palm Shore",        // 35
	"N64 Big Donut",        // 36
	"GBA Battle Course 1",  // 37
	"Gctr_WinningRun",      // 38

	"???",      // 39
	"???",      // 40
	"???",      // 41

	// CTGP-7
	"Concord Town", // 2A
	"SNES Mario Circuit 1", // 2B
	"Galvarny Falls", // 2C
	"GBA Sky Garden", // 2D
	"Autumn Forest", // 2E
	"N64 Choco Mountain", // 2F
	"DS Shroom Ridge", // 30
	"GBA Bowser Castle 3", // 31
	"Evergreen Crossing", // 32
	"CTR Crash Cove", // 33
	"Archipelago Avenue", // 34
	"N64 Frappe Snowland", // 35
	"N64 Moo Moo Farm", // 36
	"Banshee Boardwalk 2", // 37
	"CTR Cortex Castle", // 38
	"SNES Ghost Valley 2", // 39
	"Melody Sanctum", // 3A
	"N64 Mario Raceway", // 3B
	"Warp Pipe Island", // 3C
	"SNES Choco Island 2", // 3D
	"Elemental Cave", // 3E
	"DS Yoshi Falls", // 3F
	"Star Slope", // 40
	"DS Cheep Cheep Beach", // 41
	"DS Desert Hills", // 42
	"DS Tick Tock Clock", // 43
	"GBA Riverside Park", // 44
	"Castle Of Time", // 45
	"N64 Rainbow Road", // 46
	"GBA Rainbow Road", // 47
	"GCN Bowser Castle", // 48
	"Miku's Birthday Spectacular", // 49
	"Sandcastle Park", // 4A
	"DS Mario Circuit", // 4B
	"GCN Luigi Circuit", // 4C
	"Volcano Beach Ruins", // 4D
	"GCN Yoshi Circuit", // 4E
	"GBA Peach Circuit", // 4F
	"Metro Madness", // 50
	"GBA Luigi Circuit", // 51
	"SMO RC Challenge", // 52
	"GBA Bowser Castle 4", // 53
	"SNES Donut Plains 1", // 54
	"Secret Slide", // 55
	"DS Wario Stadium", // 56
	"Ermii Circuit", // 57
	"GCN Baby Park", // 58
	"Revo Circuit", // 59
	"SNES Mario Circuit 3", // 5A
	"Big Blue", // 5B
	"GBA Shy Guy Beach", // 5C
	"Bingo Party", // 5D
	"Doge Desert", // 5E
	"N64 Banshee Boardwalk", // 5F
	"GCN Mario Circuit", // 60
	"Rainbow Road DX", // 61
	"Stargaze Summit", // 62
	"Sunset Raceway", // 63
	"GBA Broken Pier", // 64
	"Glacier Mine", // 65
	"Flowerbed Fortress", // 66
	"Seaside Palace", // 67
	"DKR Star City", // 68
	"Mushroom Mountain", // 69
	"N64 Sherbet Land", // 6A
	"Block Island", // 6B
	"DS Bowser Castle", // 6C
	"DKR Jungle Falls", // 6D
	"Retro Raceway", // 6E
	"Frozen Grotto", // 6F
	"GBA Lakeside Park", // 70
	"Dragon Burial Grounds", // 71
	"RMX Rainbow Road 1", // 72
	"Neo Metropolis", // 73
	"Frosty Heights", // 74
	"Gnasty Gnorc's Lair", // 75
	"RMX Vanilla Lake 1", // 76
	"Cliffside Circuit", // 77
	"Interstellar Laboratory", // 78
	"Dark Matter Fortress", // 79
};

static const char* cup_name[] =
{
	"Mushroom Cup", // 0
	"Flower Cup",   // 1
	"Star Cup",     // 2
	"Special Cup",  // 3
	"Shell Cup",    // 4
	"Banana Cup",   // 5
	"Leaf Cup",     // 6
	"Lightning Cup", // 7
	"???", // 8
	"Bell Cup", // 9
	"Acorn Cup", // 10
};