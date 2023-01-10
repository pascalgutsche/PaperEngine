#pragma once
#include "Engine.h"

class ExampleLayer : public core::Layer
{
public:
	void attach() override;
	void detach() override;
	void update(const float dt) override;
	void imgui(const float dt) override;
	void event(core::Event& event) override;
};

