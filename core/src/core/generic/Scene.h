#pragma once
#include "utility.h"

namespace core { class Scene; }
#include "generic/Camera.h"
#include "renderer/Renderer.h"
#include "generic/GameObject.h"

namespace core {

    class CORE_API Scene {
    private:
        bool isRunning = false;
    protected:
        Camera* camera = nullptr;
        Renderer* renderer = nullptr;
        GameObject* activeGameObject = nullptr;
        glm::vec4 backcolor;
    public:
        std::vector<GameObject*> gameObjects;
    public:
        Scene();
        virtual ~Scene();

        Camera* getCamera();
        glm::vec4 getBackcolor();
        std::vector<GameObject*> getGameObjects();

        void addGameObjectToScene(GameObject* gameObject);
        void start();
        void disable();
        void sceneImgui(float deltaTime);
        void initGeneral();

        virtual void imgui(float deltaTime);
        virtual void init();
        virtual void loadResources();
        virtual void update(float deltaTime);
    };

}