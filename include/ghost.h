#pragma once

#include "cfl.h"
#include "common/utils.h"

#define GHOST_SIZE 0x2898

enum KCPFlag
{
	UNKNOWN = 0x00,
	SECTION = 0x01,
	LAP = 0x03,
};

#pragma pack(push, 1)
struct PackedTime
{
// 	uint8_t min : 7;
// 	uint8_t sec : 7;
// 	uint8_t ms : 10;

	uint8_t data[0x03];

	inline uint8_t min() const
	{
		return data[0] & 0x7f;
	}

	inline uint8_t sec() const
	{
		uint8_t lo = ((data[0] & 0x80) >> 7);
		uint8_t hi = ((data[1] & 0x3f) << 1);
		return (lo | hi) & 0x7f;
	}

	inline uint16_t ms() const
	{
		return ((data[1] & 0xc0) >> 6) | ((data[2] << 2) & 0x3ff);
	}
};
static_assert(sizeof(PackedTime) == 0x03, "invalid size of PackedTime");

struct GhostRawData
{
	uint32_t magic : 32;

	PackedTime finished_time;
	
	uint8_t data[0xb9];

	inline uint8_t kcp() const { return data[0] & 0x07; }
	inline bool gyro_flag() const { return (data[0] >> 3) & 0x01; }

	// would loved to have made these all bitfields but we cant always have nice things
	
	inline uint8_t lap1_min() const 
	{ 
		return data[1] & 0x7f; 
	}

	inline uint8_t lap1_sec() const 
	{ 
		return (data[1] >> 7) & 0x1 | (data[2] << 1) & 0x3f; 
	}

	inline uint16_t lap1_ms() const 
	{ 
		return (data[2] >> 6) & 0x03 | (data[3] << 2) & 0x3fc;
	}

	inline uint8_t lap2_min() const
	{
		return (data[4] >> 1) & 0x7f;
	}

	inline uint8_t lap2_sec() const
	{
		return data[5] & 0x7f;
	}

	inline uint16_t lap2_ms() const
	{
		return (data[5] >> 7) & 0x01 | (data[6] << 1) & 0x3fe;
	}

	inline uint8_t lap3_min() const
	{
		return (data[7] >> 1) & 0x7f;
	}

	inline uint8_t lap3_sec() const
	{
		return data[8] & 0x7f;
	}

	inline uint16_t lap3_ms() const
	{
		return (data[9] & 0xff) | (data[10] & 0xff << 8);
	}

	inline uint8_t course() const
	{
		return data[13] & 0x3f;
	}

	inline uint8_t character() const
	{
		return (data[13] >> 6) & 0x3 | (data[14] << 2) & 0xf;
	}

	inline uint8_t kart() const
	{
		return (data[14] >> 3) & 0x1f;
	}

	inline uint8_t tire() const
	{
		return data[15] & 0x0f;
	}

	inline uint8_t wing() const
	{
		return (data[15] >> 4) & 0x0f;
	}

	inline uint16_t country() const
	{
		return data[0x89] | (data[0x8a] << 8);
	}
};
static_assert(sizeof(GhostRawData) == 0xc0, "invalid size of GhostRawData");
#pragma pack(pop)

struct Ghost
{
	uint32_t file_offset = 0;
	uint32_t ghost_id = 0;

	std::basic_string<char16_t> player_name;

	GhostRawData serialized;
	CFLStoreData mii_data;

	uint8_t country_id;
	uint8_t course_id;
	uint8_t character_id;
	uint8_t kart_id;
	uint8_t tire_id;
	uint8_t glider_id;

	// Ghost's raw input data, padding included
	uint8_t kdpad_data[0x27d8];

	// Assumes ghost_buffer is at least 0x2898 bytes
	void cpy_to_buffer(char* ghost_buffer)
	{
		// set file header (DGDC)
		*(uint32_t*)&ghost_buffer[0] = 0x43444744;

		memcpy(ghost_buffer + 0x04, serialized.finished_time.data, sizeof(PackedTime));
		memcpy(ghost_buffer + 0x07, serialized.data, 0xb9);
		memcpy(ghost_buffer + 0xc0, kdpad_data, 0x27d8);

		uint32_t crc32 = crc32b((unsigned char*)ghost_buffer, GHOST_SIZE);

		*(uint32_t*)&ghost_buffer[GHOST_SIZE] = crc32;
	}

	void save_mii(std::wstring path)
	{
		const uint64_t system_id = mii_data.system_id;

		std::fstream mii_stream(path, std::ios::out | std::ios::binary);

		bin_write<CFLStoreData>(&mii_data, mii_stream, 0u, sizeof(CFLStoreData));

		mii_stream.close();
	}
};