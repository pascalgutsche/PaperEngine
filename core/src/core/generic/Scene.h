#pragma once
#include "utility.h"

#include "generic/Camera.h"
#include "renderer/Renderer.h"
#include "generic/GameObject.h"

namespace core {

    class Scene {
    private:
        bool isRunning = false;
    protected:
        Camera* camera = nullptr;
        Renderer* renderer = nullptr;
        GameObject* activeGameObject = nullptr;
        glm::vec4 backcolor = glm::vec4();
    public:
        std::vector<GameObject*> gameObjects;

    public:
        Scene();
        virtual ~Scene();

        Camera* GetCamera();
        glm::vec4& GetBackcolor();

        void AddGameObjectToScene(GameObject* gameObject);
        void Start();
        void Disable();
        void SceneImgui(float dt);
        void InitGeneral();
        void Render();

        Renderer* GetRenderer() const { return renderer; }

        virtual void update(float dt) = 0;
        virtual void OnEvent(Event& e) = 0;
    private:
        virtual void imgui(float dt) {};
        virtual void init() = 0;
        virtual void loadResources() = 0;
    };

}