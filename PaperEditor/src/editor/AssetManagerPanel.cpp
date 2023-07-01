#include "Editor.h"
#include "WindowsOpen.h"

#include "PELayer.h"

static bool displayIcon(const std::filesystem::directory_entry& item, const float size)
{
	if (item.is_directory())
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton((void*)DataPool::GetTexture("folder_icon.png")->GetID(), ImVec2(size, size), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, 0))
		{
			ImGui::PopStyleColor();
			return true;
		}
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		if (ImGui::ImageButton((void*)DataPool::GetTexture("file_icon.png")->GetID(), ImVec2(size, size), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, 0))
		{
			ImGui::PopStyleColor();
			return true;
		}
		ImGui::PopStyleColor();
	}
	return false;
}

static void displayText(std::vector<std::string>& filenames)
{
	ImGui::TableNextRow();
	int i = 0;
	for (std::string name : filenames)
	{
		ImGui::TableSetColumnIndex(i);
		ImGui::TextWrapped(filenames.at(i).c_str());
		i++;
	}
}

void PELayer::AssetManagerPanel()
{
	static bool first = true;
	static float size = 120.0f;
	static std::filesystem::path path = "assets/";

	//std::filesystem::path itemPath;
	bool itemClicked = false;

	const char* name = "Asset Manager: ";
	std::stringstream stream;

	if (first)
		DockPanel(name, GetDockspaceBottom());

	const ImGuiTableFlags flags = ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX;

	ImGui::Begin(name, &show_asset_manager_panel);
	std::stringstream ss;
	for (const auto& pathPart : path)
	{
		if (pathPart.string().empty()) continue;
		ss << pathPart.string() << "/";
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.5, 3));
		if (ImGui::Button(pathPart.filename().string().c_str()))
		{
			path = ss.str();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			break;
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::SameLine();
		ImGui::Text("/");
		ImGui::SameLine();
	}
	ImGui::Text("");

	int cols = size > (ImGui::GetContentRegionAvail().x) ? 1 : ((int)ImGui::GetContentRegionAvail().x) / size;
	if (cols > 64) cols = 64;

	if (ImGui::BeginTable("dirTable", cols, flags, ImVec2(cols * size, 0.0f)))
	{
		ImGui::TableSetupColumn("dirCol");

		ImGui::TableNextRow();
		std::vector<std::string> filenames;

		while (!exists(path))
		{
			path = path.parent_path();
			if (path == "assets") break;
		}
		int i = 0;
		for (const auto item : std::filesystem::directory_iterator(path))
		{
			if (i >= cols)
			{
				i = 0;

				displayText(filenames);
				ImGui::TableNextRow();
				filenames.clear();
			}
			ImGui::TableSetColumnIndex(i);

			ImGui::PushID(item.path().filename().string().c_str());

			if (displayIcon(item, size))
			{
				if (item.is_directory())
					path = item.path();
				else
				{
					//itemPath = path.string() + "/" + item.path().filename().string();
					itemClicked = true;
				}
			}

			if (!item.is_directory() && ImGui::BeginDragDropSource())
			{
				const wchar_t* item_path = item.path().c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopID();

			filenames.push_back(item.path().filename().string());

			i++;
		}

		displayText(filenames);
		ImGui::EndTable();
	}

	if (itemClicked)
	{
		//LOG_TRACE(itemPath);
	}

	ImGui::End();

	first = false;
}