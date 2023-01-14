#pragma once
#include "Engine.h"
#include "layers/ExampleLayer.h"

class TestScene : public core::Scene
{
private:
	ExampleLayer* example_layer;

public:
	TestScene();
	virtual ~TestScene() override;

	void loadResources() override;
	void init() override;
	void update(float deltaTime) override;
	void imgui(float deltaTime) override;
};

