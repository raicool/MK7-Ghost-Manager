#pragma once

struct window;
struct ghost;

enum panels
{
	PANEL_MENU_BAR   = (1 << (0)), // acts as a way to enable or disable panels, as well as opening spotpass files
	PANEL_CUPS_LIST  = (1 << (1)), // list all currently loaded spotpass cup files
	PANEL_GHOST_LIST = (1 << (2)), // list all ghost in the selected track from PANEL_CUPS_LIST

	PANEL_DEFAULT = PANEL_MENU_BAR | PANEL_CUPS_LIST | PANEL_GHOST_LIST,
};

struct spotpass;
struct panel
{
	uint32_t panel_flags = PANEL_DEFAULT; //
	std::shared_ptr<spotpass> current_cup = nullptr; //< which cup to display ghosts for in PANEL_GHOST_LIST
	int32_t course_idx = 0; //< which course to display ghosts for in PANEL_GHOST_LIST
	bool is_cup_selected = false;
	bool display_flags   = true;
	
	void draw_ghost_details(std::unique_ptr<ghost>& _ghost);
	void render();
};

void render_mii_image(mii* mii_data, uint16_t size = 64);
void draw_flag(char nation_id);