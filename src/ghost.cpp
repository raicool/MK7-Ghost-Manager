#include "pch.h"
#include "ghost.h"

#include "cfl.h"

bool Ghost::parse_data(const uint8_t* data)
{
	if (((uint32_t*)data)[0] != 0x43444744)
	{
		LOG_ERROR("parse_ghost() : invalid CDGD header magic");
		return false;
	}

	memcpy(&this->serialized, data, sizeof(GhostRawData));
	memcpy(this->kdpad_data, data + 0xC0, 0x27D8);

	this->course_id = this->serialized.course();
	this->character_id = this->serialized.character();
	this->kart_id = this->serialized.kart();
	this->tire_id = this->serialized.tire();
	this->glider_id = this->serialized.wing();

	char mii_name[0x14];
	memcpy(mii_name, data + 0x18, 0x14);
	this->player_name = utf16be(mii_name, 0x14).c_str();

	memcpy(&this->mii_data, data + 0x30, sizeof(CFLStoreData));

	this->country_id = this->serialized.country();
	return true;
}