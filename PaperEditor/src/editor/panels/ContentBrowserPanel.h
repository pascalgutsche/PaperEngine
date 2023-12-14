#pragma once
#include "EditorPanel.h"
#include "asset/Asset.h"

namespace PaperED
{
	enum class ActionResult
	{
		None		= 0,
		Activated	= BIT(0),
		Selected	= BIT(1),
		Hovered		= BIT(2),
		Reload		= BIT(3),
		Delete		= BIT(4),
		Copy		= BIT(5),
		Duplicate	= BIT(6),
		ShowInExplorer	= BIT(7),
		OpenExternally	= BIT(8),
	};

	struct CBActionResult
	{
		uint32_t flags = (uint32_t)ActionResult::None;

		void Set(ActionResult flag, bool value)
		{
			if (value)
				flags |= (uint32_t)flag;
			else
				flags &= ~(uint32_t)flag;
		}

		bool IsSet(ActionResult flag) const
		{
			return flags & (uint32_t)flag;
		}
	};

	class ContentBrowserItem : public ShrCounted
	{
	public:
		enum class ItemType
		{
			Directory,
			Asset
		};

		ContentBrowserItem(ItemType type, AssetHandle handle, std::string name, Ref<Texture> icon);
		virtual ~ContentBrowserItem() = default;

		CBActionResult Render() const;

		ItemType GetItemType() const { return itemType; }
		AssetHandle GetAssetHandle() const { return itemID; }
		const std::string& GetName() const { return itemName; }
		const Ref<Texture>& GetIcon() const { return itemIcon; }

		bool operator<(const ContentBrowserItem& other) const;

	protected:
		ItemType itemType;
		AssetHandle itemID;
		std::string itemName;
		Ref<Texture> itemIcon;

	private:
		void HandleItemActions(CBActionResult& result) const;
		void RenderContextMenu(CBActionResult& result) const;
		void RenderShadow(const ImVec2& topLeft, const ImVec2& bottomRight) const;
	};

	struct DirectoryInfo : public ShrCounted
	{
		AssetHandle handle;

		std::filesystem::path path;

		Shr<DirectoryInfo> parent;
		std::unordered_map<AssetHandle, Shr<DirectoryInfo>> subdirs;

		std::vector<AssetHandle> assets;
	};

	class ContentBrowserDir : public ContentBrowserItem
	{
	public:
		ContentBrowserDir(const Shr<DirectoryInfo>& dirInfo);
		~ContentBrowserDir() override = default;
		
		const Shr<DirectoryInfo>& GetDirInfo() const { return dirInfo; }

	private:
		Shr<DirectoryInfo> dirInfo;
	};

	class ContentBrowserAsset : public ContentBrowserItem
	{
	public:
		ContentBrowserAsset(AssetMetadata& metadata, Ref<Texture> icon);
		~ContentBrowserAsset() = default;

		const AssetMetadata& GetAssetMetadata() const { return metadata; }
	private:
		AssetMetadata metadata;
	};

	struct ContentBrowserItemList
	{
		struct less_than_key
		{
			inline bool operator() (const Shr<ContentBrowserItem>& item1, const Shr<ContentBrowserItem>& item2) const
			{
				return *item1 < *item2;
			}
		};

		const static size_t InvalidItem = std::numeric_limits<size_t>::max();

		std::vector<Shr<ContentBrowserItem>> items;

		std::vector<Shr<ContentBrowserItem>>::iterator begin() { return items.begin(); }
		std::vector<Shr<ContentBrowserItem>>::iterator end() { return items.end(); }
		std::vector<Shr<ContentBrowserItem>>::const_iterator begin() const { return items.begin(); }
		std::vector<Shr<ContentBrowserItem>>::const_iterator end() const { return items.end(); }

		Shr<ContentBrowserItem>& operator[](size_t index) { return items[index]; }
		const Shr<ContentBrowserItem>& operator[](size_t index) const { return items[index]; }

		ContentBrowserItemList() = default;

		ContentBrowserItemList(const ContentBrowserItemList& other)
			: items(other.items)
		{
			
		}

		void Clear()
		{
			items.clear();
		}

		bool Erase(AssetHandle handle)
		{
			size_t index = FindIndex(handle);
			if (index == InvalidItem)
				return false;

			return items.erase(items.begin() + index) != items.end();
		}

		size_t FindIndex(AssetHandle handle)
		{
			if (items.empty())
				return InvalidItem;

			for (size_t i = 0; i < items.size(); i++)
			{
				if (items[i]->GetAssetHandle() == handle)
					return i;
			}

			return InvalidItem;
		}
	};

	class ContentBrowserPanel : public EditorPanel
	{
	public:
		ContentBrowserPanel();
		void OnImGuiRender(bool& isOpen) override;

		void OnProjectChanged(const Ref<Project>& project) override;


		Shr<DirectoryInfo> GetDirInfo(AssetHandle handle);
		Shr<DirectoryInfo> GetDirInfo(const std::filesystem::path& path);

	private:
		void RenderHeader();

		int BeginTable();
		void EndTable();

		AssetHandle ProcessDir(const std::filesystem::path& path, const Shr<DirectoryInfo>& parent);

		void SetDir(const Shr<DirectoryInfo>& directoryInfo);

		void ChangeDir(const Shr<DirectoryInfo>& directoryInfo);

		void BackDir();
		void ForwardDir();

		void Refresh();
		void RefreshHistory(std::vector<Shr<DirectoryInfo>>& stack);

		Ref<Project> project;

		std::string searchbuffer = "";

		std::unordered_map<AssetHandle, Shr<DirectoryInfo>> directories;

		ContentBrowserItemList currentItemList;

		Shr<DirectoryInfo> baseDirInfo;
		Shr<DirectoryInfo> currentDirInfo;
		std::vector<Shr<DirectoryInfo>> nextDirInfoStack;
		std::vector<Shr<DirectoryInfo>> prevDirInfoStack;
	};
}

