#include "Editor.h"
#include "ContentBrowserPanel.h"

#include "project/Project.h"
#include "renderer/Font.h"

#include "imgui/ImGuiLayer.h"
#include "imgui/imgui_internal.h"
#include "imgui/UI.h"

namespace PaperED
{
	ContentBrowserItem::ContentBrowserItem(ItemType type, AssetHandle handle, std::string name, Ref<Texture> icon)
		: itemType(type), itemID(handle), itemName(name), itemIcon(icon)
	{
	}

	void ContentBrowserItem::Render()
	{
		const float thumbnailSize = 128.0f;

		const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
		const float infoPanelHeight = textLineHeight * 1.6;

		ImVec2 topLeft = ImGui::GetCursorScreenPos();
		ImVec2 thumbnailBottomRight = { topLeft.x + thumbnailSize, topLeft.y + thumbnailSize };
		ImVec2 infoTopLeft = { topLeft.x, topLeft.y + thumbnailSize };
		ImVec2 bottomRight = { topLeft.x + thumbnailSize, topLeft.y + thumbnailSize + infoPanelHeight};

		ImVec2 cursor = ImGui::GetMousePos();

		//LOG_DEBUG("TopLeft: {}, {}; BottomRight: {}, {}; Cursor: {} {}", topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, cursor.x, cursor.y);

		auto drawShadow = [](const ImVec2& topLeft, const ImVec2& bottomRight, bool directory)
		{
			auto* drawList = ImGui::GetWindowDrawList();
			const ImRect itemRect = UI::RectOffset(ImRect(topLeft, bottomRight), 1.0f, 1.0f);
			drawList->AddRect(itemRect.Min, itemRect.Max, Colors::Theme::propertyField, 6.0f, directory ? 0 : ImDrawFlags_RoundCornersAll, 6.0f);
		};

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawShadow(topLeft, bottomRight, false);
		//drawList->AddRectFilled(topLeft, bottomRight, Colors::Theme::backgroundDark);
		drawList->AddRectFilled(topLeft, bottomRight, Colors::Theme::background, 6.0f, ImDrawFlags_RoundCornersAll);

		//draw thumbnail

		UI::ImageButton(itemIcon, IM_COL32(255, 255, 255, 225),
			IM_COL32(200, 200, 200, 255),
			IM_COL32(255, 200, 200, 255),
			ImVec2(thumbnailSize, thumbnailSize));

		//draw infobox

		if (itemType == ItemType::Directory)
		{
			ImGui::BeginVertical((std::string("InfoPanel") + m_DisplayName).c_str(), ImVec2(thumbnailSize - edgeOffset * 2.0f, infoPanelHeight - edgeOffset));
			{
				// Centre align directory name
				ImGui::BeginHorizontal(m_FileName.c_str(), ImVec2(thumbnailSize - 2.0f, 0.0f));
				ImGui::Spring();
				{
					ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 3.0f));
					const float textWidth = std::min(ImGui::CalcTextSize(m_DisplayName.c_str()).x, thumbnailSize);
					if (m_IsRenaming)
					{
						ImGui::SetNextItemWidth(thumbnailSize - edgeOffset * 3.0f);
						renamingWidget();
					}
					else
					{
						ImGui::SetNextItemWidth(textWidth);
						ImGui::Text(m_DisplayName.c_str());
					}
					ImGui::PopTextWrapPos();
				}
				ImGui::Spring();
				ImGui::EndHorizontal();

				ImGui::Spring();
			}
			ImGui::EndVertical();
		}
	}

	void ContentBrowserPanel::OnImGuiRender(bool& isOpen)
	{
		if (!ImGui::Begin(panelName.c_str(), &isOpen))
		{
			ImGui::End();
			return;
		}

		ContentBrowserItem item(ContentBrowserItem::ItemType::Asset, PaperID(), "Bunker", DataPool::GetTexture("file_icon.png"));
		item.Render();


		ImGui::End();
	}

	void ContentBrowserPanel::OnProjectChanged(const Ref<Project>& project)
	{
		
	}


	/*
	enum FileType
	{
		Texture,
		Font,
		UNDEFINED
	};

	FileType isItemType(const std::filesystem::directory_entry& item)
	{
		if (item.path().extension() == ".png" || item.path().extension() == ".jpg")
			return FileType::Texture;
		if (item.path().extension() == ".ttf")
			return FileType::Font;
		return FileType::UNDEFINED;
	}

	static bool displayIcon(const std::filesystem::directory_entry& item, const float size)
	{
		if (item.is_directory())
		{
			UI::ScopedColour button_col(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton((void*)DataPool::GetTexture("folder_icon.png")->GetID(), ImVec2(size, size), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, 0))
				return true;
		}
		else
		{
			UI::ScopedColour button_col(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			void* textureID = (void*)DataPool::GetTexture("file_icon.png")->GetID();

			if (isItemType(item) == FileType::Texture)
				textureID = (void*)DataPool::GetAssetTexture(item.path().string(), true)->GetID();

			if (ImGui::ImageButton(textureID, ImVec2(size, size), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, 0))
				return true;
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

	void ContentBrowserPanel::OnImGuiRender(bool& isOpen)
	{
		static float size = 120.0f;

		bool itemClicked = false;

		const ImGuiTableFlags flags = ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_NoPadOuterX;

		ImGui::Begin(panelName.c_str(), &isOpen);

	if (!Project::GetActive())
		{
			ImGui::End();
			return;
		}

		std::stringstream ss;
		for (const auto& pathPart : currentAssetsPath)
		{
			if (pathPart.string().empty()) continue;
			ss << pathPart.string() << "/";
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.5, 3));
			if (ImGui::Button(pathPart.filename().string().c_str()))
			{
				currentAssetsPath = ss.str();
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

			while (!std::filesystem::exists(Project::GetProjectPath() / currentAssetsPath))
			{
				currentAssetsPath = currentAssetsPath.parent_path();
				if (currentAssetsPath == Project::GetActive()->GetConfig().assetPath) break;
			}
			int i = 0;
			for (const auto item : std::filesystem::directory_iterator(Project::GetProjectPath() / currentAssetsPath))
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
					{
						currentAssetsPath = Project::GetRelativePathFromProject(item.path());
					}
					else
					{
						//itemPath = path.string() + "/" + item.path().filename().string();
						itemClicked = true;
					}
				}

				if (!item.is_directory() && ImGui::BeginDragDropSource())
				{
					const wchar_t* item_path = item.path().c_str();
					switch (isItemType(item))
					{
						case FileType::Texture:
							ImGui::SetDragDropPayload("TEXTURE", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));
							ImGui::Image((void*)DataPool::GetAssetTexture(item.path().string(), true)->GetID(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));
							break;

						case FileType::Font:
							ImGui::SetDragDropPayload("FONT", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));
							ImGui::Text(DataPool::GetFont(item.path().string(), true)->GetFontName().c_str());
							break;

						case FileType::UNDEFINED:
							ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));
							break;
					}


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
	}

	void ContentBrowserPanel::OnProjectChanged(const Ref<Project>& project)
	{
		currentAssetsPath = project->GetConfig().assetPath;
	}
	*/
}
