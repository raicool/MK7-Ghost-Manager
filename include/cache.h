#pragma once

#define CACHE_DIRECTORY "cached"

namespace CacheDirectory
{
	static uint8_t hash_shorthand(size_t hash)
	{
		return (hash & 0xff);
	}

	static std::string get_cache_filepath(const std::string& identifier)
	{
		const char* base = SDL_GetBasePath();
		uint64_t hashed = std::hash<std::string>{}(identifier);
		uint8_t shorthand = hash_shorthand(hashed);

		const std::string path = std::vformat("{}{}/{:02x}/{:016x}", std::make_format_args(
			base,
			CACHE_DIRECTORY,
			shorthand,
			hashed
		));

		return path;
	}

	static std::string get_cache_path(const std::string& identifier)
	{
		const char* base = SDL_GetBasePath();
		uint64_t hashed = std::hash<std::string>{}(identifier);
		uint64_t shorthand = hash_shorthand(hashed);

		const std::string path = std::vformat("{}{}/{:02x}", std::make_format_args(
			base,
			CACHE_DIRECTORY,
			shorthand
		));

		return path;
	}

	static bool cache_exists(const std::string& identifier)
	{
		return std::filesystem::exists(get_cache_filepath(identifier));
	}

	static bool save_cache_file(char* data, size_t size, const std::string& identifier, bool overwrite = false)
	{
		if (overwrite == false && cache_exists(identifier))
		{
			// cannot overwrite
			return false;
		}

		const auto absolute_path = get_cache_path(identifier);

		std::filesystem::create_directories(absolute_path);

		std::fstream stream(get_cache_filepath(identifier), std::ios::out | std::ios::binary);

		if (stream.is_open() == false)
		{
			LOG_ERROR("save_cache_file(): could not open file \"{}\"", absolute_path);
			return false;
		}

		stream.write(data, size);
		stream.close();
		return true;
	}

	static void delete_all()
	{
		const char* base = SDL_GetBasePath();

		std::filesystem::remove_all(std::format("{}{}", base, CACHE_DIRECTORY));
	}
}