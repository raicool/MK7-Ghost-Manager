#pragma once

#include <yaml-cpp/yaml.h>

#define CONFIG_FILE_NAME "settings.cfg"

namespace Config
{
	void load();
	void save();

	YAML::Node get_setting(std::string identifier);
	void set_setting(std::string identifier, YAML::Node new_value);
};

template<typename T>
YAML::Node get_nodeleaf_safe(YAML::Node node, std::string leaf, T placeholder)
{
	if (node[leaf].IsDefined())
	{
		return node[leaf];
	}
	return YAML::Node(placeholder);
}