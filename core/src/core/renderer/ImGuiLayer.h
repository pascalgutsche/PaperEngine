#pragma once
#include "_Core.h"

#include "generic/Scene.h"

namespace core {

    class ImGuiLayer {
    private:
    public:
        void init();
        void destroy();
        void startFrame(float deltaTime);
        void endFrame();
        void update(float deltaTime, Scene* curentScene);
    };

}