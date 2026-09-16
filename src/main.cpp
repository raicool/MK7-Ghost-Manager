#include "pch.h"

#include "common/common.h"
#include "gfx/texture.h"
#include "spotpass.h"
#include "version.h"
#include "window.h"

#define NANOSECONDS 1000000000
#define FRAMETIME (NANOSECONDS / 240)
#define SDL_CONVERT_PERFORMANCE_TIME \
	(SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency()) *	NANOSECONDS

#define _ms_cast(x) std::chrono::duration_cast<std::chrono::milliseconds>(x)

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
texture g_texture_manager;

int main()
{
	frame __frametime;
	SDL_Event __sdl_event;
	panel __imgui_panel;

	g_window = SDL_CreateWindow(std::format("MK7 Spotpass Ghost Manager v{}", VERSION).c_str(), 848, 480, SDL_WINDOW_RESIZABLE);
	g_renderer = SDL_CreateRenderer(g_window, NULL);

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

	io.Fonts->AddFontFromFileTTF("res/font/rodin-b.otf", 12.0f, 0, ranges.Data);
	io.Fonts->Build();

	logger::init_logger();

	g_texture_manager.current_renderer = g_renderer;
	g_texture_manager.load_ghost_textures();

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
	httplib::Client cli = httplib::Client("https://api.github.com");
	auto res = cli.Get("/repos/raicool/mk7-ghost-manager/releases");
	if (res->status >= 300 || res->status < 200)
	{
		LOG_ERROR("failed to fetch github repository releases\n\t(res->status != 202); res->status == {}", res->status);
	}
	std::string str = res->body;
#endif

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
		SDL_DelayPrecise(FRAMETIME - (SDL_CONVERT_PERFORMANCE_TIME - _start_interv));
	}
}