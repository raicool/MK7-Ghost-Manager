#include "pch.h"
#include "ghost.h"

#include "cfl.h"

void Ghost::cpy_to_buffer(char* ghost_buffer)
{
	// set file header (DGDC)
	*(uint32_t*)&ghost_buffer[0] = 0x43444744;

	memcpy(ghost_buffer + 0x04, serialized.finished_time.data, sizeof(PackedTime));
	memcpy(ghost_buffer + 0x07, serialized.data, 0xb9);
	memcpy(ghost_buffer + 0xc0, kdpad_data, 0x27d8);

	uint32_t crc32 = crc32b((unsigned char*)ghost_buffer, GHOST_SIZE);

	*(uint32_t*)&ghost_buffer[GHOST_SIZE] = crc32;
}

void Ghost::save_mii(std::wstring path)
{
	const uint64_t system_id = mii_data.system_id;

	std::fstream mii_stream(path, std::ios::out | std::ios::binary);
	bin_write<CFLStoreData>(&mii_data, mii_stream, 0u, sizeof(CFLStoreData));
	mii_stream.close();
}

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