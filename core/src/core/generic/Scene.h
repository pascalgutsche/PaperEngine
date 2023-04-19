#pragma once
#include "_Core.h"
#include "utility.h"

namespace core {

    class GameObject;
    class Camera;
    class Layer;

    class Scene {
    private:
        bool isRunning = false;
    protected:
        Shr<Camera> camera = nullptr;
        glm::vec4 backcolor = glm::vec4();
    public:
        std::vector<GameObject*> gameObjects;

    public:
        Scene() = default;
        virtual ~Scene() = default;

        Camera* GetCamera();
        glm::vec4& GetBackcolor();

        
        void StartUp();
        void OnUpdate();

        void AddLayer(Layer* layer);
        void AddOverlay(Layer* layer);
        void RemoveLayer(Layer* layer) const;
        void RemoveOverlay(Layer* layer) const;

    private:
        virtual void Init() = 0;
        virtual void Start() = 0;
        virtual void Stop() = 0;
        virtual void Update() = 0;
        virtual void OnEvent(Event& e) = 0;
        virtual void Imgui(float dt) {}
    };

}