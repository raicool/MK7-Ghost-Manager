#pragma once

#include "ghost.h"

using course_ghosts_array_t = std::array<std::unique_ptr<ghost>, 20>;

struct spotpass
{
	bool ready = false;

	std::fstream spotpass_data;
	std::string file_directory;

	uint8_t header_data[0x64];

	course_ghosts_array_t course_1;
	course_ghosts_array_t course_2;
	course_ghosts_array_t course_3;
	course_ghosts_array_t course_4;

	uint32_t ghost_count = 0;
	uint8_t cup_id = 0;

	course_ghosts_array_t* get_course(uint8_t index);

	uint8_t load(std::string dir);
	void load_course_ghosts(course_ghosts_array_t& courses, size_t file_offset);
	void parse_ghost(std::unique_ptr<ghost>& ghost, const uint8_t* data);
	void reload();

	void overwrite_ghost(uint32_t offset, const char* ghost_directory);
	void delete_ghost(std::unique_ptr<ghost>& _ghost);
	void extract_ghost(std::unique_ptr<ghost>& _ghost);
	bool add_ghost(const char* ghost_directory);
};

void open_spotpass_file();
void open_spotpass_folder();