#pragma once

// 1 - 4
// stores where each course is inside of cup file
static const uint8_t cup_course_index[] =
{
	
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

	2, // 0
	4, // 1
	3, // 2
	2, // 3

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
	4  // 31
};

// course id for every course in each cup
static const uint8_t cup_courses[8][4] =
{
	{  4,  3,  2,  5 }, // Mushroom Cup
	{  8,  0, 15,  1 }, // Flower Cup
	{ 12, 14,  6,  9 }, // Star Cup
	{  7, 10, 11, 13 }, // Special Cup
	{ 26, 29, 19, 20 }, // Shell Cup
	{ 28, 30, 16, 23 }, // Banana Cup
	{ 27, 22, 25, 18 }, // Leaf Cup
	{ 17, 24, 21, 31 }  // Lightning Cup
};

static const char* course_name[] = 
{
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
	"Wuhu Town",            // 32
	"Honeybee Hive",        // 33
	"Sherbet Rink",         // 34
	"DS Palm Shore",        // 35
	"N64 Big Donut",        // 36
	"GBA Battle Course 1",  // 37
	"Gctr_WinningRun",      // 
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
	"Lightning Cup" // 7
};