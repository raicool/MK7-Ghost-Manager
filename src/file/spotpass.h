#pragma once

#include "mii.h"
#include "ghost.h"

struct spotpass
{
	std::vector<ghost*> ghosts;

	std::fstream spotpass_data;
	std::string file_directory;

	uint32_t ghost_count = 0;
	uint8_t cup_id;

	uint8_t load(const char* dir);
	void reload();

	void overwrite_ghost(uint32_t offset, const char* ghost_directory);
	void delete_ghost(ghost* _ghost);
	void extract_ghost(ghost* _ghost);
	bool add_ghost(const char* ghost_directory);
};