#pragma once
#include "Engine.h"
class ForegroundLayer : public Layer
{
private:
	GameObject* character1;
	GameObject* character2;
	GameObject* object1;
	GameObject* lineObject1;
public:
	ForegroundLayer() = default;
	virtual ~ForegroundLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void Update(const float dt) override;
	void Imgui(const float dt) override;
	void OnEvent(Event& event) override;

};

