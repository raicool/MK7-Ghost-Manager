#pragma once

struct ghost;
struct panel
struct BOSSRankingData;
struct ImGuiPanel
{
	std::shared_ptr<BOSSRankingData> current_file = nullptr;
	int32_t course_idx = 0;
	bool is_cup_selected = false;
	bool display_flags   = true;
	
	// Draw text for currently loaded ranking file
	void ranking_directory_text();

	// Draw ghost nation flag
	void nation_flag_image(char nation_id);

	// Draw ghost details like character icon, name, flag, etc
	void draw_ghost_details(std::unique_ptr<Ghost>& _ghost);

	// Draw mii image
	void mii_image(CFLStoreData* mii_data, uint16_t size = 64);

	void render();
};

