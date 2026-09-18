#include "pch.h"

#include "common/alias.h"
#include "common/utils.h"
#include "texture.h"
#include "ghost.h"
#include "panel.h"
#include "spotpass.h"
#include <imgui_internal.h>

extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern std::vector<std::shared_ptr<spotpass>> g_spotpass_files;
extern texture g_texture_manager;

extern ImFont* g_font_rodin;

std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf8_conv;
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

	ImGuiID dockspace_id = ImGui::GetID("Dockspace");
	static bool init = true;
	ImVec2 mainSize = ImGui::GetMainViewport()->Size;

	if (init)
	{
		init = false;
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_CentralNode);
		ImGui::DockBuilderSetNodeSize(dockspace_id, mainSize);


		ImGuiID dock_id_left = 0, dock_id_right = 0;
		ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.25f, &dock_id_left, &dock_id_right);
		ImGui::DockBuilderDockWindow("Cups", dock_id_left);
		ImGui::DockBuilderDockWindow("Ghost", dock_id_right);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::DockSpaceOverViewport(dockspace_id, ImGui::GetMainViewport());

	if (panel_flags & panels::PANEL_CUPS_LIST)
	{
		static char* items[4];
		static int idx = 0;

		ImGui::Begin("Cups", 0, ImGuiWindowFlags_NoCollapse);
		ImGui::Text("%f", ImGui::GetIO().Framerate);

		for (auto& _spdata : g_spotpass_files)
		{
			ImGui::BeginTabBar("Loaded Cups", ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_Reorderable);

			const uint8_t _cup = _spdata->cup_id;

			if (ImGui::BeginTabItem(cup_name[_cup]))
			{
				is_cup_selected = true;
				current_cup = _spdata;
				course_idx = idx;

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
			if (ImGui::ListBox("Courses", &idx, items, 4, 4))
			{
				course_idx = idx;
			}

			ImGui::Text(current_cup->file_directory.c_str());

			if (ImGui::Button("Save"))
			{
				current_cup->save();
			}

			ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Button("Add Ghost"))
			{
				const char* file_path = open_file();
				//auto path_w16 = utf8_conv.from_bytes(file_path ? file_path : "");
				if (current_cup->add_ghost(course_idx, file_path) == false)
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
		ImGui::Begin("Ghost", 0, ImGuiWindowFlags_NoCollapse);

		/*
		*	Cup ghosts will be rendered here
		*/
		if (current_cup)
		{
			ImGui::PushFont(g_font_rodin);
			ImGui::BeginTable("Ghosts", 1, ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerH);

			std::array<std::unique_ptr<ghost>, 20>* coursedata = current_cup->get_course(course_idx);

			if (coursedata)
			{
				for (uint32_t i = 0; i < 20; i++)
				{
					std::unique_ptr<ghost>& current_ghost = coursedata->at(i);

					if (current_ghost == nullptr)
					{
						continue;
					}

					ImGui::PushID(i);
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					draw_ghost_details(current_ghost);

					if (ImGui::Button("Delete Ghost"))
					{
						current_cup->delete_ghost(current_ghost);
					}

					if (ImGui::Button("Overwrite Ghost"))
					{
						current_cup->overwrite_ghost(current_ghost, open_file());
					}

					if (ImGui::Button("Extract Ghost"))
					{
						current_cup->extract_ghost(current_ghost);
					}

					if (ImGui::Button("Export Mii Data (.mii)"))
					{
						const uint64_t system_id = current_ghost->mii_data.system_id;
						auto file = create_file(std::format("{:016x}.mii", system_id).c_str(), "All\0*.*\0Mii (*.mii)\0*.mii\0");

						std::fstream mii_stream(file, std::ios::out | std::ios::binary);

						bin_write<mii>(&current_ghost->mii_data, mii_stream, (uint32_t)0, sizeof(mii));

						mii_stream.close();
					}

					ImGui::PopID();
				}
			}

			ImGui::EndTable();
			ImGui::PopFont();
		}

		ImGui::End();
	}
}

void panel::draw_ghost_details(std::unique_ptr<ghost>& _ghost)
{
	/*
	*	ghost character, player name, and flag
	*/
	ImGui::Image(g_texture_manager.driver[_ghost->character_id], { 64, 64 });
	ImGui::SameLine();
	
	if (display_flags)
	{
		draw_flag(_ghost->country_id);
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
//	if (_ghost->serialized.gyro_flag)
	if (0)
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

	ImGui::TextColored(
		ImVec4{ 1.0f, 0.9f, 0.1f, 1.0f }, 
		"Time: %i:%02i.%03i", 
		finished_time.min(), finished_time.sec(), finished_time.ms()
	);

	TOOLTIP(
		"Lap 1: %i:%02i.%03i\n"
		"Lap 2: %i:%02i.%03i\n"
		"Lap 3: %i:%02i.%03i\n",
		_ghost->serialized.lap1_min(), _ghost->serialized.lap1_sec(), _ghost->serialized.lap1_ms(),
		_ghost->serialized.lap2_min(), _ghost->serialized.lap2_sec(), _ghost->serialized.lap2_ms(),
		_ghost->serialized.lap3_min(), _ghost->serialized.lap3_sec(), _ghost->serialized.lap3_ms()
	);

	ImGui::NewLine();
}

void draw_flag(char nation_id)
{
	float flag_xoffset = (26 * (nation_id % 16)) / 416.0f;
	float flag_yoffset = (18 * (round_multiple(nation_id, 17) / 17)) / 216.0f;
	ImGui::Image(g_texture_manager.symbol[0x01], { 26, 18 }, { flag_xoffset, flag_yoffset }, { flag_xoffset + (26 / 416.0f), flag_yoffset + (18 / 216.0f) });
}