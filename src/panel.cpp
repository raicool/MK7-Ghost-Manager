#include "pch.h"

#include "common/alias.h"
#include "common/utils.h"
#include "gfx/texture.h"
#include "ghost.h"
#include "panel.h"
#include "spotpass.h"
#include "window.h"

std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf8_conv;

extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern spdata_vector g_spotpass_files;
extern texture g_texture_manager;

#define TOOLTIP(x, ...) if (ImGui::IsItemHovered()) ImGui::SetTooltip(x, __VA_ARGS__)

void panel::render()
{
	static int w;
	static int h;
	SDL_GetWindowSize(g_window, &w, &h);

	if (panel_flags & panels::PANEL_MENU_BAR)
	{
		if (ImGui::BeginMainMenuBar());
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open SpotPass Save Folder")) open_spotpass_folder();
			if (ImGui::MenuItem("Open SpotPass Save File")) open_spotpass_file();
//			if (ImGui::MenuItem("Connect to 3DS debugger")) connect_3ds();

//			ImGui::Separator();

//			if (ImGui::MenuItem("Create Empty SpotPass Files")) create_spotpass_folder();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::CheckboxFlags("Cups List", &panel_flags, panels::PANEL_CUPS_LIST);
			ImGui::CheckboxFlags("Ghost List", &panel_flags, panels::PANEL_GHOST_LIST);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	if (panel_flags & panels::PANEL_CUPS_LIST)
	{
		ImGui::SetNextWindowSize({ (w / 3.0f), h - 20.0f });
		ImGui::SetNextWindowPos({ 1, 20 });

		static char* items[4];
		static int idx = 0;

		ImGui::Begin("Cups", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration);
		ImGui::Text("%f", ImGui::GetIO().Framerate);

		for (auto& _spdata : g_spotpass_files)
		{
			ImGui::BeginTabBar("Loaded Cups", ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_Reorderable);

			const uint8_t _cup = _spdata->cup_id;

			if (ImGui::BeginTabItem(cup_name[_cup]))
			{
				is_cup_selected = true;
				current_cup = _spdata;
				course = cup_courses[_cup][idx];

				items[0] = const_cast<char*>(course_name[cup_courses[_cup][0]]);
				items[1] = const_cast<char*>(course_name[cup_courses[_cup][1]]);
				items[2] = const_cast<char*>(course_name[cup_courses[_cup][2]]);
				items[3] = const_cast<char*>(course_name[cup_courses[_cup][3]]);

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		if (is_cup_selected)
		{
			if (ImGui::ListBox("Courses", &idx, items, 4, 4)) course = cup_courses[current_cup->cup_id][idx];
			ImGui::Text(current_cup->file_directory.c_str());

			ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Button("Add Ghost"))
			{
				const char* file_path = open_file();
				//auto path_w16 = utf8_conv.from_bytes(file_path ? file_path : "");
				if (current_cup->add_ghost(file_path) == false)
				{
					ImGui::PushID("Load Failed");
					ImGui::OpenPopup("Load Failed");
					ImGui::PopID();
					ImVec2 center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
				}
			}
		}

		ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();
		ImGui::Checkbox("Display All Courses In Cup", &show_all_course);
		ImGui::Checkbox("Display Flags", &display_flags);

		ImGui::PushID("Load Failed");
		if (ImGui::BeginPopupModal("Load Failed", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Failed to add ghost. see console for more details.");
			if (ImGui::Button("OK")) { ImGui::CloseCurrentPopup(); }

			ImGui::EndPopup();
		}
		ImGui::PopID();

		ImGui::End();
	}

	if (panel_flags & panels::PANEL_GHOST_LIST)
	{
		ImGui::SetNextWindowSize({ w - (w / 3.0f), h - 20.0f });
		ImGui::SetNextWindowPos({ 1 + (w / 3.0f), 20 });
		ImGui::Begin("Ghost", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		/*
		*	Cup ghosts will be rendered here
		*/
		if (current_cup)
		{
			ImGui::BeginTable("Ghosts", 1, ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerH);

			for (uint32_t i = 0; i < current_cup->ghost_count; i++)
			{
				if (current_cup->ghosts[i]->course_id == course || show_all_course)
				{
					ImGui::PushID(i);
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					draw_ghost_details(current_cup->ghosts[i]);

					if (ImGui::Button("Delete Ghost"))    current_cup->delete_ghost(current_cup->ghosts[i]);
					if (ImGui::Button("Overwrite Ghost")) current_cup->overwrite_ghost(current_cup->ghosts[i]->file_offset, open_file());
					if (ImGui::Button("Extract Ghost"))   current_cup->extract_ghost(current_cup->ghosts[i]);

					ImGui::PopID();
				}
			}
			ImGui::EndTable();
		}

		ImGui::End();
	}
}

void panel::draw_ghost_details(ghost* _ghost)
{
	/*
	*	ghost character, player name, and flag
	*/
	ImGui::Image(g_texture_manager.driver[_ghost->character_id], { 64, 64 });
	ImGui::SameLine();
	
	if (display_flags)
	{
		draw_flag(_ghost->country_id);
		//ImGui::Text("%i", _ghost->country_id);
	}

	ImGui::SameLine();
	ImGui::Text("%s", utf8_conv.to_bytes(_ghost->player_name).c_str());
	
	/*
	*	display ghost kart config
	*/
	ImGui::SameLine(ImGui::GetWindowWidth() / 3); ImGui::Image(g_texture_manager.body[_ghost->kart_id], { 128, 64 });
	ImGui::SameLine(); ImGui::Image(g_texture_manager.tire[_ghost->tire_id], { 128, 64 });
	ImGui::SameLine(); ImGui::Image(g_texture_manager.wing[_ghost->glider_id], { 128, 64 });

	/*
	*	first person indicator
	*/
	if (_ghost->serialized.gyro_flag)
	{
		ImGui::SameLine(ImGui::GetWindowWidth() - 64);
		ImGui::Image(g_texture_manager.symbol[0x00], { 32, 32 });
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("This ghost was using first person 80% or more of the time");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	ImGui::NewLine();

	ImGui::Text("Course: %s", course_name[_ghost->course_id]);

	const packed_time finished_time = _ghost->serialized.finished_time;
	const packed_time lap_times[3] =
	{
		_ghost->serialized.lap1_time,
		_ghost->serialized.lap2_time,
		_ghost->serialized.lap3_time,
	};

	ImGui::TextColored(
		ImVec4{ 1.0f, 0.9f, 0.1f, 1.0f }, 
		"Time: %i:%02i.%03i", 
		finished_time.min, finished_time.sec, finished_time.ms
	);

	TOOLTIP(
		"Lap 1: %i:%02i.%03i\n"
		"Lap 2: %i:%02i.%03i\n"
		"Lap 3: %i:%02i.%03i\n",
		lap_times[0].min, lap_times[0].sec, lap_times[0].ms,
		lap_times[1].min, lap_times[1].sec, lap_times[1].ms,
		lap_times[2].min, lap_times[2].sec, lap_times[2].ms
	);

	ImGui::NewLine();
}

void draw_flag(char nation_id)
{
	float flag_xoffset = (26 * (nation_id % 16)) / 416.0f;
	float flag_yoffset = (18 * (round_multiple(nation_id, 17) / 17)) / 216.0f;
	ImGui::Image(g_texture_manager.symbol[0x01], { 26, 18 }, { flag_xoffset, flag_yoffset }, { flag_xoffset + (26 / 416.0f), flag_yoffset + (18 / 216.0f) });
}