#pragma once

struct texture
{
	void* driver[0x12];
	void* body[0x11];
	void* tire[0x0a];
	void* wing[0x07];
	void* symbol[0x02];

	std::unordered_map<std::string_view, SDL_Texture*> textures;
	SDL_Renderer* current_renderer;
	uint32_t texture_count;

	void load_ghost_textures(); // loads all the character, kart, tire, and glider textures into global arrays with the raw texture data
	void* add_texture(const char* dir, std::string_view id);
};