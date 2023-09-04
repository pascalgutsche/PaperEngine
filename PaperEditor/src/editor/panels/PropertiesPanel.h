#pragma once
#include "EditorPanel.h"

class PaperLayer;

namespace PaperED
{
	class PropertiesPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;
	};
}

