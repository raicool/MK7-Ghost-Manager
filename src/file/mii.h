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
	char data[0x60];
};