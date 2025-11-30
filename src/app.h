#pragma once

#include "core/window.h"
#include "file/spotpass.h"
#include "gfx/texture.h"

struct app
{
	window main_window;
	texture texture_manager;
//	socket gdb_iosocket;

	spotpass* spotpass_files[32]{ nullptr };

	void open_spotpass_file();
	void open_spotpass_folder();
//	void connect_3ds();
	void create_spotpass_folder();

	void init();
	int update();

	~app();
};