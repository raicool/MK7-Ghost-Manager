#pragma once

#include <string_view>

#define DRIVER_TEXTURE_COUNT 0x12
#define BODY_TEXTURE_COUNT 0x11
#define TIRE_TEXTURE_COUNT 0x0a
#define WING_TEXTURE_COUNT 0x07
#define SYMBOL_TEXTURE_COUNT 0x03

struct mii;
enum async_texture_status
{
	WORKING,
	SURFACE_CREATED,
	OK
};
struct async_texture
{
	async_texture_status status;
	SDL_Surface* surface;
	SDL_Texture* data;
};


struct texture
{
	void* driver[DRIVER_TEXTURE_COUNT];
	void* body[BODY_TEXTURE_COUNT];
	void* tire[TIRE_TEXTURE_COUNT];
	void* wing[WING_TEXTURE_COUNT];
	void* symbol[SYMBOL_TEXTURE_COUNT];

	std::unordered_map<std::string_view, SDL_Texture*> textures;
	std::unordered_map<uint16_t, async_texture> mii_textures;
	SDL_Renderer* current_renderer;
	uint32_t texture_count;

	void load_ghost_textures(); // loads all the character, kart, tire, and glider textures into global arrays with the raw texture data
	void* add_texture(const char* dir, std::string_view id);
	void* request_mii_texture(mii* mii_raw_data);
};