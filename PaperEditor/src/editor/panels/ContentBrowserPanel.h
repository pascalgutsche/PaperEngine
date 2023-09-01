#pragma once
#include "EditorPanel.h"

namespace PaperED
{
	class ContentBrowserPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;

		void OnProjectChanged(const Shr<Project>& project) override;

	private:
		std::filesystem::path currentAssetsPath = "";
	};
}

