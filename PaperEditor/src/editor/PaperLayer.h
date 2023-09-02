#pragma once
#include "Editor.h"

#include "PanelManager.h"
#include "ViewPort.h"
#include "project/Project.h"
#include "scene/Entity.h"

enum CameraModes
{
	Single, Split, Triple, Quadro
};

inline std::string CameraModesToString(CameraModes mode)
{
	switch (mode) {
		case Single: return "Single";
		case Split: return "Split";
		case Triple: return "Triple";
		case Quadro: return "Quadro";
		default: return "";
	}
}

enum class SceneState
{
	Edit,
	Play,
	Simulate
};

class PaperLayer : public Layer
{
	friend struct ViewPort;
public:
	PaperLayer();
	~PaperLayer() override;

	void OnAttach() override;
	void OnDetach() override;
	void Update(const float dt) override;
	void OnEvent(Event& event) override;
	void EntityDragging();
	void Imgui(const float dt) override;

	
	void CreateProject(const std::filesystem::path& projPath, const std::string& projName);
	void OpenProject(const std::filesystem::path& projPath);
	void OpenProject(const Shr<Project>& project);
	void EmptyProject();
	void SaveProject() const;
	void CloseProject();

	void NewScene(const std::string& sceneName = "NewScene");
	void OpenScene(const Shr<Scene>& scene);
	void SaveScene() const;
	void SaveSceneAs(const std::filesystem::path& filePath) const;

	void GenerateProjectSolution(const std::filesystem::path& projPath) const;

	Entity drag_entity;
	Entity hovered_entity;
	//Entity active_entity;

	int GetGuizmoType() const { return gizmo_type; }

	bool AnyCameraActive() const;
	const ViewPort* GetViewportFocused() const;

	void SwitchScene(const Shr<Scene>& scene);

//private:
public: // temporary until proper rewrite


	//Shr<Scene> activeScene = nullptr;
	Shr<Scene> editorScene = nullptr;

	//void CheckSceneChange();
	//Shr<Scene> new_scene = nullptr;

	void MousePicking();

	//PANELS

	PanelManager panelManager;

	void UI_MenuBar();
	void UI_Toolbar();

	void ChangeWindowTitle() const;

	void ViewPortPanel();
	void CameraMode();
	void DockCameraPanel(CameraModes mode, ImGuiID main_id, const ImVec2& dockspace_size);
	void EnableCamera(CameraModes mode);


	ViewPort* viewPortCurrentlySimulating = nullptr;
	ViewPort* lastFocusedViewPort = nullptr;

	int gizmo_type = -1;

	CameraModes camera_mode = Single;
	bool camera_mode_changed = true;
	std::vector<ViewPort> viewports;

	SceneState sceneState = SceneState::Edit;

	void OnScenePlay();
	void OnSceneSimulate();
	void OnSceneStop();
};

