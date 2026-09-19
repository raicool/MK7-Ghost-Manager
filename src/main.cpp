#include "pch.h"

#include "common/common.h"
#include "texture.h"
#include "spotpass.h"
#include "version.h"
#include "panel.h"

#define NANOSECONDS 1000000000
#define FRAMETIME (NANOSECONDS / 60)
#define SDL_CONVERT_PERFORMANCE_TIME \
	(SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency()) *	NANOSECONDS

#define _ms_cast(x) std::chrono::duration_cast<std::chrono::milliseconds>(x)

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
texture g_texture_manager;

ImFont* g_font_default = nullptr;
ImFont* g_font_rodin = nullptr;
ImFont* g_font_monospace = nullptr;

int main()
{
	SDL_Event __sdl_event;
	panel __imgui_panel;

	logger::init_logger();
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

		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), g_renderer);

		SDL_RenderPresent(g_renderer);
		SDL_DelayPrecise(std::clamp(FRAMETIME - (SDL_CONVERT_PERFORMANCE_TIME - _start_interv), 0.0, (double)FRAMETIME));
	}
}