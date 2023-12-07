#include "Editor.h"
#include "ContentBrowserPanel.h"
#include "ContentBrowserPanel.h"
#include "ContentBrowserPanel.h"

#include "project/Project.h"
#include "renderer/Font.h"

#include "imgui/ImGuiLayer.h"
#include "imgui/imgui_internal.h"
#include "imgui/UI.h"

namespace PaperED
{
	// ===========================================
	//	ContentBrowserItem
	// ===========================================

	ContentBrowserItem::ContentBrowserItem(ItemType type, AssetHandle handle, std::string name, Ref<Texture> icon)
		: itemType(type), itemID(handle), itemName(name), itemIcon(icon)
	{
	}

	void ContentBrowserItem::Render()
	{
		const float thumbnailSize = 128.0f;

		const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
		const float infoPanelHeight = textLineHeight * 1.8f;

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

		ImGui::BeginGroup();

		
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		//drawList->AddRectFilled(topLeft, bottomRight, Colors::Theme::backgroundDark);
		if (itemType == ItemType::Asset)
		{
			drawShadow(topLeft, bottomRight, false);
			drawList->AddRectFilled(topLeft, bottomRight, Colors::Theme::background, 6.0f, ImDrawFlags_RoundCornersAll);
		}

		//draw thumbnail
		ImVec2 size = { bottomRight.x - topLeft.x, bottomRight.y - topLeft.y };
		bool pressed = ImGui::InvisibleButton("lol", size);
		UI::ImageEffects(itemIcon, ImRect(topLeft, thumbnailBottomRight), ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
			ImVec4(0.8f, 0.8f, 0.8f, 1.0f),
			ImVec4(1.0f, 0.8f, 0.8f, 1.0f), ImRect(topLeft, bottomRight));

		//draw infobox
		ImVec2 screen = ImGui::GetCursorScreenPos();
		//LOG_DEBUG("InfoTopLeft: {}, {}; Cursor: {} {}", infoTopLeft.x, infoTopLeft.y, cursor.x, cursor.y);

		//ImGui::SetCursorPos(infoTopLeft);

		UI::ShiftCursorY(-50);

		float panelLength = bottomRight.x - topLeft.x;

		float textLength = ImGui::CalcTextSize(itemName.c_str()).x;

		float textCoord = (panelLength / 2) - (textLength / 2);


		UI::ShiftCursorX(textCoord);

		ImGui::Text(itemName.c_str());

		/* AssetText
		std::string assetText = "SCENE";
		ImVec2 assetTextLength = ImGui::CalcTextSize(assetText.c_str());

		ImGui::SetCursorScreenPos({ bottomRight.x - assetTextLength.x - 5, bottomRight.y - assetTextLength.y - 2 });
		ImGui::Text(assetText.c_str());
		*/

		ImGui::EndGroup();
	}

	// ===========================================
	//	ContentBrowserDir
	// ===========================================

	ContentBrowserDir::ContentBrowserDir(const Shr<DirectoryInfo>& dirInfo)
		: ContentBrowserItem(ItemType::Directory, 
			dirInfo->handle, 
			dirInfo->path.filename().string(), 
			DataPool::GetTexture("folder_icon2.png")
		),
		dirInfo(dirInfo)
	{

	}

	// ===========================================
	//	ContentBrowserAsset
	// ===========================================

	ContentBrowserAsset::ContentBrowserAsset(AssetMetadata& metadata, Ref<Texture> icon)
		: ContentBrowserItem(ItemType::Asset,
			metadata.handle,
			metadata.filePath.filename().string(),
			icon)
	{
	}

	// ===========================================
	//	ContentBrowserPanel
	// ===========================================

	ContentBrowserPanel::ContentBrowserPanel()
	{
	}

	void ContentBrowserPanel::OnImGuiRender(bool& isOpen)
	{
		if (!ImGui::Begin(panelName.c_str(), &isOpen))
		{
			ImGui::End();
			return;
		}

		int columnCount = BeginTable();

		for (int i = 0; i < 20; i++)
		{
			

			if (ImGui::TableGetColumnIndex() >= columnCount)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
			}
			else
				ImGui::TableNextColumn();
		}

		EndTable();

		ImGui::End();
	}

	void ContentBrowserPanel::OnProjectChanged(const Ref<Project>& project)
	{
		this->project = project;
		ProcessDir(project->GetProjectPath(), nullptr);
	}

	int ContentBrowserPanel::BeginTable()
	{
		int columnCount = ImGui::GetContentRegionAvail().x / panelWidth;
		if (columnCount < 1)
			columnCount = 1;
		ImGui::Text(std::to_string(columnCount).c_str());
		ImGui::BeginTable("##contentbrowser", columnCount, 0, ImVec2(columnCount * panelWidth + 2, 0));
		ImGui::TableNextColumn();
		ImGui::TableNextRow();

		return columnCount;
	}

	void ContentBrowserPanel::EndTable()
	{
		ImGui::EndTable();
	}

	AssetHandle ContentBrowserPanel::ProcessDir(const std::filesystem::path& path, const Shr<DirectoryInfo>& parent)
	{

		Shr<DirectoryInfo> dirInfo = Shr<DirectoryInfo>::Create();
		dirInfo->handle = AssetHandle();
		dirInfo->parent = parent;

		if ()

		for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
		{
			
		}

		return AssetHandle();
	}
}
