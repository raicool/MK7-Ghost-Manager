#include "pch.h"
#include "spotpass.h"

#include "common/common.h"

#define GHOST_SIZE 0x2898

uint8_t spotpass::load(const char* dir)
{
	uint32_t offset = 0;
	uint32_t u32buffer = 0;
	uint8_t  buffer = 0;

	file_directory = dir;

	if (spotpass_data.is_open()) spotpass_data.close();
	
	spotpass_data.open(dir, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	size_t file_size = spotpass_data.tellg();

	if (file_size != 0xCAFE4)
	{
		LOG_ERROR("load error : the given spotpass file was the incorrect size!");
		return -1;
	}

	// check if file exists
	if (!spotpass_data.is_open())
	{
		LOG_ERROR("load error : could not open \"{}\"", dir);
		return -1;
	}

	// delete all old ghost data and clear
	for (int i = 0; i < ghosts.size(); i++) if (ghosts[i]) delete ghosts[i];
	ghosts.clear();
	ghosts.reserve(80);
	ghost_count = 0;

	bin_read<uint8_t>(&cup_id, spotpass_data, 0x2f);

	for (int i = 0; i < 80; i++)
	{
		// each ghost inside of a spotpass file have a padding size of 0x2898
		// the maximum amount of ghosts inside of a spotpass file is 80
		offset = 0x64 + (GHOST_SIZE * i);

		if (verify_magic("DGDC", spotpass_data, sizeof(char) * 4, offset) != 0)
		{
			ghosts.emplace_back(new ghost());
			ghosts[ghost_count]->file_offset = offset;
			ghosts[ghost_count]->ghost_id = i;

			offset += 4;
			bin_read<uint32_t>(&u32buffer, spotpass_data, &offset);
			ghosts[ghost_count]->fp_flag      = (u32buffer >> 27) & 0x01;
			ghosts[ghost_count]->finished_ms  = (u32buffer >> 14) & 0x3ff;
			ghosts[ghost_count]->finished_sec = (u32buffer >> 7)  & 0x7f;
			ghosts[ghost_count]->finished_min = (u32buffer >> 0)  & 0x7f;

			offset += 12;
			// do some bit manipulation magic to get character and kart config
			bin_read<uint32_t>(&u32buffer, spotpass_data, offset);
			ghosts[ghost_count]->course_id    = (u32buffer >> 0)  & 0x3f;
			ghosts[ghost_count]->character_id = (u32buffer >> 6)  & 0x0f;
			ghosts[ghost_count]->kart_id      = (u32buffer >> 11) & 0x0f;
			ghosts[ghost_count]->tire_id      = (u32buffer >> 16) & 0x0f;
			ghosts[ghost_count]->glider_id    = (u32buffer >> 20) & 0x0f;

			offset += 4;
			char mii_name[0x14];
			bin_read<char>(mii_name, spotpass_data, offset, 0x14);
			ghosts[ghost_count]->player_name = utf16be(mii_name, 0x14).c_str();
			ghost_count++;
		}
	}

	return cup_id;
}

// replaces ghost data at a given offset with new data from replay file
void spotpass::overwrite_ghost(uint32_t offset, const char* ghost_dir)
{
	std::fstream ghost_data;
	ghost_data.open(ghost_dir, std::ios::in | std::ios::binary);

	if (!ghost_data.is_open()) return;

	for (int i = 0; i < GHOST_SIZE; i++)
	{
		char byte;
		bin_read<char>(&byte, ghost_data, i);
		bin_write<char>(&byte, spotpass_data, offset + i);
	}

	ghost_data.close();

	// reload ghost since its overwritten
	reload();
}

/*
*	deletes ghost data at offset and shifts all lower course ghosts up
*	mk7 reads ghosts for each courses at specific offsets, so its important not to have ghosts for other courses be moved into another course's data
*	the passed ghost object is deleted when this is called
*/
void spotpass::delete_ghost(ghost* _ghost)
{
	uint16_t next_course_ghost_id = round_multiple(_ghost->ghost_id, 20) + 20;
	uint32_t offset               = _ghost->file_offset;
	
	for (uint32_t i = _ghost->ghost_id; i < next_course_ghost_id; i++)
	{
		if (offset + GHOST_SIZE >= ((next_course_ghost_id * GHOST_SIZE) + 0x64)) break;

		bin_move(spotpass_data, offset + GHOST_SIZE, offset, GHOST_SIZE, true);
		offset += GHOST_SIZE;
	}

	// reload ghost since its overwritten
	reload();
}

bool spotpass::add_ghost(const char* ghost_dir)
{
	std::fstream ghost_data(ghost_dir,    std::ios::in | std::ios::binary);

	if (!ghost_data.is_open()) return false;

	uint8_t buffer = 0;

	uint32_t course_id;
	bin_read<uint32_t>(&course_id, ghost_data, 20);
	course_id &= 0x3f;

	if (course_id > 31) return false;

	uint32_t next_course_ghost_offset =  ((cup_course_index[course_id] * 20)       * GHOST_SIZE) + 0x64;
	uint32_t offset                   = (((cup_course_index[course_id] * 20) - 20) * GHOST_SIZE) + 0x64;

	for (uint32_t i = cup_course_index[course_id] * 15;; i++)
	{
		if (offset >= next_course_ghost_offset)
		{
			LOG_ERROR("ghost add error : there is no more room for a new ghost in this course, please delete ghosts to make space!");
			return false;
		}

		bin_read<uint8_t>(&buffer, spotpass_data, offset);

		if (!buffer)
		{
			// theres no helper function to copy data from one fstream to another yet, so im doing this manually
			char* ghost_buffer = new char[GHOST_SIZE];
			ghost_data.seekg(0);
			ghost_data.read(reinterpret_cast<char*>(ghost_buffer), GHOST_SIZE);
			spotpass_data.seekp(offset);
			spotpass_data.write(reinterpret_cast<const char*>(ghost_buffer), GHOST_SIZE);
			delete[] ghost_buffer;
			LOG_INFO("ghost added at 0x{:08x}", offset);
			break;
		}

		offset += GHOST_SIZE;
	}

	ghost_data.close();

	// reload ghost since its overwritten
	reload();
	return true;
}

void spotpass::reload()
{
	load(file_directory.c_str());
}
