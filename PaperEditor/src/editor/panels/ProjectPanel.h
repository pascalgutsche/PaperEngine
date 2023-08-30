#pragma once
#include "EditorPanel.h"

namespace PaperED
{
	class NewProjectPanel : public EditorPanel
	{
	public:
		void OnImGuiRender(bool& isOpen) override;

	private:
		std::string projName = "";
		std::string projPath = "";
	};
}
