#pragma once
#include "EditorPanel.h"

class PaperLayer;

class SceneDebuggingPanel : public EditorPanel
{
public:
	SceneDebuggingPanel(PaperLayer* paperLayer); // tempory until proper rewrite of panel system

	void OnImGuiRender(bool& isOpen, bool firstRender) override;

private:
	PaperLayer* paperLayer;
};
