#include "pch.h"

#include "panel.h"
#include "window.h"

#include "app.h"

#include "common/alias.h"
#include "common/utils.h"
#include "gfx/texture.h"

#define COL_GRAY  ImVec4{ 0.5, 0.5, 0.5, 1 }
#define COL_GREEN ImVec4{ 0.5,   1, 0.5, 1 }
#define COL_RED   ImVec4{   1, 0.5, 0.5, 1 }
#define COL_WHITE ImVec4{   1,   1,   1, 1 }

static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;

void panel::render()
{
	static int w;
	static int h;
	SDL_GetWindowSize(window_ptr->sdlwindow, &w, &h);

	if (panel_flags & panels::PANEL_MENU_BAR)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("open spotpass save folder")) app_ptr->open_spotpass_folder();
				if (ImGui::MenuItem("open spotpass save file"))   app_ptr->open_spotpass_file();

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
	}

	if (panel_flags & panels::PANEL_CUPS_LIST)
	{
		ImGui::SetNextWindowSize({ (w / 3.0f), h - 20.0f });
		ImGui::SetNextWindowPos({ 1, 20 });

		static char* items[4];

		if (ImGui::Begin("Cups", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration))
		{
			for (int i = 0; i < 8; i++)
			{
				if (app_ptr->spotpass_files[i])
				{
					ImGui::BeginTabBar("Loaded Cups", ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_Reorderable);

					if (ImGui::BeginTabItem(cup_name[i]))
					{
						is_cup_selected = true;
						cup = i; 

						items[0] = (char*)course_name[cup_courses[i][0]];
						items[1] = (char*)course_name[cup_courses[i][1]];
						items[2] = (char*)course_name[cup_courses[i][2]];
						items[3] = (char*)course_name[cup_courses[i][3]];

						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}
			}

			static int idx = 0;
			if (is_cup_selected)
			{
				if (ImGui::ListBox("Courses", &idx, items, 4, 4)) course = cup_courses[cup][idx];

				ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

				if (ImGui::Button("Add Ghost"))
				{
					if (app_ptr->spotpass_files[cup]->add_ghost(open_file()) == false)
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
			ImGui::Checkbox("Display all courses within cup", &show_all_course);

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
	}

	if (panel_flags & panels::PANEL_GHOST_LIST)
	{
		ImGui::SetNextWindowSize({ w - (w / 3.0f), h - 20.0f });
		ImGui::SetNextWindowPos({ 1 + (w / 3.0f), 20 });

		if (ImGui::Begin("Ghost", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration))
		{
			if (app_ptr->spotpass_files[cup])
			{
				spotpass& current_cup = *app_ptr->spotpass_files[cup];

				ImGui::Spacing();

				ImGui::BeginTable("Ghosts", 1, ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerH);
				for (int i = 0; i < current_cup.ghost_count; i++)
				{
					if (current_cup.ghosts[i]->course_id == course || show_all_course)
					{
						ImGui::PushID(i);
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						draw_ghost_details(current_cup.ghosts[i]);

						if (ImGui::Button("Delete Ghost"))    current_cup.delete_ghost(current_cup.ghosts[i]);
						if (ImGui::Button("Overwrite Ghost")) current_cup.overwrite_ghost(current_cup.ghosts[i]->file_offset, open_file());

						ImGui::PopID();
					}
				}
				ImGui::EndTable();
			}

			ImGui::End();
		}
	}
}

void panel::draw_ghost_details(ghost* _ghost)
{
	ImGui::Image(app_ptr->texture_manager.driver[_ghost->character_id], { 64, 64 });
	ImGui::SameLine();
	ImGui::Text(utf8_conv.to_bytes(_ghost->player_name).c_str());

	ImGui::SameLine(ImGui::GetWindowWidth() / 4); ImGui::Image(app_ptr->texture_manager.body[_ghost->kart_id], { 128, 64 });
	ImGui::SameLine(); ImGui::Image(app_ptr->texture_manager.tire[_ghost->tire_id], { 128, 64 });
	ImGui::SameLine(); ImGui::Image(app_ptr->texture_manager.wing[_ghost->glider_id], { 128, 64 });

	if (_ghost->fp_flag)
	{
		ImGui::SameLine(ImGui::GetWindowWidth() - 64);
		ImGui::Image(app_ptr->texture_manager.symbol[0x00], { 32, 32 });
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted("This ghost was using first person 80% or more of the time");
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}


	if (ImGui::TreeNode("Name Details"))
	{
		ImGui::DebugTextEncoding(utf8_conv.to_bytes(_ghost->player_name).c_str());
		ImGui::TreePop();
	}

	ImGui::Text("Course: %s", course_name[_ghost->course_id]);

	ImGui::TextColored(ImVec4{ 1.0, 0.9, 0.1, 1.0 }, "Time: %i:%02i.%03i", _ghost->finished_min, _ghost->finished_sec, _ghost->finished_ms);
}