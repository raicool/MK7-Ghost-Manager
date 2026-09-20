#include "pch.h"
#include "imgui_cfg.h"

#include "common/utils.h"
#include "cfg.h"

extern double g_window_framerate;
extern bool g_display_flags;

bool g_imgui_config_screen_open = false;

static void imgui_cfg_render()
{
	if (g_imgui_config_screen_open == false) return;

	if (ImGui::Begin("Settings", &g_imgui_config_screen_open))
	{
		ImGui::BeginTabBar("##");
		
		imgui_cfg_settings_tab();

		ImGui::EndTabBar();
		ImGui::End();
	}
}

static void imgui_cfg_settings_tab()
{
	double _tmp_framerate = Config::get_setting("framerate").as<double>();
	ImGui::InputDouble("Framerate", &_tmp_framerate, 0, 0, "%.0f");
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		Config::set_setting("framerate", YAML::Node{ _tmp_framerate });
		g_window_framerate = FRAMETIME(_tmp_framerate);
	}

	if (ImGui::Checkbox("Display Flags", &g_display_flags))
	{
		Config::set_setting("display_flags", YAML::Node{ g_display_flags });
	}
}