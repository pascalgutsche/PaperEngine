#pragma once
#include "_Core.h"

#include "layer/Layer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <implot.h>
#include <implot_internal.h>

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

        std::unordered_map<std::string, ImGuiID> dock_panel_queue;

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

        void DockPanel(std::string name, ImGuiID dock_id);

        ImGuiID getDockspaceMAIN() const { return dock_id_main; }
        ImGuiID getDockspaceTOP() const { return dock_id_top; }
        ImGuiID getDockspaceBOTTOM() const { return dock_id_down; }
        ImGuiID getDockspaceLEFT() const { return dock_id_left; }
        ImGuiID getDockspaceRIGHT() const { return dock_id_right; }
        ImGuiID getDockspaceRIGHT2() const { return dock_id_right2; }

    };

}

struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};