#pragma once

#include "core/window.h"
#include "file/spotpass.h"
#include "gfx/texture.h"

struct app
{
	window sdlwindow;
	texture texture_manager;

	spotpass* spotpass_files[8]{ nullptr }; // 8 file managers for 8 different spotpass cups

	void open_spotpass_file();
	void open_spotpass_folder();

	void init();
	int update();

	~app();
};