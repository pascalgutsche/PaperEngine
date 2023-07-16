#pragma once
#include "Engine.h"

#include "layer/Layer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <implot.h>
#include <implot_internal.h>

namespace ppr {

    class ImGuiLayer : public Layer {
    
    public:
        ImGuiLayer();
    	~ImGuiLayer() override;

        void SetDarkThemeV2Colors();

        void OnAttach() override;
        void OnDetach() override;
        void Update(const float dt) override { };
        void Imgui(const float dt) override { };
        void OnEvent(Event& e) override { };

        void Begin(const float dt);
        void End();
        

    };

}

struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.resize(max_size);
        for (int i = 0; i < max_size; i++)
        {
            Data[i].x = -i * 0.016f;
            Data[i].y = 10000;
        }
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