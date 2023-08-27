#pragma once

class EditorPanel
{
public:
	virtual ~EditorPanel() = default;

	virtual void OnImGuiRender(bool& isOpen, bool firstRender) = 0;
	virtual void OnEvent(Event& e) {}
};