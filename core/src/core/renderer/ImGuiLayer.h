#pragma once
#include "_Core.h"

class ImGuiLayer;
#include "generic/Scene.h"

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
