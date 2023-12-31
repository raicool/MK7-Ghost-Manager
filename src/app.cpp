#include "pch.h"

#include "common/common.h"
#include "core/window.h"
#include "gfx/texture.h"
#include "app.h"

void app::init()
{
	main_window.init(this, "MK7 Spotpass Ghost Manager v0.2.4", 848, 480, 0, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);

	logger::init_logger();

	texture_manager.current_renderer = main_window.sdlrenderer;
	texture_manager.load_ghost_textures();
}

int app::update()
{
	while (1)
	{
		main_window.deltatime.before = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		SDL_PollEvent(&main_window.sdlevent);
		ImGui_ImplSDL2_ProcessEvent(&main_window.sdlevent);

		switch (main_window.sdlevent.type)
		{
		case SDL_QUIT:
			return 0;
		}

		main_window.update();

		SDL_RenderPresent(main_window.sdlrenderer);

		main_window.deltatime.apply_delay((int32_t)((1.0f / main_window.framerate) * 1000.0f));
	}
}

void app::open_spotpass_file()
{
	const char* file_dir = open_file();
	
	spotpass* new_spotpass = new spotpass();
	uint8_t cup = new_spotpass->load(file_dir);

	if (cup >= 0 && cup <= 8)
	{
		LOG_DEBUG("Cup = {}, file = {}", cup, std::filesystem::path(file_dir).filename().string().c_str());
		if (spotpass_files[cup - 1]) delete spotpass_files[cup - 1];
		spotpass_files[cup - 1] = new_spotpass;
	}
}

void app::open_spotpass_folder()
{
	uint8_t cup = 0;
	const char* folder_dir = open_folder();

	if (!folder_dir) return;

	for (const std::filesystem::directory_entry& file : std::filesystem::recursive_directory_iterator(folder_dir))
	{
		auto file_name = file.path().filename();

		spotpass* new_spotpass = new spotpass();
		cup = new_spotpass->load(file.path().string().c_str());

		if (cup >= 1 && cup <= 8)
		{
			LOG_DEBUG("Cup = {}, file = {}", cup, file_name.string().c_str());
			if (spotpass_files[cup - 1]) delete spotpass_files[cup - 1];
			spotpass_files[cup - 1] = new_spotpass;
		}
	}
}

app::~app()
{
	for (int i = 0; i < 8; i++) if (spotpass_files[i]) delete spotpass_files[i];
}