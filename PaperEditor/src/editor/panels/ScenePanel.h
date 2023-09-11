#pragma once
#include "EditorPanel.h"

namespace PaperED
{
	class ScenePanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;
	};
}

