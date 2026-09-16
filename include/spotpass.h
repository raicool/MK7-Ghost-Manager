#pragma once

#include "ghost.h"

struct spotpass
{
	bool ready = false;

	std::vector<ghost*> ghosts;

	std::fstream spotpass_data;
	std::string file_directory;

	uint32_t ghost_count = 0;
	uint8_t cup_id = 0;

	uint8_t load(std::string dir);
	void reload();

	void overwrite_ghost(uint32_t offset, const char* ghost_directory);
	void delete_ghost(ghost* _ghost);
	void extract_ghost(ghost* _ghost);
	bool add_ghost(const char* ghost_directory);
};

using spdata_vector = std::vector<std::shared_ptr<spotpass>>;

void open_spotpass_file();
void open_spotpass_folder();