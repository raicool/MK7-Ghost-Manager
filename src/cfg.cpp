#include "pch.h"
#include "cfg.h"

#include "common/log.h"

#include <yaml-cpp/yaml.h>

static float s_cfg_framerate_default = 60;

namespace Config
{
	std::fstream g_cfg_stream;
	std::unordered_map<std::string, YAML::Node> g_configmap =
	{
		{ "framerate", YAML::Node{ 60.0f } },
		{ "display_flags", YAML::Node{ true } },
		{ "opened_files", YAML::Node{ std::vector<std::string>{} }},
	};

	void load()
	{
		const std::string full_path = std::format("{}/{}", SDL_GetBasePath(), CONFIG_FILE_NAME);
		g_cfg_stream.open(full_path, std::ios::in);

		if (g_cfg_stream.is_open() == false)
		{
			LOG_ERROR("configuration file could not be opened.");
			return;
		}

		YAML::Node cache_yaml = YAML::LoadFile(full_path);

		for (YAML::const_iterator it = cache_yaml.begin(); it != cache_yaml.end(); ++it)
		{
			std::string identifier = it->first.as<std::string>();
			if (g_configmap.contains(identifier))
			{
				g_configmap[identifier] = it->second;
			}
		}

		g_cfg_stream.close();
	}

	void save()
	{
		const std::string full_path = std::format("{}/{}", SDL_GetBasePath(), CONFIG_FILE_NAME);
		g_cfg_stream.open(full_path, std::ios::out | std::ios::trunc);

		YAML::Node __dummy_node;

		for (auto& [identifier, node] : g_configmap)
		{
			__dummy_node[identifier] = node;
		}

		g_cfg_stream << __dummy_node;
		g_cfg_stream.close();
	}

	YAML::Node get_setting(std::string identifier)
	{
		if (g_configmap.contains(identifier) == false)
		{
			LOG_ERROR("get_setting() : config map has no entry \"{}\"", identifier);
			return YAML::Node(0);
		}

		return g_configmap[identifier];
	}

	void set_setting(std::string identifier, YAML::Node new_value)
	{
		if (g_configmap.contains(identifier) == false)
		{
			LOG_ERROR("set_setting() : config map has no entry \"{}\"", identifier);
			return;
		}

		g_configmap[identifier] = new_value;
		save();
	}
};