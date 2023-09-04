#pragma once
#include "EditorPanel.h"

namespace PaperED
{
	class SettingsPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;
	};
}
