#pragma once
#include "Editor.h"

class UILayer;
class PELayer;

class EditorScene : public Scene
{
public:
	EditorScene();
	~EditorScene() override;

	void OnStart() override;
	void OnStop() override;
	void OnUpdate() override;
	void OnEvent(Event& e) override;

	void CameraMovement();

	bool MouseScrolled(MouseScrolledEvent& event) const;
	bool WindowFocus(WindowFocusEvent& event) const;
	bool WindowLostFocus(WindowLostFocusEvent& event) const;

private:
	

	UILayer* uiLayer;
	PELayer* peLayer;
};

