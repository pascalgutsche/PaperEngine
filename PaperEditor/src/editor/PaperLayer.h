#pragma once
#include "Editor.h"

#include "ViewPort.h"

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
	void Imgui(const float dt) override;

	void DockPanel(std::string name, ImGuiID dock_id);

	ImGuiID dockspace_id = 0;
	ImGuiID dock_id_main = 0;
	ImGuiID dock_id_down = 0;
	ImGuiID dock_id_left = 0;
	ImGuiID dock_id_right = 0;
	ImGuiID dock_id_right_bottom = 0;
	ImGuiID dock_id_right2 = 0;
	ImGuiID dock_id_left_bottom = 0;
	ImGuiID dock_id_up = 0;

	Entity hovered_entity;
	Entity active_entity;

	int GetGuizmoType() const { return gizmo_type; }

	bool AnyCameraActive() const;
	const ViewPort* GetViewportFocused() const;

	void SwitchScene(const Shr<Scene>& scene);

private:

	std::unordered_map<std::string, ImGuiID> dockPanelQueue;

	Shr<Scene> activeScene = nullptr;
	Shr<Scene> editorScene = nullptr;

	void CheckSceneChange();
	Shr<Scene> new_scene = nullptr;

	void MousePicking();

	//PANELS

	void MainMenuBar();
	void ApplicationPanel();
	void SceneDebugger();
	void AssetManagerPanel();
	void OutlinerPanel();
	void PropertiesPanel();

	void ToolbarPanel();

	void CameraSettingsPanel();

	void ViewPortPanel();
	void CameraMode();
	void DockCameraPanel(CameraModes mode, ImGuiID main_id, const ImVec2& dockspace_size);
	void EnableCamera(CameraModes mode);

	void ViewPortDebugging();

	bool first_frame = true;
	bool viewport_panel_first = true;
	bool application_panel_first = true;
	bool scene_debugger_panel_first = true;
	bool asset_manager_panel_first = true;
	bool properties_panel_first = true;
	bool outliner_panel_first = true;
	bool camera_settings_panel_first = true;
	bool viewport_debugging_panel_first = true;

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

