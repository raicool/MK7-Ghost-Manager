#include "pch.h"
#include "spotpass.h"

#include "common/common.h"

#define GHOST_SIZE 0x2898

std::vector<std::shared_ptr<spotpass>> g_spotpass_files;

void open_spotpass_file()
{
	const char* file_path = open_file();

	std::shared_ptr<spotpass> _spdata = std::make_shared<spotpass>();
	_spdata->load(file_path);

	if (_spdata->cup_id != -1)
	{
		LOG_DEBUG("Cup = {}, file = {}", _spdata->cup_id, file_path);

		g_spotpass_files.emplace_back(_spdata);
	}
}

void open_spotpass_folder()
{
	g_spotpass_files.clear();

	const char* folder_dir = open_folder();
	if (!folder_dir) return;

	for (const std::filesystem::directory_entry& file : std::filesystem::directory_iterator(folder_dir))
	{
		auto file_path = file.path().string();

		std::shared_ptr<spotpass> _spdata = std::make_shared<spotpass>();
		_spdata->load(file_path);

		if (_spdata->cup_id != -1)
		{
			LOG_DEBUG("Cup = {}, file = {}", _spdata->cup_id, file_path);

			g_spotpass_files.emplace_back(_spdata);
		}
	}
}

std::vector<std::shared_ptr<spotpass>> get_spotpass_files()
{
	return g_spotpass_files;
}

course_ghosts_array_t* spotpass::get_course(uint8_t index)
{
	
	switch (index)
	{
	case 0: return &this->course_1;
	case 1: return &this->course_2;
	case 2: return &this->course_3;
	case 3: return &this->course_4;
	default: return nullptr;
	}
}

uint8_t spotpass::load(std::string dir)
{
	uint32_t offset = 0;
	uint32_t u32buffer = 0;

	file_directory = dir;

	if (spotpass_data.is_open())
	{
		spotpass_data.close();
	}
	
	spotpass_data.open(dir, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
	size_t file_size = spotpass_data.tellg();

	if (file_size != 0xCAFE4)
	{
		LOG_ERROR("load error : the given spotpass file was the incorrect size!");
		spotpass_data.close();
		return -1;
	}

	if (!spotpass_data.is_open())
	{
		LOG_ERROR("load error : could not open \"{}\"", dir);
		return -1;
	}


	bin_read<uint8_t>(&cup_id, spotpass_data, 0x2f);
	bin_read<uint8_t>(header_data, spotpass_data, (uint32_t)0, 0x64);
	ghost_count = 0;

	this->load_course_ghosts(course_1, 0x64);
	this->load_course_ghosts(course_2, 0x32c44);
	this->load_course_ghosts(course_3, 0x65824);
	this->load_course_ghosts(course_4, 0x98404);

	ready = true;
	return cup_id;
}

void spotpass::load_course_ghosts(course_ghosts_array_t& ghosts, size_t file_offset)
{
	for (int i = 0; i < 20; i++)
	{
		// each ghost inside of a spotpass file have a padding size of 0x2898
		// the maximum amount of ghosts inside of a course is 20
		uint32_t offset = file_offset + (GHOST_SIZE * i);

		if (verify_magic("DGDC", spotpass_data, sizeof(char) * 4, offset) == false)
			continue; // invalid ghost header, skip

		auto _ghost = std::make_unique<ghost>();

		_ghost->file_offset = offset;
		_ghost->ghost_id = i;

		{
			uint8_t* __ghost_data_buffer = new uint8_t[GHOST_SIZE];

			bin_read<uint8_t>(__ghost_data_buffer, spotpass_data, offset, GHOST_SIZE);

			this->parse_ghost(_ghost, __ghost_data_buffer);

			delete[] __ghost_data_buffer;
		}

		ghosts[i] = std::move(_ghost);
		ghost_count++;
	}
}

void spotpass::parse_ghost(std::unique_ptr<ghost>& ghost, const uint8_t* data)
{
	// 0x04 (7 bits) -> finshed time (minutes)
		// 0x04.7 (7 bits) -> finshed time (seconds)
		// 0x05.6 (10 bits) -> finshed time (milliseconds)
	memcpy(&ghost->serialized, data, sizeof(raw_ghost));
	memcpy(ghost->kdpad_data, data + 0xC0, 0x27D8);

	uint32_t u32buffer = *(uint32_t*)(data + 0x14);
	ghost->course_id = (u32buffer >> 0) & 0x3f; // 7 bit
	ghost->character_id = (u32buffer >> 6) & 0x1f; // 5 bit
	ghost->kart_id = (u32buffer >> 11) & 0x1f; // 5 bit
	ghost->tire_id = (u32buffer >> 16) & 0x0f; // 4 bit
	ghost->glider_id = (u32buffer >> 20) & 0x0f; // 4 bit

	char mii_name[0x14];
	memcpy(mii_name, data + 0x18, 0x14);
	ghost->player_name = utf16be(mii_name, 0x14).c_str();

	memcpy(&ghost->mii_data, data + 0x30, sizeof(mii));

	ghost->country_id = data[0x7c];
}

// replaces ghost data at a given offset with new data from replay file
void spotpass::overwrite_ghost(uint32_t offset, const char* ghost_dir)
{
	std::fstream ghost_file;
	ghost_file.open(ghost_dir, std::ios::in | std::ios::binary);

	if (!ghost_file.is_open()) return;
	char* ghost_data = new char(GHOST_SIZE);

	bin_read(ghost_data, ghost_file, 0u, GHOST_SIZE);
	bin_write(&ghost_data, spotpass_data, offset, GHOST_SIZE);

	ghost_file.close();

	delete ghost_data;

	// reload ghost since its overwritten
	reload();
}

/*
*	deletes ghost data at offset and shifts all lower course ghosts up
*	mk7 reads ghosts for each courses at specific offsets, so its important not to have ghosts for other courses be moved into another course's data
*	the passed ghost object is deleted when this is called
*/
void spotpass::delete_ghost(std::unique_ptr<ghost>& _ghost)
{
	uint16_t next_course = round_multiple(_ghost->ghost_id, 20) + 20;
	uint32_t offset = _ghost->file_offset;
	
	for (uint32_t i = _ghost->ghost_id; i < next_course; i++)
	{
		if (offset + GHOST_SIZE >= ((next_course * GHOST_SIZE) + 0x64)) break;

		bin_move(spotpass_data, offset + GHOST_SIZE, offset, GHOST_SIZE, true);
		offset += GHOST_SIZE;
	}

	// reload ghost since its overwritten
	reload();
}

/*
*	extract a ghost from a spotpass file
*	also adds crc-32 checksum to end of file
*/
void spotpass::extract_ghost(std::unique_ptr<ghost>& _ghost)
{
	char* file_name = new char[13];
	char* ghost_buffer = new char[GHOST_SIZE];
	const char* replay_dir;
	uint32_t offset = 0;

	snprintf(file_name, 13, "replay%02i.dat", _ghost->course_id);
	replay_dir = create_file(file_name);

	std::fstream replay(replay_dir, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!replay.is_open())
	{
		LOG_ERROR("ghost extract error : could not access file {}", replay_dir);
		return;
	}
	
	bin_read(ghost_buffer, spotpass_data, _ghost->file_offset, GHOST_SIZE);
	uint32_t crc32 = crc32b((unsigned char*)ghost_buffer, GHOST_SIZE);

	bin_write(ghost_buffer, replay, &offset, GHOST_SIZE);
	bin_write(&crc32, replay, offset);

	replay.close();

	delete[] file_name;
	delete[] ghost_buffer;
}

/*
*	adds a ghost to a spotpass file
*	returns true if there is room for the ghost, false if there is no room
*/
bool spotpass::add_ghost(const char* ghost_dir)
{
	std::fstream ghost_data(ghost_dir, std::ios::in | std::ios::binary);

	if (!ghost_data.is_open()) return false;

	uint8_t buffer = 0;

	uint32_t course_id;
	bin_read<uint32_t>(&course_id, ghost_data, 20);
	course_id &= 0x3f;

	if (course_id > 0x79) return false;

	uint32_t next_course_offset =  cup_course_index[course_id] * 20       * GHOST_SIZE + 0x64;
	uint32_t offset             = (cup_course_index[course_id] * 20 - 20) * GHOST_SIZE + 0x64;

	for (uint32_t i = cup_course_index[course_id] * 15;; i++)
	{
		if (offset >= next_course_offset)
		{
			LOG_ERROR("ghost add error : there is no more room for a new ghost in this course, please delete ghosts to make space!");
			ghost_data.close();
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
	load(file_directory);
}