#pragma once

struct mii;
struct ghost
{
	uint32_t file_offset = 0; //< where ghost's data starts in spotpass file (if applicable)
	uint32_t ghost_id    = 0; //< where ghost's data starts in spotpass file (if applicable)

	std::basic_string<char16_t> player_name;

	bool fp_flag; //< true if ghost used first person 80% or more

	uint16_t finished_ms;  //< finished ghost time milliseconds
	uint16_t finished_sec; //< finished ghost time seconds
	uint16_t finished_min; //< finished ghost time minutes

	uint8_t course_id;

	uint8_t character_id;
	uint8_t kart_id;
	uint8_t tire_id;
	uint8_t glider_id;

	mii mii_data;
};