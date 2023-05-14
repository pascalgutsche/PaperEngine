#pragma once
#include "Engine.h"



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

    glm::vec2 GetViewportSize() const { return viewportSize; };
    glm::ivec2 GetMousePosViewportRelative() const { return mousePosViewportRelative; }
    bool IsMouseInsideViewport() const { return mousePosViewportRelative.x >= 0 && mousePosViewportRelative.y >= 0 && mousePosViewportRelative.x < viewportSize.x&& mousePosViewportRelative.y < viewportSize.y; }

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
    void CustomPanel(const float dt, bool first);
    void LayerPanel(const float dt, bool first);
    void InspectorPanel(const float dt, bool first);
    void ViewPortPanel(const float dt, bool first);

    glm::vec2 viewportSize;
    glm::vec2 viewportBounds[2];

    bool viewportFocused = false, viewportHovered = false;

    glm::ivec2 mousePosViewportRelative;

    Object* selectedObject = nullptr;

    std::unordered_map<std::string, void*> variablePool;

    

};

