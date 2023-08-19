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

struct mii
{
	char version : 4; // always 3

	bool copy           : 1;
	bool name_profanity : 1;
	char region         : 2;
	char character_set  : 2;
	char : 0;

	char page_idx : 2;
	char slot_idx : 2;
	char : 0;

	char console : 3;
	char : 0;

	unsigned long sys_id : 8;

	char data[0x3c];
};