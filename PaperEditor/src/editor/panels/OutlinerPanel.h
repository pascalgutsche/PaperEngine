#pragma once
#include "EditorPanel.h"

namespace PaperED
{
	class OutlinerPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;
	};
}

