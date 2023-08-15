#include "pch.h"

#include "common/common.h"
#include "core/window.h"
#include "gfx/texture.h"
#include "app.h"

void app::init()
{
	sdlwindow.init(this, "MK7 Spotpass Ghost Manager", 1280, 720, 144, SDL_WINDOW_RESIZABLE);

	logger::init_logger();

	texture_manager.current_renderer = sdlwindow.sdlrenderer;
	texture_manager.load_ghost_textures();
}

int app::update()
{
	while (1)
	{
		sdlwindow.deltatime.before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		SDL_PollEvent(&sdlwindow.sdlevent);
		ImGui_ImplSDL2_ProcessEvent(&sdlwindow.sdlevent);

		switch (sdlwindow.sdlevent.type)
		{
		case SDL_QUIT:
			return 0;
		}

		sdlwindow.update();

		SDL_RenderPresent(sdlwindow.sdlrenderer);

		sdlwindow.deltatime.apply_delay((uint32_t)((1.0f / sdlwindow.framerate) * 1000.0f));
	}
}

void app::open_spotpass_file()
{
	const char* file_dir = open_file();
	uint8_t cup = filename_to_cup(std::filesystem::path(file_dir).filename().string().c_str());
	LOG_DEBUG("Cup = {}, file = {}", cup, std::filesystem::path(file_dir).filename().string().c_str());
	if (cup >= 0 && cup <= 8)
	{
		if (spotpass_files[cup]) delete spotpass_files[cup];
		spotpass_files[cup] = new spotpass();
		spotpass_files[cup]->load(file_dir);
		spotpass_files[cup]->cup_id = cup;
	}
	else
	{
		LOG_ERROR("load error : could not get cup_id from file name \"{}\"", file_dir);
	}

	
}

void app::open_spotpass_folder()
{
	uint8_t cup = 0;
	const char* folder_dir = open_folder();

	for (const std::filesystem::directory_entry& file : std::filesystem::recursive_directory_iterator(folder_dir))
	{
		auto file_name = file.path().filename();

		cup = filename_to_cup(file_name.string().c_str());
		LOG_DEBUG("Cup = {}, file = {}", cup, file_name.string().c_str());

		if (cup >= 0 && cup <= 8)
		{
			if (spotpass_files[cup]) delete spotpass_files[cup];
			spotpass_files[cup] = new spotpass();
			spotpass_files[cup]->load(file.path().string().c_str());
			spotpass_files[cup]->cup_id = cup;
		}
	}
}

app::~app()
{
	for (int i = 0; i < 8; i++) delete spotpass_files[i];
}