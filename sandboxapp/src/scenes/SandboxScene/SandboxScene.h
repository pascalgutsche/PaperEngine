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
	bool MouseScrolled(MouseScrolledEvent& event) const;
};

