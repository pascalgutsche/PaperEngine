#pragma once
#include "Engine.h"

class TestScene : public core::Scene
{
public:
	TestScene();
	~TestScene() override;

	void loadResources() override;
	void init() override;
	void update(float deltaTime) override;
	void imgui(float deltaTime) override;
};

