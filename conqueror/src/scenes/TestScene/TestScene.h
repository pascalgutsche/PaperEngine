#pragma once
#include "Engine.h"
#include "layers/SoundLayer.h"
#include "layers/ForegroundLayer.h"


class TestScene : public Scene
{
private:
	ForegroundLayer* foreground_layer;
	SoundLayer* sound_layer;
public:
	TestScene();
	virtual ~TestScene() override;

	void LoadResources() override;
	void Init() override;
	void Update() override;
	void Imgui(float dt) override;
	void OnEvent(Event& e) override;
	

	bool OnMouseScroll(MouseScrolledEvent& e);
	bool OnKeyPressed(KeyPressedEvent& e);
};

