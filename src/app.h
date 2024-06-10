#pragma once

#include "core/window.h"
#include "file/spotpass.h"
#include "gfx/texture.h"

struct app
{
	window main_window;
	texture texture_manager;
//	socket gdb_iosocket;

	spotpass* spotpass_files[8]{ nullptr }; // 8 file managers for 8 different spotpass cups

	void open_spotpass_file();
	void open_spotpass_folder();
//	void connect_3ds();

	void init();
	int update();

	~app();
};