#include "pch.h"

#include "window.h"
#include "common/font.h"

void window::init(app* application, const char* window_title, uint16_t win_width, uint16_t win_height, uint16_t win_framerate, uint32_t flags)
{
	title = window_title;
	width = win_width;
	height = win_height;
	framerate = win_framerate;

	if (!SDL_CreateWindowAndRenderer(window_title, win_width, win_height, 0, &main_window, &sdlrenderer))
	{
		LOG_ERROR("SDL Window/Renderer could not be created \n\tSDL_GetError(): {}", SDL_GetError());
	}

	panels = new panel();
	panels->link_window(this);
	panels->link_app(application);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplSDL3_InitForSDLRenderer(main_window, sdlrenderer);
	ImGui_ImplSDLRenderer3_Init(sdlrenderer);

	// TODO: find 3ds' entire unicode range
	ImGuiIO& io = ImGui::GetIO();
	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;
	builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());
	builder.AddRanges(io.Fonts->GetGlyphRangesGreek());
	builder.AddRanges(extended_symbols);
	builder.BuildRanges(&ranges);

	io.Fonts->AddFontFromFileTTF("res/font/rodin-b.otf", 12.0f, 0, ranges.Data);
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
	ImGui_ImplSDL3_NewFrame();
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow(nullptr);

	panels->render();

	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdlrenderer);
}