#include "pch.h"

#include "common/common.h"
#include "texture.h"
#include "boss.h"
#include "version.h"
#include "panel.h"
#include "cfg.h"

#define _ms_cast(x) std::chrono::duration_cast<std::chrono::milliseconds>(x)

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
TextureManager g_texture_manager;

// functions to be called at the end of a frame
std::vector<std::function<void()>> g_funcqueue;

std::vector<std::shared_ptr<SerializedFile>> g_opened_files;

ImFont* g_font_default = nullptr;
ImFont* g_font_rodin = nullptr;
ImFont* g_font_monospace = nullptr;

double g_window_framerate = FRAMETIME(60);

int main()
{
	SDL_Event __sdl_event;
	ImGuiPanel __imgui_panel;

	Logger::init_logger();
	Config::load();
	const double config_framerate = Config::get_setting("framerate").as<double>();
	g_window_framerate = FRAMETIME(config_framerate);

	std::vector<std::string> config_opened_files =
		Config::get_setting("opened_files").as<std::vector<std::string>>();

	LOG_INFO("\n"
		"------------------------------------------------------------------------------------\n"
		" MK7 Spotpass Ghost Manager\n"
		" Version: {}\n"
		" Commit: {}\n"
		" Build Type: {}\n"
		"------------------------------------------------------------------------------------",
		VERSION,
		GIT_COMMIT_HASH,
		DEBUG ? "DEBUG" : "RELEASE"
	);

	g_window = SDL_CreateWindow(std::format("MK7 Spotpass Ghost Manager v{} ({})", VERSION, GIT_COMMIT_HASH).c_str(), 848, 480, SDL_WINDOW_RESIZABLE);
	g_renderer = SDL_CreateRenderer(g_window, NULL);

	SDL_SetRenderVSync(g_renderer, SDL_RENDERER_VSYNC_ADAPTIVE);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForSDLRenderer(g_window, g_renderer);
	ImGui_ImplSDLRenderer3_Init(g_renderer);

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;
	builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
	builder.AddRanges(io.Fonts->GetGlyphRangesGreek());
	builder.AddRanges(extended_symbols);
	builder.BuildRanges(&ranges);

	g_font_default = io.Fonts->AddFontFromFileTTF("res/font/rodin-db.otf", 12.0f, 0, ranges.Data);
	g_font_rodin = io.Fonts->AddFontFromFileTTF("res/font/rodin-b.otf", 12.0f, 0, ranges.Data);
	g_font_monospace = io.Fonts->AddFontDefault();

	io.Fonts->Build();

	g_texture_manager.current_renderer = g_renderer;
	g_texture_manager.load_ghost_textures();

	for (std::string& path : config_opened_files)
	{
		open_spotpass_file(path.c_str());
	}

	double _start_interv = 0;
	while (1)
	{
		_start_interv = SDL_CONVERT_PERFORMANCE_TIME;

		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
		SDL_RenderClear(g_renderer);

		SDL_PollEvent(&__sdl_event);
		ImGui_ImplSDL3_ProcessEvent(&__sdl_event);

		if (__sdl_event.type == SDL_EVENT_QUIT) break;

		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui::NewFrame();

		__imgui_panel.render();

		if (g_funcqueue.empty() == false)
		{
			for (auto it = g_funcqueue.begin(); it != g_funcqueue.end(); it++)
			{
				auto& func = *it;
				func();
			}
			g_funcqueue.clear();
		}

		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), g_renderer);

		SDL_RenderPresent(g_renderer);
		SDL_DelayPrecise(std::clamp(g_window_framerate - (SDL_CONVERT_PERFORMANCE_TIME - _start_interv), 0.0, g_window_framerate));
	}

	std::vector<std::string> opened_files;

	for (auto& file : g_opened_files)
	{
		opened_files.emplace_back(file->file_directory);
	}

	Config::set_setting("opened_files", YAML::Node(opened_files));

	Config::save();
	g_texture_manager.terminate_thread();
}