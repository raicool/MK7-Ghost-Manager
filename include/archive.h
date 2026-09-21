#pragma once

#include "cfl.h"
#include "file.h"

enum GPRecord : uint32_t
{
	GP_RECORD_NO_RANK,
	GP_RECORD_A,
	GP_RECORD_B,
	GP_RECORD_C,
	GP_RECORD_1_STAR,
	GP_RECORD_2_STAR,
	GP_RECORD_3_STAR,
};

enum PlayerDataFlags : uint32_t
{
	COUNTRY_SET_TO_VISIBLE = 0x01,
	ONLINE_STATUS_VISIBLE = 0x02,
	GOLD_WHEEL_UNLOCKED = 0x04,
	RANK_3_STARS = 0x08,
	RANK_2_STARS = 0x10,
	RANK_1_STARS = 0x20,
};

#pragma pack(push, 1)
struct PlayerID
{
	uint64_t transterable_id;
	uint64_t system_tick;
};

struct GrandPrixData
{
	uint8_t data;
};

union union_1
{
	uint32_t flags;
	uint32_t vr;
	GPRecord gp_record;
};

struct PlayerData
{
	union_1 data;
	uint32_t wins;
	uint32_t losses;
	int32_t wins_opponent;
	int32_t losses_opponent;
	uint16_t driver_unlock_flags;
	uint16_t body_unlock_flags;
	uint16_t tire_unlock_flags;
	uint16_t wing_unlock_flags;
	uint32_t unknown_0x20;
	PlayerID player_id;
	int32_t my_principal_id;
	uint16_t country;
	uint16_t latitude;
	uint16_t longitude;
	uint8_t region;
	uint8_t flags;
	CFLStoreData mii;
	bool player_data_filled;
	uint8_t unknown_0x9d;
	uint8_t unknown_0x9e;
	uint8_t unknown_0x9f;
};
static_assert(sizeof(PlayerData) == 0xa0, "invalid size for struct PlayerData");

struct OpponentData
{
	uint32_t is_initialized; // checked as a bitflag
	uint32_t unknown_0x04;
	PlayerData player_data;
};
static_assert(sizeof(OpponentData) == 0xa8, "invalid size for struct OpponentData");

struct FlagData
{
	uint8_t unknown_0x00[0x358];
	uint32_t coins;
	uint8_t unknown_0x35c[0x1f4];
	uint32_t unknown_0x550;
	uint32_t streetpass_tags;
	uint32_t unknown_0x558[0x03];

	uint32_t wins;
	uint32_t losses;
	uint32_t vr_title_cup;

	uint16_t cup_fresh_unlock_flags;
	uint16_t driver_unlock_flags;
	uint16_t driver_fresh_unlock_flags;
	uint16_t driver_fresh_unlock_flags2;
	uint16_t body_unlock_flags;
	uint16_t body_fresh_unlock_flags;
	uint16_t tire_unlock_flags;
	uint16_t tire_fresh_unlock_flags;
	uint16_t wing_unlock_flags;
	uint16_t wing_fresh_unlock_flags;
	uint32_t sequence_open_flag;
	uint32_t unknown_0x588;
	uint8_t my_cec_cources;
	uint8_t unknown_0x58d[0x3];
	uint8_t gyro_percentage[0x64];
};
static_assert(sizeof(FlagData) == 0x5f4, "invalid size for struct FlagData");

struct RaceLogSaveData
{
	int32_t unknown_0x00;
	int32_t race_time;
	int32_t drift_time;
	int32_t race_time_in_first_place;
	uint32_t race_time_in_first_person;
	int16_t drift_start;
	uint16_t bump_into_enemy;
	int16_t start_drift_jump;
	uint16_t trick;
	int16_t miniturbo_level;
	int16_t respawns;
	int16_t num_enemies_hit_with_items;
	uint8_t rankup_while_flying;
	uint8_t rankup_while_underwater;
	uint16_t used_item_and_hit_someone;
	uint16_t throw_item;
	uint16_t unknown_0x28;
	uint16_t unknown_0x2a;
	uint16_t unknown_0x2c;
	uint16_t unknown_0x2e;
	uint8_t unknown_0x30;
	uint8_t unknown_0x31;
	uint16_t unknown_0x32;
};
static_assert(sizeof(RaceLogSaveData) == 0x34, "invalid size for struct RaceLogSaveData");

struct PlayerFlagSaveData
{
	FlagData flag_data;
	RaceLogSaveData race_log[0x0a];
	uint8_t unknown_0x7fc[0x04];
};
static_assert(sizeof(PlayerFlagSaveData) == 0x800, "invalid size for struct PlayerFlagSaveData");

struct SystemSaveArchive
{
	uint32_t magic;
	uint16_t unknown_0x04;
	uint16_t unknown_0x06;
	CFLStoreData mii;
	PlayerID id;
	uint8_t cec_comment[0x22];

	uint8_t padding[0x5e];

	int32_t last_upload_player_log_time; // unix timestamp
	uint32_t unknown_0xfc;
	uint32_t unknown_0x100;
	uint32_t ghost_update_bit_flag;
	uint32_t unknown_0x108;

	uint8_t ranking_data[0x600];
	GrandPrixData gp_data[32];

	uint8_t unknown_0x72c;
	uint8_t unknown_0x72d;
	uint8_t unknown_0x72e;
	uint8_t unknown_0x72f;

	OpponentData opponents[100];
	PlayerFlagSaveData flags;

	uint32_t crc32;
};
static_assert(sizeof(SystemSaveArchive) == 0x50d4, "invalid size for struct SystemSaveArchive");
#pragma pack(pop)

struct SaveArchiveFile : SerializedFile
{
	std::unique_ptr<SystemSaveArchive> serialized;

	void load(std::string dir) override;
	void save(bool prompt_file = true) override;
	void reload() override;
	void close() override;

	SaveArchiveFile() { type = SerializedFileType::ARCHIVE; };
};

void open_archive_file(const char* file_path);