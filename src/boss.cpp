#include "pch.h"
#include "boss.h"

#include "common/common.h"

std::vector<std::shared_ptr<BOSSRankingData>> g_spotpass_files;

void open_spotpass_file()
{
	const char* file_path = open_file();

	std::shared_ptr<BOSSRankingData> _spdata = std::make_shared<BOSSRankingData>();
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

		std::shared_ptr<BOSSRankingData> _spdata = std::make_shared<BOSSRankingData>();
		_spdata->load(file_path);

		if (_spdata->cup_id != -1)
		{
			LOG_DEBUG("Cup = {}, file = {}", _spdata->cup_id, file_path);

			g_spotpass_files.emplace_back(_spdata);
		}
	}
}

std::vector<std::shared_ptr<BOSSRankingData>> get_spotpass_files()
{
	return g_spotpass_files;
}

std::array<std::unique_ptr<Ghost>, 20>* BOSSRankingData::get_course(uint8_t index)
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

uint8_t BOSSRankingData::load(std::string dir)
{
	uint32_t offset = 0;
	uint32_t u32buffer = 0;

	file_directory = dir;

	if (spotpass_data.is_open())
	{
		spotpass_data.close();
	}
	
	spotpass_data.open(dir, std::ios::in | std::ios::binary | std::ios::ate);
	size_t file_size = spotpass_data.tellg();

	if (file_size != 0xcafe4)
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

	ghost_count[0] = this->load_course_ghosts(course_1, 0x64);
	ghost_count[1] = this->load_course_ghosts(course_2, 0x32c44);
	ghost_count[2] = this->load_course_ghosts(course_3, 0x65824);
	ghost_count[3] = this->load_course_ghosts(course_4, 0x98404);

	spotpass_data.close();

	ready = true;
	return cup_id;
}

void BOSSRankingData::save(bool prompt_file)
{
	char* _spotpass_buffer = new char[0xcafe4];

#ifndef NDEBUG
	// set values of buffer to 0x00 instead of debug placeholder 0xcd

	memset(_spotpass_buffer, 0, 0xcafe4);
#endif

	memcpy(_spotpass_buffer, this->header_data, 0x64);

	uint8_t _file_ghost_index = 0;
	for (int i = 0; i < 4; i++)
	{
		const std::array<std::unique_ptr<Ghost>, 20>* course = this->get_course(i);

		for (auto&& ghost : *course)
		{	
			_file_ghost_index++;

			if (ghost == nullptr)
			{
				continue;
			}

			size_t offset = 0x64 + (GHOST_SIZE * (_file_ghost_index - 1));

			ghost->cpy_to_buffer(_spotpass_buffer + offset);
		}
	}

	auto file = prompt_file ? create_file(nullptr, "All\0*.*\0") : this->file_directory.c_str();

	if (file)
	{
		std::fstream __new_spotpass_stream(file, std::ios::out | std::ios::binary | std::ios::trunc);

		bin_write(_spotpass_buffer, __new_spotpass_stream, 0u, 0xcafe4);

		__new_spotpass_stream.close();
	}

	delete[] _spotpass_buffer;

	edited = false;
}

uint8_t BOSSRankingData::load_course_ghosts(std::array<std::unique_ptr<Ghost>, 20>& ghosts, size_t file_offset)
{
	uint8_t _ghost_count = 0;

	for (int i = 0; i < 20; i++)
	{
		// each ghost inside of a spotpass file have a padding size of 0x2898
		// the maximum amount of ghosts inside of a course is 20
		uint32_t offset = file_offset + (GHOST_SIZE * i);

		if (verify_magic("DGDC", spotpass_data, sizeof(char) * 4, offset) == false)
			continue; // invalid ghost header, skip

		auto _ghost = std::make_unique<Ghost>();

		_ghost->file_offset = offset;
		_ghost->ghost_id = _ghost_count;

		{
			uint8_t* __ghost_data_buffer = new uint8_t[GHOST_SIZE];

			bin_read<uint8_t>(__ghost_data_buffer, spotpass_data, offset, GHOST_SIZE);

			_ghost->
			this->parse_ghost(_ghost, __ghost_data_buffer);

			delete[] __ghost_data_buffer;
		}

		ghosts[i] = std::move(_ghost);
		_ghost_count++;
	}

	return _ghost_count;
}

// replaces ghost data at a given offset with new data from replay file
bool BOSSRankingData::overwrite_ghost(std::unique_ptr<Ghost>& ghost, const char* ghost_dir)
{
	std::fstream ghost_file;
	ghost_file.open(ghost_dir, std::ios::in | std::ios::binary);

	if (!ghost_file.is_open())
	{
		LOG_ERROR("overwrite_ghost() : could not open ghost file");
		return false;
	}

	uint8_t* ghost_data = new uint8_t[GHOST_SIZE];
	bin_read(ghost_data, ghost_file, 0u, GHOST_SIZE);
	ghost_file.close();

	if (this->parse_ghost(ghost, ghost_data) == false)
	{
		delete[] ghost_data;
		return false;
	}
	else
	{
		delete[] ghost_data;
		edited = true;
		return true;
	}
}

void BOSSRankingData::delete_ghost(uint8_t course_index, std::unique_ptr<Ghost>& _ghost)
{
	if (course_index >= 4)
	{
		LOG_ERROR("delete_ghost() : invalid course index passed");

#ifndef NDEBUG
		assert(false);
#endif

		return;
	}

	_ghost.release();

	ghost_count[course_index]--;

	edited = true;
}

/*
*	extract a ghost from a spotpass file
*	also adds crc-32 checksum to end of file
*/
void BOSSRankingData::extract_ghost(std::unique_ptr<Ghost>& _ghost)
{
	char* file_name = new char[13];
	char* ghost_buffer = new char[GHOST_SIZE + 4];
	const char* replay_dir;
	uint32_t offset = 0;

	snprintf(file_name, 13, "replay%02i.dat", _ghost->course_id);
	replay_dir = create_file(file_name);

	delete[] file_name;

	std::fstream replay(replay_dir, std::ios::out | std::ios::binary | std::ios::trunc);

	if (!replay.is_open())
	{
		LOG_ERROR("extract_ghost() : could not access file {}", replay_dir);
		return;
	}

	_ghost->cpy_to_buffer(ghost_buffer);

	bin_write<char>(ghost_buffer, replay, 0u, GHOST_SIZE + 4);

	replay.close();

	delete[] ghost_buffer;

	edited = true;
}

/*
*	adds a ghost to a spotpass file
*	returns true if there is room for the ghost, false if there is no room
*/
bool BOSSRankingData::add_ghost(uint8_t course_index, const char* ghost_dir)
{
	if (ghost_dir == nullptr)
	{
		LOG_ERROR("add_ghost() : invalid directory passed");
		return false;
	}

	std::array<std::unique_ptr<Ghost>, 20>* course = this->get_course(course_index);

	if (course == nullptr)
	{
		LOG_ERROR("add_ghost() : invalid course index passed");

#ifndef NDEBUG
		assert(false);
#endif

		return false;
	}

	for (auto&& it = course->begin(); it != course->end(); it++)
	{
		if (*it == false)
		{
			*it = std::make_unique<Ghost>();
			if (overwrite_ghost(*it, ghost_dir) == false)
			{
				LOG_ERROR("add_ghost() : error occured while trying to overwrite ghost data");
				return false;
			}
			
			ghost_count[course_index]++;
			return true;

			edited = true;
		}
	}

	LOG_ERROR("add_ghost() : course has maximum number of ghosts added!");
	return false;
}

void BOSSRankingData::save_all_ghost_miis(const std::u16string directory_utf16)
{
	wchar_t filename[512];

	for_each_file([&filename, directory_utf16](std::unique_ptr<Ghost>& ghost)
		{
			const uint64_t system_id = ghost->mii_data.system_id;

			swprintf(filename, L"%s/%016llx (%s).cfsd", (char*)directory_utf16.c_str(), system_id, (char*)ghost->player_name.c_str());

			ghost->save_mii(filename);
		}
	);
}

void BOSSRankingData::reload()
{
	edited = false;

	this->for_each_file([](std::unique_ptr<Ghost>& ghost)
		{
			ghost.release();
		}
	);

	load(file_directory);
}