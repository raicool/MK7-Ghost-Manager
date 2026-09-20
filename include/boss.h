#pragma once

#include "ghost.h"

struct BOSSRankingData
{
	bool ready = false;
	bool edited = false;

	std::fstream spotpass_data;
	std::string file_directory;

	uint8_t header_data[0x64];

	std::array<std::unique_ptr<Ghost>, 20> course_1;
	std::array<std::unique_ptr<Ghost>, 20> course_2;
	std::array<std::unique_ptr<Ghost>, 20> course_3;
	std::array<std::unique_ptr<Ghost>, 20> course_4;

	uint8_t ghost_count[4] = {0, 0, 0, 0};
	uint8_t cup_id = 0;

	std::array<std::unique_ptr<Ghost>, 20>* get_course(uint8_t index);

	uint8_t load(std::string dir);
	void save(bool prompt_file = true);

	uint8_t load_course_ghosts(std::array<std::unique_ptr<Ghost>, 20>& courses, size_t file_offset);
	bool parse_ghost(std::unique_ptr<Ghost>& ghost, const uint8_t* data);
	void reload();

	bool overwrite_ghost(std::unique_ptr<Ghost>& ghost, const char* ghost_dir);
	void extract_ghost(std::unique_ptr<Ghost>& _ghost);
	bool add_ghost(uint8_t course_index, const char* ghost_dir);
	void delete_ghost(uint8_t course_index, std::unique_ptr<Ghost>& _ghost);
};

void open_spotpass_file();
void open_spotpass_folder();