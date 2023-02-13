#pragma once
#include "Engine.h"
#include "layers/SoundLayer.h"
#include "layers/ForegroundLayer.h"
#include "layers/BackgroundLayer.h"


class TestScene : public core::Scene
{
private:
	BackgroundLayer* background_layer;
	ForegroundLayer* foreground_layer;
	SoundLayer* sound_layer;
public:
	TestScene();
	virtual ~TestScene() override;

	void loadResources() override;
	void init() override;
	void update(float dt) override;
	void imgui(float dt) override;
	void OnEvent(Event& e) override
	{
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(TestScene::OnMouseScroll));
		dispatcher.dispatch<GameObjectPressedEvent>(BIND_EVENT_FN(TestScene::OnGameObjectPressed));
		dispatcher.dispatch<GameObjectReleasedEvent>(BIND_EVENT_FN(TestScene::OnGameObjectReleased));
		dispatcher.dispatch<GameObjectHoverBeginEvent>(BIND_EVENT_FN(TestScene::OnGameObjectHoverBegin));
		dispatcher.dispatch<GameObjectHoverEndEvent>(BIND_EVENT_FN(TestScene::OnGameObjectHoverEnd));


	};

	bool OnMouseScroll(MouseScrolledEvent& e);

	bool OnGameObjectPressed(GameObjectPressedEvent& e);
	bool OnGameObjectReleased(GameObjectReleasedEvent& e);
	bool OnGameObjectHoverBegin(GameObjectHoverBeginEvent& e);
	bool OnGameObjectHoverEnd(GameObjectHoverEndEvent& e);

};

