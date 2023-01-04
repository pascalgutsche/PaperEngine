#pragma once
#include "_Core.h"



namespace core { class ImGuiLayer; }
#include "generic/Scene.h"

namespace core {

    class ImGuiLayer {
    private:
        bool* p_open;
        void configureDocking();
    public:
        void init();
        void destroy();
        void startFrame(float deltaTime);
        void endFrame();
        void update(float deltaTime, Scene* curentScene);
    };

}