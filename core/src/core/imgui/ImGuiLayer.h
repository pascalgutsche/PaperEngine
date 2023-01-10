#pragma once
#include "_Core.h"

#include "layer/Layer.h"

#include <imgui.h>
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

namespace core {

    class ImGuiLayer : public Layer {
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
    };

}