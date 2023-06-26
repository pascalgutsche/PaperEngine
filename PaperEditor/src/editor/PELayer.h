#pragma once
#include "Editor.h"

class PELayer : public Layer
{
public:
	PELayer();
	~PELayer() override;

	void OnAttach() override;
	void OnDetach() override;
	void Update(const float dt) override;
	void OnEvent(Event& event) override;
	void Imgui(const float dt) override;

	void DockPanel(std::string name, ImGuiID dock_id);
	
	glm::vec2 GetViewportSize() const { return viewport_size; };
	glm::ivec2 GetMousePosViewportRelative() const { return mouse_pos_viewport_relative; }
	bool IsMouseInsideViewport() const { return mouse_pos_viewport_relative.x >= 0 && mouse_pos_viewport_relative.y >= 0 && mouse_pos_viewport_relative.x < viewport_size.x&& mouse_pos_viewport_relative.y < viewport_size.y; }
	bool IsViewportHovered() const { return viewport_hovered; }
	bool IsCameraControlModeActive() const { return camera_control_mode; }

	ImGuiID GetDockspaceMain() const { return dock_id_main; }
	ImGuiID GetDockspaceTop() const { return dock_id_top; }
	ImGuiID GetDockspaceBottom() const { return dock_id_down; }
	ImGuiID GetDockspaceLeft() const { return dock_id_left; }
	ImGuiID GetDockspaceRight() const { return dock_id_right; }
	ImGuiID GetDockspaceRight2() const { return dock_id_right2; }
	ImGuiID GetDockspaceLeftBottom() const { return dock_id_left_bottom; }

private:
	Shr<Framebuffer> framebuffer;

	ImGuiID dockspace_id = 0;
	ImGuiID dock_id_main = 0;
	ImGuiID dock_id_top = 0;
	ImGuiID dock_id_down = 0;
	ImGuiID dock_id_left = 0;
	ImGuiID dock_id_right = 0;
	ImGuiID dock_id_right2 = 0;
	ImGuiID dock_id_left_bottom = 0;
	std::unordered_map<std::string, ImGuiID> dockPanelQueue;

	void MenuBar();
	void ApplicationPanel(const float dt, bool first);
	void AssetManagerPanel(const float dt, bool first);
	void LayerPanel(const float dt, bool first);
	void InspectorPanel(const float dt, bool first);
	void ViewPortPanel(const float dt, bool first);

	void CameraMovement();

	glm::vec2 viewport_size;
	glm::vec2 viewport_bounds[2];

	bool viewport_focused = false, viewport_hovered = false;

	bool camera_control_mode = false;

	glm::ivec2 mouse_pos_viewport_relative;

	std::unordered_map<std::string, void*> variable_pool;

	std::vector<Shr<EditorCamera>> cameras;
};

