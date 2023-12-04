#pragma once
#include "EditorPanel.h"
#include "asset/Asset.h"

namespace PaperED
{

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

		void Render();

	protected:
		ItemType itemType;
		AssetHandle itemID;
		std::string itemName;
		Ref<Texture> itemIcon;
	};

	class ContentBrowserDir : public ContentBrowserItem
	{
		
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

	class ContentBrowserPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;

		void OnProjectChanged(const Ref<Project>& project) override;

	private:
		std::filesystem::path currentAssetsPath = "";
	};
}

