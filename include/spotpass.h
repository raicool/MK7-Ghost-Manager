#pragma once

#include "ghost.h"

struct spotpass
{
	bool ready = false;

	std::fstream spotpass_data;
	std::string file_directory;

	uint8_t header_data[0x64];

	std::array<std::unique_ptr<ghost>, 20> course_1;
	std::array<std::unique_ptr<ghost>, 20> course_2;
	std::array<std::unique_ptr<ghost>, 20> course_3;
	std::array<std::unique_ptr<ghost>, 20> course_4;

	uint32_t ghost_count = 0;
	uint8_t cup_id = 0;

	std::array<std::unique_ptr<ghost>, 20>* get_course(uint8_t index);

	uint8_t load(std::string dir);
	void save();

	void load_course_ghosts(std::array<std::unique_ptr<ghost>, 20>& courses, size_t file_offset);
	void parse_ghost(std::unique_ptr<ghost>& ghost, const uint8_t* data);
	void reload();

	void overwrite_ghost(std::unique_ptr<ghost>& ghost, const char* ghost_dir);
	void delete_ghost(std::unique_ptr<ghost>& _ghost);
	void extract_ghost(std::unique_ptr<ghost>& _ghost);
	bool add_ghost(uint8_t course_index, const char* ghost_dir);
};

void open_spotpass_file();
void open_spotpass_folder();