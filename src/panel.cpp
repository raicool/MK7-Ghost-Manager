#include "pch.h"

#include "common/alias.h"
#include "common/utils.h"
#include "common/type.h"
#include "texture.h"
#include "ghost.h"
#include "panel.h"
#include "boss.h"
#include <imgui_internal.h>

extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern std::vector<std::shared_ptr<BOSSRankingData>> g_spotpass_files;
extern TextureManager g_texture_manager;

extern ImFont* g_font_rodin;
extern ImFont* g_font_monospace;

std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf8_conv;
#define TOOLTIP(x, ...) if (ImGui::IsItemHovered()) ImGui::SetTooltip(x, __VA_ARGS__)

void render_hex_view(uint8_t* src, size_t size, uint16_t view_length = 17, uint16_t view_width = 16)
{
	ImGui::BeginTable("Hex Viewer", 3, ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit);
	ImGui::TableSetupScrollFreeze(1, 0);
	ImGui::TableSetupColumn("Offset", 0, 64);
	ImGui::TableSetupColumn("Data");
	ImGui::TableSetupColumn("Ascii");
	ImGui::TableHeadersRow();

	ImGui::PushFont(g_font_monospace);
	uint8_t ptr_val = 0;
	uint32_t offset = 0;
	bool end = false;

	for (uint16_t i = 0; i < view_length; i++)
	{
		if (end) break;

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

		ImGui::Text("%04x", i * view_length);

		ImGui::TableSetColumnIndex(1);

		// hex display
		for (int j = 0; j < view_width; j++)
		{

			offset = (view_width * i) + j;
			if (offset > size)
			{
				end = true;
				ptr_val = 0;
			}
			else
			{
				ptr_val = src[offset];
			}

			uint32_t col = ptr_val ? 
				IM_COL32(255, 255, 255, 255) : 
				IM_COL32(127, 127, 127, 255);

			ImGui::PushStyleColor(ImGuiCol_Text, col);

			ImGui::SameLine();
			ImGui::Text("%02x", ptr_val);

			ImGui::PopStyleColor();
		}

		ImGui::TableSetColumnIndex(2);

		// ascii display
		for (int j = 0; j < view_width; j++)
		{
			offset = (view_width * i) + j;
			if (offset > size)
			{
				end = true;
				ptr_val = 0;
			}
			else
			{
				ptr_val = src[offset];
			}

			uint32_t col = (ptr_val <= 0x20 || ptr_val > 0x7f) ?
				IM_COL32(255, 255, 255, 255) :
				IM_COL32(127, 127, 127, 255);

			ImGui::PushStyleColor(ImGuiCol_Text, col);

			ImGui::SameLine();
			ImGui::Text("%c", ptr_val);

			ImGui::PopStyleColor();
		}
	}
	ImGui::PopFont();
	ImGui::EndTable();
}

void ImGuiPanel::render()
{
	static int w;
	static int h;
	SDL_GetWindowSize(g_window, &w, &h);

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		// File open operations

		if (ImGui::MenuItem("Open SpotPass Save Folder")) open_spotpass_folder();
		if (ImGui::MenuItem("Open SpotPass Save File")) open_spotpass_file();

		ImGui::Separator();
		
		// File manipulation operations

		this->ranking_directory_text();

		if (ImGui::MenuItem("Close", nullptr, false, current_file != nullptr))
		{
			auto it = std::find(g_spotpass_files.begin(), g_spotpass_files.end(), current_file);

			if (it != g_spotpass_files.end())
			{
				g_spotpass_files.erase(it);
			}

			current_file = nullptr;
			is_cup_selected = false;
		}
		TOOLTIP("Closes spotpass file\nSave before closing!");

		if (ImGui::MenuItem("Save")) current_file->save(false);
		if (ImGui::MenuItem("Save As")) current_file->save();
		if (ImGui::MenuItem("Reload from File")) current_file->reload();

		const size_t _file_count = g_spotpass_files.size();
		if (_file_count > 0)
		{
			ImGui::Separator();

			if (ImGui::MenuItem("Save All"))
			{
				for (auto& file : g_spotpass_files)
				{
					file->save(false);
				}
			}
			TOOLTIP("Save all loaded spotpass files");

			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(127, 127, 127, 255));
			ImGui::Text(" %i file(s)", _file_count);
			ImGui::PopStyleColor();

			ImGui::Separator();

			if (ImGui::MenuItem("Save All Miis"))
			{
				const char* directory = open_folder();
				auto directory_utf16 = utf8_conv.from_bytes(directory);

				if (directory)
				{
					for (auto& file : g_spotpass_files)
					{
						for (auto& ghosts : file->course_1)
						{
							if (ghosts)
							{
								const uint64_t system_id = ghosts->mii_data.system_id;

								wchar_t format[512];
								swprintf(format, L"%s/%016llx (%s).cfsd", (char*)directory_utf16.c_str(), system_id, (char*)ghosts->player_name.c_str());

								std::fstream mii_stream(format, std::ios::out | std::ios::binary);

								bin_write<CFLStoreData>(&ghosts->mii_data, mii_stream, 0u, sizeof(CFLStoreData));

								mii_stream.close();
							}

							for (auto& ghosts : file->course_2)
							{
								if (ghosts)
								{
									const uint64_t system_id = ghosts->mii_data.system_id;

									wchar_t format[512];
									swprintf(format, L"%s/%016llx (%s).cfsd", (char*)directory_utf16.c_str(), system_id, (char*)ghosts->player_name.c_str());

									std::fstream mii_stream(format, std::ios::out | std::ios::binary);

									bin_write<CFLStoreData>(&ghosts->mii_data, mii_stream, 0u, sizeof(CFLStoreData));

									mii_stream.close();
								}
							}

							for (auto& ghosts : file->course_3)
							{
								if (ghosts)
								{
									const uint64_t system_id = ghosts->mii_data.system_id;

									wchar_t format[512];
									swprintf(format, L"%s/%016llx (%s).cfsd", (char*)directory_utf16.c_str(), system_id, (char*)ghosts->player_name.c_str());

									std::fstream mii_stream(format, std::ios::out | std::ios::binary);

									bin_write<CFLStoreData>(&ghosts->mii_data, mii_stream, 0u, sizeof(CFLStoreData));

									mii_stream.close();
								}
							}

							for (auto& ghosts : file->course_4)
							{
								if (ghosts)
								{
									const uint64_t system_id = ghosts->mii_data.system_id;

									wchar_t format[512];
									swprintf(format, L"%s/%016llx (%s).cfsd", (char*)directory_utf16.c_str(), system_id, (char*)ghosts->player_name.c_str());

									std::fstream mii_stream(format, std::ios::out | std::ios::binary);

									bin_write<CFLStoreData>(&ghosts->mii_data, mii_stream, 0u, sizeof(CFLStoreData));

									mii_stream.close();
								}
							}
						}
					}
				}
			}
			TOOLTIP("Saves miis from all ghosts within loaded spotpass files");
		}

		ImGui::EndMenu();
	}

	ImGui::EndMainMenuBar();

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

	ImGui::Begin("Cups", 0, ImGuiWindowFlags_NoCollapse);
	ImGui::Text("%.03f Fps", ImGui::GetIO().Framerate);

	uint32_t idx = 0;
	ImGui::BeginTabBar("Loaded Cups", ImGuiTabBarFlags_AutoSelectNewTabs | ImGuiTabBarFlags_FittingPolicyResizeDown | ImGuiTabBarFlags_Reorderable);

	for (auto& _spdata : g_spotpass_files)
	{
		const std::string _label = std::format("{}{}", _spdata->edited ? "*" : "", cup_name[_spdata->cup_id]);
		const uint32_t _label_color = _spdata->edited ? IM_COL32(250, 110, 90, 178) : IM_COL32(46, 89, 148, 178);
		const uint32_t _label_active_color = _spdata->edited ? IM_COL32(250, 110, 90, 255) : IM_COL32(46, 89, 148, 255);

		ImGui::PushID(idx);
		ImGui::PushStyleColor(ImGuiCol_Tab, _label_color);
		ImGui::PushStyleColor(ImGuiCol_TabActive, _label_active_color);
		if (ImGui::BeginTabItem(_label.c_str()))
		{
			is_cup_selected = true;
			current_file = _spdata;

			ImGui::EndTabItem();
		}
		ImGui::PopStyleColor(2);
		TOOLTIP("%s%s", _spdata->file_directory.c_str(), _spdata->edited ? "\n(Modified)" : "");

		ImGui::PopID();
		idx++;
	}

	ImGui::EndTabBar();

	if (is_cup_selected)
	{
		for (int i = 0; i < 4; i++)
		{
			const uint8_t course_id = cup_courses[current_file->cup_id][i];

			ImGui::PushID(i);
			if (ImGui::Selectable(course_name[course_id], course_idx == i))
			{
				course_idx = i;
			}
			ImGui::PopID();

			ImGui::SameLine();

			const uint8_t __ghost_count = current_file->ghost_count[i];

			ImGui::PushStyleColor(ImGuiCol_Text, __ghost_count >= 20 ? ImVec4(1.0, 0.5, 0.5, 1.0) : ImVec4(0.5, 0.5, 0.5, 1.0));
			ImGui::Text("(%i/20)", __ghost_count);
			ImGui::PopStyleColor();
		}			

		if (ImGui::Button("Save"))
		{
			current_file->save(false);
		}

		this->ranking_directory_text();
		

		ImGui::NewLine(); ImGui::Separator(); ImGui::NewLine();

		if (ImGui::Button("Add Ghost"))
		{
			const char* file_path = open_file();

			if (current_file->add_ghost(course_idx, file_path) == false)
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


	ImGui::Begin("Ghost", 0, ImGuiWindowFlags_NoCollapse);
	/*
	*	Cup ghosts will be rendered here
	*/
	if (current_file)
	{
		if (ImGui::BeginTable("Ghosts", 1, ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_BordersInnerH))
		{

			std::array<std::unique_ptr<Ghost>, 20>* coursedata = current_file->get_course(course_idx);

			if (coursedata)
			{
				for (uint32_t i = 0; i < 20; i++)
				{
					std::unique_ptr<Ghost>& current_ghost = coursedata->at(i);

					if (current_ghost == nullptr)
					{
						continue;
					}

					ImGui::PushID(current_ghost.get());
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);

					draw_ghost_details(current_ghost);

					if (ImGui::Button("Delete Ghost"))
					{
						current_file->delete_ghost(course_idx, current_ghost);
					}

					if (ImGui::Button("Overwrite Ghost"))
					{
						current_file->overwrite_ghost(current_ghost, open_file());
					}

					if (ImGui::Button("Extract Ghost"))
					{
						current_file->extract_ghost(current_ghost);
					}

					if (ImGui::Button("Export Mii Data (.cfsd)"))
					{
						const uint64_t system_id = current_ghost->mii_data.system_id;

						auto file = create_file(std::format("{:016x}.cfsd", system_id).c_str(), "CTR Face Store Data (*.cfsd)\0*.cfsd\0All\0*.*\0");

						if (file)
						{
							std::fstream mii_stream(file, std::ios::out | std::ios::binary);

							bin_write<CFLStoreData>(&current_ghost->mii_data, mii_stream, (uint32_t)0, sizeof(CFLStoreData));

							mii_stream.close();
						}
					}

					ImGui::PopID();
				}
			}

			ImGui::EndTable();
		}
	}
	ImGui::End();
}

void ImGuiPanel::ranking_directory_text()
{
	if (current_file)
	{
		const char* _file_directory = current_file->file_directory.c_str();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(127, 127, 127, 255));
		ImGui::Text(_file_directory);
		ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip("Right Click to Copy Path to Clipboard");

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				ImGui::SetClipboardText(_file_directory);
		}
	}
	else
	{
		ImGui::NewLine();
	}
}

void ImGuiPanel::draw_ghost_details(std::unique_ptr<Ghost>& _ghost)
{
	/*
	*	ghost character, player name, and flag
	*/
	if (_ghost->character_id == character::CHAR_MII_MALE || _ghost->character_id == character::CHAR_MII_FEMALE)
	{
		mii_image(&_ghost->mii_data);
	}
	else
	{
		ImGui::Image(g_texture_manager.driver[_ghost->character_id], { 64, 64 });
	}
	ImGui::SameLine();
	
	if (display_flags)
	{
		nation_flag_image(_ghost->country_id);
	}

	ImGui::SameLine();
	ImGui::PushFont(g_font_rodin);
	ImGui::Text("%s", utf8_conv.to_bytes(_ghost->player_name).c_str());
	ImGui::PopFont();

	/*
	*	display ghost kart config
	*/
	ImGui::SameLine(ImGui::GetWindowWidth() / 3); ImGui::Image(g_texture_manager.body[_ghost->kart_id], { 128, 64 });
	ImGui::SameLine(); ImGui::Image(g_texture_manager.tire[_ghost->tire_id], { 128, 64 });
	ImGui::SameLine(); ImGui::Image(g_texture_manager.wing[_ghost->glider_id], { 128, 64 });

	/*
	*	first person indicator
	*/
	if (_ghost->serialized.gyro_flag())
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

	const PackedTime finished_time = _ghost->serialized.finished_time;

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
	
	if (ImGui::TreeNode("View Mii Image"))
	{
		mii_image(&_ghost->mii_data, 128);

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Header Hex View"))
	{
		render_hex_view((uint8_t*)&_ghost->serialized, sizeof(GhostRawData));

		ImGui::TreePop();
	}

	ImGui::NewLine();
}

void ImGuiPanel::mii_image(CFLStoreData* mii_data, uint16_t size)
{
	void* __texture = g_texture_manager.request_mii_texture(mii_data);
	if (__texture)
	{
		ImGui::Image(__texture, { (float)size, (float)size });
	}
	else
	{
		ImGui::Image(g_texture_manager.symbol[2], { (float)size, (float)size });
	}
}

void ImGuiPanel::nation_flag_image(char nation_id)
{
	float flag_xoffset = (26 * (nation_id % 16)) / 416.0f;
	float flag_yoffset = (18 * (round_multiple(nation_id, 17) / 17)) / 216.0f;
	ImGui::Image(g_texture_manager.symbol[0x01], { 26, 18 }, { flag_xoffset, flag_yoffset }, { flag_xoffset + (26 / 416.0f), flag_yoffset + (18 / 216.0f) });
}