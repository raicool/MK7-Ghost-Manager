#include "pch.h"

#include "panel.h"
#include "window.h"

#include "app.h"

#include "common/alias.h"
#include "common/utils.h"
#include "gfx/texture.h"

std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf8_conv;

void panel::render()
{
	static int w;
	static int h;
	SDL_GetWindowSize(window_ptr->main_window, &w, &h);

	if (panel_flags & panels::PANEL_MENU_BAR)
	{
		if (ImGui::BeginMainMenuBar());
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open SpotPass save folder")) app_ptr->open_spotpass_folder();
			if (ImGui::MenuItem("Open SpotPass save file"))   app_ptr->open_spotpass_file();

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

		for (int i = 0; i < 8; i++)
		{
			if (app_ptr->spotpass_files[i])
			{
				ImGui::BeginTabBar("Loaded Cups", ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_Reorderable);

				if (ImGui::BeginTabItem(cup_name[i]))
				{
					is_cup_selected = true;
					cup_ptr = app_ptr->spotpass_files[i];
					course = cup_courses[i][idx];

					items[0] = (char*)course_name[cup_courses[i][0]];
					items[1] = (char*)course_name[cup_courses[i][1]];
					items[2] = (char*)course_name[cup_courses[i][2]];
					items[3] = (char*)course_name[cup_courses[i][3]];

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}

		if (is_cup_selected)
		{
			if (ImGui::ListBox("Courses", &idx, items, 4, 4)) course = cup_courses[cup_ptr->cup_id][idx];
			ImGui::Text("Current file: %s", cup_ptr->file_directory.c_str());

			ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

			if (ImGui::Button("Add Ghost"))
			{
				if (cup_ptr->add_ghost(open_file()) == false)
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
		ImGui::Checkbox("Display flags", &display_flags);

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
		if (cup_ptr)
		{
			ImGui::BeginTable("Ghosts", 1, ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerH);

			for (uint32_t i = 0; i < cup_ptr->ghost_count; i++)
			{
				if (cup_ptr->ghosts[i]->course_id == course || show_all_course)
				{
					ImGui::PushID(i);
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					draw_ghost_details(cup_ptr->ghosts[i]);

					if (ImGui::Button("Delete Ghost"))    cup_ptr->delete_ghost(cup_ptr->ghosts[i]);
					if (ImGui::Button("Overwrite Ghost")) cup_ptr->overwrite_ghost(cup_ptr->ghosts[i]->file_offset, open_file());

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
	ImGui::Image(app_ptr->texture_manager.driver[_ghost->character_id], { 64, 64 });
	ImGui::SameLine();
	
	if (display_flags)
	{
		// 26
		// 18
		float flag_xoffset = (26 * (_ghost->country_id % 16)) / 416.0f;
		float flag_yoffset = (18 * (round_multiple(_ghost->country_id, 17) / 17)) / 216.0f;
		ImGui::Image(app_ptr->texture_manager.symbol[0x01], { 26, 18 }, { flag_xoffset, flag_yoffset }, { flag_xoffset + (26 / 416.0f), flag_yoffset + (18 / 216.0f) });
	}

	ImGui::SameLine();
	ImGui::Text("%s", utf8_conv.to_bytes(_ghost->player_name).c_str());
	
	/*
	*	display ghost kart config
	*/
	ImGui::SameLine(ImGui::GetWindowWidth() / 3); ImGui::Image(app_ptr->texture_manager.body[_ghost->kart_id], { 128, 64 });
	ImGui::SameLine(); ImGui::Image(app_ptr->texture_manager.tire[_ghost->tire_id], { 128, 64 });
	ImGui::SameLine(); ImGui::Image(app_ptr->texture_manager.wing[_ghost->glider_id], { 128, 64 });

	/*
	*	first person indicator
	*/
	if (_ghost->fp_flag)
	{
		ImGui::SameLine(ImGui::GetWindowWidth() - 40);
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

	ImGui::NewLine();

	if (ImGui::TreeNode("Name Details"))
	{
		ImGui::DebugTextEncoding(utf8_conv.to_bytes(_ghost->player_name).c_str());
		ImGui::TreePop();
	}

	// todo
	if (ImGui::TreeNode("Mii Details"))
	{
		//ImGui::Text("System ID = %lu", _ghost->mii_data.sys_id);
		//ImGui::Text("Region = %i", _ghost->mii_data.region);
		ImGui::TreePop();
	}

	ImGui::NewLine();

	ImGui::Text("Course: %s", course_name[_ghost->course_id]);
	ImGui::TextColored(ImVec4{ 1.0f, 0.9f, 0.1f, 1.0f }, "Time: %i:%02i.%03i", _ghost->finished_min, _ghost->finished_sec, _ghost->finished_ms);
	ImGui::NewLine();
}