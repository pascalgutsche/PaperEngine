#include "Editor.h"
#include "ContentBrowserPanel.h"

#include "imgui/ImGuiFont.h"
#include "project/Project.h"
#include "renderer/Font.h"

#include "imgui/ImGuiLayer.h"
#include "imgui/imgui_internal.h"
#include "imgui/UI.h"

namespace PaperED
{
	constexpr int panelWidth = 128 + (2 * 5); //padding left-right 5px
	
	// ===========================================
	//	ContentBrowserItem
	// ===========================================

	ContentBrowserItem::ContentBrowserItem(ItemType type, AssetHandle handle, std::string name, Ref<Texture> icon)
		: itemType(type), itemID(handle), itemName(name), itemIcon(icon)
	{
		
	}

	CBActionResult ContentBrowserItem::Render() const
	{
		CBActionResult result;

		const float thumbnailSize = 128.0f;

		const float textLineHeight = ImGui::GetTextLineHeightWithSpacing();
		const float infoPanelHeight = textLineHeight * 2.8f;

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
			drawList->AddRectFilled(topLeft, thumbnailBottomRight, Colors::Theme::background, 6.0f, ImDrawFlags_RoundCornersTop);
			drawList->AddRectFilled(infoTopLeft, bottomRight, Colors::Theme::backgroundLight, 6.0f, ImDrawFlags_RoundCornersBottom);
		}

		//draw thumbnail
		ImVec2 size = { bottomRight.x - topLeft.x, bottomRight.y - topLeft.y };
		bool pressed = ImGui::InvisibleButton("lol", size);
		UI::ImageEffects(itemIcon, ImRect(topLeft, thumbnailBottomRight), ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
			ImVec4(0.8f, 0.8f, 0.8f, 1.0f),
			ImVec4(1.0f, 0.8f, 0.8f, 1.0f), ImRect(topLeft, bottomRight));

		if (ImGui::IsItemHovered())
		{
			result.Set(ActionResult::Hovered, true);
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				result.Set(ActionResult::Activated, true);
			else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				result.Set(ActionResult::Selected, true);
		}

		//draw infobox
		ImVec2 screen = ImGui::GetCursorScreenPos();
		//LOG_DEBUG("InfoTopLeft: {}, {}; Cursor: {} {}", infoTopLeft.x, infoTopLeft.y, cursor.x, cursor.y);

		//ImGui::SetCursorPos(infoTopLeft);

		UI::ShiftCursorY(-70);

		float panelLength = bottomRight.x - topLeft.x;

		float textLength = std::min(ImGui::CalcTextSize(itemName.c_str()).x, thumbnailSize - 4);

		float textCoord = (panelLength / 2) - (textLength / 2);

		std::string shortenedName = itemName;
		if (itemName.size() > 24)
		{
			shortenedName = itemName.substr(0, 24) + " ...";
		}

		if (itemType == ItemType::Directory)
			UI::ShiftCursorX(textCoord);

		UI::ShiftCursor(4.0f, 4.0f);

		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - 4 * 3.0f));

		ImGui::Text(shortenedName.c_str());

		ImGui::PopTextWrapPos();

		
		if (itemType == ItemType::Asset)
		{
			//ImGui::SetNextItemWidth(textLength);
			

			ImGuiFont::PushFont("Small");
			const AssetMetadata& metadata = Project::GetEditorAssetManager()->GetMetadata(itemID);
			if (metadata.IsValid())
			{
				std::string assetText = Utils::ConvertAssetTypeToString(metadata.type);

				assetText = Utils::ToUpper(assetText);
				ImVec2 assetTextLength = ImGui::CalcTextSize(assetText.c_str());

				ImGui::SetCursorScreenPos({ bottomRight.x - assetTextLength.x - 5, bottomRight.y - assetTextLength.y - 2 });
				ImGui::Text(assetText.c_str());
			}
			
			ImGuiFont::PopFont();
		}
		//*/

		ImGui::EndGroup();

		return result;
	}

	bool ContentBrowserItem::operator<(const ContentBrowserItem& other) const
	{
		std::string thisName = itemName;
		std::string otherName = other.itemName;

		ASSERT(itemName.size(), "");
		ASSERT(other.itemName.size(), "");
		LOG_DEBUG("sorting");
		int index = 0;
		while ((thisName[index] == otherName[index]))
		{
			index++;

			if (index < thisName.size())
			{
				LOG_DEBUG("sorting1");
				return true;
			}
			if (index < otherName.size())
			{
				LOG_DEBUG("sorting2");
				return false;
			}
		}
		//if (index < )

		LOG_DEBUG("sorting3");

		return false;
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
			metadata.filePath.filename().stem().string(),
			icon)
	{
	}

	// ===========================================
	//	ContentBrowserPanel
	// ===========================================

	ContentBrowserPanel::ContentBrowserPanel()
	{
	}

	//Render shit

	void ContentBrowserPanel::OnImGuiRender(bool& isOpen)
	{
		if (!ImGui::Begin(panelName.c_str(), &isOpen))
		{
			ImGui::End();
			return;
		}

		//Header
		RenderHeader();

		int columnCount = BeginTable();

		for (const auto& item : currentItemList)
		{
			CBActionResult result = item->Render();

			if (result.IsSet(ActionResult::Activated))
			{
				if (item->GetItemType() == ContentBrowserItem::ItemType::Directory)
				{
					ChangeDir(item.As<ContentBrowserDir>()->GetDirInfo());
					break;
				}


			}

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

#if 0
		ImGui::Begin("dirStack");

		std::stack tempPrevStack(prevDirInfoStack);
		std::stack tempNextStack(nextDirInfoStack);

		ImGui::BeginTable("##stack_table", 2, ImGuiTableFlags_Borders);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		while (tempPrevStack.size() > 0)
		{
			std::string dir = tempPrevStack.top()->path.filename().string();
			if (dir.empty())
				dir = "assets";
			ImGui::Text(dir.c_str());
			tempPrevStack.pop();
		}

		ImGui::TableNextColumn();

		while (tempNextStack.size() > 0)
		{
			std::string dir = tempNextStack.top()->path.filename().string();
			if (dir.empty())
				dir = "assets";
			ImGui::Text(dir.c_str());
			tempNextStack.pop();
		}

		ImGui::EndTable();

		ImGui::End();
#endif
	}

	

	void ContentBrowserPanel::RenderHeader()
	{
		float height = 26;
		auto contenBrowserButton = [](const char* labelId, const Ref<Texture>& icon, float iconSize, bool disabled = false)
		{
			ImGui::BeginDisabled(disabled);
			const bool clicked = UI::ImageButton(icon, ImVec2(iconSize, iconSize));
			ImGui::EndDisabled();
			return clicked;
		};

		const float iconSize = std::min(24.0f, height);

		if (contenBrowserButton("##back", DataPool::GetTexture("resources/editor/contentbrowser/back.png", true), iconSize, prevDirInfoStack.empty()))
		{
			BackDir();
		};

		ImGui::SameLine();

		if (contenBrowserButton("##prev", DataPool::GetTexture("resources/editor/contentbrowser/forward.png", true), iconSize, nextDirInfoStack.empty()))
		{
			ForwardDir();
		};

		ImGui::SameLine();

		if (contenBrowserButton("##parent", DataPool::GetTexture("resources/editor/contentbrowser/up.png", true), iconSize, !currentDirInfo->parent))
		{
			ChangeDir(currentDirInfo->parent);
		};

		//path display
		std::vector<Shr<DirectoryInfo>> pathDirInfo;
		Shr<DirectoryInfo> temp = currentDirInfo;
		while (temp && temp->parent != nullptr)
		{
			pathDirInfo.push_back(temp);
			temp = temp->parent;
		}

		std::reverse(pathDirInfo.begin(), pathDirInfo.end());

		std::string assetDirName = project->GetConfig().assetPath.string();
		ImVec2 textSize = ImGui::CalcTextSize(assetDirName.c_str());

		ImGui::SameLine();
		if (ImGui::Selectable(assetDirName.c_str(), false, 0, textSize))
		{
			ChangeDir(baseDirInfo);
		}

		for (auto& dir : pathDirInfo)
		{
			ImGui::SameLine();
			ImGui::Text("/");
			ImGui::SameLine();

			std::string folderName = dir->path.filename().string();
			ImVec2 textSize = ImGui::CalcTextSize(folderName.c_str());

			if (ImGui::Selectable(folderName.c_str(), false, 0, textSize))
			{
				ChangeDir(dir);
			}
		}

		//search
		const float searchIconWidth = iconSize;
		const float searchWidgetWidth = 250.0f;
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - searchWidgetWidth - 2.0f - searchIconWidth - 5.0f - iconSize);
		ImGui::Image((void*)DataPool::GetTexture("resources/editor/contentbrowser/search.png", true)->GetID(), ImVec2(searchIconWidth, searchIconWidth), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::SameLine(ImGui::GetContentRegionAvail().x - searchWidgetWidth - iconSize - 5.0f); // 5.0f = padding
		ImGui::SetNextItemWidth(searchWidgetWidth);
		if (UI::StringControl("##search", searchbuffer))
		{
			SetDir(currentDirInfo);
		};

		//refresh
		ImGui::SameLine(ImGui::GetContentRegionAvail().x - iconSize);

		if (contenBrowserButton("##reload", DataPool::GetTexture("resources/editor/contentbrowser/reload.png", true), iconSize))
		{
			Refresh();
		};


	}

	//utilities overrides

	void ContentBrowserPanel::OnProjectChanged(const Ref<Project>& project)
	{
		this->project = project;
		directories.clear();
		AssetHandle baseDirHandle = ProcessDir(project->GetAssetPath(), nullptr);
		baseDirInfo = directories[baseDirHandle];

		SetDir(baseDirInfo);

	}

	Shr<DirectoryInfo> ContentBrowserPanel::GetDirInfo(AssetHandle handle)
	{
		if (!directories.contains(handle))
			return nullptr;
		return directories[handle];
	}

	Shr<DirectoryInfo> ContentBrowserPanel::GetDirInfo(const std::filesystem::path& path)
	{
		for (Shr<DirectoryInfo> dir : directories | std::views::values)
		{
			if (dir->path == path)
				return dir;
		}
		return nullptr;
	}

	//Directory shit

	AssetHandle ContentBrowserPanel::ProcessDir(const std::filesystem::path& path, const Shr<DirectoryInfo>& parent)
	{

		Shr<DirectoryInfo> dirInfo = Shr<DirectoryInfo>::Create();
		dirInfo->handle = AssetHandle();
		dirInfo->parent = parent;
		
		if (path == project->GetAssetPath())
		{
			dirInfo->path = "";
		}
		else
		{
			dirInfo->path = std::filesystem::relative(path, project->GetAssetPath());
		}
		
		for (std::filesystem::directory_entry entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_directory())
			{
				AssetHandle subDirHandle = ProcessDir(entry.path(), dirInfo);
				dirInfo->subdirs[subDirHandle] = directories[subDirHandle];
				continue;
			}
		
			const AssetMetadata& metadata = Project::GetEditorAssetManager()->GetMetadata(entry.path());
			if (!metadata.IsValid())
			{
				AssetHandle handle = Project::GetEditorAssetManager()->AddAsset(entry.path());
				dirInfo->assets.push_back(handle);
			}
			else
				dirInfo->assets.push_back(metadata.handle);
		}
		
		directories[dirInfo->handle] = dirInfo;
		
		return dirInfo->handle;
	}

	void ContentBrowserPanel::SetDir(const Shr<DirectoryInfo>& directoryInfo)
	{
		if (!directoryInfo)
			return;
		
		currentItemList.Clear();
		
		for (const auto [handle, subDirInfos] : directoryInfo->subdirs)
		{
			currentItemList.items.push_back(Shr<ContentBrowserDir>::Create(subDirInfos));
		}
		
		for (AssetHandle handle : directoryInfo->assets)
		{
			AssetMetadata metadata = Project::GetEditorAssetManager()->GetMetadata(handle);
		
			currentItemList.items.push_back(Shr<ContentBrowserAsset>::Create(metadata, DataPool::GetTexture("file_icon.png")));
		}

		if (!searchbuffer.empty())
		{
			std::vector<AssetHandle> handlesToRemove;
			for (size_t i = 0; i < currentItemList.items.size(); i++)
			{
				std::string name = Utils::ToLower(currentItemList.items[i]->GetName());
				size_t index = name.find(Utils::ToLower(searchbuffer));
				LOG_DEBUG("{}: {}", currentItemList.items[i]->GetName(), index);
				if (index == std::string::npos)
				{
					handlesToRemove.push_back(currentItemList[i]->GetAssetHandle());
				}
			}
			for (AssetHandle handle : handlesToRemove)
			{
				currentItemList.Erase(handle);
			}
			LOG_DEBUG("");
		}

		std::sort(currentItemList.begin(), currentItemList.end(), [](const Shr<ContentBrowserItem>& item1, const Shr<ContentBrowserItem>& item2)
		{
			if (item1->GetItemType() != item2->GetItemType())
				return item1->GetItemType() == ContentBrowserItem::ItemType::Directory;
			return (strcmp(item1->GetName().c_str(), item2->GetName().c_str()) < 0);
		});

		

		currentDirInfo = directoryInfo;
	}

	void ContentBrowserPanel::ChangeDir(const Shr<DirectoryInfo>& directoryInfo)
	{
		prevDirInfoStack.push_back(currentDirInfo);
		SetDir(directoryInfo);
	}

	void ContentBrowserPanel::BackDir()
	{
		if (prevDirInfoStack.empty())
			return;
		if (nextDirInfoStack.empty() || currentDirInfo != nextDirInfoStack.back())
			nextDirInfoStack.push_back(currentDirInfo);
		SetDir(Utils::TopAndPop(prevDirInfoStack));
	}

	void ContentBrowserPanel::ForwardDir()
	{
		if (nextDirInfoStack.empty())
			return;
		if (prevDirInfoStack.empty() || currentDirInfo != prevDirInfoStack.back())
			prevDirInfoStack.push_back(currentDirInfo);
		SetDir(Utils::TopAndPop(nextDirInfoStack));
	}

	void ContentBrowserPanel::Refresh()
	{
		currentItemList.Clear();
		directories.clear();

		Shr<DirectoryInfo> tempCurrentDir = currentDirInfo;
		AssetHandle newBaseDirHandle = ProcessDir(Project::GetAssetPath(), nullptr);
		baseDirInfo = directories[newBaseDirHandle];

		//if directory was removed choose the next parent that exists -> until baseDir
		currentDirInfo = nullptr;
		while (currentDirInfo == nullptr)
		{
			currentDirInfo = GetDirInfo(tempCurrentDir->path);
			tempCurrentDir = tempCurrentDir->parent;
			if (!tempCurrentDir)
				break;
		}
		if (!currentDirInfo)
			currentDirInfo = baseDirInfo;

		//refresh history
		RefreshHistory(prevDirInfoStack);
		RefreshHistory(nextDirInfoStack);

		SetDir(currentDirInfo);
	}

	void ContentBrowserPanel::RefreshHistory(std::vector<Shr<DirectoryInfo>>& stack)
	{
		for (int i = 0; i < stack.size(); i++)
		{
			Shr<DirectoryInfo> newDirInfo = GetDirInfo(stack[i]->path);
			if (!newDirInfo)
			{
				stack.erase(stack.begin() + i);
				i--;
				continue;
			}
			stack[i] = newDirInfo;
		}
	}


	//Table shit

	int ContentBrowserPanel::BeginTable()
	{
		int columnCount = ImGui::GetContentRegionAvail().x / panelWidth;
		if (columnCount < 1)
			columnCount = 1;
		//ImGui::Text(std::to_string(columnCount).c_str());
		ImGui::BeginTable("##contentbrowser", columnCount, 0, ImVec2(columnCount * panelWidth, 0));
		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		return columnCount;
	}

	void ContentBrowserPanel::EndTable()
	{
		ImGui::EndTable();
	}
	
}
