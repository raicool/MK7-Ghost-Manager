#pragma once

#include "ghost.h"
#include "file.h"

struct BOSSRankingData : SerializedFile
{
	uint8_t header_data[0x64];

	std::array<std::unique_ptr<Ghost>, 20> course_1;
	std::array<std::unique_ptr<Ghost>, 20> course_2;
	std::array<std::unique_ptr<Ghost>, 20> course_3;
	std::array<std::unique_ptr<Ghost>, 20> course_4;

	uint8_t ghost_count[4] = {0, 0, 0, 0};
	uint8_t cup_id = 0;

	std::array<std::unique_ptr<Ghost>, 20>* get_course(uint8_t index);

	void for_each_file(std::function<void(std::unique_ptr<Ghost>& ghost)> func)
	{
		for (auto& ghost : course_1)
		{
			if (ghost == nullptr) continue;
			func(ghost);
		}

		for (auto& ghost : course_2)
		{
			if (ghost == nullptr) continue;
			func(ghost);
		}

		for (auto& ghost : course_3)
		{
			if (ghost == nullptr) continue;
			func(ghost);
		}

		for (auto& ghost : course_4)
		{
			if (ghost == nullptr) continue;
			func(ghost);
		}
	}

	void load(std::string dir) override;
	void save(bool prompt_file = true) override;
	void reload() override;
	void close() override;

	uint8_t load_course_ghosts(std::array<std::unique_ptr<Ghost>, 20>& courses, size_t file_offset);
	bool overwrite_ghost(std::unique_ptr<Ghost>& ghost, const char* ghost_dir);
	void extract_ghost(std::unique_ptr<Ghost>& _ghost);
	bool add_ghost(uint8_t course_index, const char* ghost_dir);
	void delete_ghost(uint8_t course_index, std::unique_ptr<Ghost>& _ghost);

	void save_all_ghost_miis(const std::u16string directory_utf16);

	BOSSRankingData() { type = SerializedFileType::BOSS; };
};

void open_spotpass_file(const char* file_path);