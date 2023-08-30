#pragma once
#include "EditorPanel.h"

namespace PaperED
{
	class AssetManagerPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;
	};
}

