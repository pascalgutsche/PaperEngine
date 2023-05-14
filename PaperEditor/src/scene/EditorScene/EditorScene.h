#pragma once
#include "Editor.h"

class EditorScene : public Scene
{
public:
	EditorScene();
	~EditorScene() override;

	void OnStart() override;
	void OnStop() override;
	void OnUpdate() override;
	void OnEvent(Event& e) override;
};

