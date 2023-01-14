#pragma once
#include "Engine.h"

class ExampleLayer : public core::Layer
{
public:
	virtual ~ExampleLayer() = default;

	void attach() override;
	void detach() override;
	void update(const float dt) override;
	void imgui(const float dt) override;

	bool OnMouseScrolled(core::MouseScrolledEvent& e) override;
};

