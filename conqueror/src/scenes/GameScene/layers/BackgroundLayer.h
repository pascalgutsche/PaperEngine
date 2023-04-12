#pragma once
#include "Engine.h"

class BackgroundLayer : public Layer
{
private:
	GameObject* background1;
	GameObject* background2;

	Panel* panel;
	Panel* panel4;
public:
	BackgroundLayer()
		: Layer("Background") { }

	virtual ~BackgroundLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void Update(const float dt) override;
	void Imgui(const float dt) override;
	void OnEvent(Event& event) override;

	void ButtonPressed();
	void ButtonPressed4();
};

