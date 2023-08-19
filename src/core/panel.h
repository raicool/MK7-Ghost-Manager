#pragma once

struct window;
struct app;
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
	panel(window* main_window)
		: window_ptr(main_window) {};

	panel() {};

	window* window_ptr = nullptr;         //
	app* app_ptr = nullptr;               //
	uint32_t panel_flags = PANEL_DEFAULT; //
	spotpass* cup_ptr = nullptr;          //< which cup to display ghosts for in PANEL_GHOST_LIST
	int32_t course = 0;                   //< which course to display ghosts for in PANEL_GHOST_LIST
	bool show_all_course = false;         //< toggle whether to show all courses within a cup or just 1
	bool is_cup_selected = false;
	
	void draw_ghost_details(ghost* _ghost);

	void render();
	void link_window(window* new_window) { window_ptr = new_window; };
	void link_app(app* new_application) { app_ptr = new_application; };
};