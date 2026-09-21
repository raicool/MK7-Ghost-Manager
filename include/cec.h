#pragma once

#include "common/type.h"

/*
	CECArchiveData - Streetpass Extdata

	Offsets:
		0x00000000: Favorite Communities

		0x00002800: Recommended Communities
*/

enum CurrentCommunityGameMode : uint8_t
{
	SPDCLASS_50,
	SPDCLASS_100,
	SPDCLASS_150,
	MIRROR,
	BALLOON_BATTLE,
	COIN_RUNNERS
};

enum CommunityItemMode : uint8_t
{
	ALL,
	SHELL,
	BANANA,
	MUSHROOMS,
	BOMB,
	NONE
};

enum CfgRegionCode : uint8_t
{
	CFG_REGION_JAPAN,
	CFG_REGION_AMERICA,
	CFG_REGION_EUROPE,
	CFG_REGION_AUSTRALIA,
	CFG_REGION_CHINA,
	CFG_REGION_KOREA,
	CFG_REGION_MAX,
};

struct CECArchiveData
{

};

#pragma pack(push, 1)
struct CommunityExtDataInfo
{
	uint32_t gathering_id;
	uint16_t name[16]; // utf16-big endian
	uint16_t description[16]; // utf16-big endian
	CurrentCommunityGameMode game_mode;
	uint8_t unknown_0x45;
	uint8_t unknown_0x46;
	CommunityItemMode item_mode;
	CfgRegionCode region;
	uint8_t icon;
	uint8_t unknown_0x4a;
	bool is_my_community;
};
static_assert(sizeof(CommunityExtDataInfo) == 0x4c, "invalid size for struct CurrentCommunityGameMode");

struct CommunityPlayerRankingInfo
{
	bool is_valid;
	uint8_t unknown_0x01;
	uint8_t unknown_0x02;
	uint8_t unknown_0x03;
	uint32_t points;
	CFLStoreData mii;
	uint16_t name[11];
	uint16_t country;
	uint32_t my_principal_id;
};
static_assert(sizeof(CommunityPlayerRankingInfo) == 0x84, "invalid size for struct CommunityPlayerRankingInfo")
#pragma pack(pop)