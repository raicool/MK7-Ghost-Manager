#pragma once

#include <string_view>

#define DRIVER_TEXTURE_COUNT 0x12
#define BODY_TEXTURE_COUNT 0x11
#define TIRE_TEXTURE_COUNT 0x0a
#define WING_TEXTURE_COUNT 0x07
#define SYMBOL_TEXTURE_COUNT 0x03

struct CFLStoreData;
enum AsyncTextureStatusEnum
{
	// surface/texture is not yet processed by texture loader thread
	WORKING,

	// surface data is created, texture data not ready
	SURFACE_CREATED,

	// texture is ready to be read by main thread
	READY
};

// texture data that is loaded from a separate thread
// status set to AsyncTextureStatusEnum::READY when finished loading
struct AsyncTextureData
{
	AsyncTextureStatusEnum status;
	SDL_Surface* surface;
	SDL_Texture* data;
};

struct TextureManager
{
	void* driver[DRIVER_TEXTURE_COUNT];
	void* body[BODY_TEXTURE_COUNT];
	void* tire[TIRE_TEXTURE_COUNT];
	void* wing[WING_TEXTURE_COUNT];
	void* symbol[SYMBOL_TEXTURE_COUNT];

	std::unordered_map<std::string_view, SDL_Texture*> textures;
	std::unordered_map<uint16_t, AsyncTextureData> mii_textures;
	SDL_Renderer* current_renderer;
	uint32_t texture_count;

	void terminate_thread();
	void load_ghost_textures(); // loads all the character, kart, tire, and glider textures into global arrays with the raw texture data
	void* add_texture(const char* dir, std::string_view id);
	void* request_mii_texture(CFLStoreData* mii_raw_data);
};