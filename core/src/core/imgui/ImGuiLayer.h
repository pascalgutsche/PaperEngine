#pragma once
#include "_Core.h"

#include "layer/Layer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

namespace core {

    class ImGuiLayer : public Layer {
    private:
        ImGuiID dockspace_id;
        ImGuiID dock_id_top;
        ImGuiID dock_id_down;
        ImGuiID dock_id_left;
        ImGuiID dock_id_right;
        ImGuiID dock_id_right2;

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