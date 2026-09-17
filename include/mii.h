#pragma once

enum character_set_t
{
	JPN_USA_EUR,
	CHN,
	KOR,
	TWN,
};

enum console_t
{
	WII,
	DS,
	N3DS,
	WIIU_SWITCH,
};

#pragma pack(push, 1)
// https://www.3dbrew.org/wiki/Mii#Mii_format
struct mii
{
	// usually set to 3
	uint8_t version;
	uint8_t flags;
	// position for the mii in Mii Maker app
	uint8_t maker_position;

	// (1=Wii, 2=DS, 3=3DS, 4=Wii U/Switch)
	uint8_t device;

	// System ID (identifies owner, for purpose of enforcing editing restrictions and blue pants).
	// Is not tied to the MAC address anymore.
	uint64_t system_id;

	// Mii ID(big - endian 32bit unsigned integer) :
	// Bit 0..27 : (bit[0..27] * 2) = date of creation(seconds since 01 / 01 / 2010 00:00 : 00)
	// Bit 28 : Always set ?
	// Bit 29 : set for temporary Mii
	// Bit 30 : Set for DSi mii ?
	// Bit 31 : not set if Mii is special
	uint32_t mii_id;

	// device mac address of mii's creator
	uint8_t creator_mac[0x6];

	uint16_t padding_0x16;

	// bit 0: sex (0 if male, 1 if female)
	// bit 1-4: birthday month
	// bit 5-9: birthday day
	// bit 10-13: favorite color
	// bit 14: favorite mii (0 if false, 1 if true)
	uint16_t misc_detail_0;

	// mii's name, null terminated UTF-16 string
	uint8_t name_utf16[0x14];

	uint8_t weight;
	uint8_t height;

	uint8_t detail[0x18];

	uint8_t author_name_utf16[0x14];

	uint16_t padding_0x5c;
	uint16_t crc16;

};
static_assert(sizeof(mii) == 0x60, "invalid size for mii struct");
#pragma pack(pop)