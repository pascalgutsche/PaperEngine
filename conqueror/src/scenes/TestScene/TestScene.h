#pragma once
#include "Engine.h"
#include "layers/SoundLayer.h"
#include "layers/ForegroundLayer.h"
#include "layers/BackgroundLayer.h"


class TestScene : public Scene
{
private:
	BackgroundLayer* background_layer;
	ForegroundLayer* foreground_layer;
	SoundLayer* sound_layer;
public:
	TestScene();
	virtual ~TestScene() override;

	void LoadResources() override;
	void Init() override;
	void Update() override;
	void Imgui(float dt) override;
	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(TestScene::OnMouseScroll));
	};

	bool OnMouseScroll(MouseScrolledEvent& e);

};

