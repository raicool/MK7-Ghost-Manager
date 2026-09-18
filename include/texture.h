#pragma once

#include <string_view>

#define DRIVER_TEXTURE_COUNT 0x12
#define BODY_TEXTURE_COUNT 0x11
#define TIRE_TEXTURE_COUNT 0x0a
#define WING_TEXTURE_COUNT 0x07
#define SYMBOL_TEXTURE_COUNT 0x02

struct texture
{
	void* driver[DRIVER_TEXTURE_COUNT];
	void* body[BODY_TEXTURE_COUNT];
	void* tire[TIRE_TEXTURE_COUNT];
	void* wing[WING_TEXTURE_COUNT];
	void* symbol[SYMBOL_TEXTURE_COUNT];

	std::unordered_map<std::string_view, SDL_Texture*> textures;
	SDL_Renderer* current_renderer;
	uint32_t texture_count;

	void load_ghost_textures(); // loads all the character, kart, tire, and glider textures into global arrays with the raw texture data
	void* add_texture(const char* dir, std::string_view id);
};