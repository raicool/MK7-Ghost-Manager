#pragma once

#include <SDL.h>

#include "panel.h"
#include "frame.h"

struct panel;

struct window
{
	panel* panels;

	SDL_Event sdlevent;
	SDL_Window* sdlwindow;
	SDL_Renderer* sdlrenderer;

	frame deltatime;

	const char* title;
	uint16_t width;
	uint16_t height;
	uint16_t framerate;

	void init(app* application, const char* window_title, uint16_t win_width, uint16_t win_height, uint16_t framerate, uint32_t flags);
	void update();

	void render_panels();
};