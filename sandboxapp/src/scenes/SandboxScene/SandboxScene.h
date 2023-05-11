#pragma once
#include "_Game.h"

#include "layers/UILayer.h"

class SandboxScene : public Scene
{
public:
	SandboxScene();
	~SandboxScene() override;

	void OnStart() override;
	void OnStop() override;
	void OnUpdate() override;
	void OnEvent(Event& e) override;

	UILayer* uiLayer;

	bool KeyPressed(KeyPressedEvent& event) const;
	bool KeyReleased(KeyReleasedEvent& event) const;
	bool MouseScrolled(MouseScrolledEvent& event) const;
	bool WindowFocus(WindowFocusEvent& event) const;
	bool WindowLostFocus(WindowLostFocusEvent& event) const;

private:
	Entity* cube;
	Entity* lightCube;
};

