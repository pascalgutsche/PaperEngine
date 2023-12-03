#pragma once
#include "Runtime.h"

#include "project/Project.h"

class RuntimeLayer : public Layer
{
	friend struct ViewPort;
public:
	RuntimeLayer();
	~RuntimeLayer() override;

	void OnAttach() override;
	void OnDetach() override;
	void Update(const float dt) override;
	void OnEvent(Event& event) override;
	void Imgui(const float dt) override;

	void UseProject(const std::filesystem::path& projPath);
	void UseProject(const Ref<Project>& project);
	void CloseProject();

	void OpenScene(Ref<Scene> scene);
	void CloseScene();
public:

	void ChangeWindowTitle() const;

	bool sceneChanged = false;

	Ref<Framebuffer> framebuffer;
};

