#include "pch.h"

#include "window.h"
#include "common/font.h"

void window::init(app* application, const char* window_title, uint16_t win_width, uint16_t win_height, uint16_t win_framerate, uint32_t flags)
{
	title = window_title;
	width = win_width;
	height = win_height;
	framerate = win_framerate;

	main_window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_width, win_height, flags);
	sdlrenderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE, "overscan");

	if (!main_window)
	{
		LOG_ERROR("SDL Window could not be created {}", SDL_GetError());
	}

	panels = new panel();
	panels->link_window(this);
	panels->link_app(application);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForSDLRenderer(main_window, sdlrenderer);
	ImGui_ImplSDLRenderer2_Init(sdlrenderer);

	// TODO: find 3ds' entire unicode range
	ImGuiIO& io = ImGui::GetIO();
	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;
	builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
	builder.AddRanges(io.Fonts->GetGlyphRangesGreek());
	builder.AddRanges(extended_symbols);
	builder.BuildRanges(&ranges);

	io.Fonts->AddFontFromFileTTF("res/font/rodin-b.otf",  14.0f, 0, ranges.Data);
	io.Fonts->Build();
}

void window::update()
{
	SDL_SetRenderDrawColor(sdlrenderer, 0, 0, 0, 255);
	SDL_RenderClear(sdlrenderer);
	render_panels();
}

void window::render_panels()
{
	ImGui_ImplSDL2_NewFrame(main_window);
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow(nullptr);

	panels->render();

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}