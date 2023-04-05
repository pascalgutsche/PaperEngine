#pragma once
#include "Engine.h"
#include "layers/BackgroundLayer.h"


class GameScene : public Scene
{
private:
	BackgroundLayer* background_layer;
public:
	GameScene();
	virtual ~GameScene() override;

	void LoadResources() override;
	void Init() override;
	void Update() override;
	void Imgui(float dt) override;
	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(GameScene::OnMouseScroll));
	};

	bool OnMouseScroll(MouseScrolledEvent& e);

};

