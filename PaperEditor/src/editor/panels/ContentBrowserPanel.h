#pragma once
#include "EditorPanel.h"

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


	protected:
		ItemType type;

	};

	class ContentBrowserDir : public ContentBrowserItem
	{
		
	};

	class ContentBrowserAsset : public ContentBrowserItem
	{
		
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

