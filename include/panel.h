#pragma once

struct ghost;
struct panel
struct BOSSRankingData;
{
	std::shared_ptr<BOSSRankingData> current_file = nullptr;
	int32_t course_idx = 0;
	bool is_cup_selected = false;
	bool display_flags   = true;
	
	void draw_ghost_details(std::unique_ptr<ghost>& _ghost);
	void render();
};

void render_mii_image(mii* mii_data, uint16_t size = 64);
void draw_flag(char nation_id);