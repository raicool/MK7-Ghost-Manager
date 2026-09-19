#pragma once

struct ghost;
struct spotpass;
struct panel
{
	std::shared_ptr<spotpass> current_cup = nullptr; //< which cup to display ghosts for in PANEL_GHOST_LIST
	int32_t course_idx = 0; //< which course to display ghosts for in PANEL_GHOST_LIST
	bool is_cup_selected = false;
	bool display_flags   = true;
	
	void draw_ghost_details(std::unique_ptr<ghost>& _ghost);
	void render();
};

void render_mii_image(mii* mii_data, uint16_t size = 64);
void draw_flag(char nation_id);