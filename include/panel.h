#pragma once

struct Ghost;
struct SerializedFile;
struct ImGuiPanel
{
	std::shared_ptr<SerializedFile> current_file = nullptr;
	int32_t course_idx = 0;
	bool is_cup_selected = false;
	
	void boss_info();
	void boss_details();

	void archive_info();
	void archive_details();

	void render_hex_view(uint8_t* src, size_t size, uint16_t view_length = 17, uint16_t view_width = 16);

	// Draw text for currently loaded ranking file
	void ranking_directory_text();

	// Draw menu items for currently selected file
	void single_file_operations();

	// Draw ghost nation flag
	void nation_flag_image(char nation_id);

	// Draw ghost details like character icon, name, flag, etc
	void draw_ghost_details(std::unique_ptr<Ghost>& _ghost);

	// Draw mii image
	void mii_image(CFLStoreData* mii_data, uint16_t size = 64);

	void render();
};

