#pragma once

#include "mii.h"

// 3 byte bitfield
#pragma pack(push, 0)
struct packed_time
{
	unsigned min : 7;
	unsigned sec : 7;
	unsigned ms  : 10;
};

enum kcp
{
	UNKNOWN = 0X00,
	SECTION = 0x01,
	LAP = 0x03,
};

struct raw_ghost
{
	char magic[0x4];
	packed_time finished_time;
	unsigned padding_0x8 : 4;
	bool gyro_flag : 1;
	uint8_t kcp_mode : 3;

	packed_time lap1_time;
	packed_time lap2_time;
	packed_time lap3_time;
	unsigned wing : 4;
	unsigned tire : 4;
	unsigned kart : 5;
	unsigned character : 5;
	unsigned course : 6;
	char name_utf16be[0x14];
	char padding_0x00[0x18];
};
#pragma pack(pop)

struct ghost
{
	uint32_t file_offset = 0; //< where ghost's data starts in spotpass file (if applicable)
	uint32_t ghost_id = 0; //< where ghost's data starts in spotpass file (if applicable)

	std::basic_string<char16_t> player_name;

	raw_ghost serialized;
	
	uint8_t country_id;

	uint8_t course_id;

	uint8_t character_id;
	uint8_t kart_id;
	uint8_t tire_id;
	uint8_t glider_id;

	mii mii_data;
};