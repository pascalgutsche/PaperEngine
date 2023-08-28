#pragma once
#include "EditorPanel.h"


class NewProjectPanel : public EditorPanel
{
public:
	NewProjectPanel() = default;

	void OnImGuiRender(bool& isOpen) override;
};
