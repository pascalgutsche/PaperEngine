#pragma once
#include "_Core.h"

#include "layer/Layer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace core {

    class ImGuiLayer : public Layer {
    private:
        ImGuiID dockspace_id = 0;
        ImGuiID dock_id_main = 0;
        ImGuiID dock_id_top = 0;
        ImGuiID dock_id_down = 0;
        ImGuiID dock_id_left = 0;
        ImGuiID dock_id_right = 0;
        ImGuiID dock_id_right2 = 0;

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void attach() override;
        void detach() override;
        void update(const float dt) override { }
        void imgui(const float dt) override;
        void event(Event& event) override { }

        void begin(const float dt);
        void end();

        ImGuiID getDockspaceTOP() const { return dock_id_top; }
        ImGuiID getDockspaceBOTTOM() const { return dock_id_down; }
        ImGuiID getDockspaceLEFT() const { return dock_id_left; }
        ImGuiID getDockspaceRIGHT() const { return dock_id_right; }
        ImGuiID getDockspaceRIGHT2() const { return dock_id_right2; }

    };

}