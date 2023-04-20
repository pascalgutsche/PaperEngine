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

	void OnStart() override;
	void OnStop() override;
	void OnUpdate() override;
	void Imgui(float dt) override;
	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(GameScene::OnMouseScroll));
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(GameScene::OnKeyPressed));
	};

	bool OnMouseScroll(MouseScrolledEvent& e);
	bool OnKeyPressed(KeyPressedEvent& e);


};

