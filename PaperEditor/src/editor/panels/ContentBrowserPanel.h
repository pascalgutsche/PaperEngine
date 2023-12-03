#pragma once
#include "EditorPanel.h"

namespace PaperED
{
	class ContentBrowserPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;

		void OnProjectChanged(const Ref<Project>& project) override;

	private:
		std::filesystem::path currentAssetsPath = "";
	};
}

