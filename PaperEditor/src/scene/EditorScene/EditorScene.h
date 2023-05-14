#pragma once
#include "Editor.h"

class EditorScene : public Scene
{
public:
	EditorScene();
	~EditorScene() override;

	void OnStart() override;
	void OnStop() override;
	void OnUpdate() override;
	void OnEvent(Event& e) override;

	bool KeyPressed(KeyPressedEvent& event) const;
	bool MouseScrolled(MouseScrolledEvent& event) const;
	bool WindowFocus(WindowFocusEvent& event) const;
	bool WindowLostFocus(WindowLostFocusEvent& event) const;
};

