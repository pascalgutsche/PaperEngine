#pragma once
#include "Engine.h"

#include "layer/Layer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <implot.h>
#include <implot_internal.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

namespace Colors
{
    static inline float Convert_sRGB_FromLinear(float theLinearValue);
    static inline float Convert_sRGB_ToLinear(float thesRGBValue);
    ImVec4 ConvertFromSRGB(ImVec4 colour);
    ImVec4 ConvertToSRGB(ImVec4 colour);

    // To experiment with editor theme live you can change these constexpr into static
    // members of a static "Theme" class and add a quick ImGui window to adjust the colour values
    namespace Theme
    {
        constexpr auto accent = IM_COL32(236, 158, 36, 255);
        constexpr auto highlight = IM_COL32(39, 185, 242, 255);
        constexpr auto niceBlue = IM_COL32(83, 232, 254, 255);
        constexpr auto compliment = IM_COL32(78, 151, 166, 255);
        constexpr auto background = IM_COL32(36, 36, 36, 255);
        constexpr auto backgroundDark = IM_COL32(26, 26, 26, 255);
        constexpr auto titlebar = IM_COL32(21, 21, 21, 255);
        constexpr auto propertyField = IM_COL32(15, 15, 15, 255);
        constexpr auto text = IM_COL32(192, 192, 192, 255);
        constexpr auto textBrighter = IM_COL32(210, 210, 210, 255);
        constexpr auto textDarker = IM_COL32(128, 128, 128, 255);
        constexpr auto textError = IM_COL32(230, 51, 51, 255);
        constexpr auto muted = IM_COL32(77, 77, 77, 255);
        constexpr auto groupHeader = IM_COL32(47, 47, 47, 255);
        constexpr auto selection = IM_COL32(237, 192, 119, 255);
        constexpr auto selectionMuted = IM_COL32(237, 201, 142, 23);
        constexpr auto backgroundPopup = IM_COL32(50, 50, 50, 255);
        constexpr auto validPrefab = IM_COL32(82, 179, 222, 255);
        constexpr auto invalidPrefab = IM_COL32(222, 43, 43, 255);
        constexpr auto missingMesh = IM_COL32(230, 102, 76, 255);
        constexpr auto meshNotSet = IM_COL32(250, 101, 23, 255);

        //Buttons
        constexpr auto button = IM_COL32(56, 56, 56, 200);
        constexpr auto button_hovered = IM_COL32(70, 70, 70, 255);
        constexpr auto button_active = IM_COL32(56, 56, 56, 150);
    }
}

namespace Paper {

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